//#ifndef SPHERE_HPP
//#define SPHERE_HPP
//
//#include "Math.hpp"
//#include "Cube.hpp"
//#include "Plane.hpp"
//
//class sphere
//{
//public:
//    vec3 position, color, positionPrev, velocity, velocityPrev;
//    float rayon = 1.0f, dt = 0.0f, dis = -1.0f;
//    bool s = false, isCollided = false, hit = false;
//
//    sphere(vec3 const& positionOrigine, float const& rayon, vec3 const& color, vec3 const& positionForce);
//    vec3 linePlaneInter(vec3 Pnormal, vec3 PO, vec3 ray);
//    void UpdatePositionByTime(double const& timeDiff);
//    bool testMovingSphereSphere(sphere& sphere, float& t);
//    bool testMovingPlaneSphere(plane& plane, float& t);
//    int  TestIntersionPlane(const plane plane, const vec3& direction, const vec3 planeNormal);
//    bool IsCollided(plane & plane);
//    bool IsCollided(cube & box);
//    bool IsCollided(sphere & sphere);
//};
//
//
//#endif

#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "Math.hpp"
#include "Cube.hpp"
#include "Plane.hpp"

class sphere
{
public:
    vec3 position, positionForce, color, positionPrev, positionOrigin;
    float radius = 0.0f, timeDiff = 0.0f, mass = 0.0f, timer = 0.0f, timerPrev = 0.0f, speed = 0.0f;
    bool isCollided = false;

    sphere(vec3 const& positionOrigin, float const& dimension, vec3 const& color, vec3 const& positionForce);
    void UpdatePositionByTimer();
};


#endif
