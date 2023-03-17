#include "Math.hpp"
#include "Plane.hpp"
#include "Object.hpp"

void cout(vec3 v, std::string str) { std::cout << str << " " << v.x << " " << v.y << " " << v.z << std::endl; } // TEST

// This function returns a perspective projection matrix given the field of view, aspect ratio, and near and far clipping planes
mat4 Perspective(float const& fov, float const& aspect, float const& zNear, float const& zFar)
{
    // Calculate the tangent of half of the field of view angle
    float tanHalfFov = tan(RADIAN(fov) / 2.0f);
    // Construct and return the perspective projection matrix using the input parameters
    return {
        { 1.0f / (aspect * tanHalfFov), 0.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f / (tanHalfFov), 0.0f, 0.0f },
        { 0.0f, 0.0f, -(zFar + zNear) / (zFar - zNear), -1.0f },
        { 0.0f, 0.0f, -(2.0f * zFar * zNear) / (zFar - zNear), 0.0f }
    };
}

// This function takes in three vectors, eye, center, and up, and returns a 4x4 matrix representing the camera view transformation
// eye: the position of the camera
// center: the position of the point that the camera is looking at
// up: the up direction of the camera
mat4 LookAt(vec3 const& eye, vec3 const& center, vec3 const& up)
{
    // Calculate the camera direction vector
    vec3 cameraDirection = ((eye - center)).Normalize();
    // Calculate the camera right vector
    vec3 cameraRight = ((cameraDirection ^ up)).Normalize();
    // Calculate the camera up vector
    vec3 cameraUp = ((cameraRight ^ cameraDirection)).Normalize();
    // Create the view transformation matrix using the camera right, up, and direction vectors, and the camera position
    return {
        {cameraRight.x, cameraUp.x, cameraDirection.x, 0.0f},
        {cameraRight.y, cameraUp.y, cameraDirection.y, 0.0f},
        {cameraRight.z, cameraUp.z, cameraDirection.z, 0.0f},
        {-(cameraRight * eye), -(cameraUp * eye), -(cameraDirection * eye), 1.0f}
    };
}

bool isInsideRect(const vec3& point, const vec3& p1, const vec3& p2, const vec3& p3, const vec3& p4) {
    vec3 p2_p1 = p2 - p1;
    vec3 p1_point = point - p1;
    float dot1 = (p2_p1 * p1_point);
    if (dot1 < 0)
        return false;
    vec3 p3_p2 = p3 - p2;
    vec3 p2_point = point - p2;
    float dot2 = (p3_p2 * p2_point);
    if (dot2 < 0)
        return false;
    vec3 p4_p3 = p4 - p3;
    vec3 p3_point = point - p3;
    float dot3 = (p4_p3 * p3_point);
    if (dot3 < 0)
        return false;
    vec3 p1_p4 = p1 - p4;
    vec3 p4_point = point - p4;
    float dot4 = (p1_p4 * p4_point);
    if (dot4 < 0)
        return false;
    return true;
}

