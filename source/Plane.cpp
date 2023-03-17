#include "Plane.hpp"

plane::plane(vec3 const& position, vec3 const& dimensions, vec3 const& color, vec3 const& rotationAxis, float const& degree) :
    position(position), dimensions(dimensions), color(color), rotationAxis(rotationAxis), degree(degree)
{
    mat4 matrix;
    matrix.Scaling(dimensions);
    if (degree != 0.0f)
        matrix.RotateArbitraryAxis(rotationAxis, degree);

    for (auto i = 0; i < 4; i++)
        vertices[i] = position + matrix * vertices[i];

    axis1 = vertices[1] - vertices[0];
    axis2 = vertices[3] - vertices[0];
    normal = (axis1 ^ axis2).Normalize();
    axis1dot = axis1 * axis1;
    axis2dot = axis2 * axis2;
}
