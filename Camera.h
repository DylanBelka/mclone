#ifndef CAMERA_H
#define CAMERA_H

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

enum direction
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera
{
public:
	Camera();

	void move(direction d);
	void updateRot(double x, double y);

	glm::mat4 lookAt();

private:

	void updateCameraVectors();

	float speed = .5;

	double lastX;
	double lastY;
	bool firstMouseMovement = true;

	double yaw = -90.0;
	double pitch = 0.0;

	glm::vec3 pos;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;
};

#endif // CAMERA_H