#ifndef MATH_H
#define MATH_H

#include <glm.hpp>

namespace math
{
float map(float x, float xmin, float xmax, float dmin, float dmax);

// distance squared between point and rectangle
float sqrdDistPtToRect(glm::vec2 ptCoords, glm::vec2 rectCoords, float width, float height);
};

#endif // MATH_H