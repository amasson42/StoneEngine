// Copyright 2024 Stone-Engine

#include "Core/Assets/Bundle.hpp"
#include "Core/Exceptions.hpp"
#include "Core/Image/ImageSource.hpp"
#include "Scene/Assets/AssetResource.hpp"
#include "Scene/Node/MeshNode.hpp"
#include "Scene/Node/Node.hpp"
#include "Scene/Node/PivotNode.hpp"
#include "Scene/Node/SkinMeshNode.hpp"
#include "Scene/Renderable/Material.hpp"
#include "Scene/Renderable/Mesh.hpp"
#include "Scene/Renderable/SkinMesh.hpp"
#include "Scene/Renderable/Texture.hpp"

#include <assimp/Exporter.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace Stone::Scene {

static std::unique_ptr<Assimp::Importer> _assimpImporter = nullptr;

static Assimp::Importer &getAssimpImporter() {
	if (!_assimpImporter) {
		_assimpImporter = std::make_unique<Assimp::Importer>();
	}
	return *_assimpImporter;
}

inline glm::vec3 convert(const aiVector3D &vector) {
	return {vector.x, vector.y, vector.z};
}

inline glm::vec2 convert(const aiVector2D &vector) {
	return {vector.x, vector.y};
}

inline glm::mat4 convert(const aiMatrix4x4 &matrix) {
	return {matrix.a1, matrix.a2, matrix.a3, matrix.a4, matrix.b1, matrix.b2, matrix.b3, matrix.b4,
			matrix.c1, matrix.c2, matrix.c3, matrix.c4, matrix.d1, matrix.d2, matrix.d3, matrix.d4};
}

inline glm::quat convert(const aiQuaternion &quaternion) {
	return {quaternion.w, quaternion.x, quaternion.y, quaternion.z};
}

inline glm::vec3 convert(const aiColor3D &color) {
	return {color.r, color.g, color.b};
}

inline glm::vec4 convert(const aiColor4D &color) {
	return {color.r, color.g, color.b, color.a};
}

template <typename VertexType>
void emplace_vertices(std::vector<VertexType> &vertices, const aiMesh *mesh) {
	vertices.reserve(mesh->mNumVertices);
	if (mesh->HasNormals()) {
		if (mesh->HasTangentsAndBitangents()) {
			for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
				vertices.emplace_back(convert(mesh->mVertices[i]), convert(mesh->mNormals[i]),
									  convert(mesh->mTangents[i]), convert(mesh->mBitangents[i]),
									  mesh->mTextureCoords[0] ? convert(mesh->mTextureCoords[0][i]) : glm::vec2(0.0f));
			}
		} else {
			for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
				vertices.emplace_back(convert(mesh->mVertices[i]), convert(mesh->mNormals[i]),
									  mesh->mTextureCoords[0] ? convert(mesh->mTextureCoords[0][i]) : glm::vec2(0.0f));
			}
		}
	} else {
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			vertices.emplace_back(convert(mesh->mVertices[i]),
								  mesh->mTextureCoords[0] ? convert(mesh->mTextureCoords[0][i]) : glm::vec2(0.0f));
		}
	}
}

void emplace_indices(std::vector<uint32_t> &indices, const aiMesh *mesh) {
	indices.reserve(mesh->mNumFaces * 3);
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		const aiFace &face = mesh->mFaces[i];
		assert(face.mNumIndices == 3);
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
}

void loadMesh(AssetResource &assetResource, const aiMesh *mesh) {
	std::shared_ptr<DynamicMesh> newMesh = std::make_shared<DynamicMesh>();

	emplace_vertices(newMesh->verticesRef(), mesh);
	emplace_indices(newMesh->indicesRef(), mesh);

	std::shared_ptr<StaticMesh> newStaticMesh = std::make_shared<StaticMesh>();
	newStaticMesh->setSourceMesh(newMesh);

	assetResource.getMeshesRef().push_back(newStaticMesh);
}

void loadSkinMesh(AssetResource &assetResource, const aiMesh *mesh) {
	std::shared_ptr<DynamicSkinMesh> newMesh = std::make_shared<DynamicSkinMesh>();

	emplace_vertices(newMesh->verticesRef(), mesh);
	emplace_indices(newMesh->indicesRef(), mesh);

	// TODO: Load bones and weights. REQUIREMENT: Skeleton must be loaded first.

	std::shared_ptr<StaticSkinMesh> newStaticMesh = std::make_shared<StaticSkinMesh>();
	newStaticMesh->setSourceMesh(newMesh);

	assetResource.getMeshesRef().push_back(newStaticMesh);
}

