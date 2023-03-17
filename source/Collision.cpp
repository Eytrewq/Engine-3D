#include "Collision.hpp"

void collision::GetVectorSizes() {

    /* Récupère les tailles des vectors */
    planesCollidedSize = planesCollided.size();
    objectsCollidedSize = objectsCollided.size();

}

vec3 collision::GetObjectNormal(unsigned int const& i) {

    /* Calcule de la moyenne des normales des plans */
    vec3 normal;
    for (auto j = 0; j < planesCollidedSize; j++)
        normal += planesCollided[j]->normal;
    for (auto j = 0; j < objectsCollidedSize; j++) if (i != j)
        normal += (objectsCollided[j]->position - objectsCollided[i]->position).Normalize();
    normal = normal.Normalize();
    return normal;
}

vec3 collision::GetObjectSpeedAtTimer(unsigned int const& i) {

    /* Calcule la vitesse lors de la collision en fonction de timer */
    return GRAVITY_FORCE * (objectsCollided[i]->timer) + objectsCollided[i]->positionForce;
}

vec3 collision::GetNewDirection(vec3 const& speedAtTimer, vec3 const& normal) {

    /* Calcule la nouvelle direction après la collision */
    return (speedAtTimer - normal * (speedAtTimer * normal) * 2.0f).Normalize();
}

void collision::SetNewRotationAxisOrigin(unsigned int const& i) {

    // Vérifie s'il y a une rotation en cours
    // (sinon rotation d'origine ne change pas)
    if (objectsCollided[i]->degree != 0.0f) {
        // Vérifie s'il y a une rotation d'origine
        if (objectsCollided[i]->degreeOrigin != 0.0f) {
            // Calcule la multiplication matricielle des rotations
            mat4 model;
            model.RotateArbitraryAxis(objectsCollided[i]->rotationAxisOrigin, objectsCollided[i]->degreeOrigin);
            model.RotateArbitraryAxis(objectsCollided[i]->rotationAxis, objectsCollided[i]->degree);
            // Récupère le nouvel axe et le nouvel angle pour la nouvelle origine de rotation
            float trace = (model.x.x + model.y.y + model.z.z);
            float cos_theta = 0.5f * (trace - 1.0f);
            // Vérifie si les deux rotations sont visuellement identiques
            if (fabs(cos_theta) < 1.0f) {
                float sin_theta = sqrt(1.0f - cos_theta * cos_theta);
                objectsCollided[i]->rotationAxisOrigin = { model.z.y - model.y.z, model.x.z - model.z.x, model.y.x - model.x.y };
                objectsCollided[i]->rotationAxisOrigin = objectsCollided[i]->rotationAxisOrigin / sin_theta;
                objectsCollided[i]->degreeOrigin = acos(cos_theta);
            } // (sinon rotation d'origine ne change pas)
        }
        else {
            // (sinon rotation d'origine devient rotation en cours)
            objectsCollided[i]->rotationAxisOrigin = objectsCollided[i]->rotationAxis;
            objectsCollided[i]->degreeOrigin = objectsCollided[i]->degree;
        }
    }
}

void collision::GetNewRotationAxis(unsigned int const& i, vec3 const& normal) {

    // Récupère le.s coin.s impliqués dans la collision 
    std::vector<vec3> verticesSelected;
    float maxDistance = -INFINITY;
    bool isCollided = false;
    for (auto const& vertex : objectsCollided[i]->vertices) {
        float distance = (int)(((vertex - objectsCollided[i]->position) * normal) / timerDiff + 1.0f) * timerDiff; // Arrondie la distance pour éviter les erreurs d'imprécisions
        if (distance > maxDistance) {
            isCollided = true;
            maxDistance = distance;
            verticesSelected.clear();
            std::vector<vec3>().swap(verticesSelected);
            verticesSelected.clear();
            verticesSelected.push_back(vertex);
        }
        else if (distance == maxDistance) {
            verticesSelected.push_back(vertex);
        }
    }

    // Cacule la nouvelle rotation
    objectsCollided[i]->degree = 0.0f;
    float verticesCollisionNumber = (float)(verticesSelected.size());
    vec3 vToCenter;
    for (auto const& vertex : verticesSelected)
        vToCenter += vertex;
    vToCenter = (objectsCollided[i]->position - (vToCenter / verticesCollisionNumber)).Normalize();
    vec3 rotationAxisPrev = objectsCollided[i]->rotationAxis.Normalize();
    objectsCollided[i]->rotationAxis = (vToCenter ^ normal).Normalize();
    float rotationForcePrev = objectsCollided[i]->rotationForce;
    float dot1 = fabs(vToCenter * normal) * 0.975f;
    if (dot1 < 0.975f) {
        objectsCollided[i]->rotationForce = objectsCollided[i]->speed * (0.975f - dot1 * dot1);
        float dot2 = (objectsCollided[i]->rotationAxis * rotationAxisPrev);
        if (dot2 > 0.0f) objectsCollided[i]->rotationForce += rotationForcePrev * (1.0f - fabs(sqrt(dot2)));
        objectsCollided[i]->speed += rotationForcePrev - objectsCollided[i]->rotationForce;
    }
    else {
        objectsCollided[i]->speed += rotationForcePrev + objectsCollided[i]->rotationForce;
        objectsCollided[i]->rotationForce = 0.0f;
    }
}

