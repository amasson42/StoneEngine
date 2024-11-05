#include "Scene.hpp"
#include "Utils/Json.hpp"

#include <cassert>
#include <iostream>

using namespace Stone::Scene;

void testNode() {
	// Create a root node
	std::shared_ptr<WorldNode> world = std::make_shared<WorldNode>();
	world->getMetadatas()["description"] = Stone::Json::string("The root node of the scene graph.");
	world->getMetadatas()["is_root"] = Stone::Json::boolean(true);
	world->getMetadatas()["version"] = Stone::Json::number(1.0);

	std::shared_ptr<Texture> blank_texture = std::make_shared<Texture>();
	std::shared_ptr<Material> blank_material = std::make_shared<Material>();
	blank_material->setTextureParameter("diffuse", blank_texture);

	std::shared_ptr<DynamicMesh> mesh = std::make_shared<DynamicMesh>();

	// Create child nodes
	std::shared_ptr<PivotNode> child1 = world->addChild<PivotNode>("child1");
	child1->getTransform().translate(glm::vec3(4.0f, 4.0f, 0.0f));
	std::shared_ptr<MeshNode> child2 = world->addChild<MeshNode>("child2");
	child2->setMesh(mesh);
	child2->setMaterial(blank_material);

	std::shared_ptr<InstancedMeshNode> child3 = world->addChild<InstancedMeshNode>("child3");
	std::shared_ptr<Node> child4 = child3->addChild<AmbientLightNode>("child4");
	std::shared_ptr<Node> child5 = child4->addChild<PointLightNode>();
	std::shared_ptr<Node> child6 = child4->addChild<DirectionalLightNode>();
	std::shared_ptr<Node> child7 = child6->addChild<SpotLightNode>("child7");

	std::shared_ptr<PerspectiveCameraNode> child1_1 = child1->addChild<PerspectiveCameraNode>("child1_1");
	world->setActiveCamera(child1_1);

	std::shared_ptr<Node> child1_2 = child1->addChild<OrthographicCameraNode>("child1_2");
	std::shared_ptr<Node> child1_3 = child1->addChild<Node>("child1_3");

	std::shared_ptr<PivotNode> human = world->addChild<PivotNode>("human");
	std::shared_ptr<SkinMeshNode> humanMesh = human->addChild<SkinMeshNode>("human_mesh");
	std::shared_ptr<SkeletonNode> skeleton = humanMesh->addChild<SkeletonNode>("skeleton");
	std::shared_ptr<PivotNode> spine = skeleton->addChild<PivotNode>("spine");
	std::shared_ptr<PivotNode> chest = spine->addChild<PivotNode>("chest");
	std::shared_ptr<PivotNode> elbow = chest->addChild<PivotNode>("elbow");
	std::shared_ptr<PivotNode> arm = elbow->addChild<PivotNode>("arm");
	std::shared_ptr<PivotNode> hand = arm->addChild<PivotNode>("hand");
	std::shared_ptr<PointLightNode> torch = hand->addChild<PointLightNode>("torch");

	chest->getTransform().translate(glm::vec3(0.0f, 1.0f, 0.0f));
	elbow->getTransform().translate(glm::vec3(1.0f, 0.0f, 0.0f));
	arm->getTransform().translate(glm::vec3(1.0f, 0.0f, 0.0f));
	arm->getTransform().rotate(glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(45.0f))));
	hand->getTransform().translate(glm::vec3(1.0f, 0.0f, 0.0f));
	skeleton->addBone(spine);
	skeleton->addBone(chest);
	skeleton->addBone(elbow);
	skeleton->addBone(arm);
	skeleton->addBone(hand);

	humanMesh->setSkeleton(skeleton);

	world->writeHierarchy(std::cout);

	glm::mat4 t = hand->getTransformMatrixRelativeToNode(elbow);

	std::cout << "textures: [" << blank_texture << "]" << std::endl;
	std::cout << "materials: [" << blank_material << "]" << std::endl;
	std::cout << "mesh: [" << mesh << "]" << std::endl;

	std::cout << "hand_to_elbow: " << std::to_string(t[3][0]) << "," << std::to_string(t[3][1]) << ","
			  << std::to_string(t[3][2]) << std::endl;

	// getc(stdin);
}

template <typename T, typename... Args>
std::shared_ptr<T> makeNode(const std::string &type, Args... args) {
	return std::dynamic_pointer_cast<T>(
		Stone::DynamicObjectFactory<Node, const std::string &>::getInstance().create(type, args...));
}

void testNodeDynamic() {
	auto world = makeNode<WorldNode>("WorldNode", "world");
	auto child1 = makeNode<PivotNode>("PivotNode", "child1");
	world->addChild(child1);
	auto child2 = makeNode<MeshNode>("MeshNode", "child2");
	world->addChild(child2);
	auto child3 = makeNode<InstancedMeshNode>("InstancedMeshNode", "child3");
	world->addChild(child3);
	auto child4 = makeNode<AmbientLightNode>("AmbientLightNode", "child4");
	world->addChild(child4);
	auto child5 = makeNode<PointLightNode>("PointLightNode", "child5");
	child4->addChild(child5);
	auto child6 = makeNode<DirectionalLightNode>("DirectionalLightNode", "child6");
	child4->addChild(child6);
	auto child7 = makeNode<SpotLightNode>("SpotLightNode", "child7");
	child6->addChild(child7);
	auto child1_1 = makeNode<PerspectiveCameraNode>("PerspectiveCameraNode", "child1_1");
	child1->addChild(child1_1);
	auto child1_2 = makeNode<OrthographicCameraNode>("OrthographicCameraNode", "child1_2");
	child1->addChild(child1_2);
	auto child1_3 = makeNode<Node>("Node", "child1_3");
	child1->addChild(child1_3);
	auto human = makeNode<PivotNode>("PivotNode", "human");
	world->addChild(human);
	auto humanMesh = makeNode<SkinMeshNode>("SkinMeshNode", "human_mesh");
	human->addChild(humanMesh);
	auto skeleton = makeNode<SkeletonNode>("SkeletonNode", "skeleton");
	humanMesh->addChild(skeleton);
	auto spine = makeNode<PivotNode>("PivotNode", "spine");
	skeleton->addChild(spine);
	auto chest = makeNode<PivotNode>("PivotNode", "chest");
	spine->addChild(chest);
	auto elbow = makeNode<PivotNode>("PivotNode", "elbow");
	chest->addChild(elbow);
	auto arm = makeNode<PivotNode>("PivotNode", "arm");
	elbow->addChild(arm);
	auto hand = makeNode<PivotNode>("PivotNode", "hand");
	arm->addChild(hand);
	auto torch = makeNode<PointLightNode>("PointLightNode", "torch");
	hand->addChild(torch);

	world->writeHierarchy(std::cout, true);
}
