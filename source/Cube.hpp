#ifndef CUBOID_HPP
#define CUBOID_HPP

#include <iostream>
#include <vector>

#include "Math.hpp"
#include "Plane.hpp"
#include "Sphere.hpp"

enum {
    IS_COLLIDED_FALSE,
    IS_COLLIDED_TRUE,
    IS_COLLIDED_OFF
};

class cube
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
    vec3 positionOrigin, position, positionForce;
    vec3 rotationAxisOrigin, rotationAxis;
    vec3 color;
    float degreeOrigin = 0.0f, degree = 0.0f, rotationForce = 0.0f;
    float timer = 0.0f, mass = 0.0f, radius = 0.0f, dimensions = 1.0f;
    float speed = 0.0;

    vec3 positionPrev;
    float timerPrev = 0.0f;

    int isCollided = false;

    std::vector<vec3> pointToShow; // TEST

    cube(vec3 const& positionOrigin, float const& dimensions, vec3 const& color, vec3 const& rotationAxisOrigin, float const& degreeOrigin, vec3 const& positionForce);
    void UpdatePositionByTimer();

    vec3 getMin()
    {
        float minX = vertices[0].x;
        int i = 0;
        while (i < 7) {
            if (vertices[i + 1].x < vertices[i].x)
                minX = vertices[i + 1].x;
            i++;
        }

        float minY = vertices[0].y;
        i = 0;
        while (i < 7) {
            if (vertices[i + 1].y < vertices[i].y)
                minY = vertices[i + 1].y;
            i++;
        }

        float minZ = vertices[0].z;
        i = 0;
        while (i < 7) {
            if (vertices[i + 1].z < vertices[i].z)
                minZ = vertices[i + 1].z;
            i++;
        }

        return (vec3{ minX, minY, minZ });
    }

    vec3 getMax()
    {
        float maxX = vertices[0].x;
        int i = 0;
        while (i < 7) {
            if (vertices[i + 1].x > vertices[i].x)
                maxX = vertices[i + 1].x;
            i++;
        }

        float maxY = vertices[0].y;
        i = 0;
        while (i < 7) {
            if (vertices[i + 1].y > vertices[i].y)
                maxY = vertices[i + 1].y;
            i++;
        }

        float maxZ = vertices[0].z;
        i = 0;
        while (i < 7) {
            if (vertices[i + 1].z > vertices[i].z)
                maxZ = vertices[i + 1].z;
            i++;
        }

        return (vec3{ maxX, maxY, maxZ });
    }

private:

};

#endif
