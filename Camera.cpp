#include "Camera.h"

Camera::Camera()
{
	pos = glm::vec3(0.0, 0.0, 3.0);
	front = glm::vec3(0.0, 0.0, -1.0);
	up = glm::vec3(0.0, 1.0, 0.0);
	worldUp = up;

	updateCameraVectors();
}

void Camera::move(direction d)
{
	if (d == direction::FORWARD)
	{
		pos += speed * front;
	}
	else if (d == direction::BACKWARD)
	{
		pos -= speed * front;
	}
	else if (d == direction::LEFT)
	{
		pos -= glm::normalize(glm::cross(front, up)) * speed;
	}
	else if (d == direction::RIGHT)
	{
		pos += glm::normalize(glm::cross(front, up)) * speed;
	}
}

glm::mat4 Camera::lookAt()
{
	return glm::lookAt(pos, pos + front, up);
}

void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 newFront;
	newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	newFront.y = sin(glm::radians(pitch));
	newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(newFront);
	// Also re-calculate the Right and Up vector
	right = glm::normalize(glm::cross(front, worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	up = glm::normalize(glm::cross(right, front));
}

void Camera::updateRot(double x, double y)
{
	if (firstMouseMovement)
	{
		lastX = x;
		lastY = y;
		firstMouseMovement = false;
	}

	double xoffset = x - lastX;
	double yoffset = lastY - y;
	lastX = x;
	lastY = y;

	double sensitivity = .5;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	updateCameraVectors();
}