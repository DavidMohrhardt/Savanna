/**
 * @file JobSystemDescriptor.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-01-30
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <SavannaEngine.h>
#include <Utilities/SavannaCoding.h>

typedef struct seJobSystemDescriptor
{
    size_t activeThreadCount;
    size_t maxThreadCount;
} seJobSystemDescriptor;
