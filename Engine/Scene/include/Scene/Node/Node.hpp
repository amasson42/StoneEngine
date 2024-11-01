// Copyright 2024 Stone-Engine

#pragma once

#include "Core/Object.hpp"
#include "Logging/TermColor.hpp"
#include "Scene/Node/NodeMacros.hpp"
#include "Scene/Renderer/RenderContext.hpp"
#include "Utils/Json.hpp"

#include <functional>
#include <vector>

namespace Stone::Scene {

class WorldNode;

/**
 * @class Node
 * @brief Represents a node in the scene graph.
 *
 * The Node class is the base class for all nodes in the scene graph. It provides basic functionality for managing
 * the node hierarchy, updating and rendering nodes, and accessing node properties.
 */
class Node : public Core::Object {
	STONE_OBJECT(Node);

public:
	explicit Node(const std::string &name = "node");
	Node(const Node &other) = default;

	~Node() override = default;

	/**
	 * @brief Writes the node to the output stream.
	 *
	 * @param stream The output stream to write to.
	 * @param closing_bracer Whether to write the closing brace '}' after writing the node.
	 * @return The output stream.
	 */
	std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const override;

	const static std::string nodeClassName;
	virtual const char *getNodeClassName() const;

	/**
	 * @brief Updates the node.
	 *
	 * This function is called every frame to update the node's state.
	 *
	 * @param deltaTime The time elapsed since the last frame, in seconds.
	 */
	virtual void update(float deltaTime);

	/**
	 * @brief Renders the node.
	 *
	 * This function is called every frame to render the node.
	 *
	 * @param context The rendering context.
	 */
	virtual void render(RenderContext &context);

	/**
	 * @brief Sets the name of the node.
	 */
	void setName(const std::string &name);

	/**
	 * @brief Gets the name of the node.
	 */
	[[nodiscard]] const std::string &getName() const;

	/**
	 * @brief Gets the global name of the node.
	 *
	 * The global name is the concatenation of the names of all the ancestor nodes, separated by '/'.
	 *
	 * @return The global name of the node.
	 */
	[[nodiscard]] std::string getGlobalName() const;

	/**
	 * @brief Adds a child node to this node.
	 *
	 * @param child The child node to add.
	 */
	void addChild(const std::shared_ptr<Node> &child);

	/**
	 * @brief Adds a child node of type T to this node.
	 *
	 * @tparam T The type of the child node.
	 * @param name The name of the child node.
	 * @return The shared pointer to the child node.
	 */
	template <typename T, typename... Args>
	std::shared_ptr<T> addChild(Args &&...arg) {
		auto child = std::make_shared<T>(arg...);
		addChild(child);
		return child;
	}

	/**
	 * @brief Removes a child node from this node.
	 *
	 * @param child The child node to remove.
	 */
	void removeChild(const std::shared_ptr<Node> &child);

	/**
	 * @brief Removes this node from its parent.
	 */
	void removeFromParent();

	/**
	 * @brief Gets the parent node of this node.
	 */
	[[nodiscard]] std::shared_ptr<Node> getParent() const;

	/**
	 * @brief Checks if this node has a parent.
	 */
	[[nodiscard]] bool hasParent() const;

	/**
	 * @brief Checks if this node is an ancestor of the given node.
	 *
	 * @param node The node to check.
	 * @return True if this node is an ancestor of the given node, false otherwise.
	 */
	[[nodiscard]] bool isAncestorOf(const std::shared_ptr<Node> &node) const;

	/**
	 * @brief Checks if this node is a descendant of the given node.
	 *
	 * @param node The node to check.
	 * @return True if this node is a descendant of the given node, false otherwise.
	 */
	[[nodiscard]] bool isDescendantOf(const std::shared_ptr<Node> &node) const;

	/**
	 * @brief Gets the children nodes of this node.
	 */
	const std::vector<std::shared_ptr<Node>> &getChildren() const;

	/**
	 * @brief Gets the child node with the given name.
	 *
	 * @param name The name of the child node.
	 * @return The child node with the given name, or nullptr if not found.
	 */
	[[nodiscard]] std::shared_ptr<Node> getChild(const std::string &name) const;

	/**
	 * @brief Get a child node with a given relative path.
	 *
	 * @param path The path to the child node.
	 *
	 * Path starting with '*\/' will look for any descendant with the given name
	 *
	 * @example getChildByPath("child1/child2/child3")
	 * @example getChildByPath("*\/child3")
	 *
	 * @return The child node with the given path, or nullptr if not found.
	 */
	[[nodiscard]] std::shared_ptr<Node> getChildByPath(const std::string &path) const;

