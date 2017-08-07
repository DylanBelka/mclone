#ifndef PLAYER_H
#define PLAYER_H

#include <glm.hpp>

#include "Camera.h"
#include "World.h"

class Player
{
public:
	Player();

	void move(direction dir) { camera.move(dir); }
	void moveTo(const glm::vec3& pos) { camera.pos = pos; }

	const glm::mat4& lookAt() { return camera.lookAt(); }
	const glm::vec3& getPos() { return camera.getPos(); }
	const glm::vec3& getFront() { return camera.getFront(); }

	void updateRot(double x, double y) { camera.updateRot(x, y); }

private:
	Camera camera;
};

#endif // PLAYER_H