QPI QuadraticPlaneIntersection(vec3 const& G, vec3 const& v0, vec3 const& p0, plane const& plane)
{
    // Initialize variables
    vec3 n = plane.normal; // Plane normal vector
    vec3 p = plane.position; // Plane position vector
    float t2, t1, u1, u2, v1, v2; // Intersection parameters
    t1 = t2 = u1 = u2 = v1 = v2 = std::numeric_limits<float>::quiet_NaN(); // Initialize parameters to NaN

    // If the normal vector has no y component, the equation cannot be quadratic, so the curve intersects the plane at a single point
    if (n.y == 0.0f) {
        float nDotv0 = (n * v0); // Dot product of the normal vector and the direction vector of the curve
        if (nDotv0 == 0.0f) // If the dot product is 0, the curve is parallel to the plane and doesn't intersect it
            return { t1, t2, u1, u2, v1, v2 }; // Return NaN values
        t1 = (n * (p - p0)) / nDotv0; // Calculate the intersection parameter for the point of intersection
    }
    // If the normal vector has a y component, the equation is quadratic and can have up to two solutions
    else {
        float a = G * n; // Coefficient a of the quadratic equation
        float b = v0 * n; // Coefficient b of the quadratic equation
        float c = (p0 - p) * n; // Coefficient c of the quadratic equation
        float discriminant = b * b - 4.0f * a * c; // Calculate the discriminant of the quadratic equation
        if (discriminant < 0.0f) // If the discriminant is negative, there are no real solutions and the curve doesn't intersect the plane
            return { t1, t2, u1, u2, v1, v2 }; // Return NaN values
        else if (discriminant == 0.0f) // If the discriminant is zero, there is one real solution and the curve intersects the plane at one point
            t1 = (-b) / (a * 2.0f); // Calculate the intersection parameter for the point of intersection
        else if (discriminant >= 0.0f) { // If the discriminant is positive, there are two real solutions and the curve intersects the plane at two points
            float discriminant_sqrt = sqrt(discriminant); // Calculate the square root of the discriminant
            t1 = (-b + discriminant_sqrt) / (a * 2.0f); // Calculate the intersection parameter for the first point of intersection
            t2 = (-b - discriminant_sqrt) / (a * 2.0f); // Calculate the intersection parameter for the second point of intersection
        }
    }

    // Calculate the u and v parameters for each point of intersection
    if (!std::isnan(t1)) { // If the first intersection parameter is not NaN, calculate the corresponding u and v parameters
        vec3 intersection1 = G * t1 * t1 + v0 * t1 + p0; // Calculate the first point of intersection
        vec3 intersection1Vector = (intersection1 - plane.vertices[0]); // Vector from the first point of intersection to the first vertex of the plane
        u1 = (intersection1Vector * plane.axis1) / (plane.axis1 * plane.axis1); // Calculate the u parameter for the first point of intersection
        v1 = (intersection1Vector * plane.axis2) / (plane.axis2 * plane.axis2); // Calculate the v parameter for the first point of intersection
    }
    if (!std::isnan(t2)) { // If the second intersection parameter is not NaN, calculate the corresponding u and v parameters
        vec3 intersection2 = G * t2 * t2 + v0 * t2 + p0; // Calculate the second point of intersection
        vec3 intersection2Vector = (intersection2 - plane.vertices[0]); // Vector from the second point of intersection to the first vertex of the plane
        u2 = (intersection2Vector * plane.axis1) / (plane.axis1 * plane.axis1); // Calculate the u parameter for the second point of intersection
        v2 = (intersection2Vector * plane.axis2) / (plane.axis2 * plane.axis2); // Calculate the v parameter for the second point of intersection
    }
    return { t1, t2, u1, u2, v1, v2 }; // Return the intersection parameters
}

LPI LinePlaneIntersection(vec3 const& rayOrigin, vec3 const& rayDirection, vec3 const& planePosition, vec3 const& planeVector1, vec3 const& planeVector2)
{
    // Calculate the distance between the ray origin and the plane position
    vec3 distance = (rayOrigin - planePosition);   
    // Calculate the plane normal vector using the cross product of planeVector1 and planeVector2
    vec3 planeNormal = (planeVector1 ^ planeVector2);  
    // Calculate the divisor, which will be used in the calculation of the intersection point
    float divisor = -(rayDirection * planeNormal);   
    // If the divisor is zero, the ray and plane are parallel and do not intersect
    if (divisor == 0)
        return { std::numeric_limits<float>::quiet_NaN() };    
    // Calculate the intersection point of the ray and the plane, as well as the u and v values
    float t = (planeNormal * distance) / divisor;
    float u = -((planeVector2 ^ rayDirection) * distance) / divisor;
    float v = -((rayDirection ^ planeVector1) * distance) / divisor;   
    // Return the intersection point and the u and v values as a LPI (Line Plane Intersection) struct
    return { t, u, v };
    // wikipedia :
    // t [0, 1] the intersection point is on the line segment la and lb
    // u,v [0, 1] the intersection point is in the parallelogram formed by the point P0 and vectors P01 and P02
    // (u + v) <= 1 the intersection point lies in the triangle formed by the three vertices P0, P01 and P02
}


