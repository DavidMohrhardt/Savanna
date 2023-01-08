#pragma once

#include <functional>
#include <vector>

#include "SavannaClass.h"

// TODO @davidmo: move this over to a manager
#undef DECLARE_PLAYERLOOP_PHASE
#define DECLARE_PLAYERLOOP_PHASE(name)                  \
struct name : public Savanna::PlayerLoopPhase           \
{                                                       \
    name() {}                                           \
                                                        \
    void Invoke()                                       \
    {                                                   \
        for (const auto &updateFunc : m_UpdateLoopList) \
        {                                               \
            updateFunc();                               \
        }                                               \
    }                                                   \
    std::vector<std::function<void()>> m_UpdateLoopList;\
};                                                      \

#define DECLARE_PLAYER_LOOP_PHASE_CALLBACK(name, phase) \

namespace Savanna
{
    struct PlayerLoopPhase
    {
    public:
        virtual ~PlayerLoopPhase() {}

        virtual void Invoke() = 0;
    };

    struct PlayerLoop : public SavannaClass<PlayerLoop>
    {
    public:
        void Execute()
        {
            for (PlayerLoopPhase& playerLoopPhase : m_PlayerLoopPhases)
            {
                playerLoopPhase.Invoke();
            }
        }

        void RegisterPhaseCallback(PlayerLoopPhase* phase)
        {
            m_PlayerLoopPhases.push_back(*phase);
        }

    private:
        std::vector<PlayerLoopPhase> m_PlayerLoopPhases;
    };

    DECLARE_PLAYERLOOP_PHASE(Initialization);
    DECLARE_PLAYERLOOP_PHASE(Update);
    DECLARE_PLAYERLOOP_PHASE(Rendering);
}