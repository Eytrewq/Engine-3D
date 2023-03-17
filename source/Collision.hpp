#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "Math.hpp"
#include "plane.hpp"
#include "Object.hpp"

class collision
{
public:
    float timerDiff = 0.005f;

    std::vector<plane*> planesCollided;
    std::vector<object*> objectsCollided;

    size_t planesCollidedSize;
    size_t objectsCollidedSize;

    collision() {};

    void GetVectorSizes();
    vec3 GetObjectNormal(unsigned int const& i);
    vec3 GetObjectSpeedAtTimer(unsigned int const& i);
    vec3 GetNewDirection(vec3 const& speedAtTimer, vec3 const& normal);
    void SetNewRotationAxisOrigin(unsigned int const& i);
    void GetNewRotationAxis(unsigned int const& i, vec3 const& normal);
    void ResetTimer(unsigned int const& i);
    void Separating(unsigned int const& i, vec3 const& normal);

    void Caculate1();
    void Caculate2();
};

#endif COLLISION_HPP