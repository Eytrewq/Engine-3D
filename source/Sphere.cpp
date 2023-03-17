//#include "Sphere.hpp"
//
//sphere::sphere(vec3 const& positionOrigine, float const& rayon, vec3 const& color, vec3 const& vel)
//{
//    this->rayon = rayon/2;
//    this->color = color;
//    this->velocityPrev = vel;
//    this->velocity = vel;
//    this->positionPrev = positionOrigine;
//    this->position = positionOrigine;
//};
//
//void sphere::UpdatePositionByTime(double const& dt)
//{
//    if (s)
//        return ;
//    this->velocityPrev = this->velocity;
//    this->velocity = this->velocity + GRAVITY_FORCE * dt;
//    this->positionPrev = this->position;
//    this->position = this->position + this->velocity * dt;
//}
//
//double check(const double p, const double min, const double max)
//{
//    double out = 0;
//    double val;
//
//    if (p < min)
//        val = (min - p);
//
//    if (p > max)
//        val = (p - max);
//
//    if (p > max || p < min)
//        out = val * val;
//
//    return out;
//}
//
//float SquaredDistPointAABB(const vec3& p, cube& box)
//{
//    vec3 min = box.getMin();
//    vec3 max = box.getMax();
//
//    double sq = 0.0;
//
//    sq += check(p.x, min.x, max.x);
//    sq += check(p.y, min.y, max.y);
//    sq += check(p.z, min.z, max.z);
//
//    return sq;
//}
//
//bool TestSphereAABB(const sphere & sphere, cube & box)
//{
//    float squaredDistance = SquaredDistPointAABB(sphere.position, box);
//
//    return squaredDistance <= (sphere.rayon * sphere.rayon);
//}
//
//bool sphere::IsCollided(plane& plane)
//{
//    const vec3 planeAxisX = plane.vertices[1] - plane.vertices[0];
//    const vec3 planeAxisZ = plane.vertices[3] - plane.vertices[0];
//    vec3 planeNormal = (planeAxisX ^ planeAxisZ).Normalize();
//
//    vec3 _dist = this->position - plane.vertices[0];
//    float dist = _dist * planeNormal;
//
//    if (dist > this->rayon)
//        return false;
//
//    vec3 tmpP = this->position;
//    this->position = this->position + (planeNormal * (this->rayon - dist));
//
//    vec3 min = plane.getMin();
//    vec3 max = plane.getMax();
//
//    vec3 p = this->position - (planeNormal * this->rayon);
//    vec3 _di = this->positionPrev - p;
//    float di = length_vec(_di);
//
//    if ((min.x > p.x || max.x < p.x
//        || min.y > p.y || max.y < p.y
//        || min.z > p.z || max.z < p.z)
//        && (dis == -1.0f || this->dis > di)) {
//        this->position = tmpP;
//        this->dis = di;
//        return false;
//    }
//
//    vec3 tmp = { 1.0f,1.0f,1.0f };
//    if (planeNormal.x != -1.0f && planeNormal.x != 1.0f && this->velocity * tmp == 0.0f && dist == this->rayon) {
//        this->position = this->positionPrev;
//        s = true;
//        return false;
//    }
//
//    s = false;
//    double VelMag = length_vec(this->velocity);
//    vec3 VelNorm = this->velocity.Normalize();
//
//    this->velocity = (planeNormal * (planeNormal * 2 * -VelNorm) + VelNorm).Normalize();
//    this->velocity = this->velocity * VelMag;
//
//    return true;
//}
//
//bool sphere::IsCollided(cube & box)
//{
//    if (TestSphereAABB(*this, box)) {
//        this->velocity = -this->velocity;
//        return true;
//    }
//
//    return false;
//}
//
//bool sphere::IsCollided(sphere & sphere)
//{
//    vec3 vecDist = this->position - sphere.position;
//    vec3 vecV = this->position - this->positionPrev; 
//    double Dist = length_vec(vecDist);
//
//    float SumRayon = this->rayon + sphere.rayon;
//
//    if (SumRayon > Dist) {
//        float sep = Dist - SumRayon;
//        this->position = this->position - (vecDist.Normalize() * (sep / 2));
//        sphere.position = sphere.position - (-vecDist.Normalize() * (sep / 2));
//        vec3 X_Axis = (this->position - sphere.position).Normalize();
//        float p = X_Axis * this->velocity;
//        vec3 U1x = X_Axis * p;
//        vec3 U1y = this->velocity - U1x;
//        X_Axis = X_Axis * -1.0f;
//        float p_ = X_Axis * sphere.velocity;
//        vec3 U2x = X_Axis * p_;
//        vec3 U2y = sphere.velocity - U2x;
//        vec3 V1x = (U1x + U2x - (U1x - U2x)) * 0.5f;
//        vec3 V2x = (U1x + U2x - (U2x - U1x)) * 0.5f;
//        vec3 V1y = U1y;
//        vec3 V2y = U2y;
//        this->velocity = V1x + V1y;
//        sphere.velocity = V2x + V2y;
//
//        return true;
//    }
//
//    return false;
//}

#include "Sphere.hpp"

sphere::sphere(vec3 const& positionOrigin, float const& dimension, vec3 const& color, vec3 const& positionForce)
{
    this->positionOrigin = positionOrigin;
    this->color = color;
    this->positionForce = positionForce;
    this->radius = dimension * 0.5f;
    this->mass = (4.0f / 3.0f) * PI * pow(radius, 3.0f);

    this->position = positionOrigin;
};

void sphere::UpdatePositionByTimer()
{
    position = GRAVITY_FORCE * (timer * timer) * 0.5f + positionForce * timer + positionOrigin;
}

