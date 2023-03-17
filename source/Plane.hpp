#ifndef PLANE_HPP
#define PLANE_HPP

#include "Math.hpp"

class plane
{
public:
    vec3 vertices[4] = {
        { -0.5f, 0.0f, -0.5f },
        { -0.5f, 0.0f, 0.5f },
        { 0.5f, 0.0f, 0.5f },
        { 0.5f, 0.0f, -0.5f }
    };
    vec3 position, dimensions, color, rotationAxis;
    float degree = 0.0f;
    bool isCollided = false;
    vec3 axis1, axis2, normal;
    float axis1dot, axis2dot;

    plane(vec3 const& position, vec3 const& dimensions, vec3 const& color, vec3 const& rotationAxis, float const& degree);
};

#endif
