#pragma once
// System includes
#include <stdlib.h>

// Buffer Includes
#include "./InverseKinematicsBufferStructs.h"

// Math Library
#include "glm/ext.hpp"

///////////////////////////////////////////////////////////////
// KinematicJoint
///////////////////////////////////////////////////////////////

struct KinematicJoint
{
    KinematicJoint(unsigned int jointID, glm::mat4 * jointMat, KinematicJoint * pJointList, glm::mat4 jointToWorld, KinematicVertex base, KinematicVertex endEffector);
    ~KinematicJoint();

    void updateWorldPosition(glm::mat4 jointToWorld);

    glm::vec4 getBasePosition() { return m_basePos; }
    glm::vec4 getEndEffectorPosition() { return m_endEffectorPos; }
    glm::mat4 getJointMat4Ptr() { return *m_jointMat; }

private:
    // Indices and link ID
    unsigned int                m_jointID;

    // The CPU side data that governs where the vertices are
    glm::vec4                    m_jointVector;
    glm::vec4                    m_basePos;
    glm::vec4                    m_endEffectorPos;

    // JointMatrix
    const glm::mat4*            m_jointMat;

    // Is this connected to the immutable base
    bool                        m_isBase;

    // Base Joint;
    KinematicJoint*                m_baseJoint;
};
