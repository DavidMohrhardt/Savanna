#include "PhysicsManager.h"




#define GETMAX(a, b) (a > b ? a : b)
#define GETMIN(a, b) (a < b ? a : b)

namespace savanna::Reference
{
namespace Components
{

using namespace System;
using namespace Events;

// Construction and Initialization of the Singleton
Handle PhysicsManager::s_hTitleInstance;
Handle PhysicsManager::s_hInstance;
Handle PhysicsManager::s_hCurInstance;

void PhysicsManager::Construct(, arena)
{
    Handle h("PHYSICS_MANAGER", sizeof(PhysicsManager));
    PhysicsManager *pPM = new (h) PhysicsManager(context, arena, h);
    pPM->addDefaultComponents();
    SetInstance(h);
    context.m_pPhysicsManager = pPM;
}

void PhysicsManager::addDefaultComponents()
{
    Component::addDefaultComponents();

    //PE_REGISTER_EVENT_HANDLER(Event_UPDATE, PhysicsManager::do_UPDATE);
    PE_REGISTER_EVENT_HANDLER(Event_PHYSICS_START, PhysicsManager::do_PhysicsStart);
    PE_REGISTER_EVENT_HANDLER(Event_PHYSICS_END, PhysicsManager::do_PhysicsEnd);
}

// Member Methods ---------------------------------------------------------------
Handle PhysicsManager::generateTriggerFromRawData(TriggerVolume * pVolume, const char * asset, const char * package, Vector3 * AABB, int collisionMask)
{
    char key[StrTPair<Handle>::StrSize];
    sprintf(key, "%s%s", package, asset);

    int index = m_colliderMap.findIndex(key);

    if (index != -1)
    {
        return m_colliderMap.m_pairs[index].m_value;
    }
    else
    {
        savanna::Handle hPC(asset, sizeof(PhysicsComponent));
        PhysicsComponent *pPhysicsComponent = new (hPC) PhysicsComponent(*m_pContext, m_arena, hPC);

        Vector3 vecMid, vec;
        vec = vecMid = AABB[1] - AABB[0];
        vecMid *= 0.5; // Midpoint
        vecMid = AABB[0] + vecMid;

        pPhysicsComponent->generateCapsuleFromBox(AABB[0], vecMid);

        m_colliderMap.add(key, hPC);
        return hPC;
    }
}

Handle PhysicsManager::generateLofiFromRawColliderData(const char * asset, const char * package, Vector3 * AABB, int & threadOwnership, int collisionMask)
{
    char key[StrTPair<Handle>::StrSize];
    sprintf(key, "%s%s", package, asset);

    int index = m_colliderMap.findIndex(key);

    if (index != -1)
    {
        return m_colliderMap.m_pairs[index].m_value;
    }
    else
    {
        savanna::Handle hPC(asset, sizeof(PhysicsComponent));
        PhysicsComponent *pPhysicsComponent = new (hPC) PhysicsComponent(*m_pContext, m_arena, hPC);

        Vector3 vecMid, vec;
        vec = vecMid = AABB[1] - AABB[0];
        vecMid *= 0.5; // Midpoint
        vecMid = AABB[0] + vecMid;

        switch (collisionMask)
        {
        case SPHERE:
            //pPhysicsComponent->gener

            break;

        case CAPSULE:
            pPhysicsComponent->generateCapsuleFromBox(AABB[0], vecMid);
            break;

        case TRIGGER:
            pPhysicsComponent->generateCapsuleFromBox(AABB[0], vecMid);
            break;

        case BOX:
            pPhysicsComponent->generateAABoxCollider(AABB[0], vecMid);
            break;

        case PLANE:
            pPhysicsComponent->generatePlaneFromBox(AABB[0], AABB[1]);
            break;
        };

        m_colliderMap.add(key, hPC);
        return hPC;
    }
}

Handle PhysicsManager::readLoFiColliderFromFile(
    const char * asset, const char * package, int & threadOwnershipMask, int collisionMask
)
{
    char key[StrTPair<Handle>::StrSize];
    sprintf(key, "%s%s", package, asset);

    int index = m_colliderMap.findIndex(key);

    if (index != -1)
    {
        return m_colliderMap.m_pairs[index].m_value;
    }
    else if (StringOps::endswith(asset, "lfca"))
    {
        PEString::generatePathname(*m_pContext, asset, package, "ColliderGenerators", PEString::s_buf, PEString::BUF_SIZE);
        FileReader f(PEString::s_buf, true);

        if (f.open)
        {
            char buf[256];
            f.nextNonEmptyLine(buf, 255);
            //f.nextNonEmptyLine(buf, 255);

            float x, y, z, halfX, halfY, halfZ;
            f.nextFloat32(x);
            f.nextFloat32(y);
            f.nextFloat32(z);
            f.nextFloat32(halfX);
            f.nextFloat32(halfY);
            f.nextFloat32(halfZ);

            Vector3 pos(x, y, z);
            Vector3 max(halfX, halfY, halfZ);

            Vector3 vec = max - pos;
            vec *= 0.5f;
            // Vec = midpoint

            savanna::Handle hPC(asset, sizeof(PhysicsComponent));
            PhysicsComponent *pPhysicsComponent = new (hPC) PhysicsComponent(*m_pContext, m_arena, hPC);
            switch (collisionMask)
            {
            case SPHERE:
                //pPhysicsComponent->gener

                break;

            case CAPSULE:
                pPhysicsComponent->generateCapsuleFromBox(pos, vec);
                break;

            case BOX:
                pPhysicsComponent->generateAABoxCollider(pos, vec);
                break;

            case PLANE:

                // TODO
                break;
            };

            m_colliderMap.add(key, hPC);
            return hPC;
        }
    }
}

bool PhysicsManager::checkForColliderData(const char * asset, const char * package)
{
    char key[StrTPair<Handle>::StrSize];
    sprintf(key, "%s%s", package, asset);

    int index = m_colliderMap.findIndex(key);

    if (index != -1)
    {
        return true;
    }
    return false;
}

void PhysicsManager::addTriggerInstance(Handle hInstance)
{
    //h_triggerPhysComp_List.add(hInstance);
}

void PhysicsManager::addPhysicsInstance(Handle hInstance)
{
    PhysicsInstance * pPhysInstance = hInstance.getObject<PhysicsInstance>();

    switch (pPhysInstance->getType())
    {
    case CAPSULE:
        h_dynPhysComp_List.add(hInstance);
        break;

    /*case TRIGGER:
        h_triggerPhysComp_List.add(hInstance);
        break;*/

    case BOX:
        h_staticPhysComp_List.add(hInstance);
        break;

    case PLANE:
        h_staticPhysComp_List.add(hInstance);
        break;
    }
}

void PhysicsManager::removeTriggerVolumeFromList(Handle hInstance)
{
    /*for (int i = 0; i < h_triggerPhysComp_List.m_size; i++)
    {
        if (h_triggerPhysComp_List.getByIndexUnchecked(i) == hInstance)
        {
            h_triggerPhysComp_List.remove(i);
        }
    }*/
}

// Job Initialization Methods ---------------------------------------------------
int PhysicsManager::generateJobs(int jobType, int index)
{
    PhysicsJob pPhysicsJob = PhysicsJob(
        jobType, index, &h_dynPhysComp_List, &h_staticPhysComp_List, &m_triggers/*&h_triggerPhysComp_List*/
    );

    PhysicsInstance *pPhysInst = h_dynPhysComp_List.getByIndexUnchecked(index).getObject<PhysicsInstance>();

    if (pPhysInst)
    {
        if (pPhysInst->getIsActive())
        {
            applyGravity(&pPhysicsJob);
            handleDynamicDynamicCollision(&pPhysicsJob);
            handleDynamicStaticCollision(&pPhysicsJob);
            handleTriggerVolumeCollision(&pPhysicsJob);
        }
    }

    return 1;
}

// EventHandlers ----------------------------------------------------------------
void PhysicsManager::do_PhysicsStart(Event *pEvt)
{
    // Update Target Positions
}

void PhysicsManager::do_PhysicsEnd(Event *pEvt)
{
    Event_PHYSICS_END *pRealEvent = (Event_PHYSICS_END *)pEvt;
    m_currentDeltaTime = pRealEvent->m_frameTime;

    if (h_dynPhysComp_List.m_size > 0)
    {
        if (m_state++ % 2 == 0)
        {
            for (int i = 0; i < h_dynPhysComp_List.m_size; i++)
                generateJobs(1, i);
        }
        else
        {
            for (int i = h_dynPhysComp_List.m_size - 1; i > -1; i--)
                generateJobs(1, i);
        }
    }
}

// Systems ----------------------------------------------------------------------
int PhysicsManager::applyGravity(PhysicsJob *pJob)
{
    PhysicsInstance *pInst = pJob->getMyInstanceHandle().getObject<PhysicsInstance>();

    SceneNode *pSN = pInst->getSceneNodePtr();

    if (!pSN)
        return 0;

    pSN->m_base.moveDown(0.05f);

    return 1;
}

// Trigger Volumes
int PhysicsManager::handleTriggerVolumeCollision(PhysicsJob *pJob)
{
    //// Calculate collisions
    PhysicsJob *pRealJob = pJob;

    bool collisionDetected = false;

    if (!pRealJob)
        return 0;

    PhysicsInstance *pMyInstance = pRealJob->getMyInstanceHandle().getObject<PhysicsInstance>();
    AttackVolume *pTrigger;

    SceneNode *pMySN = pMyInstance->getSceneNodePtr();

    // Only Capsules

    Capsule myCopy = *(pMyInstance->getColliderShapeHandle(0).getObject<Capsule>());

    Sphere myEnds[2] = {
        *myCopy.getCapsuleEnd(0),
        *myCopy.getCapsuleEnd(1)
    };

    myEnds[0].m_position = pMySN->m_base * myEnds[0].m_position;
    myEnds[1].m_position = pMySN->m_base * myEnds[1].m_position;

    for (int i = 0; i < m_triggers.size(); i++)
    {
        // We could use a virtual table here but I am too lazy to recode for that support
        pTrigger = &(m_triggers.at(i));

        if (pTrigger->m_hOwnerPhysInst == pRealJob->getMyInstanceHandle())
            continue;

        if (pTrigger->m_lifespan > 0.f)
        {
            Sphere otherEnds[2] = {
                *myCopy.getCapsuleEnd(0),
                *myCopy.getCapsuleEnd(1)
            };

            otherEnds[0].m_radius = pTrigger->m_radius;
            otherEnds[1].m_radius = pTrigger->m_radius;
            otherEnds[0].m_position = pTrigger->m_base * pTrigger->m_posA;
            otherEnds[1].m_position = pTrigger->m_base * pTrigger->m_posB;

            Vector3 pos = computeClosestPoint(myEnds[0].m_position, myEnds[1].m_position, otherEnds[1].m_position, myEnds[0].m_position - myEnds[1].m_position);
            Vector3 dist = pos - otherEnds[1].m_position;

            float length = dist.length();

            if (length < otherEnds[0].m_radius * 2 + myEnds[0].m_radius)
            {
                collisionDetected = true;
                pTrigger->RunTriggerFunction(pRealJob->getMyInstanceHandle());
            }
        }

        pTrigger->m_lifespan -= m_currentDeltaTime;

        if (pTrigger->m_lifespan < 0.f)
        {
            pTrigger->~AttackVolume();
            m_triggers.erase(m_triggers.begin() + i);
        }
    }
    return 1;
}

int PhysicsManager::handleDynamicStaticCollision(PhysicsJob *pJob)
{
    // Calculate collisions
    PhysicsJob *pRealJob = pJob;

    if (!pRealJob)
        return 0;

    PhysicsInstance *pMyInstance = pRealJob->getMyInstanceHandle().getObject<PhysicsInstance>();
    PhysicsInstance *pOtherInstance;

    SceneNode *pMySN = pMyInstance->getSceneNodePtr();

    Capsule * pMyCapsule = (pMyInstance->getColliderShapeHandle(0).getObject<Capsule>());
    // Not just spheres but boxes and planes too!

    for (int i = 0; i < pRealJob->getStatPhysArray()->m_size; i++)
    {
        // Reset the sphere ends
        Sphere myEnds[2] = {
            *pMyCapsule->getCapsuleEnd(0),
            *pMyCapsule->getCapsuleEnd(1)
        };

        // Place my capsule ends in the world space
        for (int j = 0; j < 2; j++)
            myEnds[j].m_position = pMySN->m_base * myEnds[j].m_position;

        pOtherInstance = pRealJob->getStatPhysArray()->getByIndexUnchecked(i).getObject<PhysicsInstance>();

        if (!pOtherInstance)
            continue;

        SceneNode *pOtherSN = pOtherInstance->getSavedSceneNodePtr();

        if (!pOtherSN)
            continue;

        // Figure out what the collider is for this object
        hPhysShape = pOtherInstance->getColliderShapeHandle(0);

        if (pOtherInstance->getType() == PLANE)
        {
            // Is plane
            Plane *pPlane = hPhysShape.getObject<Plane>();
            Box pBox = *pPlane->m_hBox.getObject<Box>();
            performCapsuleBoxOrPlaneCollision(&pBox, myEnds, pMySN, pOtherSN, true, &pPlane->m_faceVector);

        }
        else
        {
            // Is box
            Box pBox = *hPhysShape.getObject<Box>();
            performCapsuleBoxOrPlaneCollision(&pBox, myEnds, pMySN, pOtherSN, false, nullptr);
        }
    }

    return 1;
}

int PhysicsManager::handleDynamicDynamicCollision(PhysicsJob *pJob)
{
    // Calculate collisions
    PhysicsJob *pRealJob = pJob;

    if (!pRealJob)
        return 0;

    PhysicsInstance *pMyInstance = pRealJob->getMyInstanceHandle().getObject<PhysicsInstance>();
    PhysicsInstance *pOtherInstance;

    SceneNode *pMySN = pMyInstance->getSceneNodePtr();

    // Only Capsules

    Capsule myCopy = *(pMyInstance->getColliderShapeHandle(0).getObject<Capsule>());

    Sphere myEnds[2] = {
        *myCopy.getCapsuleEnd(0),
        *myCopy.getCapsuleEnd(1)
    };

    myEnds[0].m_position = pMySN->m_base * myEnds[0].m_position;
    myEnds[1].m_position = pMySN->m_base * myEnds[1].m_position;

    for (int i = 0; i < pRealJob->getDynPhysArray()->m_size; i++)
    {
        pOtherInstance = pRealJob->getDynPhysArray()->getByIndexUnchecked(i).getObject<PhysicsInstance>();

        if (i == pRealJob->getMyIndex() || !pOtherInstance || !pOtherInstance->getIsActive())
            continue;

        SceneNode *pOtherSN = pOtherInstance->getSceneNodePtr();

        Capsule otherCopy = *(pOtherInstance->getColliderShapeHandle(0).getObject<Capsule>());

        Sphere otherEnds[2] = {
            *otherCopy.getCapsuleEnd(0),
            *otherCopy.getCapsuleEnd(1)
        };

        otherEnds[0].m_position = pOtherSN->m_base * otherEnds[0].m_position;
        otherEnds[1].m_position = pOtherSN->m_base * otherEnds[1].m_position;

        performCapsuleCapsuleCollision(myEnds, otherEnds, pMySN, pOtherSN, false);
    }

    return 1;
}


// Physics Collision Checking functions
bool PhysicsManager::performCapsuleBoxOrPlaneCollision(Box *pBox, Sphere *pCapsuleEnds, SceneNode *pSNA, SceneNode *pSNB, bool isPlane, Vector3 *planeNormal)
{
    int i, j;
    PrimitiveTypes::Float32 radius, forceScalar, transientLength;
    PrimitiveTypes::Bool isColliding;

    Vector3 capsuleRay, capsuleBase, capsuleTop, boxMinPos, boxMidPos, boxMaxPos, boxDiag,
        capsuleBaseToBoxMid, closestPos, closestBoxPos, normal;

    Matrix4x4 boxTransformInverse, boxTransformNoScale;
    boxTransformNoScale = pSNB->m_base;

    capsuleBase = pCapsuleEnds[0].m_position;
    capsuleTop = pCapsuleEnds[1].m_position;
    radius = pCapsuleEnds[0].m_radius;

    boxMinPos = pBox->m_position;
    boxMidPos = pBox->m_halfLength;
    boxDiag = ((boxMidPos - boxMinPos) * 2);
    boxMaxPos = boxMinPos + boxDiag;

    // Place Box into the world
    boxMinPos = boxTransformNoScale * boxMinPos;
    boxMidPos = boxTransformNoScale * boxMidPos;
    boxMaxPos = boxTransformNoScale * boxMaxPos;
    boxDiag = (boxMaxPos - boxMinPos);

    computeMat4WithoutScale(boxTransformNoScale);

    // Place the scaled box into the collision space
    boxTransformInverse = boxTransformNoScale.inverse();
    boxMinPos = boxTransformInverse * boxMinPos;
    boxMidPos = boxTransformInverse * boxMidPos;
    boxMaxPos = boxTransformInverse * boxMaxPos;
    boxDiag = (boxMaxPos - boxMinPos);

    // Place the capsule in the collision space
    capsuleBase = boxTransformInverse * capsuleBase;
    capsuleTop = boxTransformInverse * capsuleTop;
    capsuleRay = capsuleTop - capsuleBase;

    // Compute the closest point
    closestPos = computeClosestPoint(capsuleBase, capsuleTop, boxMidPos, capsuleRay);
    closestBoxPos = computeClosestBoxPoint(boxMaxPos, boxMinPos, closestPos);

    // Compute the distance
    capsuleBaseToBoxMid = closestBoxPos - closestPos;
    transientLength = capsuleBaseToBoxMid.length();

    isColliding = transientLength < radius;

    if (isColliding)
    {
        int breakp = 0;

        forceScalar = radius - transientLength + 0.00005;

        if (isPlane)
        { // Easy adjust along the plane normal
            normal = *planeNormal;
            // Remove the translation and transform the normal by the rotation
            computeMat4WithoutTranslate(boxTransformNoScale);
            normal = boxTransformNoScale * normal;
            normal = (normal * forceScalar) + pSNA->m_base.getPos();
            pSNA->m_base.setPos(normal);
        }
        else
        { // Compute the face normal for the box
            breakp = 0;
            computeMat4WithoutTranslate(boxTransformNoScale);
            normal = computeBoxFaceNormal(-capsuleBaseToBoxMid);
            // Remove the translation and transform the normal by the rotation
            normal = boxTransformNoScale * normal;
            normal = (normal * forceScalar) + pSNA->m_base.getPos();
            pSNA->m_base.setPos(normal);
        }
    }

    return isColliding;
}

// Collision functions
bool PhysicsManager::performCapsuleCapsuleCollision(Sphere *capsuleA, Sphere *capsuleB, SceneNode *pSNA, SceneNode *pSNB, bool isTrigger)
{
    PrimitiveTypes::Bool isIntersecting = false;
    PrimitiveTypes::Float32 distanceSqr;
    PrimitiveTypes::Float32 radiiSqr =
        capsuleA[0].m_radius + capsuleB[0].m_radius;

    radiiSqr *= radiiSqr;

    Vector3 posA = capsuleA[0].m_position;
    Vector3 posB = capsuleB[0].m_position;
    Vector3 lineA = (capsuleA[1].m_position) - (posA);
    Vector3 lineB = (capsuleB[1].m_position) - (posB);

    // Find the points perpendicular
    Vector3 p, q, posAtoP, posBtoQ;

    PrimitiveTypes::Float32 a, b, c, d, e;

    a = lineA.dotProduct(lineA);
    b = lineA.dotProduct(lineB);
    c = lineB.dotProduct(lineB);
    d = lineA.dotProduct(posA - posB);
    e = lineB.dotProduct(posA - posB);

    PrimitiveTypes::Float32 pScalar, qScalar;
    if (a * c - b * b == 0) // infinite solutions parallel lines
    {
        p = posA;
        q = posB;

        posAtoP = p - posA;
        posBtoQ = q - posB;
    }
    else
    {
        pScalar = (b * e - c * d) / (a * c - b * b);
        qScalar = (a * e - b * d) / (a * c - b * b);

        p = posA + (pScalar * lineA);
        q = posB + (qScalar * lineB);

        posAtoP = p - posA;
        posBtoQ = q - posB;
    }


    if (posAtoP.lengthSqr() > lineA.lengthSqr())
    {
        Vector3 distFar = p - capsuleA[0].m_position;
        Vector3 distBase = p - capsuleA[0].m_position;

        if (distFar.lengthSqr() > distBase.lengthSqr())
            p = posA;
        else
            p = capsuleA[1].m_position;
    }
    if (posBtoQ.lengthSqr() > lineB.lengthSqr())
    {
        Vector3 distFar = q - capsuleB[1].m_position;
        Vector3 distBase = q - capsuleB[0].m_position;

        if (distFar.lengthSqr() > distBase.lengthSqr())
            p = posB;
        else
            p = capsuleB[1].m_position;
    }

    Vector3 finalDistanceVec = (p - q);

    isIntersecting = finalDistanceVec.lengthSqr() <= radiiSqr;

    // Check if we are a trigger volume
    if (isIntersecting && !isTrigger)
    {
        float penetrationForce = sqrt(radiiSqr) - (finalDistanceVec).length();

        Vector3 penetrationVector = posA - posB;
        pSNA->m_base.setPos(pSNA->m_base.getPos() + (penetrationForce + 0.0001) * penetrationVector);
    }
    return isIntersecting;
}

// Helper Methods
inline void PhysicsManager::computeMat4WithoutScale(Matrix4x4 &mat)
{
    int i, j;
    PrimitiveTypes::Float32 scalarLength, scaleFactors[3];

    for (i = 0; i < 3; i++)
        scaleFactors[i] =
            Vector3(mat.m[0][i], mat.m[1][i], mat.m[2][i]).length();

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            scalarLength = scaleFactors[i];
            mat.m[j][i] /= scalarLength;
        }
    }
}

