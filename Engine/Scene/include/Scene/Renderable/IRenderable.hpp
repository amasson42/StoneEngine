// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Renderer/RenderContext.hpp"
#include "Utils/SigSlot.hpp"

namespace Stone::Scene {

/**
 * @brief Interface for a renderer object
 *
 * The `IRendererObject` is an interface that provides methods that the renderer metadata must implement.
 */
class IRendererObject {
public:
	/**
	 * @brief Destructor
	 *
	 * Virtual destructor for proper cleanup of derived classes.
	 */
	virtual ~IRendererObject() = default;

	/**
	 * @brief Render the element
	 *
	 * This function is responsible for rendering the element in the scene.
	 *
	 * @param context The render context
	 */
	virtual void render(RenderContext &context) = 0;
};

class RendererObjectManager;

/**
 * @brief Interface for renderable elements
 *
 * This interface adds two properties to a class that are used by the renderer to manage its rendering behavior.
 */
class IRenderable {
public:
	Signal<IRenderable *, bool> onDirty; /**< Signal that is emitted when the render element is marked as dirty */

	IRenderable() : _rendererObject(nullptr), _dirty(true) {
	}

	IRenderable(const IRenderable &other) : _rendererObject(other._rendererObject), _dirty(true) {
	}

	virtual ~IRenderable() = default;

	/**
	 * @brief Check if the render element is dirty
	 */
	[[nodiscard]] bool isDirty() const {
		return _dirty;
	}

	/**
	 * @brief Mark the render element as dirty
	 *
	 * This function marks the render element as dirty, indicating that it needs to be updated.
	 */
	void markDirty() {
		_dirty = true;
		onDirty.broadcast(this, true);
	}

	/**
	 * @brief Get the renderer object as a specific type
	 *
	 * This function returns the associated renderer object as a specific type.
	 *
	 * @tparam T The type of the renderer object
	 * @return The renderer object as the specified type, or nullptr if the types do not match
	 */
	template <typename T>
	[[nodiscard]] std::shared_ptr<T> getRendererObject() const {
		assert(std::dynamic_pointer_cast<T>(_rendererObject) != nullptr);
		return std::static_pointer_cast<T>(_rendererObject);
	}

protected:
	friend class RendererObjectManager;

	/**
	 * @brief Set the renderer object
	 *
	 * This function sets the renderer object associated with the render element.
	 *
	 * @param rendererObject The renderer object
	 */
	void setRendererObject(const std::shared_ptr<IRendererObject> &rendererObject) {
		_rendererObject = rendererObject;
	}

	/**
	 * @brief Mark the render element as not dirty
	 *
	 * This function marks the render element as cleaned up.
	 */
	void markUndirty() {
		_dirty = false;
		onDirty.broadcast(this, false);
	}

	std::shared_ptr<IRendererObject> _rendererObject; /**< The renderer object associated with the render element */
	bool _dirty;									  /**< Flag indicating whether the render element is dirty or not */
};

} // namespace Stone::Scene
