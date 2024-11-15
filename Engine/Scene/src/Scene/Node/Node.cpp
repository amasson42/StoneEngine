// Copyright 2024 Stone-Engine

#include "Scene/Node/Node.hpp"

#include <algorithm>
#include <cassert>
#include <list>

namespace Stone::Scene {

STONE_NODE_IMPLEMENTATION(Node)

Node::Node(const std::string &name) : Object(), _name(name), _children(), _parent(), _world() {
	// LOG: Warning: Node name cannot contain '/'
	assert(name.find('/') == std::string::npos);
}

std::ostream &Node::writeToStream(std::ostream &stream, bool closing_bracer) const {
	Object::writeToStream(stream, false);
	stream << ",name:\"" << _name << "\"";
	stream << ",metadatas:" << Json::Value(_metadatas).serialize();
	if (closing_bracer)
		stream << "}";
	return stream;
}

void Node::update(float deltaTime) {
	for (auto &child : _children) {
		child->update(deltaTime);
	}
}

// TODO: Benchmark using `RenderContext &context` as a reference or as a pointer and dynamic cast
void Node::render(RenderContext &context) {
	for (auto &child : _children) {
		child->render(context);
	}
}

void Node::setName(const std::string &name) {
	_name = name;
}

const std::string &Node::getName() const {
	return _name;
}

std::string Node::getGlobalName() const {
	if (auto parent = getParent()) {
		return parent->getGlobalName() + "/" + getName();
	}
	return "/" + getName();
}

void Node::addChild(const std::shared_ptr<Node> &child) {
	// LOG: Error: Cannot add a parent as a child
	assert(!child->isAncestorOf(std::static_pointer_cast<Node>(shared_from_this())));
	child->_parent = std::static_pointer_cast<Node>(shared_from_this());
	child->_world = _world;
	_children.push_back(child);
}

void Node::removeChild(const std::shared_ptr<Node> &child) {
	auto it = std::find(_children.begin(), _children.end(), child);
	if (it != _children.end()) {
		(*it)->_parent.reset();
		(*it)->_world.reset();
		_children.erase(it);
	}
}

void Node::removeFromParent() {
	if (auto parent = getParent()) {
		parent->removeChild(std::static_pointer_cast<Node>(shared_from_this()));
	}
}

std::shared_ptr<Node> Node::getParent() const {
	return _parent.lock();
}

bool Node::hasParent() const {
	return !_parent.expired();
}

bool Node::isAncestorOf(const std::shared_ptr<Node> &node) const {
	if (auto parent = node->getParent()) {
		if (parent.get() == this) {
			return true;
		}
		return isAncestorOf(parent);
	}
	return false;
}

bool Node::isDescendantOf(const std::shared_ptr<Node> &node) const {
	if (auto parent = getParent()) {
		if (parent == node) {
			return true;
		}
		return parent->isDescendantOf(node);
	}
	return false;
}

const std::list<std::shared_ptr<Node>> &Node::getChildren() const {
	return _children;
}

std::shared_ptr<Node> Node::getChild(const std::string &name) const {
	for (auto &child : _children) {
		if (child->getName() == name) {
			return child;
		}
	}
	return nullptr;
}

std::shared_ptr<Node> Node::getChildByPath(const std::string &path) const {
	if (path.size() <= 1) {
		return getChild(path);
	}

	if (path[0] == '*' && path[1] == '/') {
		std::string childName = path.substr(2);
		for (auto &child : _children) {
			if (child->getName() == childName) {
				return child;
			}
			std::shared_ptr<Node> grandChild = child->getChildByPath(path);
			if (grandChild) {
				return grandChild;
			}
		}
		return nullptr;
	}

	auto firstSlash = path.find('/');

	if (firstSlash != std::string::npos) {
		std::string childName = path.substr(0, firstSlash);
		auto childNode = getChild(childName);
		if (!childNode) {
			return nullptr;
		}
		std::string subpath = path.substr(firstSlash + 1);
		return childNode->getChildByPath(subpath);
	}

	return getChild(path);
}

std::shared_ptr<WorldNode> Node::getWorld() const {
	return _world.lock();
}

void Node::transformRelativeMatrix(glm::mat4 &relative) const {
	(void)relative;
}

glm::mat4 Node::getWorldTransformMatrix() const {
	return getTransformMatrixRelativeToNode(nullptr);
}

glm::mat4 Node::getTransformMatrixRelativeToNode(const std::shared_ptr<Node> &otherNode) const {
	glm::mat4 transform(1);
	if (this == otherNode.get()) {
		return transform;
	}

	transformRelativeMatrix(transform);

	std::shared_ptr<Node> node = getParent();
	while (true) {
		if (node == nullptr) {
			if (!otherNode)
				break;

			return glm::inverse(otherNode->getWorldTransformMatrix()) * transform;
		}
		if (node == otherNode) {
			break;
		}
		node->transformRelativeMatrix(transform);
		node = node->getParent();
	}
	return transform;
}

void Node::traverseTopDown(const std::function<void(const std::shared_ptr<Node> &)> &func) {
	std::function<void(const std::shared_ptr<Node> &)> traverseRecursive;
	traverseRecursive = [&](const std::shared_ptr<Node> &node) {
		func(node);
		for (auto &child : node->getChildren()) {
			traverseRecursive(child);
		}
	};

	traverseRecursive(std::static_pointer_cast<Node>(shared_from_this()));
}

void Node::traverseBottomUp(const std::function<void(const std::shared_ptr<Node> &)> &func) {
	std::function<void(const std::shared_ptr<Node> &)> traverseRecursive;
	traverseRecursive = [&](const std::shared_ptr<Node> &node) {
		for (auto &child : node->getChildren()) {
			traverseRecursive(child);
		}
		func(node);
	};

	traverseRecursive(std::static_pointer_cast<Node>(shared_from_this()));
}

void Node::traverseTopDownBreakable(const std::function<bool(const std::shared_ptr<Node> &)> &func) {
	std::function<bool(const std::shared_ptr<Node> &)> traverseRecursive;
	traverseRecursive = [&](const std::shared_ptr<Node> &node) {
		if (!func(node))
			return false;
		for (auto &child : node->getChildren()) {
			if (!traverseRecursive(child))
				return false;
		}
		return true;
	};

	traverseRecursive(std::static_pointer_cast<Node>(shared_from_this()));
}

void Node::traverseBottomUpBreakable(const std::function<bool(const std::shared_ptr<Node> &)> &func) {
	std::function<bool(const std::shared_ptr<Node> &)> traverseRecursive;
	traverseRecursive = [&](const std::shared_ptr<Node> &node) {
		for (auto &child : node->getChildren()) {
			if (!traverseRecursive(child))
				return false;
		}
		if (!func(node))
			return false;
		return true;
	};

	traverseRecursive(std::static_pointer_cast<Node>(shared_from_this()));
}

void Node::writeHierarchy(std::ostream &stream, bool colored, const std::string &linePrefix,
						  const std::string &firstPrefix, const std::string &lastPrefix) const {
	stream << linePrefix << firstPrefix;
	if (colored) {
		stream << TERM_COLOR_BOLD TERM_COLOR_WHITE << _name << TERM_COLOR_RESET;
		stream << " [" << _termClassColor() << getClassName() << TERM_COLOR_RESET << "] ";
	} else {
		stream << _name << " [" << getClassName() << "] ";
	}
	stream << *this << std::endl;
	const auto last = _children.back();
	for (auto &child : _children) {
		if (child == last) {
			child->writeHierarchy(stream, colored, linePrefix + lastPrefix, "└─", "  ");
		} else {
			child->writeHierarchy(stream, colored, linePrefix + lastPrefix, "├─", "│ ");
		}
	}
}

const Json::Object &Node::getMetadatas() const {
	return _metadatas;
}

Json::Object &Node::getMetadatas() {
	return _metadatas;
}

const char *Node::_termClassColor() const {
	return TERM_COLOR_BOLD TERM_COLOR_GRAY;
}

} // namespace Stone::Scene