void loadMeshes(AssetResource &assetResource, const aiScene *scene) {
	for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
		const aiMesh *mesh = scene->mMeshes[i];
		if (mesh->HasBones()) {
			loadSkinMesh(assetResource, mesh);
		} else {
			loadMesh(assetResource, mesh);
		}
	}
}

void loadTexture(AssetResource &assetResource, const aiTexture *texture) {
	auto image = assetResource.getBundle()->loadResource<Core::Image::ImageSource>(texture->mFilename.C_Str());

	std::shared_ptr<Texture> newTexture = std::make_shared<Texture>();
	newTexture->setImage(image);

	assetResource.getTexturesRef().push_back(newTexture);
}

void loadTextures(AssetResource &assetResource, const aiScene *scene) {
	for (unsigned int i = 0; i < scene->mNumTextures; i++) {
		const aiTexture *texture = scene->mTextures[i];
		loadTexture(assetResource, texture);
	}
}

void addMaterialScalar(const aiMaterial *material, const std::shared_ptr<Material> &newMaterial, const char *key,
					   unsigned int type, unsigned int idx, const std::string &name) {

	float scalar;
	if (material->Get(key, type, idx, scalar) == AI_SUCCESS) {
		newMaterial->setScalarParameter(name, scalar);
	}
}

void addMaterialColor(const aiMaterial *material, const std::shared_ptr<Material> &newMaterial, const char *key,
					  unsigned int type, unsigned int idx, const std::string &name) {

	aiColor3D color;
	if (material->Get(key, type, idx, color) == AI_SUCCESS) {
		newMaterial->setVectorParameter(name, convert(color));
	}
}

void addMaterialTexture(AssetResource &assetResource, const aiMaterial *material,
						const std::shared_ptr<Material> &newMaterial, aiTextureType type, const std::string &name) {

	if (material->GetTextureCount(type) <= 0)
		return;

	aiString texturePath;
	if (material->GetTexture(type, 0, &texturePath) != AI_SUCCESS)
		return;

	std::string texturePathStr = texturePath.C_Str();
	if (texturePathStr.empty())
		return;

	texturePathStr = assetResource.getSubDirectory() + "/" + texturePathStr;

	if (texturePathStr[0] == '*') {
		int textureIndex = std::stoi(texturePathStr.substr(1));
		if (textureIndex >= 0 && textureIndex < static_cast<int>(assetResource.getTextures().size())) {
			std::shared_ptr<Texture> texture = assetResource.getTextures()[textureIndex];
			newMaterial->setTextureParameter(name, texture);
		}
	} else {
		std::shared_ptr<Core::Image::ImageSource> textureSource =
			assetResource.getBundle()->loadResource<Core::Image::ImageSource>(texturePathStr);
		std::shared_ptr<Texture> texture = std::make_shared<Texture>();
		texture->setImage(textureSource);
		newMaterial->setTextureParameter(name, texture);
	}
}

