#include "PhysicsInstance.h"

namespace savanna::Reference
{
namespace Components
{
    PhysicsInstance::PhysicsInstance(Handle parentSN, int type, bool isActive)
        : Component()
    {
        m_type = type;
        m_isActive = isActive;
        m_hSceneNode = parentSN;
    }

    PhysicsInstance::PhysicsInstance(Matrix4x4 & base, int type, bool isActive)
        : Component()
    {
        m_type = type;
        m_isActive = isActive;
        m_base = base;
    }

    void PhysicsInstance::addDefaultComponents()
    {
        Component::addDefaultComponents();
    }

    void PhysicsInstance::initFromRawValues(const char * asset, const char * package, int & threadOwnershipMask, Vector3 * AABB, int collisionMask)
    {
        PrimitiveTypes::Float32 volume;
        Vector3 vec = AABB[1] - AABB[0];

        volume = vec.m_x * vec.m_y * vec.m_z;

        if (abs(volume) < 0.005 && collisionMask == BOX)
            m_type = PLANE;


        m_hPhysicsComponent =
            PhysicsManager::Instance()->generateLofiFromRawColliderData(asset, package, AABB, threadOwnershipMask,  collisionMask);

        if (collisionMask != TRIGGER)
        PhysicsManager::Instance()->addPhysicsInstance(m_hMyself);
    }

    void PhysicsInstance::initFromFile(const char * asset, const char * package, int & threadOwnershipMask, int collisionMask)
    {
        m_hPhysicsComponent =
            PhysicsManager::Instance()->readLoFiColliderFromFile(asset, package, threadOwnershipMask, collisionMask);

        PhysicsManager::Instance()->addPhysicsInstance(m_hMyself);
    }

}
}
