#include "./KinematicJoint.h"

///////////////////////////////////////////////////////////////
// KinematicJoint
///////////////////////////////////////////////////////////////

// Static member variables

// Default constructor
KinematicJoint::KinematicJoint(
    unsigned int jointID,
    glm::mat4 * jointMat,
    KinematicJoint * pJointList,
    glm::mat4 jointToWorld,
    KinematicVertex base,
    KinematicVertex endEffector
)
    : m_jointID(jointID), m_jointMat(jointMat)
{
    if (jointID != 0)
        m_baseJoint = &pJointList[jointID - 1];
    m_basePos = base.position;
    m_endEffectorPos = endEffector.position;



    m_basePos = jointToWorld * m_basePos;
    m_endEffectorPos = jointToWorld * m_endEffectorPos;
    m_jointVector = m_endEffectorPos - m_basePos;

}

KinematicJoint::~KinematicJoint()
{
}

void KinematicJoint::updateWorldPosition(glm::mat4 jointToWorld)
{
    m_basePos = jointToWorld * m_basePos;
    m_endEffectorPos = jointToWorld * m_endEffectorPos;
    m_jointVector = m_endEffectorPos - m_basePos;
}
