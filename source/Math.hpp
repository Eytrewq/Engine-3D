#ifndef MATH_HPP
#define MATH_HPP

#include <iostream>
#include <cmath>
#include <vector>

#define PI 3.14159265358979323846f
#define EPSILON 1e-6f

#define ZERO(v) v.x = (v.x > -EPSILON && v.x < EPSILON ? 0.0f : v.x), v.y = (v.y > -EPSILON && v.y < EPSILON ? 0.0f : v.y), v.z = (v.z > -EPSILON && v.z < EPSILON ? 0.0f : v.z)
#define RADIAN(x) ((x * PI) / 180.0f)

struct vec3 {
    float x, y, z;
    vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}

    vec3 operator- (void) const {
        vec3 v = { -x, -y, -z };
        return { ZERO(v) };
    }
    vec3 operator- (const vec3& m) const {
        vec3 v = { x - m.x, y - m.y, z - m.z };
        return { ZERO(v) };
    }
    void operator-= (const vec3& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        ZERO((*this));
    }

    vec3 operator+ (const vec3& m) const {
        vec3 v = { x + m.x, y + m.y, z + m.z };
        return { ZERO(v) };
    }
    void operator+= (const vec3& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        ZERO((*this));
    }

    float operator* (const vec3& m) const { // DOT PRODUCT
        float v = { x * m.x + y * m.y + z * m.z };
        if (v > -EPSILON && v < EPSILON) return 0.0f;
        return v;
    }

    vec3 operator^ (const vec3& m) const { // CROSS PRODUCT
        vec3 v = { y * m.z - z * m.y, z * m.x - x * m.z, x * m.y - y * m.x };
        return { (v) };
    }

    vec3 operator* (const float& m)const {
        vec3 v = { x * m, y * m, z * m };
        return { ZERO(v) };
    }
    friend inline const vec3 operator*(const float& m, const vec3& v) {
        vec3 result = { v.x * m, v.y * m, v.z * m };
        return ZERO(result);
    }
    void operator*= (const float& v) {
        x *= v;
        y *= v;
        z *= v;
        ZERO((*this));
    }

    vec3 operator/ (const float& m) const { 
        if (m == 0.0f) return { 0.0f, 0.0f, 0.0f };
        vec3 v = { x / m, y / m, z / m };
        return { ZERO(v) };
    }
    void operator/= (const float& v) {
        if (v == 0.0f) x = y = z = std::numeric_limits<float>::quiet_NaN();
        x /= v;
        y /= v;
        z /= v;
        ZERO((*this));
    }

    vec3 Normalize() const {
        float d = sqrt(x * x + y * y + z * z);
        if (d == 0.0f) return { 0.0f, 0.0f, 0.0f };
        vec3 v = { x / d, y / d, z / d };
        return { ZERO(v) };
    }
};

struct vec4 {
    float x, y, z, t;
    vec4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float t = 0.0f) : x(x), y(y), z(z), t(t) {}
};

struct mat4 {
    vec4 x, y, z, t;
    mat4(const vec4& x = { 1.0f, 0.0f, 0.0f, 0.0f },
        const vec4& y = { 0.0f, 1.0f, 0.0f, 0.0f },
        const vec4& z = { 0.0f, 0.0f, 1.0f, 0.0f },
        const vec4& t = { 0.0f, 0.0f, 0.0f, 1.0f }) : x(x), y(y), z(z), t(t) {}

