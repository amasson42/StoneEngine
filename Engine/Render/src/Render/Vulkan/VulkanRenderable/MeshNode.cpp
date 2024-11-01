// Copyright 2024 Stone-Engine

#include "MeshNode.hpp"

#include "../Device.hpp"
#include "../RenderContext.hpp"
#include "../RenderPass.hpp"
#include "../SwapChain.hpp"
#include "../Utilities/VertexBinding.hpp"
#include "Render/Vulkan/VulkanRenderer.hpp"
#include "Scene/Node/MeshNode.hpp"
#include "Scene/Renderable/Material.hpp"
#include "Scene/Renderable/Mesh.hpp"
#include "Scene/Renderable/Shader.hpp"
#include "Scene/Renderable/Texture.hpp"
#include "Texture.hpp"
#include "Utils/FileSystem.hpp"

#include <cstring>
#include <stdexcept>

namespace Stone::Render::Vulkan {


MeshNode::MeshNode(const std::shared_ptr<Scene::MeshNode> &meshNode, const std::shared_ptr<VulkanRenderer> &renderer)
	: _device(renderer->getDevice()), _sceneMeshNode(meshNode) {
	_createDescriptorSetLayout();
	_createGraphicPipeline(renderer->getRenderPass(), renderer->getSwapChain()->getExtent());
	_createVertexBuffer();
	_createIndexBuffer();
	_createUniformBuffers(renderer->getSwapChain());
	_createDescriptorPool(renderer->getSwapChain());
	_createDescriptorSets(renderer->getSwapChain());
}

MeshNode::~MeshNode() {
	_destroyDescriptorSets();
	_destroyDescriptorPool();
	_destroyUniformBuffers();
	_destroyVertexBuffer();
	_destroyIndexBuffer();
	_destroyGraphicPipeline();
	_destroyDescriptorSetLayout();
}

void MeshNode::render(Scene::RenderContext &context) {
	assert(dynamic_cast<Vulkan::RenderContext *>(&context));
	auto vulkanContext = reinterpret_cast<Vulkan::RenderContext *>(&context);

	_updateUniformBuffers(*vulkanContext);

	vkCmdBindPipeline(vulkanContext->commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _graphicPipeline);

	VkBuffer vertexBuffers[] = {_vertexBuffer};
	VkDeviceSize offsets[] = {0};
	vkCmdBindVertexBuffers(vulkanContext->commandBuffer, 0, 1, vertexBuffers, offsets);

	vkCmdBindIndexBuffer(vulkanContext->commandBuffer, _indexBuffer, 0, VK_INDEX_TYPE_UINT32);

	vkCmdBindDescriptorSets(vulkanContext->commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelineLayout, 0, 1,
							&_descriptorSets[vulkanContext->imageIndex], 0, nullptr);

	auto mesh = std::dynamic_pointer_cast<Scene::DynamicMesh>(_sceneMeshNode.lock()->getMesh());
	vkCmdDrawIndexed(vulkanContext->commandBuffer, mesh->getIndices().size(), 1, 0, 0, 0);
}

void MeshNode::_updateUniformBuffers(Vulkan::RenderContext &context) {
	std::memcpy(_uniformBuffersMapped[context.imageIndex], &context.mvp, sizeof(Scene::MvpMatrices));
}

void MeshNode::_createDescriptorSetLayout() {
	std::vector<VkDescriptorSetLayoutBinding> bindings = {};

	bindings.push_back({});
	VkDescriptorSetLayoutBinding &uboLayoutBinding = bindings.back();
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uboLayoutBinding.pImmutableSamplers = nullptr;

	auto material = _sceneMeshNode.lock()->getMaterial();
	if (material) {
		auto shader = material->getFragmentShader(); // TODO: Take default fragment shader if none is found
		if (shader) {
			material->forEachTextures(
				[&](const std::pair<const Scene::Material::Location &, std::shared_ptr<Scene::Texture>> &texture) {
					VkDescriptorSetLayoutBinding samplerLayoutBinding;

					// TODO: Factor this code in scene function
					int location = -1;
					if (std::holds_alternative<int>(texture.first)) {
						location = std::get<int>(texture.first);
					} else if (std::holds_alternative<std::string>(texture.first)) {
						location = shader->getLocation(std::get<std::string>(texture.first));
					}
					if (location == -1) {
						return;
					}
					samplerLayoutBinding.binding = location;
					samplerLayoutBinding.descriptorCount = 1;
					samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
					samplerLayoutBinding.pImmutableSamplers = nullptr;
					bindings.push_back(samplerLayoutBinding);
				});
		}
	}

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(_device->getDevice(), &layoutInfo, nullptr, &_descriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout!");
	}
}

void MeshNode::_destroyDescriptorSetLayout() {
	if (_device) {
		vkDestroyDescriptorSetLayout(_device->getDevice(), _descriptorSetLayout, nullptr);
	}
}

void MeshNode::_createGraphicPipeline(const std::shared_ptr<RenderPass> &renderPass, VkExtent2D extent) {
	auto vertShaderCode = Utils::readBinaryFile("shaders/vert.spv");
	auto fragShaderCode = Utils::readBinaryFile("shaders/frag.spv");

	auto vertShaderModule = _device->createShaderModule(vertShaderCode);
	auto fragShaderModule = _device->createShaderModule(fragShaderCode);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

	std::vector<VkDynamicState> dynamicStates = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR,
	};

	VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {};
	dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicStateCreateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicStateCreateInfo.pDynamicStates = dynamicStates.data();

	auto bindingDescription = vertexBindingDescription<Scene::Vertex>();
	auto attributeDescriptions = vertexAttributeDescriptions<Scene::Vertex, 5>();

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(extent.width);
	viewport.height = static_cast<float>(extent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = {0, 0};
	scissor.extent = extent;

	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f;
	rasterizer.depthBiasClamp = 0.0f;
	rasterizer.depthBiasSlopeFactor = 0.0f;

	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling.minSampleShading = 1.0f;
	multisampling.pSampleMask = nullptr;
	multisampling.alphaToCoverageEnable = VK_FALSE;
	multisampling.alphaToOneEnable = VK_FALSE;

	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_TRUE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &_descriptorSetLayout;
	pipelineLayoutInfo.pushConstantRangeCount = 0;

	if (vkCreatePipelineLayout(_device->getDevice(), &pipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create pipeline layout");
	}

	VkPipelineDepthStencilStateCreateInfo depthStencil = {};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.minDepthBounds = 0.0f;
	depthStencil.maxDepthBounds = 1.0f;
	depthStencil.stencilTestEnable = VK_FALSE;
	depthStencil.front = {};
	depthStencil.back = {};

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = &depthStencil;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &dynamicStateCreateInfo;
	pipelineInfo.layout = _pipelineLayout;
	pipelineInfo.renderPass = renderPass->getRenderPass();
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineInfo.basePipelineIndex = -1;

	if (vkCreateGraphicsPipelines(_device->getDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &_graphicPipeline) !=
		VK_SUCCESS) {
		throw std::runtime_error("Failed to create graphics pipeline");
	}

	vkDestroyShaderModule(_device->getDevice(), vertShaderModule, nullptr);
	vkDestroyShaderModule(_device->getDevice(), fragShaderModule, nullptr);
}

void MeshNode::_destroyGraphicPipeline() {
	if (_graphicPipeline != VK_NULL_HANDLE) {
		vkDestroyPipeline(_device->getDevice(), _graphicPipeline, nullptr);
	}
	_graphicPipeline = VK_NULL_HANDLE;
	if (_pipelineLayout != VK_NULL_HANDLE) {
		vkDestroyPipelineLayout(_device->getDevice(), _pipelineLayout, nullptr);
	}
	_pipelineLayout = VK_NULL_HANDLE;
}

void MeshNode::_createVertexBuffer() {
	std::shared_ptr<Scene::MeshNode> meshNode = _sceneMeshNode.lock();
	assert(meshNode);

	auto mesh = std::dynamic_pointer_cast<Scene::DynamicMesh>(meshNode->getMesh());
	const std::vector<Scene::Vertex> &vertices = mesh->getVertices();

	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

	auto [stagingBuffer, stagingBufferMemory] =
		_device->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
							  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	void *data;
	vkMapMemory(_device->getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
	std::memcpy(data, vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(_device->getDevice(), stagingBufferMemory);

	std::tie(_vertexBuffer, _vertexBufferMemory) =
		_device->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
							  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	_device->bufferCopy(_vertexBuffer, stagingBuffer, bufferSize);

	_device->destroyBuffer(stagingBuffer, stagingBufferMemory);
}

void MeshNode::_destroyVertexBuffer() {
	if (_device) {
		_device->destroyBuffer(_vertexBuffer, _vertexBufferMemory);
	}
}

void MeshNode::_createIndexBuffer() {
	std::shared_ptr<Scene::MeshNode> meshNode = _sceneMeshNode.lock();
	assert(meshNode);

	auto mesh = std::dynamic_pointer_cast<Scene::DynamicMesh>(meshNode->getMesh());
	const std::vector<uint32_t> &indices = mesh->getIndices();

	VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

	auto [stagingBuffer, stagingBufferMemory] =
		_device->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
							  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	void *data;
	vkMapMemory(_device->getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
	std::memcpy(data, indices.data(), (size_t)bufferSize);
	vkUnmapMemory(_device->getDevice(), stagingBufferMemory);

	std::tie(_indexBuffer, _indexBufferMemory) =
		_device->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
							  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	_device->bufferCopy(_indexBuffer, stagingBuffer, bufferSize);

	_device->destroyBuffer(stagingBuffer, stagingBufferMemory);
}

void MeshNode::_destroyIndexBuffer() {
	if (_device) {
		_device->destroyBuffer(_indexBuffer, _indexBufferMemory);
	}
}

void MeshNode::_createUniformBuffers(const std::shared_ptr<SwapChain> &swapChain) {
	std::shared_ptr<Scene::MeshNode> meshNode = _sceneMeshNode.lock();
	assert(meshNode);

	VkDeviceSize bufferSize = sizeof(Scene::MvpMatrices);

	uint32_t imageCount = swapChain->getImageCount();
	_uniformBuffers.resize(imageCount);
	_uniformBuffersMemory.resize(imageCount);
	_uniformBuffersMapped.resize(imageCount);

	for (uint32_t i = 0; i < imageCount; i++) {
		std::tie(_uniformBuffers[i], _uniformBuffersMemory[i]) =
			_device->createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
								  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		vkMapMemory(_device->getDevice(), _uniformBuffersMemory[i], 0, bufferSize, 0, &_uniformBuffersMapped[i]);
	}
}

void MeshNode::_destroyUniformBuffers() {
	if (_device) {
		for (size_t i = 0; i < _uniformBuffers.size(); i++) {
			vkUnmapMemory(_device->getDevice(), _uniformBuffersMemory[i]);
			_device->destroyBuffer(_uniformBuffers[i], _uniformBuffersMemory[i]);
		}
	}
}

void MeshNode::_createDescriptorPool(const std::shared_ptr<SwapChain> &swapChain) {
	std::vector<VkDescriptorPoolSize> poolSizes = {};
	poolSizes.push_back({});
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = swapChain->getImageCount();

	auto material = _sceneMeshNode.lock()->getMaterial();
	if (material) {
		auto shader = material->getFragmentShader();
		if (shader) {
			material->forEachTextures(
				[&](const std::pair<const Scene::Material::Location &, std::shared_ptr<Scene::Texture>> &texture) {
					(void)texture;
					VkDescriptorPoolSize poolSize = {};
					poolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					poolSize.descriptorCount = swapChain->getImageCount();
					poolSizes.push_back(poolSize);
				});
		}
	}

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = swapChain->getImageCount();

	if (vkCreateDescriptorPool(_device->getDevice(), &poolInfo, nullptr, &_descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

void MeshNode::_destroyDescriptorPool() {
	if (_device) {
		vkDestroyDescriptorPool(_device->getDevice(), _descriptorPool, nullptr);
	}
}

void MeshNode::_createDescriptorSets(const std::shared_ptr<SwapChain> &swapChain) {
	std::vector<VkDescriptorSetLayout> layouts(swapChain->getImageCount(), _descriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = _descriptorPool;
	allocInfo.descriptorSetCount = swapChain->getImageCount();
	allocInfo.pSetLayouts = layouts.data();

	_descriptorSets.resize(swapChain->getImageCount());
	if (vkAllocateDescriptorSets(_device->getDevice(), &allocInfo, _descriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	for (size_t i = 0; i < _descriptorSets.size(); ++i) {
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = _uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(Scene::MvpMatrices);

		std::vector<VkWriteDescriptorSet> descriptorWrites = {};
		descriptorWrites.push_back({});

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = _descriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		std::vector<VkDescriptorImageInfo> imagesInfo;
		auto material = _sceneMeshNode.lock()->getMaterial();
		if (material) {
			auto shader = material->getFragmentShader();
			if (shader) {
				material->forEachTextures(
					[&](const std::pair<const Scene::Material::Location &, std::shared_ptr<Scene::Texture>> &texture) {
						int location = -1;
						if (std::holds_alternative<int>(texture.first)) {
							location = std::get<int>(texture.first);
						} else if (std::holds_alternative<std::string>(texture.first)) {
							location = shader->getLocation(std::get<std::string>(texture.first));
						}
						if (location == -1) {
							return;
						}

						auto textureObject = texture.second->getRendererObject<Texture>();

						imagesInfo.push_back({});
						VkDescriptorImageInfo &imageInfo(imagesInfo.back());
						imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
						imageInfo.imageView = textureObject->getImageView();
						imageInfo.sampler = textureObject->getSampler();

						VkWriteDescriptorSet descriptorWrite = {};
						descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
						descriptorWrite.dstSet = _descriptorSets[i];
						descriptorWrite.dstBinding = location;
						descriptorWrite.dstArrayElement = 0;
						descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
						descriptorWrite.descriptorCount = 1;
						descriptorWrite.pImageInfo = &imageInfo;
						descriptorWrites.push_back(descriptorWrite);
					});
			}
		}

		vkUpdateDescriptorSets(_device->getDevice(), static_cast<uint32_t>(descriptorWrites.size()),
							   descriptorWrites.data(), 0, nullptr);
	}
}

void MeshNode::_destroyDescriptorSets() {
}

} // namespace Stone::Render::Vulkan
