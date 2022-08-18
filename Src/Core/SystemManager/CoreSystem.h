#pragma once

namespace Savanna
{
    template <typename T>
    class CoreSystem : public Singleton<T>
    {
    public:
        CoreSystem(Context& context)
        {
            context.RegisterSystem(reinterpret_cast<T*>(this));
        }

        virtual ~CoreSystem() {}

        virtual void Initialize() = 0;
        virtual void Update() = 0;
        virtual void Shutdown() = 0;
    }
} // namespace Savanna