    mat4 operator* (mat4 const& m) {
        return {
            {(x.x * m.x.x) + (x.y * m.y.x) + (x.z * m.z.x) + (x.t * m.t.x), (x.x * m.x.y) + (x.y * m.y.y) + (x.z * m.z.y) + (x.t * m.t.y), (x.x * m.x.z) + (x.y * m.y.z) + (x.z * m.z.z) + (x.t * m.t.z), (x.x * m.x.t) + (x.y * m.y.t) + (x.z * m.z.t) + (x.t * m.t.t)},
            {(y.x * m.x.x) + (y.y * m.y.x) + (y.z * m.z.x) + (y.t * m.t.x), (y.x * m.x.y) + (y.y * m.y.y) + (y.z * m.z.y) + (y.t * m.t.y), (y.x * m.x.z) + (y.y * m.y.z) + (y.z * m.z.z) + (y.t * m.t.z), (y.x * m.x.t) + (y.y * m.y.t) + (y.z * m.z.t) + (y.t * m.t.t)},
            {(z.x * m.x.x) + (z.y * m.y.x) + (z.z * m.z.x) + (z.t * m.t.x), (z.x * m.x.y) + (z.y * m.y.y) + (z.z * m.z.y) + (z.t * m.t.y), (z.x * m.x.z) + (z.y * m.y.z) + (z.z * m.z.z) + (z.t * m.t.z), (z.x * m.x.t) + (z.y * m.y.t) + (z.z * m.z.t) + (z.t * m.t.t)},
            {(t.x * m.x.x) + (t.y * m.y.x) + (t.z * m.z.x) + (t.t * m.t.x), (t.x * m.x.y) + (t.y * m.y.y) + (t.z * m.z.y) + (t.t * m.t.y), (t.x * m.x.z) + (t.y * m.y.z) + (t.z * m.z.z) + (t.t * m.t.z), (t.x * m.x.t) + (t.y * m.y.t) + (t.z * m.z.t) + (t.t * m.t.t)},
        };
    }
    vec3 operator* (vec3 const& v) {
        return {
            x.x * v.x + x.y * v.y + x.z * v.z,
            y.x * v.x + y.y * v.y + y.z * v.z,
            z.x * v.x + z.y * v.y + z.z * v.z,
        };
    }
    void Scaling(vec3 const& v) {
        mat4 mat;
        mat.x.x = v.x;
        mat.y.y = v.y;
        mat.z.z = v.z;
        *this = mat * *this;
    }
    void RotateArbitraryAxis(vec3 rotationAxis, float const& degree) {
        rotationAxis = rotationAxis.Normalize();
        float c = std::cos(degree), s = std::sin(degree), t = 1.0f - c;
        float x = rotationAxis.x, y = rotationAxis.y, z = rotationAxis.z;
        *this = mat4{
            {{ t * x * x + c     }, { t * x * y - s * z }, { t * x * z + s * y }},
            {{ t * x * y + s * z }, { t * y * y + c     }, { t * y * z - s * x }},
            {{ t * x * z - s * y }, { t * y * z + s * x }, { t * z * z + c     }},
        } * *this;
    }
    void Translate(vec3 const& pos) {
        mat4 mat;
        mat.t.x = pos.x;
        mat.t.y = pos.y;
        mat.t.z = pos.z;
        *this = mat * *this;
    }
};


inline vec3 GRAVITY_FORCE = vec3{ 0.0f, -9.80f, 0.0f };

struct LLI { float t, u; };
struct LPI { float t, u, v; };
struct QPI { float t1, t2, u1, u2, v1, v2; };

template <typename T>
void cout(T v, std::string str) { std::cout << str << " " << v << std::endl; }
void cout(vec3 v, std::string str);

mat4 Perspective(float const& fov, float const& aspect, float const& zNear, float const& zFar);
mat4 LookAt(vec3 const& eye, vec3 const& center, vec3 const& up);
LPI LinePlaneIntersection(vec3 const& rayOrigin, vec3 const& rayDirection, vec3 const& planePosition, vec3 const& planeVector1, vec3 const& planeVector2);
float LinePlaneIntersectionFast(vec3 const& rayOrigin, vec3 const& rayDirection, vec3 const& planePosition, vec3 const& planeNormal);
bool isInsideRect(const vec3& point, const vec3& p1, const vec3& p2, const vec3& p3, const vec3& p4);
QPI QuadraticPlaneIntersection(vec3 const& G, vec3 const& v0, vec3 const& p0, class plane const& plane);
LLI LineLineIntersection(float* x, float* y, float* z);
bool SeparatingCubeCube(class object const& cube1, object const& cube2);
bool SeparatingCubeSphere(object const& cube, object const& sphere);
bool SeparatingCubePlane(object const& cube, plane const& plane);
bool SeparatingSpherePlane(object const& sphere, plane const& plane);
bool SeparatingSphereSphere(object const& sphere1, object const& sphere2);

#endif
