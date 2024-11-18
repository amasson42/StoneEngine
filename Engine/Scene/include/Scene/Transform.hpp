// Copyright 2024 Stone-Engine

#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/io.hpp>
#include <iostream>

namespace Stone::Json {
struct Value;
}

namespace Stone::Scene {

/**
 * @brief 2D transformation class representing position, rotation, and scale.
 */
struct Transform2D {
	Transform2D();
	Transform2D(const Transform2D &other) = default;

	Transform2D &operator=(const Transform2D &other) = default;

	/**
	 * @brief Set the position of the transform.
	 * @param position The new position.
	 */
	void setPosition(const glm::vec2 &position);

	/**
	 * @brief Set the rotation of the transform.
	 * @param rotation The new rotation in radians.
	 */
	void setRotation(float rotation);

	/**
	 * @brief Set the scale of the transform.
	 * @param scale The new scale.
	 */
	void setScale(const glm::vec2 &scale);
	void setMatrix(const glm::mat3 &matrix);

	/**
	 * @brief Get the position of the transform.
	 * @return The position.
	 */
	[[nodiscard]] const glm::vec2 &getPosition() const;

	/**
	 * @brief Get the rotation of the transform.
	 * @return The rotation in radians.
	 */
	[[nodiscard]] float getRotation() const;

	/**
	 * @brief Get the scale of the transform.
	 * @return The scale.
	 */
	[[nodiscard]] const glm::vec2 &getScale() const;

	/**
	 * @brief Translate the transform by a given vector.
	 * @param translation The translation vector.
	 */
	void translate(const glm::vec2 &translation);

	/**
	 * @brief Rotate the transform by a given angle.
	 * @param rotation The rotation angle.
	 */
	void rotate(float rotation);

	/**
	 * @brief Scale the transform by a given vector.
	 * @param scale The scale vector.
	 */
	void scale(const glm::vec2 &scale);

	/**
	 * @brief Update the transform matrix if needed and return it.
	 * @return The transform matrix.
	 */
	const glm::mat3 &getTransformMatrix();

	/**
	 * @brief Return the transform matrix if cached or calculate it.
	 * @return The transform matrix.
	 */
	[[nodiscard]] glm::mat3 getTransformMatrix() const;

	/**
	 * @brief Convert into json
	 * @return The Json value
	 */
	Json::Value toJson() const;

protected:
	glm::vec2 _position; /**< The position of the transform. */
	float _rotation;	 /**< The rotation of the transform. */
	glm::vec2 _scale;	 /**< The scale of the transform. */

	glm::mat3 _transformMatrix; /**< The cached transform matrix. */
	bool _transformMatrixDirty; /**< Flag indicating if the transform matrix needs to be recalculated. */

	/**
	 * @brief Calculate the transform matrix and store it in the reference.
	 * @param m The reference to the output transform matrix.
	 */
	void calculateTransformMatrix(glm::mat3 &m) const;
};

/**
 * @brief 3D transformation class representing position, rotation, and scale.
 */
struct Transform3D {
	Transform3D();
	Transform3D(const Transform3D &other) = default;

	Transform3D &operator=(const Transform3D &other) = default;

	/**
	 * @brief Set the position of the transform.
	 * @param position The new position.
	 */
	void setPosition(const glm::vec3 &position);

	/**
	 * @brief Set the rotation of the transform.
	 * @param rotation The new rotation.
	 */
	void setRotation(const glm::quat &rotation);

	/**
	 * @brief Set the rotation of the transform using Euler angles.
	 * @param eulerAngles The new rotation in Euler angles in radians.
	 */
	void setEulerAngles(const glm::vec3 &eulerAngles);

	/**
	 * @brief Set the scale of the transform.
	 * @param scale The new scale.
	 */
	void setScale(const glm::vec3 &scale);
	void setMatrix(const glm::mat4 &matrix);

	/**
	 * @brief Get the position of the transform.
	 * @return The position.
	 */
	[[nodiscard]] const glm::vec3 &getPosition() const;

	/**
	 * @brief Get the quaternion rotation of the transform.
	 * @return The rotation quaternion.
	 */
	[[nodiscard]] const glm::quat &getRotation() const;

	/**
	 * @brief Get the rotation of the transform in Euler angles.
	 * @return The rotation Euler angles in radians.
	 */
	[[nodiscard]] glm::vec3 getEulerAngles() const;

	/**
	 * @brief Get the scale of the transform.
	 * @return The scale.
	 */
	[[nodiscard]] const glm::vec3 &getScale() const;

	/**
	 * @brief Translate the transform by a given vector.
	 * @param translation The translation vector.
	 */
	void translate(const glm::vec3 &translation);

	/**
	 * @brief Rotate the transform by a given quaternion.
	 * @param rotation The rotation quaternion.
	 */
	void rotate(const glm::quat &rotation);

	/**
	 * @brief Rotate the transform by a given angle around an axis.
	 * @param angle The rotation angle.
	 * @param axis The rotation axis.
	 */
	void rotate(float angle, const glm::vec3 &axis);

	/**
	 * @brief Rotate the transform by a given vector of Euler angles.
	 * @param eulerAngles The rotation in Euler angles in radians.
	 */
	void rotate(const glm::vec3 &eulerAngles);

	/**
	 * @brief Scale the transform by a given vector.
	 * @param scale The scale vector.
	 */
	void scale(const glm::vec3 &scale);

	/**
	 * @brief Get the transform matrix. Internally update the cached matrix if needed.
	 * @return The transform matrix.
	 */
	const glm::mat4 &getTransformMatrix();

	/**
	 * @brief Get the transform matrix.
	 * @return The transform matrix.
	 */
	[[nodiscard]] glm::mat4 getTransformMatrix() const;

	/**
	 * @brief Convert into json
	 * @return The Json value
	 */
	Json::Value toJson() const;

protected:
	glm::vec3 _position; /**< The position of the transform. */
	glm::quat _rotation; /**< The rotation of the transform. */
	glm::vec3 _scale;	 /**< The scale of the transform. */

	glm::mat4 _transformMatrix; /**< The cached transform matrix. */
	bool _transformMatrixDirty; /**< Flag indicating if the transform matrix needs to be recalculated. */

	/**
	 * @brief Calculate the transform matrix and store it in the reference.
	 * @param m The reference to the output transform matrix.
	 */
	void calculateTransformMatrix(glm::mat4 &m) const;
};

} // namespace Stone::Scene

std::ostream &operator<<(std::ostream &stream, const Stone::Scene::Transform2D &transform);
std::ostream &operator<<(std::ostream &stream, const Stone::Scene::Transform3D &transform);
