#include "math.h"

namespace math
{
float map(float x, float xmin, float xmax, float dmin, float dmax)
{
	return (x - xmin) / (xmax - xmin) * (dmax - dmin) + dmin;
}

float sqrdDistPtToRect(glm::vec2 ptCoords, glm::vec2 rectCoords, float width, float height)
{
	float dx = glm::max(glm::abs(ptCoords.x - rectCoords.x) - width / 2.0, 0.0);
	float dy = glm::max(glm::abs(ptCoords.y - rectCoords.y) - height / 2.0, 0.0);
	return dx * dx + dy * dy;
}
}