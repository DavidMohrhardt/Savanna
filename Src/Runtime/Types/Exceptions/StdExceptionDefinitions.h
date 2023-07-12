#pragma once

#if __cplusplus
#include <stdexcept>

namespace Savanna
{
    using RuntimeErrorException = std::runtime_error;
    using BadAllocationException = std::bad_alloc;
    using BadArrayNewLengthException = std::bad_array_new_length;
} // namespace Savanna
#endif // __cplusplus
