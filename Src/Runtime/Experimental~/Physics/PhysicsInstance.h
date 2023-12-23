#pragma once

#include "PhysicsComponent.h"
#include "PhysicsManager.h"

namespace savanna::Reference
{
    namespace Components
    {
        struct PhysicsInstance
        {
            PhysicsInstance(
                Handle parentSN,
                int type,
                bool isActive
            );

            PhysicsInstance(
                GameContext & context,
                MemoryArena arena,
                ,
                Matrix4x4 &base,
                int type,
                bool isActive
            );

            virtual ~PhysicsInstance() {}

            virtual void addDefaultComponents();

            void initFromRawValues(const char * asset, const char * package, int & threadOwnershipMask, Vector3 * AABB, int collisionMask);
            void initFromFile(const char * asset, const char * package, int & threadOwnershipMask, int collisionMask);

            Handle getColliderShapeHandle(int index) {
                return m_hPhysicsComponent.getObject<PhysicsComponent>()->getShapeList()->getByIndexUnchecked(index);
            }

            // Setters
            void setActiveMarker(bool active) { m_isActive = active; }

            // Getters
            SceneNode * getSavedSceneNodePtr() { return m_hSceneNode.getObject<SceneNode>(); }
            SceneNode * getSceneNodePtr()
            {
                savanna::Handle hParent = getFirstParentByType<Component>();
                if (hParent.isValid())
                {
                    // see if parent has scene node component
                    return hParent.getObject<Component>()->getFirstComponent<SceneNode>();

                }
                return nullptr;
            }
            Matrix4x4 *getMatrix()
            {
                savanna::Handle hParent = getFirstParentByType<Component>();
                if (hParent.isValid())
                {
                    // see if parent has scene node component
                    return &hParent.getObject<Component>()->getFirstComponent<SceneNode>()->m_base;

                }
                return &m_base;
            }
            int getType() { return m_type; }
            bool getIsActive() { return m_isActive; }

        private:
            Handle m_hPhysicsComponent;
            Handle m_hSceneNode;
            Matrix4x4 m_base;

            int m_type;
            bool m_isActive;
        };
    }
}

#endif