void loadMaterial(AssetResource &assetResource, const aiMaterial *material) {
	std::shared_ptr<Material> newMaterial = std::make_shared<Material>();

	addMaterialColor(material, newMaterial, AI_MATKEY_COLOR_DIFFUSE, "diffuse");
	addMaterialColor(material, newMaterial, AI_MATKEY_COLOR_SPECULAR, "specular");
	addMaterialColor(material, newMaterial, AI_MATKEY_COLOR_AMBIENT, "ambient");
	addMaterialColor(material, newMaterial, AI_MATKEY_COLOR_EMISSIVE, "emissive");
	addMaterialScalar(material, newMaterial, AI_MATKEY_SHININESS, "shininess");
	addMaterialScalar(material, newMaterial, AI_MATKEY_OPACITY, "opacity");

	addMaterialScalar(material, newMaterial, AI_MATKEY_ROUGHNESS_FACTOR, "roughness");
	addMaterialScalar(material, newMaterial, AI_MATKEY_METALLIC_FACTOR, "metallic");
	addMaterialScalar(material, newMaterial, AI_MATKEY_REFLECTIVITY, "reflectivity");

	addMaterialTexture(assetResource, material, newMaterial, aiTextureType_DIFFUSE, "diffuse");
	addMaterialTexture(assetResource, material, newMaterial, aiTextureType_SPECULAR, "specular");
	addMaterialTexture(assetResource, material, newMaterial, aiTextureType_AMBIENT, "ambient");
	addMaterialTexture(assetResource, material, newMaterial, aiTextureType_EMISSIVE, "emissive");
	addMaterialTexture(assetResource, material, newMaterial, aiTextureType_HEIGHT, "height");
	addMaterialTexture(assetResource, material, newMaterial, aiTextureType_NORMALS, "normals");
	addMaterialTexture(assetResource, material, newMaterial, aiTextureType_SHININESS, "shininess");
	addMaterialTexture(assetResource, material, newMaterial, aiTextureType_OPACITY, "opacity");
	addMaterialTexture(assetResource, material, newMaterial, aiTextureType_DISPLACEMENT, "displacement");
	addMaterialTexture(assetResource, material, newMaterial, aiTextureType_LIGHTMAP, "lightmap");
	addMaterialTexture(assetResource, material, newMaterial, aiTextureType_REFLECTION, "reflection");
	addMaterialTexture(assetResource, material, newMaterial, aiTextureType_BASE_COLOR, "color");
	addMaterialTexture(assetResource, material, newMaterial, aiTextureType_NORMAL_CAMERA, "normal_camera");
	addMaterialTexture(assetResource, material, newMaterial, aiTextureType_EMISSION_COLOR, "emission_color");
	addMaterialTexture(assetResource, material, newMaterial, aiTextureType_METALNESS, "metalness");
	addMaterialTexture(assetResource, material, newMaterial, aiTextureType_DIFFUSE_ROUGHNESS, "roughness");
	addMaterialTexture(assetResource, material, newMaterial, aiTextureType_AMBIENT_OCCLUSION, "occlusion");
	addMaterialTexture(assetResource, material, newMaterial, aiTextureType_SHEEN, "sheen");
	addMaterialTexture(assetResource, material, newMaterial, aiTextureType_CLEARCOAT, "clearcoat");
	addMaterialTexture(assetResource, material, newMaterial, aiTextureType_TRANSMISSION, "transmission");

	assetResource.getMaterialsRef().push_back(newMaterial);
}

void loadMaterials(AssetResource &assetResource, const aiScene *scene) {
	for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
		const aiMaterial *material = scene->mMaterials[i];
		loadMaterial(assetResource, material);
	}
}

std::shared_ptr<Json::Value> loadMetadataEntry(const aiMetadataEntry &entry) {
	switch (entry.mType) {
	case AI_BOOL:
		{
			const bool data = *static_cast<bool *>(entry.mData);
			return Json::boolean(data);
		}
	case AI_INT32:
		{
			const double data = *static_cast<int32_t *>(entry.mData);
			return Json::number(data);
		}
	case AI_UINT64:
		{
			const double data = *static_cast<uint64_t *>(entry.mData);
			return Json::number(data);
		}
	case AI_FLOAT:
		{
			const double data = *static_cast<float *>(entry.mData);
			return Json::number(data);
		}
	case AI_DOUBLE:
		{
			const double data = *static_cast<double *>(entry.mData);
			return Json::number(data);
		}
	case AI_AIVECTOR3D:
		{
			const aiVector3D *data = static_cast<aiVector3D *>(entry.mData);
			Json::Array array_data;
			array_data.push_back(Json::number(data->x));
			array_data.push_back(Json::number(data->y));
			array_data.push_back(Json::number(data->z));
			return Json::array(array_data);
		}
	case AI_AIMETADATA:
		{
			const aiMetadata *data = static_cast<aiMetadata *>(entry.mData);
			Json::Object object_data;
			for (unsigned int i = 0; i < data->mNumProperties; ++i) {
				const aiString &key(data->mKeys[i]);
				const aiMetadataEntry &value(data->mValues[i]);
				object_data[key.C_Str()] = loadMetadataEntry(value);
			}
			return Json::object(object_data);
		}
	case AI_INT64:
		{
			const double data = *static_cast<int64_t *>(entry.mData);
			return Json::number(data);
		}
	case AI_UINT32:
		{
			const double data = *static_cast<uint32_t *>(entry.mData);
			return Json::number(data);
		}
	default: break;
	}
	return Json::null();
}

