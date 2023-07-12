#pragma once

#include "glm/ext.hpp"
#include "KinematicJoint.h"

namespace CCD
{
    struct CCDAxisAngle
    {
        glm::float32    m_thetaRadians;
        glm::vec3        m_rotationVector;
        glm::quat        m_quaternion;
    };

    inline glm::vec3 NormalizeWithExtendedPrecision(glm::vec3 normal);

    inline CCDAxisAngle SingleJointCCD(
        glm::vec4 jointTargetPosition,
        glm::vec4 jointEndEffectorPosition,
        glm::vec4 jointBase
    );


    int ComputeCCD(
        glm::mat4 * jointMatrices,
        glm::mat4 worldMat,
        glm::vec4 localSpaceEndEffector,
        glm::vec4 targetPosition,
        glm::float32 deltaTime,
        int numLinks
    );
};