inline void PhysicsManager::computeMat4WithoutTranslate(Matrix4x4 &mat)
{
    int i;

    for (i = 0; i < 3; i++)
        mat.m[i][3] = 0.f;
}

Vector3 PhysicsManager::computeClosestPoint(Vector3 base, Vector3 top, Vector3 boxCenter, Vector3 capsuleRay)
{
    PrimitiveTypes::Float32 transientLength;
    Vector3 capsuleBaseToBoxMid, capsuleBase, capsuleTop, projection, closestPos, baseToClosestPoint;

    capsuleBase = base;
    capsuleTop = top;
    capsuleBaseToBoxMid = boxCenter - base;
    transientLength = capsuleRay.length();

    projection = ((capsuleBaseToBoxMid.dotProduct(capsuleRay)) / (transientLength * transientLength)) * capsuleRay;

    closestPos += projection;
    baseToClosestPoint = closestPos - capsuleBase;

    if (capsuleRay.dotProduct(baseToClosestPoint) < 0.f)
    {
        swapValues<Vector3>(&capsuleBase, &capsuleTop);
        capsuleRay = -capsuleRay;
    }

    if (baseToClosestPoint.lengthSqr() > capsuleRay.lengthSqr())
        closestPos = capsuleTop;

    return closestPos;
}

