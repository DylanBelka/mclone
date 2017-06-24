#include "Camera.h"

Camera::Camera()
{
	pos = glm::vec3(0.0, 0.0, 3.0);
	front = glm::vec3(0.0, 0.0, -1.0);
	up = glm::vec3(0.0, 1.0, 0.0);
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

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	this->front = glm::normalize(front);
	up = glm::normalize(glm::cross(glm::normalize(glm::cross(this->front, up)), this->front));
}