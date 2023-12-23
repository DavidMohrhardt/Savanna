/**
 * @file PhysicsManager.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief Provides a manager to handle generation life-cycle management of physics systems.
 *
 * Note: This is a port from some work I did in college and as such is not actually used
 * but is more of a reference.
 *
 * All references to original code base have been removed.
 *
 * @version 0.1
 * @date 2022-08-03
 *
 */

#include <assert.h>

// Physics Includes
//#include "PhysicsFunctions.h"
#include "PhysicsInstance.h"
#include "PhysicsComponent.h"
#include "PhysicsJob.h"
#include "TriggerVolume.h"

namespace savanna::Physics
{

    struct PhysicsManager
    {

        static void Construct();

        //////////////////////////////////////////////////////////////////////////
        // Physics Handling
        //////////////////////////////////////////////////////////////////////////
        int applyGravity(PhysicsJob * pJob);
        int handleTriggerVolumeCollision(PhysicsJob * pJob);
        int handleDynamicStaticCollision(PhysicsJob * pJob);
        int handleDynamicDynamicCollision(PhysicsJob * pJob);

        //////////////////////////////////////////////////////////////////////////
        // Physics Collision Checking
        //////////////////////////////////////////////////////////////////////////
        bool performCapsuleBoxOrPlaneCollision(Box * pBox, Sphere * pCapsuleEnds, SceneNode * pSNA, SceneNode * pSNB, bool isPlane, Vector3 * planeNormal);
        bool performCapsuleCapsuleCollision(Sphere * capsuleA, Sphere * capsuleB, SceneNode * pSNA, SceneNode * pSNB, bool isTrigger);

        //////////////////////////////////////////////////////////////////////////
        // Physics Mangager Getters
        //////////////////////////////////////////////////////////////////////////
        static PhysicsManager *Instance() { return s_hInstance.getObject<PhysicsManager>(); }
        static PhysicsManager *TitleInstance() { return s_hTitleInstance.getObject<PhysicsManager>(); }
        static Handle InstanceHandle() { return s_hInstance; }
        static Handle TitleInstanceHandle() { return s_hTitleInstance; }
        static PhysicsManager *CurInstance() { return s_hCurInstance.getObject<PhysicsManager>(); }
        static Handle CurInstanceHandle() { return s_hCurInstance; }
        static void SetTitleAsCurrent() { s_hCurInstance = s_hTitleInstance; }
        static void SetGameAsCurrent() { s_hCurInstance = s_hInstance; }
        static bool TitleIsCurrent() { return s_hCurInstance == s_hTitleInstance; }

        //////////////////////////////////////////////////////////////////////////
        // Physics Manager Instance Handlers
        //////////////////////////////////////////////////////////////////////////
        static void SetInstance(Handle h) { s_hInstance = h; }

        //////////////////////////////////////////////////////////////////////////
        // Physics Job Generation
        //////////////////////////////////////////////////////////////////////////
        int generateJobs(int jobType, int index);

        //////////////////////////////////////////////////////////////////////////
        // Physics Collider Generators
        //////////////////////////////////////////////////////////////////////////
        Handle generateTriggerFromRawData(
            TriggerVolume *pVolume, const char * asset, const char * package, Vector3 * AABB, int collisionMask
        );
        Handle generateLofiFromRawColliderData(
            const char * asset, const char * package, Vector3 * AABB, int & threadOwnership, int collisionMask
        );
        Handle readLoFiColliderFromFile(
            const char * asset, const char * package, int & threadOwnershipMask, int collisionMask
        );
        bool checkForColliderData(const char * asset, const char * package);



        //////////////////////////////////////////////////////////////////////////
        // Physics Instance Handling
        //////////////////////////////////////////////////////////////////////////
        void addPhysicsInstance(Handle hInstance);
        void addTriggerInstance(Handle hInstance);
        void removeTriggerVolumeFromList(Handle hInstance);

        void addTrigger(AttackVolume &vol)
        {
            AttackVolume newVolume = vol;
            m_triggers.push_back(vol);
        }

    private:
        //////////////////////////////////////////////////////////////////////////
        // Constructors and Destructors
        ////////////////////////////////////////////////////////////////////////////
        PhysicsManager( )
            : Component(),
            h_dynPhysComp_List(context, arena, 32),
            h_staticPhysComp_List(context, arena, 32),
            //h_triggerPhysComp_List(context, arena, 8),
            m_colliderMap(context, arena, 32) {}
        virtual ~PhysicsManager() {}

        //////////////////////////////////////////////////////////////////////////
        // Physics Helper Function
        //////////////////////////////////////////////////////////////////////////
        template <class T>
        void swapValues(T *a, T *b)
        {
            T temp = *a;
            *a = *b;
            *b = temp;
        }

        void computeMat4WithoutScale(Matrix4x4 &mat);
        void computeMat4WithoutTranslate(Matrix4x4 &mat);
        Vector3 computeClosestPoint(Vector3 base, Vector3 top, Vector3 boxCenter, Vector3 capsuleRay);
        Vector3 computeClosestBoxPoint(Vector3 boxMax, Vector3 boxMin, Vector3 closestPoint);
        Vector3 computeBoxFaceNormal(Vector3 boxToClosestPos);

        //////////////////////////////////////////////////////////////////////////
        // Static Handles
        //////////////////////////////////////////////////////////////////////////
        static Handle s_hInstance;
        static Handle s_hTitleInstance;
        static Handle s_hCurInstance;

        //////////////////////////////////////////////////////////////////////////
        // Physics Instance Lists
        //////////////////////////////////////////////////////////////////////////
        std::vector<Handle> h_dynPhysComp_List;
        std::vector<Handle> h_staticPhysComp_List;
        //std::vector<Handle> h_triggerPhysComp_List;
        std::vector<AttackVolume> m_triggers;

        //////////////////////////////////////////////////////////////////////////
        // Physics Component Map
        //////////////////////////////////////////////////////////////////////////
        std::unordered_map<Handle> m_colliderMap;

        //////////////////////////////////////////////////////////////////////////
        // Delta Time
        //////////////////////////////////////////////////////////////////////////
        float m_currentDeltaTime;
        int m_state = 0;
    };

} // namespace Savanna