inline Vector3 PhysicsManager::computeClosestBoxPoint(Vector3 boxMax, Vector3 boxMin, Vector3 closestPoint)
{
    PrimitiveTypes::Float32 transientVec3[3];

    transientVec3[0] = GETMIN(boxMax.m_x, closestPoint.m_x);
    transientVec3[1] = GETMIN(boxMax.m_y, closestPoint.m_y);
    transientVec3[2] = GETMIN(boxMax.m_z, closestPoint.m_z);

    transientVec3[0] = GETMAX(boxMin.m_x, transientVec3[0]);
    transientVec3[1] = GETMAX(boxMin.m_y, transientVec3[1]);
    transientVec3[2] = GETMAX(boxMin.m_z, transientVec3[2]);

    return Vector3(transientVec3[0], transientVec3[1], transientVec3[2]);
}

Vector3 PhysicsManager::computeBoxFaceNormal(Vector3 boxToClosestPos)
{
    //int indices[3];
    int index;
    PrimitiveTypes::Float32 highestDotProd, dotProd;
    highestDotProd = dotProd = -2.0;
    Vector3 faces[6] = {
        Vector3(0.f, 1.f, 0.f), Vector3(0.f, -1.f, 0.f),
        Vector3(1.f, 0.f, 0.f), Vector3(-1.f, 0.f, 0.f),
        Vector3(0.f, 0.f, 1.f), Vector3(0.f, 0.f, -1.f)
    };

    for (int i = 0; i < 6; i++)
    {
        dotProd = boxToClosestPos.dotProduct(faces[i]);
        if (dotProd > highestDotProd)
        {
            index = i;
            highestDotProd = dotProd;
        }
    }

    return faces[index];
}

}
}
