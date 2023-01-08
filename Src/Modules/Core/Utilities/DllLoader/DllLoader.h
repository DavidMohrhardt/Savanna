
namespace SavannahEngine
{
    enum DllLoaderErrorCode
    {
        kDllLoadSuccess,
        kDllLoadFileDoesNotExist,
        kDllLoadIncompatibleFileType,
        kDllLoadUnknownError,
        kDllLoadInvalid
    };

    class DllLoader 
    {

    public:
        virtual DllLoaderErrorCode Load(const char* dllPath) = 0;
        virtual DllLoaderErrorCode Unload() = 0;
    private:

    };
}