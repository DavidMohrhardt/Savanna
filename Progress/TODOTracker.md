# TODO

Total TODOs: 104
## Src

### CMakeLists.txt

- [L5 :# TODO @DavidMohrhardt - Make multiple scripts for different platforms](../Src/CMakeLists.txt#L5)

### CMakeLists.txt.old

- [L5 :# TODO @DavidMohrhardt - Make multiple scripts for different platforms](../Src/CMakeLists.txt.old#L5)
- [L14 :# TODO @DavidMohrhardt - For some reason this triggers a configuration change that causes the project to rebuild every time. Disabling for now.](../Src/CMakeLists.txt.old#L14)

### DX12Application.cpp

- [L3 :* @brief TODO @DavidMohrhardt Document](../Src/Applications/DX12Renderer/DX12Application.cpp#L3)

### DX12Application.h

- [L3 :* @brief TODO @DavidMohrhardt Document](../Src/Applications/DX12Renderer/DX12Application.h#L3)

### WindowsRenderWindow.cpp

- [L3 :* @brief TODO @DavidMohrhardt Document](../Src/Applications/DX12Renderer/Window/WindowsRenderWindow.cpp#L3)

### WindowsRenderWindow.h

- [L3 :* @brief TODO @DavidMohrhardt Document](../Src/Applications/DX12Renderer/Window/WindowsRenderWindow.h#L3)

### CMakeLists.txt

- [L48 :# TODO Make find Vulkan QUIET and enable the runtime loader for vulkan in the case it's not just defined](../Src/Applications/VulkanRenderer/CMakeLists.txt#L48)

### VulkanApplication.cpp

- [L3 :* @brief TODO @DavidMohrhardt Document](../Src/Applications/VulkanRenderer/VulkanApplication.cpp#L3)
- [L114 :// TODO @DavidMohrhardt Allow for additional extensions to be added via queries](../Src/Applications/VulkanRenderer/VulkanApplication.cpp#L114)

### VulkanApplication.h

- [L3 :* @brief TODO @DavidMohrhardt Document](../Src/Applications/VulkanRenderer/VulkanApplication.h#L3)

### ISavannaJobs.h

- [L144 :* @brief TODO](../Src/Runtime/Concurrency/JobSystem/ISavannaJobs.h#L144)
- [L245 :// TODO @david.mohrhardt: Reimplement the temporary job with appropriate memory management.](../Src/Runtime/Concurrency/JobSystem/ISavannaJobs.h#L245)

### ComponentRegistry.cpp

- [L3 :* @brief TODO @DavidMohrhardt Document](../Src/Runtime/Entities/ComponentRegistry.cpp#L3)
- [L19 :#if SAVANNA_TODO_TEST_LOCKLESS_COMPONENT_REGISTRY](../Src/Runtime/Entities/ComponentRegistry.cpp#L19)
- [L29 :// TODO @DavidMohrhardt Consider moving to an atomic approach instead of a mutex.](../Src/Runtime/Entities/ComponentRegistry.cpp#L29)
- [L30 :#if SAVANNA_TODO_TEST_LOCKLESS_COMPONENT_REGISTRY](../Src/Runtime/Entities/ComponentRegistry.cpp#L30)
- [L32 :#endif // end SAVANNA_TODO_TEST_LOCKLESS_COMPONENT_REGISTRY](../Src/Runtime/Entities/ComponentRegistry.cpp#L32)
- [L42 :// TODO @DavidMohrhardt Perhaps can use a volatility lock instead of a mutex.](../Src/Runtime/Entities/ComponentRegistry.cpp#L42)
- [L43 :#if SAVANNA_TODO_TEST_LOCKLESS_COMPONENT_REGISTRY](../Src/Runtime/Entities/ComponentRegistry.cpp#L43)
- [L47 :#endif // end SAVANNA_TODO_TEST_LOCKLESS_COMPONENT_REGISTRY](../Src/Runtime/Entities/ComponentRegistry.cpp#L47)
- [L53 :// TODO @DavidMohrhardt Perhaps can use a volatility lock instead of a mutex.](../Src/Runtime/Entities/ComponentRegistry.cpp#L53)
- [L54 :#if SAVANNA_TODO_TEST_LOCKLESS_COMPONENT_REGISTRY](../Src/Runtime/Entities/ComponentRegistry.cpp#L54)
- [L58 :#endif // end SAVANNA_TODO_TEST_LOCKLESS_COMPONENT_REGISTRY](../Src/Runtime/Entities/ComponentRegistry.cpp#L58)
- [L64 :// TODO @DavidMohrhardt Perhaps can use a volatility lock instead of a mutex.](../Src/Runtime/Entities/ComponentRegistry.cpp#L64)
- [L65 :#if SAVANNA_TODO_TEST_LOCKLESS_COMPONENT_REGISTRY](../Src/Runtime/Entities/ComponentRegistry.cpp#L65)
- [L69 :#endif // end SAVANNA_TODO_TEST_LOCKLESS_COMPONENT_REGISTRY](../Src/Runtime/Entities/ComponentRegistry.cpp#L69)

### Entity.h

- [L3 :* @brief TODO @DavidMohrhardt Document](../Src/Runtime/Entities/Entity.h#L3)

### IComponentData.h

- [L3 :* @brief TODO @DavidMohrhardt Document](../Src/Runtime/Entities/IComponentData.h#L3)

### ISystem.h

- [L3 :* @brief TODO @DavidMohrhardt Document](../Src/Runtime/Entities/ISystem.h#L3)

### ComponentParadigm.cpp

- [L45 :// TODO @DavidMohrhardt Release memory](../Src/Runtime/Entities/Paradigms/ComponentParadigm.cpp#L45)
- [L136 :// TODO @DavidMohrhardt Remove this, component paradigms should be defined at construction. Reformatting layouts is unnecessary. as the all paradigms are the same size, instead just copy construct a new paradigm on top of the old one.](../Src/Runtime/Entities/Paradigms/ComponentParadigm.cpp#L136)
- [L150 :// TODO @DavidMohrhardt return an error code instead of throwing an exception.](../Src/Runtime/Entities/Paradigms/ComponentParadigm.cpp#L150)
- [L157 :// TODO @DavidMohrhardt consider making this asynchronous](../Src/Runtime/Entities/Paradigms/ComponentParadigm.cpp#L157)

### JacobianMatrix.cpp

- [L79 :// TODO](../Src/Runtime/Experimental~/Animation/IK/JacobianMatrix.cpp#L79)

### PhysicsComponent.cpp

- [L27 :// TODO](../Src/Runtime/Experimental~/Physics/PhysicsComponent.cpp#L27)
- [L41 :// TODO @DavidMohrhardt bring this code back up to speed](../Src/Runtime/Experimental~/Physics/PhysicsComponent.cpp#L41)

### PhysicsManager.cpp

- [L176 :// TODO](../Src/Runtime/Experimental~/Physics/PhysicsManager.cpp#L176)

### Assertions.h

- [L6 ://TODO](../Src/Runtime/Experimental~/Threading/Assertions.h#L6)
- [L10 :// TODO](../Src/Runtime/Experimental~/Threading/Assertions.h#L10)

### FiberAsmMacros.h

- [L93 :// TODO @davidmo](../Src/Runtime/Experimental~/Threading/Fibers/FiberAsmMacros.h#L93)

### CMakeLists.txt

- [L27 :# TODO Some of this code has OS specific code in it. Ensure it's properly excluded.](../Src/Runtime/Gfx/Vulkan/CMakeLists.txt#L27)

### VkRendererCreateInfo.h

- [L18 :// TODO @DavidMohrhardt - This is far too cumbersome. Implement a better structure creating both VK and DX12 renderers](../Src/Runtime/Gfx/Vulkan/VkRendererCreateInfo.h#L18)

### VkShaderCache.cpp

- [L42 :// TODO determine why this operator doesn't work as expected.](../Src/Runtime/Gfx/Vulkan/VkShaderCache.cpp#L42)

### VkSwapchain.cpp

- [L79 :// TODO @DavidMohrhardt: Get extents from renderer create info and pass them to the swapchain creation](../Src/Runtime/Gfx/Vulkan/VkSwapchain.cpp#L79)
- [L157 :// TODO @DavidMohrhardt - Enable setting up stereoscopic image views for VR rendering.](../Src/Runtime/Gfx/Vulkan/VkSwapchain.cpp#L157)

### VkPhysicalDeviceUtils.cpp

- [L3 :* @brief TODO @DavidMohrhardt Document](../Src/Runtime/Gfx/Vulkan/Utilities/VkPhysicalDeviceUtils.cpp#L3)
- [L98 :// TODO @DavidMohrhardt Just arbitrarily add 1 for everything and fine tune later](../Src/Runtime/Gfx/Vulkan/Utilities/VkPhysicalDeviceUtils.cpp#L98)
- [L162 :// TODO @DavidMohrhardt Just arbitrarily add the deviceDescsCount for everything and fine tune later](../Src/Runtime/Gfx/Vulkan/Utilities/VkPhysicalDeviceUtils.cpp#L162)

### VkSwapchainUtils.cpp

- [L18 :// TODO @DavidMohrhardt: For the time being prefer HDR formats over SDR formats.](../Src/Runtime/Gfx/Vulkan/Utilities/VkSwapchainUtils.cpp#L18)

### MathHelpers.h

- [L3 :* @brief TODO @DavidMohrhardt Document](../Src/Runtime/Math/MathHelpers.h#L3)

### MemoryManager.cpp

- [L8 :// TODO @DavidMohrhardt: Add initialization of memory manager based on a provided boot configuration.](../Src/Runtime/Memory/MemoryManager.cpp#L8)

### MemoryPool.h

- [L52 :// TODO @DavidMohrhardt: Practically this is probably fine but a faster to use a bit scan intrinsic. Work for the future.](../Src/Runtime/Memory/MemoryPool.h#L52)

### DynamicLibraryManager.h

- [L32 :// TODO currently unsupported. Libraries stay loaded for the duration of the program](../Src/Runtime/OS/DynamicLibraryManager.h#L32)

### ProfilerManager.h

- [L3 :* @brief TODO @DavidMohrhardt Document](../Src/Runtime/Profiling/ProfilerManager.h#L3)

### CacheLine.h

- [L3 :* @brief TODO @DavidMohrhardt Document](../Src/Runtime/Types/Memory/CacheLine.h#L3)

### ExpandableBlockAllocator.cpp

- [L18 :#if TODO_SPLIT_CONTIGUOUS_BUFFERS](../Src/Runtime/Types/Memory/ExpandableBlockAllocator.cpp#L18)
- [L95 :#if TODO_SPLIT_CONTIGUOUS_BUFFERS](../Src/Runtime/Types/Memory/ExpandableBlockAllocator.cpp#L95)

### FreeListAllocator.cpp

- [L3 :* @brief TODO @DavidMohrhardt Document](../Src/Runtime/Types/Memory/FreeListAllocator.cpp#L3)

### FreeListAllocator.h

- [L3 :* @brief TODO @DavidMohrhardt Document](../Src/Runtime/Types/Memory/FreeListAllocator.h#L3)

### Mallocator.h

- [L3 :* @brief TODO @DavidMohrhardt Document](../Src/Runtime/Types/Memory/Mallocator.h#L3)

### MemoryChunkDescriptors.h

- [L23 :// TODO: Instead of a pointer make this a uint32 offset from the start of the chunk](../Src/Runtime/Types/Memory/MemoryChunkDescriptors.h#L23)
- [L34 :// TODO: When MemoryChunkDescriptor is changed to a uint32 offset, change this to a uint32](../Src/Runtime/Types/Memory/MemoryChunkDescriptors.h#L34)

### FloatingPointUnions.h

- [L29 :* @brief TODO @DavidMohrhardt Document](../Src/Runtime/Types/Primitive/FloatingPointUnions.h#L29)
- [L58 :* @brief TODO @DavidMohrhardt Document](../Src/Runtime/Types/Primitive/FloatingPointUnions.h#L58)

### Console.h

- [L81 :// TODO](../Src/Runtime/Utilities/Console.h#L81)

### SavannaTestRunner.h

- [L3 :* @brief TODO @DavidMohrhardt Document](../Src/Tests/SavannaTestRunner.h#L3)

### CoreTestUtils.h

- [L3 :* @brief TODO @DavidMohrhardt Document](../Src/Tests/Core/CoreTestUtils.h#L3)
- [L16 :// TODO @DavidMohrhardt Replace with ifdefs when debugging is enabled](../Src/Tests/Core/CoreTestUtils.h#L16)

### MathTestUtils.h

- [L3 :* @brief TODO @DavidMohrhardt Document](../Src/Tests/Core/Math/MathTestUtils.h#L3)

### CacheLineTests.cpp

- [L47 :// TODO @DavidMohrhardt Parameterize this test https://google.github.io/googletest/advanced.html#value-parameterized-tests](../Src/Tests/Core/Memory/CacheLineTests.cpp#L47)

### MemoryArenaTests.cpp

- [L3 :* @brief TODO @DavidMohrhardt Document](../Src/Tests/Core/Memory/MemoryArenaTests.cpp#L3)

### MemoryTestUtils.h

- [L3 :* @brief TODO @DavidMohrhardt Document](../Src/Tests/Core/Memory/MemoryTestUtils.h#L3)

### AllocatorTestUtils.h

- [L3 :* @brief TODO @DavidMohrhardt Document](../Src/Tests/Core/Memory/Allocators/AllocatorTestUtils.h#L3)

### AllocatorUnitTestAdaptor.h

- [L3 :* @brief TODO @DavidMohrhardt Document](../Src/Tests/Core/Memory/Allocators/AllocatorUnitTestAdaptor.h#L3)

### AllocatorUtilsTests.cpp

- [L3 :* @brief TODO @DavidMohrhardt Document](../Src/Tests/Core/Memory/Allocators/AllocatorUtilsTests.cpp#L3)

### FreeListAllocatorTest.cpp

- [L3 :* @brief TODO @DavidMohrhardt Document](../Src/Tests/Core/Memory/Allocators/FreeListAllocatorTest.cpp#L3)

### ModuleTestUtils.h

- [L3 :* @brief TODO @DavidMohrhardt Document](../Src/Tests/Modules/ModuleTestUtils.h#L3)
- [L16 :// TODO @DavidMohrhardt Replace with ifdefs when debugging is enabled](../Src/Tests/Modules/ModuleTestUtils.h#L16)

### EntitiesTestUtils.h

- [L3 :* @brief TODO @DavidMohrhardt Document](../Src/Tests/Modules/Entities/EntitiesTestUtils.h#L3)

## BuildScripts

### SourceAggregatorUtilities.cmake

- [L161 :# TODO: Maybe collect any kind of source instead of just CPP/C files](../BuildScripts/CMake/Modules/SourceAggregatorUtilities.cmake#L161)

### SavannaArtifacts.py

- [L11 :#TODO](../BuildScripts/Python/SavannaArtifacts.py#L11)
- [L15 :# TODO change to read from artifact json](../BuildScripts/Python/SavannaArtifacts.py#L15)
- [L32 :# TODO @david.mohrhardt: For now we just skip existing folders but We may want to look into doing something else](../BuildScripts/Python/SavannaArtifacts.py#L32)
- [L35 :# TODO @david.mohrhardt: Argument parsing to specify the output folder](../BuildScripts/Python/SavannaArtifacts.py#L35)

### SavannaBuild.py

- [L5 :# TODO Add size limit](../BuildScripts/Python/SavannaBuild.py#L5)
- [L11 :# TODO](../BuildScripts/Python/SavannaBuild.py#L11)
- [L12 :parser.add_argument('--b', '--batchbuild', action="store_true", help="TODO Replace with build target.", dest='shouldBuild')](../BuildScripts/Python/SavannaBuild.py#L12)
- [L14 :# TODO](../BuildScripts/Python/SavannaBuild.py#L14)
- [L17 :# TODO](../BuildScripts/Python/SavannaBuild.py#L17)
- [L25 :# TODO Logging](../BuildScripts/Python/SavannaBuild.py#L25)
- [L33 :#TODO @david.mohrhardt: Update this to be in submodules instead of one monster python script](../BuildScripts/Python/SavannaBuild.py#L33)
- [L43 :# 2) Extract Artifacts if needed (TODO)](../BuildScripts/Python/SavannaBuild.py#L43)
- [L44 :# 3) Invoke CMake For Project Generation or Build (TODO)](../BuildScripts/Python/SavannaBuild.py#L44)

### SavannaCMake.py

- [L22 :# TODO @david.mohrhardt: Update to handle arguments like build vs project generation.](../BuildScripts/Python/SavannaCMake.py#L22)

### SavannaLogging.py

- [L45 :# TODO](../BuildScripts/Python/SavannaLogging.py#L45)

### SavannaPathUtils.py

- [L31 :# TODO: Potentially useful for Unit file generation?](../BuildScripts/Python/SavannaPathUtils.py#L31)

### SavannaPlatform.py

- [L6 :#TODO Update Build Platform class to handle generation of the required arguments and dependencies based on input meta files](../BuildScripts/Python/SavannaPlatform.py#L6)
- [L13 :#TODO](../BuildScripts/Python/SavannaPlatform.py#L13)
- [L20 :#TODO](../BuildScripts/Python/SavannaPlatform.py#L20)
- [L26 :#TODO](../BuildScripts/Python/SavannaPlatform.py#L26)
- [L50 :#TODO](../BuildScripts/Python/SavannaPlatform.py#L50)

### SavannaSubprocess.py

- [L20 :# TODO @david.mohrhardt: Add logging for other levels](../BuildScripts/Python/SavannaSubprocess.py#L20)

## Documentation

### Savanna.txt

- [L0 :TODO - ADD DOCUMENTATION](../Documentation/Savanna.txt#L0)

