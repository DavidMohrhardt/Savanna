#ifndef __PHYSICS_JOB_H__
#define __PHYSICS_JOB_H__

// API Abstraction


// Outer-Engine includes
#include <assert.h>

// Inter-Engine includes





// Physics Engine Includes

#include "PhysicsInstance.h"

namespace savanna::Physics
{

    struct PhysicsJob
    {
        PhysicsJob(
            int jobType,
            int index,
            std::vector<Handle> *dynPhysArray,
            std::vector<Handle> *statPhysArray,
            std::vector<AttackVolume> *trigPhysArray
        ) : hDynPhysArr(dynPhysArray),
            hStatPhysArr(statPhysArray),
            m_triggerArr(trigPhysArray)
        {
            pj_jobType = jobType;
            m_myIndex = index;
        }

        virtual ~PhysicsJob() {}

        // Member Methods --------------------------------------------------------
        int getJobType() { return pj_jobType; }
        int getMyIndex() { return m_myIndex; }

        Handle getMyInstanceHandle() { return hDynPhysArr->getByIndexUnchecked(m_myIndex); }

        std::vector<Handle> *getDynPhysArray() { return hDynPhysArr; }
        std::vector<Handle> *getStatPhysArray() { return hStatPhysArr; }
        //std::vector<Handle> *getTriggerVolumeArray() { return hTriggerPhysArr; }
        std::vector<AttackVolume> *getTriggerVolumeVector() { return m_triggerArr; }

    private:
        int pj_jobType;
        int m_myIndex;

        std::vector<Handle> *hDynPhysArr;
        std::vector<Handle> *hStatPhysArr;
        //std::vector<Handle> *hTriggerPhysArr;
        std::vector<Handle> *m_triggerArr;
    };

}

#endif