	/**
	 * @brief Gets the child node of type T with the given name.
	 *
	 * @tparam T The type of the child node.
	 * @param name The name of the child node.
	 * @return The child node of type T with the given name, or nullptr if not found.
	 */
	template <typename T>
	[[nodiscard]] std::shared_ptr<T> getChild(const std::string &name) const {
		return std::dynamic_pointer_cast<T>(getChild(name));
	}

	/**
	 * @brief Gets the child node of type T with the given path.
	 *
	 * The path can be a relative path or an absolute path. If the path starts with '*\/', it will look for any
	 * descendant with the given name.
	 *
	 * @tparam T The type of the child node.
	 * @param path The path to the child node.
	 * @return The child node of type T with the given path, or nullptr if not found.
	 */
	template <typename T>
	[[nodiscard]] std::shared_ptr<T> getChildByPath(const std::string &path) const {
		return std::dynamic_pointer_cast<T>(getChildByPath(path));
	}

	/**
	 * @brief Gets the child node of type T.
	 *
	 * This function searches for the first child node of type T in the node hierarchy.
	 *
	 * @tparam T The type of the child node.
	 * @return The child node of type T, or nullptr if not found.
	 */
	template <typename T>
	[[nodiscard]] std::shared_ptr<T> getChildByClass() const {
		for (auto &child : _children) {
			std::shared_ptr<T> node = std::dynamic_pointer_cast<T>(child);
			if (node)
				return node;
		}
		return nullptr;
	}

	/**
	 * @brief Gets the world node that this node belongs to.
	 */
	[[nodiscard]] std::shared_ptr<WorldNode> getWorld() const;

	/**
	 * @brief Transforms the relative matrix of this node.
	 *
	 * @param relative The relative matrix to transform.
	 */
	virtual void transformRelativeMatrix(glm::mat4 &relative) const;

	/**
	 * @brief Gets the world transform matrix of this node.
	 */
	[[nodiscard]] glm::mat4 getWorldTransformMatrix() const;

	/**
	 * @brief Gets the transform matrix of this node relative to another node.
	 *
	 * @param otherNode The other node.
	 * @return The transform matrix of this node relative to the other node.
	 */
	[[nodiscard]] glm::mat4 getTransformMatrixRelativeToNode(const std::shared_ptr<Node> &otherNode) const;

	/**
	 * @brief Traverses the node hierarchy in a top-down order and applies the given function to each node.
	 *
	 * Top-down traversal means that the function is applied to the parent node before its children.
	 *
	 * @param func The function to apply to each node.
	 */
	void traverseTopDown(const std::function<void(const std::shared_ptr<Node> &)> &func);

	/**
	 * @brief Traverses the node hierarchy in a bottom-up order and applies the given function to each node.
	 *
	 * Bottom-up traversal means that the function is applied to the children nodes before their parent.
	 *
	 * @param func The function to apply to each node.
	 */
	void traverseBottomUp(const std::function<void(const std::shared_ptr<Node> &)> &func);

	/**
	 * @brief Traverses the node hierarchy in a top-down order and applies the given function to each node.
	 *
	 * The traversal can be interrupted by returning false from the function.
	 *
	 * @param func The function to apply to each node.
	 */
	void traverseTopDownBreakable(const std::function<bool(const std::shared_ptr<Node> &)> &func);

	/**
	 * @brief Traverses the node hierarchy in a bottom-up order and applies the given function to each node.
	 *
	 * The traversal can be interrupted by returning false from the function.
	 *
	 * @param func The function to apply to each node.
	 */
	void traverseBottomUpBreakable(const std::function<bool(const std::shared_ptr<Node> &)> &func);

	/**
	 * @brief Writes the hierarchy of the node to the output stream.
	 *
	 * @param stream The output stream to write to.
	 * @param colored Whether to use colored output.
	 * @param linePrefix The prefix to add to each line.
	 * @param firstPrefix The prefix to add to the first line.
	 * @param lastPrefix The prefix to add to the last line.
	 */
	void writeHierarchy(std::ostream &stream, bool colored = true, const std::string &linePrefix = "",
						const std::string &firstPrefix = "", const std::string &lastPrefix = "") const;

	const std::unordered_map<std::string, Json::Value> &getMetadatas() const {
		return _metadata;
	}

	std::unordered_map<std::string, Json::Value> &getMetadatas() {
		return _metadata;
	};

protected:
	std::string _name;							  /**< The name of the node. */
	std::vector<std::shared_ptr<Node>> _children; /**< The children nodes of this node. */
	std::weak_ptr<Node> _parent;				  /**< The parent node of this node. */
	std::weak_ptr<WorldNode> _world;			  /**< The world node that this node belongs to. */

	std::unordered_map<std::string, Json::Value> _metadata; /**< Metadata of the node */

	/**
	 * @brief Gets the class color for terminal output.
	 */
	[[nodiscard]] virtual const char *_termClassColor() const;
};

} // namespace Stone::Scene
