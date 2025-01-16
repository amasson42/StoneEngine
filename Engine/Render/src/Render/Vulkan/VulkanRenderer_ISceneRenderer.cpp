// Copyright 2024 Stone-Engine

#include "Device.hpp"
#include "FramesRenderer.hpp"
#include "Render/Vulkan/VulkanRenderer.hpp"
#include "RenderContext.hpp"
#include "RendererObjectManager.hpp"
#include "RenderPass.hpp"
#include "Scene.hpp"
#include "SwapChain.hpp"

namespace Stone::Render::Vulkan {

void VulkanRenderer::updateDataForWorld(const std::shared_ptr<Scene::WorldNode> &world) {
	RendererObjectManager manager(std::static_pointer_cast<VulkanRenderer>(shared_from_this()));
	world->traverseTopDown([&manager](const std::shared_ptr<Scene::Node> &node) {
		auto renderElement = std::dynamic_pointer_cast<Scene::IRenderable>(node);
		if (renderElement && renderElement->isDirty()) {
			manager.updateRenderable(node);
		}
	});
}

void VulkanRenderer::renderWorld(const std::shared_ptr<Scene::WorldNode> &world) {

	if (!_framesRenderer) {
		return;
	}

	FrameContext frameContext = _framesRenderer->newFrameContext();
	SyncronizedObjects &syncObject = frameContext.syncObject;

	vkWaitForFences(_device->getDevice(), 1, &syncObject.inFlight, VK_TRUE, UINT64_MAX);

	ImageContext imageContext{};
	VkResult result = _swapChain->acquireNextImage(syncObject.imageAvailable, imageContext);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
		std::cout << "Must recreate swap chain" << std::endl;
	} else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	vkResetFences(_device->getDevice(), 1, &syncObject.inFlight);

	vkResetCommandBuffer(frameContext.commandBuffer, 0);

	_recordCommandBuffer(frameContext.commandBuffer, &imageContext, world);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = {syncObject.imageAvailable};
	VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &frameContext.commandBuffer;

	VkSemaphore signalSemaphores[] = {syncObject.renderFinished};
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(_device->getGraphicsQueue(), 1, &submitInfo, syncObject.inFlight) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = {_swapChain->getSwapChain()};
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageContext.index;

	vkQueuePresentKHR(_device->getPresentQueue(), &presentInfo);
}

void VulkanRenderer::_recordCommandBuffer(VkCommandBuffer commandBuffer, ImageContext *imageContext,
										  const std::shared_ptr<Scene::WorldNode> &world) {
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("Failed to begin recording command buffer");
	}

	std::array<VkClearValue, 2> clearValues = {};
	clearValues[0].color = {
		{0.0f, 0.0f, 0.0f, 1.0f}
	};
	clearValues[1].depthStencil = {1.0f, 0};

	VkRenderPassBeginInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = _renderPass->getRenderPass();
	renderPassInfo.framebuffer = imageContext->framebuffer;
	renderPassInfo.renderArea.offset = {0, 0};
	renderPassInfo.renderArea.extent = _swapChain->getExtent();
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(_swapChain->getExtent().width);
	viewport.height = static_cast<float>(_swapChain->getExtent().height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	VkRect2D scissor = {};
	scissor.offset = {0, 0};
	scissor.extent = _swapChain->getExtent();
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	Vulkan::RenderContext context;
	context.commandBuffer = commandBuffer;
	context.extent = _swapChain->getExtent();
	context.imageIndex = imageContext->index;

	world->initializeRenderContext(context);
	world->render(context);

	vkCmdEndRenderPass(commandBuffer);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("Failed to record command buffer");
	}
}

} // namespace Stone::Render::Vulkan