void loadMetadata(const aiMetadata *metadata, Json::Object &dst) {
	if (metadata == nullptr)
		return;

	for (unsigned int i = 0; i < metadata->mNumProperties; ++i) {
		const aiString &key(metadata->mKeys[i]);
		const aiMetadataEntry &value(metadata->mValues[i]);

		dst[key.C_Str()] = loadMetadataEntry(value);
	}
}

void loadNode(AssetResource &assetResource, const aiNode *node, const std::shared_ptr<PivotNode> &sceneNode) {

	loadMetadata(node->mMetaData, sceneNode->getMetadatas());

	sceneNode->getTransform().setMatrix(convert(node->mTransformation));

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		std::shared_ptr<PivotNode> childNode = std::make_shared<PivotNode>(node->mChildren[i]->mName.C_Str());
		sceneNode->addChild(childNode);
		loadNode(assetResource, node->mChildren[i], childNode);
	}

	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		const unsigned int meshIndex = node->mMeshes[i];
		auto assetMesh = assetResource.getMeshes()[meshIndex];
		assert(meshIndex < assetResource.getMeshes().size());

		if (auto asSkinMesh = std::dynamic_pointer_cast<ISkinMeshInterface>(assetMesh)) {
			std::shared_ptr<SkinMeshNode> skinMeshNode = std::make_shared<SkinMeshNode>("mesh_" + std::to_string(i));
			skinMeshNode->setSkinMesh(asSkinMesh);
			sceneNode->addChild(skinMeshNode);
		} else if (auto asMesh = std::dynamic_pointer_cast<IMeshInterface>(assetMesh)) {
			std::shared_ptr<MeshNode> meshNode = std::make_shared<MeshNode>("mesh_" + std::to_string(i));
			meshNode->setMesh(asMesh);
			sceneNode->addChild(meshNode);
		}
	}
}

void AssetResource::loadFromAssimp() {
	Assimp::Importer &importer = getAssimpImporter();

	const aiScene *scene =
		importer.ReadFile(getFullPath(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace |
											 aiProcess_GenSmoothNormals);

	// Additional flags:
	// aiProcess_OptimizeMeshes
	// aiProcess_SplitLargeMeshes

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		throw Core::FileLoadingError(getFullPath(), importer.GetErrorString());
	}

	_rootNode = std::make_shared<PivotNode>(scene->mRootNode->mName.C_Str());

	std::cout << "scene ";
	std::cout << "validated: " << (scene->mFlags & AI_SCENE_FLAGS_VALIDATED) << " | ";
	std::cout << "incomplete: " << (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) << " | ";
	std::cout << "mNumMeshes " << scene->mNumMeshes << " | ";
	std::cout << "mNumMaterials " << scene->mNumMaterials << " | ";
	std::cout << "mNumAnimations " << scene->mNumAnimations << " | ";
	std::cout << "mNumTextures " << scene->mNumTextures << " | ";
	std::cout << "mNumLights " << scene->mNumLights << " | ";
	std::cout << "mNumCameras " << scene->mNumCameras << " | ";
	std::cout << "mNumSkeletons " << scene->mNumSkeletons << " | ";
	std::cout << "mMetaData " << scene->mMetaData << " | ";
	std::cout << "mName" << scene->mName.C_Str() << " | " << std::endl;

	loadMetadata(scene->mMetaData, _metadatas);

	/*
	 * TODO:
	 * load:
	 * - meshes
	 *   - vertices
	 *   - indices
	 *   - tangents
	 *   - bitangents
	 *   - uv
	 * - skin meshes
	 *   - bones
	 * - materials
	 * - animations
	 * - textures
	 * - lights
	 * - cameras
	 * - skeletons
	 * create:
	 * - nodes
	 * - pivot nodes
	 * - mesh nodes
	 * - skin mesh nodes
	 */


	loadMeshes(*this, scene);
	loadTextures(*this, scene);
	loadMaterials(*this, scene);

	loadNode(*this, scene->mRootNode, _rootNode);
}

} // namespace Stone::Scene