float LinePlaneIntersectionFast(vec3 const& rayOrigin, vec3 const& rayDirection, vec3 const& planePosition, vec3 const& planeNormal)
{
    // Calculate the divisor, which will be used in the calculation of the intersection point
    float divisor = -(rayDirection * planeNormal);
    // If the divisor is zero, the ray and plane are parallel and do not intersect
    if (divisor == 0)
        return std::numeric_limits<float>::quiet_NaN();
    // Calculate the point of intersection between the ray and the plane
    return (planeNormal * (rayOrigin - planePosition)) / divisor;
}


LLI LineLineIntersection(float* x, float* y, float* z)
{
    // Calculate the divisor, which will be used in the calculations of t and u
    float divisor = (x[1] - x[2]) * (y[3] - y[4]) - (y[1] - y[2]) * (x[3] - x[4]);
    // If the divisor is zero, the two lines are parallel and do not intersect
    if (divisor == 0)
        return { std::numeric_limits<float>::quiet_NaN() };
    // Calculate t and u, which are used to determine the point of intersection between the two lines
    float t = ((x[1] - x[3]) * (y[3] - y[4]) * (y[1] - y[3]) * (x[3] - x[4])) / divisor;
    float u = ((x[1] - x[3]) * (y[1] - y[2]) * (y[1] - y[3]) * (x[1] - x[2])) / divisor;
    // Return the point of intersection as a struct containing the values of t and u
    return { t, u };
}

bool SeparatingCubeCube(object const& cube1, object const& cube2)
{
    // Compute the separating axes for the two cubes using SAT
    std::vector<vec3> axes = {
        // Compute the axes for cube1
        cube1.vertices[7] - cube1.vertices[3],
        cube1.vertices[2] - cube1.vertices[3],
        cube1.vertices[1] - cube1.vertices[3],
        // Compute the axes for cube2
        cube2.vertices[7] - cube2.vertices[3],
        cube2.vertices[2] - cube2.vertices[3],
        cube2.vertices[1] - cube2.vertices[3]};

    // Compute the minimum and maximum distances along each axis
    float min1, max1, min2, max2;
    int a = 0, b = 0;
    for (int i = 0; i < 15; ++i) {
        vec3 axis;
        if (i < 6) 
            axis = axes[i];
        else 
            axis = axes[a++ / 3] ^ axes[3 + b++ % 3]; // Compute the cross product

        // Compute the min and max distances of cube1 along the axis
        min1 = max1 = cube1.vertices[3] * axis;
        for (int j = 0; j < 8; ++j) {
            float d = cube1.vertices[j] * axis;
            if (d < min1) min1 = d;
            if (d > max1) max1 = d;
        }

        // Compute the min and max distances of cube2 along the axis
        min2 = max2 = cube2.vertices[3] * axis;
        for (int j = 0; j < 8; ++j) {
            float d = cube2.vertices[j] * axis;
            if (d < min2) min2 = d;
            if (d > max2) max2 = d;
        }

        // Check if the objects are not colliding along this axis
        if (max1 < min2 || max2 < min1) {
            // The objects are not colliding along this axis, so they are not colliding overall
            return false;
        }
    }
    // The objects are colliding along all axes, so they are colliding overall
    return true;
}

bool SeparatingCubeSphere(object const& cube, object const& sphere)
{
    // Compute the separating axes for the cube and the sphere using SAT
    std::vector<vec3> axes = {
        // Compute the axes for the cube
        cube.vertices[7] - cube.vertices[3],
        cube.vertices[2] - cube.vertices[3],
        cube.vertices[1] - cube.vertices[3] };

    // Compute the minimum and maximum distances along each axis
    float min1, max1, min2, max2;
    for (int i = 0; i < 3; ++i) {
        vec3 axis = axes[i];

        // Compute the min and max distances of cube along the axis
        min1 = max1 = cube.vertices[3] * axis;
        for (int j = 0; j < 8; ++j) {
            float d = cube.vertices[j] * axis;
            if (d < min1) min1 = d;
            if (d > max1) max1 = d;
        }

        // Compute the min and max distances of sphere along the axis
        float proj = sphere.position * axis;
        float radius = sphere.radius;
        min2 = proj - radius;
        max2 = proj + radius;

        // Check if the objects are not colliding along this axis
        if (max1 < min2 || max2 < min1) {
            // The objects are not colliding along this axis, so they are not colliding overall
            return false;
        }
    }
    // The objects are colliding along all axes, so they are colliding overall
    return true;
}

