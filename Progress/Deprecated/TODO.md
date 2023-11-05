# TODO

This is the functionality TODO. Focusing on Core functionality of the engine. It's not a roadmap as I have a tendency to bounce around when working on this thing but should be slowly burnt down over time.

## Work Queue

1. [CMEM0001]
1. [CMEM0002]
1. [CCON0001]
1. Basic Vulkan Renderer Application Drawing a Triangle
1. Compiling Custom Shaders
1. File loading utilites
1. Loading Materials
1. PBR Lighting Pipeline
1. Raytracing Support

## Functionality

### Core

#### Memory

1. Complete `FreeListAllocator` for tracking allocations from a pool of Engine owned memory. [CMEM0001]
1. Complete `StackAllocator` for short lived (1 frame) allocations from a pool of Engine owned memory. [CMEM0001]
1. Complete `LinearAllocator` for tracking allocations from a pool of Engine owned memory. [CMEM0003]
1. Reimplement the deprecated `StlAdaptor` to act as a "polymorphic" allocator for the STL Containers
1. [CMEM0003] Implement memory ID "poisoning" support for tracking down memory corruption
1. [CMEM0004] Create `ThreadLocalStorage` Allocators for handling thread specific memory allocations.

#### Containers

1. [CCON0001] define specialized version of STL containers to speed up development of other features (ie `vector`, `list`, `unordered_map`)

#### Utilites

1. File Streaming Utilities.

#### Threading

1. Define a `ThreadPool` class that can generate a set of threads that can be then started all on the same process
2. Create an `INativeJob` interface class for implementing jobs that a special engine owned thread pool can pick up for wide asynchronous processing.

#### Scripting

1. `C++` Public API Support
1. Explore `Python` scripting capabilities
1. Explore `Lua` scripting capabilities
1. Explore `C#` scripting capabilities

### Graphics

#### Vulkan

1. Basic Vulkan Renderer Application Drawing a Triangle
1. Compiling Custom Shaders
1. Loading Meshes
1. Loading Materials
1. PBR Lighting Pipeline
1. Raytracing Support

#### Abstraction

1. Abstract Vulkan implementation into an API that is can be used to interface with Vulkan and DX12

### Editor

- TBD