#ifndef __TRIGGER_VOLUME_H__
#define __TRIGGER_VOLUME_H__

namespace savanna::Reference
{
    namespace Components
    {
        // Include the heretic
        struct PhysicsInstance;

        //////////////////////////////////////////////////
        // TriggerData
        //////////////////////////////////////////////////
        struct TriggerData
        {
            TriggerData() {}
            virtual ~TriggerData() {}
        };

        //////////////////////////////////////////////////
        // TriggerVolume
        //////////////////////////////////////////////////
        struct TriggerVolume
        {
            ///////////////////////////////////////
            // Pure Virtual Functions
            ///////////////////////////////////////
            virtual int RunTriggerFunction(TriggerData *pData) = 0;
            virtual void AddDefaultComponents() = 0;

            ///////////////////////////////////////
            // Getters
            ///////////////////////////////////////
            Handle getPhysicsInstancePtr() { return m_hPhysicsInstance; }

            ///////////////////////////////////////
            // Functions
            ///////////////////////////////////////

        protected:
            Matrix4x4 m_base;
            Handle m_hPhysicsInstance;
        };
    }
}

#endif
