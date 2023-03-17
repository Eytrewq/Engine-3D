#include "Object.hpp"

object::object(vec3 const& positionOrigin, float const& size, vec3 const& color, vec3 const& rotationAxisOrigin, float const& degreeOrigin, vec3 const& positionForce, float const& mass) {
    this->positionOrigin = positionOrigin;
    this->size = size;
    this->color = color;
    this->rotationAxisOrigin = rotationAxisOrigin;
    this->degreeOrigin = RADIAN(degreeOrigin);
    this->positionForce = positionForce;

    this->mass = (this->size * this->size * this->size) * mass;
    this->radius = sqrt(this->size * this->size * 3.0f) * 0.5f;
    this->position = this->positionOrigin;
    for (auto i = 0; i < 8; i++) {
        verticesOrigin[i].x = verticesOrigin[i].x * size;
        verticesOrigin[i].y = verticesOrigin[i].y * size;
        verticesOrigin[i].z = verticesOrigin[i].z * size;
    }
    this->type = CUBE;
}

object::object(vec3 const& positionOrigin, float const& size, vec3 const& color, vec3 const& positionForce, float const& mass)
{
    this->positionOrigin = positionOrigin;
    this->size = size;
    this->color = color;
    this->positionForce = positionForce;

    this->radius = size * 0.5f;
    this->mass = ((4.0f / 3.0f) * PI * pow(radius, 3.0f)) * mass;
    this->position = positionOrigin;
    this->type = SPHERE;
};

void object::UpdatePositionByTimer() {

    position = GRAVITY_FORCE * (timer * timer) * 0.5f + positionForce * timer + positionOrigin;
    if (type == CUBE) {
        for (auto i = 0; i < 8; i++)
            vertices[i] = verticesOrigin[i];
        if (degreeOrigin != 0.0f) {
            mat4 matrixRotateOrigin;
            matrixRotateOrigin.RotateArbitraryAxis(rotationAxisOrigin, degreeOrigin);
            for (auto i = 0; i < 8; i++)
                vertices[i] = matrixRotateOrigin * vertices[i];
        }
        if (rotationForce != 0.0f) {
            degree = timer * rotationForce; // RADIAN(360) vaut un tour par seconde
            mat4 matrixRotate;
            matrixRotate.RotateArbitraryAxis(rotationAxis, degree);
            for (auto i = 0; i < 8; i++)
                vertices[i] = matrixRotate * vertices[i];
        }
        for (auto i = 0; i < 8; i++)
            vertices[i] += position;
    }
}
