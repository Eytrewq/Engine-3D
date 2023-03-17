#include "Cube.hpp"

cube::cube(vec3 const& positionOrigin, float const& dimensions, vec3 const& color, vec3 const& rotationAxisOrigin, float const& degreeOrigin, vec3 const& positionForce) {
    this->positionOrigin = positionOrigin;
    this->dimensions = dimensions;
    this->color = color;
    this->rotationAxisOrigin = rotationAxisOrigin;
    this->degreeOrigin = RADIAN(degreeOrigin);
    this->positionForce = positionForce;

    this->mass = this->dimensions * this->dimensions * this->dimensions;
    this->radius = sqrt(this->dimensions * this->dimensions * 3.0f) * 0.5f;
    this->position = this->positionOrigin;
    for (auto i = 0; i < 8; i++) {
        verticesOrigin[i].x = verticesOrigin[i].x * dimensions;
        verticesOrigin[i].y = verticesOrigin[i].y * dimensions;
        verticesOrigin[i].z = verticesOrigin[i].z * dimensions;
    }
}

void cube::UpdatePositionByTimer() {

    position = GRAVITY_FORCE * (timer * timer) * 0.5f + positionForce * timer + positionOrigin;
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
