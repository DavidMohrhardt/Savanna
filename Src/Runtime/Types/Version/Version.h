#pragma once

#include "Utilities/SavannaCoding.h"

typedef struct se_Version
{
    se_uint8 variant;
    se_uint8 major;
    se_uint8 minor;
    se_uint8 patch;
} se_Version;
// DECLARE_SAVANNA_NAMESPACED_CPP_TYPE_DEF(se_Version, Version);
