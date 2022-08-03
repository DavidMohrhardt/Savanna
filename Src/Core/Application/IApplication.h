namespace Savanna
{
    class IApplication
    {
        public:
            virtual ~IApplication() {}

            virtual void Run() = 0;
    };
} // namespace Savanna
