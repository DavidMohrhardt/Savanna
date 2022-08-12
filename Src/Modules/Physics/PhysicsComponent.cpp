#include "PhysicsComponent.h"

#define MINMAXVECCOMP(x, y)

namespace Savanna::Physics
{

    PhysicsComponent::PhysicsComponent()
        : Component(),
        m_collidersList(context, arena, 4),
        m_loFiColliderList(context, arena, 4)
    {
    }

    void PhysicsComponent::addDefaultComponents()
    {
        Component::addDefaultComponents();
    }

    // Collider Generation
    void PhysicsComponent::generateAABoxCollider(Vector3 pos, Vector3 halfVecs)
    {
        Box *pBox = new Box(*m_pContext, m_arena, hBox, pos, halfVecs);

        m_loFiColliderList.add(hBox);
    }

    // TODO
    void PhysicsComponent::generateSphereCollider()
    {
    }
    void PhysicsComponent::generateCapsuleFromBox(Vector3 pos, Vector3 halfVecs)
    {

        Capsule *pCap = new(hCapsule) Capsule(*m_pContext, m_arena, hCapsule, pos, halfVecs);

        m_loFiColliderList.add(hCapsule);
    }

    void PhysicsComponent::generatePlaneFromBox(Vector3 pos, Vector3 maxPos)
    {
        // TODO @DavidMohrhardt bring this code back up to speed
        Vector3 a, b, diag, normal;
        Float32 xV, yV, zV;
        diag = maxPos - pos;
        xV = diag.m_x;
        yV = diag.m_y;
        zV = diag.m_z;

        if (xV == 0.f)
        {
            a = Vector3(0.f, yV, 0.f);
            b = Vector3(0.f, 0.f, zV);
        }
        else if (yV == 0.f)
        {
            a = Vector3(xV, 0.f, 0.f);
            b = Vector3(0.f, 0.f, zV);
        }
        else
        {
            a = Vector3(xV, 0.f, 0.f);
            b = Vector3(0.f, yV, 0.f);
        }

        normal = b.crossProduct(a);
        normal.normalize();
        diag *= 0.5f;


        Plane *pPlane = new(hPlane) Plane(*m_pContext, m_arena, hPlane, pos, diag, normal);

        m_loFiColliderList.add(hPlane);
    }

    void PhysicsComponent::addColliderShape(Handle hCollider)
    {
        m_collidersList.add( hCollider );
    }

}
