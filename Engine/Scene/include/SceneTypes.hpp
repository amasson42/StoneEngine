// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Renderer/RenderContext.hpp"
#include "Scene/Transform.hpp"
#include "Scene/Vertex.hpp"

namespace Stone::Scene {

/** Node */
class Node;
class PivotNode;
class CameraNode;
class LightNode;
class AmbientLightNode;
class PointLightNode;
class CastingLightNode;
class DirectionalLightNode;
class SpotLightNode;
class MeshNode;
class InstancedMeshNode;
class SkeletonNode;
class SkinMeshNode;
class WorldNode;

/** Renderable */
class Texture;
class Material;
class AShader;
class FragmentShader;

class IMeshObject;
class IMeshInterface;
class DynamicMesh;
class StaticMesh;
class Skeleton;
class ISkinMeshInterface;
class DynamicSkinMesh;
class StaticSkinMesh;
class WireframeShape;

} // namespace Stone::Scene