void collision::ResetTimer(unsigned int const& i)
{
    // Remise à zéro du timer
    objectsCollided[i]->timer = 0.0f;
    objectsCollided[i]->timerPrev = 0.0f;
}

void collision::Separating(unsigned int const& i, vec3 const& normal)
{
    vec3 planesNormal;
    for (auto j = 0; j < planesCollidedSize; j++)
        planesNormal += planesCollided[j]->normal;
    planesNormal = planesNormal.Normalize();
    /* Décallage pour gérer les blocages */
    if (normal * normal != 0.0f) {
        for (auto j = 0; j < objectsCollidedSize; j++) if (i != j) {
            float m1 = objectsCollided[j]->mass / objectsCollided[i]->mass;
            float m2 = objectsCollided[i]->mass / objectsCollided[j]->mass;
            if (m1 > 20.0f) m1 = 20.0f;
            if (m2 > 20.0f) m2 = 20.0f;

            objectsCollided[i]->positionOrigin -= normal * timerDiff * m1;
            objectsCollided[i]->UpdatePositionByTimer();
            objectsCollided[j]->positionOrigin += normal * timerDiff * m2;
            objectsCollided[j]->UpdatePositionByTimer();
        }
    }
    if (planesCollidedSize > 0) {
        for (auto j = 0; j < planesCollidedSize; j++) {
            while ((objectsCollided[i]->type == CUBE &&
                SeparatingCubePlane(*objectsCollided[i], *planesCollided[j]) == true) ||
                (objectsCollided[i]->type == SPHERE &&
                    SeparatingSpherePlane(*objectsCollided[i], *planesCollided[j]) == true))
            {
                objectsCollided[i]->positionOrigin -= planesNormal * timerDiff * timerDiff;
                if ((objectsCollided[i]->timer -= timerDiff) < objectsCollided[i]->timerPrev)
                    objectsCollided[i]->timer = objectsCollided[i]->timerPrev;
                objectsCollided[i]->UpdatePositionByTimer();
            }
        }
    }
}

void collision::Caculate1()
{
    for (auto i = 0; i < objectsCollidedSize; i++) {
        vec3 normal = GetObjectNormal(i);
        if (normal * normal == 0.0f) continue;
        Separating(i, normal);
        vec3 speed = GetObjectSpeedAtTimer(i);
        objectsCollided[i]->speed = sqrt(speed * speed);
        if (objectsCollided[i]->type == CUBE) {
            SetNewRotationAxisOrigin(i);
            GetNewRotationAxis(i, normal);
        }
        objectsCollided[i]->positionForcePrev = objectsCollided[i]->positionForce;
    }
    
}

void collision::Caculate2()
{
    for (auto i = 0; i < objectsCollidedSize; i++) {
        vec3 normal = GetObjectNormal(i);
        if (normal * normal == 0.0f) continue;
        vec3 speed = GRAVITY_FORCE * (objectsCollided[i]->timer) + objectsCollided[i]->positionForcePrev;
        vec3 direction = GetNewDirection(speed, normal);
        if (normal.x * direction.x > 0.0f) direction.x = -direction.x;
        if (normal.y * direction.y > 0.0f) direction.y = -direction.y;
        if (normal.z * direction.z > 0.0f) direction.z = -direction.z;
        float c = objectsCollided.size() + planesCollided.size();
        float s = 0.0f;
        float m = 0.0f;
        for (auto j = 0; j < planesCollidedSize; j++) {
            s += objectsCollided[i]->speed;
            m += objectsCollided[i]->mass;
        }
        for (auto j = 0; j < objectsCollidedSize; j++) if (i != j) {
            s += objectsCollided[j]->speed;
            m += objectsCollided[j]->mass;
        }
        s /= c - 1.0f;

        float m1 = objectsCollided[i]->mass;
        float m2 = m;
        float v1 = objectsCollided[i]->speed;
        float v2 = s;
        float S = (((m1 - m2) / (m1 + m2)) * v1 + ((2.0f * m2) / (m1 + m2)) * v2);

        float dot = (normal * objectsCollided[i]->positionForcePrev.Normalize());
        float dotAbs = fabs(dot);

        if (m1 * v1 > m2 * v2) {
            direction = objectsCollided[i]->positionForcePrev.Normalize();
        } else S *= 0.9f;

        objectsCollided[i]->positionForce =
            direction * (S * dotAbs + v1 * (1.0f - dotAbs));

        objectsCollided[i]->positionOrigin = objectsCollided[i]->position;
        ResetTimer(i);
        objectsCollided[i]->UpdatePositionByTimer();
    }
}


