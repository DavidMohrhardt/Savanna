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

/// - Namespaces follows the following format: savanna::<ModuleName>::<OptionalSubModuleName>
///     Examples:
///     - Savanna (This is the root namespace and only used within the Core Library)
///     - savanna::Gfx (This is the Gfx namespace for the generalized graphics API)
///     - savanna::Gfx::Vk2 (This is the namespace for the Vulkan graphics implementation of the savanna::Gfx namespace)
namespace savanna::CodingStandard
{
    // Enumerations should be defined as 'enum class' in C++
    // Enumeration members should use the form
    // 'kEnumNameValue' if they are public facing.
    // Otherwise 'kValue' is fine.
    enum class ExampleCppEnum /* : optional_int_type */
    {
        kExampleCppEnumValueA,
        kExampleCppEnumValueB,
        kExampleCppEnumValueC,
        kExampleCppEnumValueCount,
    };

    struct ExampleStruct
    {
        // Structs should only contain data members and no functions.
        int m_ExampleInt;
        float m_ExampleFloat;
    };

    class ExampleClass
    {
    private:
        // Static Members
        static bool s_IsStaticExample;

        // Static Functions
        static bool GetIsExampleClass();

        // Private members go here
        bool m_IsExample;

    // The public and private for constructors/destructors can be interchanged
    // freely but try to keep the line count down.
    public:
        // Public members go here. If no public members can be ignored.
        bool m_IsPublicExample;
        int m_Counter;

        ExampleClass() = default;
        ~ExampleClass() = default;

    // Valid but could be moved above the previous public section.
    private:
        // Should have a specific copy and move constructor and '=' operators
        // defined.
        ExampleClass(const ExampleClass& other);
        ExampleClass(ExampleClass&& other);
        // Copy and Move '=' Operators should go directly below their relevant
        // constructors
        ExampleClass& operator=(const ExampleClass& other);
        ExampleClass& operator=(ExampleClass&& other);

    public:
        // Public functions go here
        void ExamplePublicFunction();

        /// - Getters may be implemented in the class definition iff they are not more than one line long.
        /// - Getters should be marked as const to ensure they do not modify the class.
        bool IsExampleClass() const { return m_IsExample; }

        // Template functions should be defined in the header
        // file. If the definition is less than one line then
        // it's okay to put in the class declaration
        template <typename T>
        T& TemplatedOneLineFunction(T& input) { return input; }

        // Otherwise it should go below the class declaration
        // in the header file.
        template <typename T>
        T& TemplatedMultiLineFunction(T& input);

    private:
        // Private functions go here
        void ExamplePrivateFunction();

    // Operator overrides that are not rule of 5 operators go at the bottom.
    public:
        // Especially if the operator is defined in the header/class
        // declaration.
        constexpr auto operator<=>(const ExampleClass& other) const { return *this <=> other; }
    };

    template <typename T>
    T& ExampleClass::TemplatedMultiLineFunction(T& input)
    {
        /**
         * Multiline code here...
         *
         *
         *
         *
         */
        return input;
    }
} // namespace savanna::CodingStandard