bool SeparatingCubePlane(object const& cube, plane const& plane)
{
    // Compute the separating axes for the cube and the plane using SAT
    std::vector<vec3> axes = {
        // Compute the axes for object
        cube.vertices[7] - cube.vertices[3],
        cube.vertices[2] - cube.vertices[3],
        cube.vertices[1] - cube.vertices[3],
        // Compute the axes for plane
        plane.vertices[3] - plane.vertices[0],
        plane.normal,
        plane.vertices[1] - plane.vertices[0] };

    // Compute the minimum and maximum distances along each axis
    float min1, max1, min2, max2;
    int a = 0, b = 0;
    for (int i = 0; i < 15; ++i) {
        vec3 axis;
        if (i < 6) axis = axes[i];
        // Compute the cross product
        else vec3 axis = axes[a++ / 3] ^ axes[3 + b++ % 3];

        // Compute the min and max distances of cube along the axis
        min1 = max1 = cube.vertices[3] * axis;
        for (int j = 0; j < 8; ++j) {
            float d = cube.vertices[j] * axis;
            if (d < min1) min1 = d;
            if (d > max1) max1 = d;
        }

        // Compute the min and max distances of plane along the axis
        min2 = max2 = plane.vertices[0] * axis;
        for (int j = 0; j < 4; ++j) {
            float d = plane.vertices[j] * axis;
            if (d < min2) min2 = d;
            if (d > max2) max2 = d;
        }
        // Check if the objects are not colliding along this axis
        if (max1 < min2 || max2 < min1) {
            // The objects are not colliding along this axis, so they are not colliding overall
            return false;
        }
    }
    // The objects are colliding along all axes, so they are colliding overall
    return true;
}

bool SeparatingSpherePlane(object const& sphere, plane const& plane)
{
    // Compute the separating axes for the sphere and the plane using SAT
    std::vector<vec3> axes = {
        // Compute the axes for plane
        plane.vertices[3] - plane.vertices[0],
        plane.normal,
        plane.vertices[1] - plane.vertices[0]
    };

    // Compute the minimum and maximum distances
    float min1, max1, min2, max2;
    for (int i = 0; i < 3; ++i) {
        vec3 axis = axes[i];

        // Compute the min and max distances of plane along the axis
        min1 = max1 = plane.vertices[0] * axis;
        for (int j = 0; j < 4; ++j) {
            float d = plane.vertices[j] * axis;
            if (d < min1) min1 = d;
            if (d > max1) max1 = d;
        }

        // Compute the min and max distances of sphere along the axis
        float proj = sphere.position * axis;
        float radius = sphere.radius;
        min2 = proj - radius;
        max2 = proj + radius;

        // Check if the objects are not colliding along this axis
        if (max1 < min2 || max2 < min1) {
            // The objects are not colliding along this axis, so they are not colliding overall
            return false;
        }
    }
    // The objects are colliding along all axes, so they are colliding overall
    return true;
}

bool SeparatingSphereSphere(object const& sphere1, object const& sphere2)
{
    // Calculate the vector between the two spheres
    vec3 vector = sphere1.position - sphere2.position;
    // Calculate the distance between the two spheres using the Pythagorean theorem
    float distance = sqrt(vector * vector);
    // Calculate the minimum distance at which the two spheres touch
    float distanceMinimum = sphere1.radius + sphere2.radius;
    // If the actual distance is less than or equal to the minimum distance, the spheres are intersecting
    if (distance <= distanceMinimum)
        return true;
    // If the actual distance is greater than the minimum distance, the spheres are not intersecting
    return false;
}
