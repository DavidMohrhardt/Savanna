/**
 * C++ Coding Standard:
 *
 * Files:
 * - Files should be named in PascalCase.
 * - Files should be named after the class they contain.
 *    - If the file contains multiple classes, it should be named after the most important class.
 * - Headers should only have the .h extension when used within the engine library code.
 * - Headers should have the .hpp extension when defining a C++ version of a public header and
 *   said header must have a C version as well. For example, ISavannaMemory.h and ISavannaMemory.hpp.
 * - Public headers must be placed within a "Public" folder within the module.
 *
 * Comments:
 * All forms of comments are allowed, however each form is dedicated to a certain kind of comment.
 * //    - Short, one-line comments.  Used for explaining things in code blocks
 * /*    - Multiline comments only.  Used for explaining the purpose of a file or function
 * ///   - Multiline Comments only.  Used for defining function/variable comments in a file
 *
 * Classes:
 *      - Use Class instead of Struct.
 *      - Structs should be reserved for pure data types.
 *
 * Templates:
 *      - Should be avoided if there are less than four generic use cases.
 *          - This is to cut down on compile time and obfuscation of the code base.
 *     - Template functions should be defined in the header file if they might be used in multiple
 *       applications.
 *
 */

/// ExampleClass.h - Header file for the ExampleClass class.

/// - Namespaces follows the following format: Savanna::<ModuleName>::<OptionalSubModuleName>
///     Examples:
///     - Savanna (This is the root namespace and only used within the Core Library)
///     - Savanna::Rendering (This is the Rendering namespace for the generalized graphics API)
///     - Savanna::Gfx::Vulkan (This is the namespace for the Vulkan graphics implementation of the Savanna::Rendering namespace)
namespace Savanna::CodingStandard
{
    struct ExampleStruct
    {
        // Structs should only contain data members and no functions.
        int m_ExampleInt;
        float m_ExampleFloat;
    };

    class ExampleClass
    {
    private:
        // Private members go here
        bool m_IsExample;

    public:
        // Public members go here. If no public members can be ignored.
        bool m_IsPublicExample;
        int m_Counter;

    public:
        // Constructors and destructors should have their own public/private/protected sections.
        ExampleClass();
        ~ExampleClass();

    private:
        ExampleClass(const ExampleClass& other);

    public:
        // Public functions go here
        void ExamplePublicFunction();

        /// - Getters may be implemented in the class definition iff they are not more than one line long.
        /// - Getters should be marked as const to ensure they do not modify the class.
        bool IsExampleClass() const { return m_IsExample; }

    private:
        // Private functions go here
        void ExamplePrivateFunction();
    };
} // namespace Savanna::CodingStandard

