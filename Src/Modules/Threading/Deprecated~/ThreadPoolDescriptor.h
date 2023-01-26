/**
 * @file ThreadPoolDescriptor.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-01-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <SavannaEngine.h>
#include <Utilities/SavannaCoding.h>

typedef struct se_ThreadPoolDescriptor
{
    se_uint8 m_ThreadCount;
} se_ThreadPoolDescriptor;
DECLARE_SAVANNA_EXTENDED_NAMESPACED_CPP_TYPE_DEF(Savanna::Threading, se_ThreadPoolDescriptor, ThreadPoolDescriptor);
