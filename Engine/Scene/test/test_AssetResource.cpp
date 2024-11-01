#include "Core/Assets/Bundle.hpp"
#include "Scene/Assets/AssetResource.hpp"

#include <gtest/gtest.h>

using namespace Stone::Scene;

TEST(AssetResource, FindBundle) {
	auto bundle = std::make_shared<Stone::Core::Assets::Bundle>("test/");

	auto resource = bundle->loadResource<AssetResource>("test.stone");

	// TODO: Implement tests that parse fbx, dae, obj, and other formats
	// and check if the meshes, textures, materials, and other assets are loaded correctly
	// check if metadatas are loaded correctly
}
