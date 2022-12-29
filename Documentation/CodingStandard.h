/**
 * C++ Coding Standard:
 *
 * Comments:
 *
 * All forms of comments are allowed, however each form is dedicated to a certain kind of comment.
 * //    - Short, one-line comments.  Used for explaining things in code blocks
 * /*    - Multiline comments only.  Used for explaining the purpose of a file or function
 * ///   - Multiline Comments only.  Used for defining function/variable comments in a file
 * Classes:
 *      - Use Class instead of Struct.
 *          - Structs should be reserved for pure data types.
 *      - Only single inheritance is allowed.  Any and all multiple inheritance classes will be tossed aside.  Don't waste your time.
 *      - Friend classes are acceptable except in the case where there would be more than one friend class. Acceptable in tests.
 * Templates:
 *      - Should be avoided if there are less than four generic use cases.
 *          - This is to cut down on compile time and obfuscation of the code base.
 *
 */

/// ExampleClass.h - Header file for the ExampleClass class.

/// - Namespaces follows the following format: Savanna::<ModuleName>::<OptionalSubModuleName>
///     Examples:
///     - Savanna (This is the root namespace and only used within the Core Library)
///     - Savanna::Rendering (This is the Rendering namespace for the generalized graphics API)
///     - Savanna::Rendering::Vulkan (This is the namespace for the Vulkan graphics implementation of the Savanna::Rendering namespace)
namespace Savanna::CodingStandard
{
    class ExampleClass
    {
    private:
        // Private members go here
        bool m_IsExample;
    public:
        // Public members go here
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

