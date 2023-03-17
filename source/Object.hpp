#ifndef CUBOID_HPP
#define CUBOID_HPP

#include <iostream>
#include <vector>

#include "Math.hpp"
#include "Plane.hpp"

enum {
    CUBE = 1,
    SPHERE,
};

class object
{
public:
    std::vector<vec3> verticesOrigin = {
        { -0.5f, 0.5f, 0.5f },
        { -0.5f, -0.5f, 0.5f },
        { -0.5f, 0.5f, -0.5f },
        { -0.5f, -0.5f, -0.5f },
        { 0.5f, 0.5f, 0.5f },
        { 0.5f, -0.5f, 0.5f },
        { 0.5f, 0.5f, -0.5f },
        { 0.5f, -0.5f, -0.5f }
    };
    std::vector<vec3> vertices = { { }, { }, { }, { }, { }, { }, { }, { } };
    vec3 positionOrigin, position, positionForce, positionForcePrev;
    vec3 rotationAxisOrigin, rotationAxis;
    vec3 color;
    float degreeOrigin = 0.0f, degree = 0.0f, rotationForce = 0.0f;
    float timer = 0.0f, mass = 0.0f, radius = 0.0f, size = 1.0f;
    float speed = 0.0;
    int type = 0;

    vec3 positionPrev;
    float timerPrev = 0.0f;

    int isCollided = false;

    std::vector<vec3> pointToShow; // TEST

    object(vec3 const& positionOrigin, float const& size, vec3 const& color, vec3 const& rotationAxisOrigin, float const& degreeOrigin, vec3 const& positionForce, float const& mass = 1.0f);
    object(vec3 const& positionOrigin, float const& size, vec3 const& color, vec3 const& positionForce, float const& mass = 1.0f);

    void UpdatePositionByTimer();

private:
    
};

#endif
