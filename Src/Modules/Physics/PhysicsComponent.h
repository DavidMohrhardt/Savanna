#pragma once

#ifndef UPVEC
#define UPVEC Vector3(0.f, 1.f, 0.f)
#endif

#include <assert.h>
#include <vector>

enum SHAPES
{
    SPHERE = 0,
    BOX,
    CAPSULE,
    PLANE,
    TRIGGER
};

namespace Savanna::Physics
{

    struct Sphere
    {
        Sphere(Vector3 position, float radius) m_position(position), m_radius(radius) {}

        ~Sphere() {}

        float getRadius() { return m_radius; }
        Vector3 getPosition() { return m_position; }

        float m_radius;
        Vector3 m_position;
    };

    struct Box
    {
        Box(Vector3 position, Vector3 AABB)
            : Component(), m_position(position), m_halfLength(AABB)

        {
            /*m_shapeMask = BOX;*/
        }
        ~Box() {}

        Vector3 m_halfLength;
        Vector3 m_position;
    };

    struct Capsule
    {
        Capsule(Vector3 position, Vector3 AABB)
            : Component()
            , m_capsuleEnds(context, arena, 2)
        {
            /*m_shapeMask = CAPSULE;*/
            float smallestXZ = (AABB.m_x < AABB.m_z) ? AABB.m_z : AABB.m_x;
            // Programmatically Figure out capsule size
            for (int i = 0; i < 2; i++)
            {
                Savanna::Handle hSphere("CapsuleSphere", sizeof(Sphere));
                Sphere *pSphere = new (hSphere) Sphere(context, arena, hSphere, Vector3(0.f, 0.f, 0.f), smallestXZ);
                m_capsuleEnds.add(hSphere);
                if (i == 0)
                    m_capsuleEnds[i].getObject<Sphere>()->m_position.m_y = position.m_y + smallestXZ;
                else
                    m_capsuleEnds[i].getObject<Sphere>()->m_position.m_y = (position.m_y + 2 * AABB.m_y - smallestXZ);
            }
        }

        ~Capsule() {}

        Sphere *getCapsuleEnd(int i)
        {
            if (i > -1 && i < 2)
                return m_capsuleEnds[i].getObject<Sphere>();

            return nullptr;
        }

        Array<Handle> m_capsuleEnds;
    };

    struct Plane
    {
        Plane( Vector3 position, Vector3 AABB)
            : Component()
        {
            // Since this is a plane specifically, we define special behaviour
            m_hBox = Handle("PlaneBox", sizeof(Box));
            Box *pBox = new (m_hBox) Box(, position, AABB);

            // Since this is a plane specifically, we define special behaviour
            m_faceVector = UPVEC;
        }
        Plane( Vector3 position, Vector3 AABB, Vector3 normal)
            : Component()
        {
            // Since this is a plane specifically, we define special behaviour
            m_hBox = Handle("PlaneBox", sizeof(Box));
            Box *pBox = new (m_hBox) Box(, position, AABB);

            m_faceVector = normal;

        }
        ~Plane() {}

        Vector3 m_faceVector;
        Handle m_hBox;
    };

    struct PhysicsComponent
    {

        // Constructor -------------------------------------------------------------
        PhysicsComponent( );
        virtual ~PhysicsComponent() {}

        // Component ------------------------------------------------------------

        virtual void addDefaultComponents();

        // Methods ---------------------------------------------------------------
        void generateAABoxCollider(Vector3 pos, Vector3 halfVecs);
        void generateSphereCollider();

        void generateCapsuleFromBox(Vector3 pos, Vector3 halfVecs);
        void generatePlaneFromBox(Vector3 pos, Vector3 maxPos);

        //Handle readLoFiColliderFromFile(const char *asset, const char *package, int &threadOwnershipMask);

        // Get the handles of the shapes for the collision, these are all in model space
        std::vector<Handle> PhysicsComponent::getShapeList()
        {
            return &m_loFiColliderList;
        }

        void addColliderShape(Handle hCollider);

        Box *getLoFiBoxCollider() { return m_loFiColliderList[0].getObject<Box>(); }
        Capsule *getLoFiCapsuleCollider() { return m_loFiColliderList[1].getObject<Capsule>(); }
        // Member Variables ------------------------------------------------------

    protected:

        // This is for defining multiple shapes in maya.
        unsigned int m_physID;
        unsigned int m_numColliders = 0;

        // Maintain a list of handles to collision objects
        std::vector<Handle> m_collidersList;
        std::vector<Handle> m_loFiColliderList;
        std::vector<Handle> m_hifiCollidersList;

    private:

    };

}
