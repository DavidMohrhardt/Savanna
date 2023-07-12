#include "JacobianMatrix.h"

JacobianMatrix::JacobianMatrix()
{
}

JacobianMatrix::JacobianMatrix(int numDOFE, int numDOFPhi)
    : m_numDOFEnd(numDOFE), m_numDOFPhi(numDOFPhi)
{
    /*if (numDOFE != numDOFPhi)
        m_pLinearJacobianMatrix = (glm::float32 *)calloc(m_numDOFEnd * m_numDOFPhi, sizeof(glm::float32));
    else {
        if (numDOFE == 2)

    }*/
    m_pLinearJacobianMatrix = (glm::float32 *)calloc(m_numDOFEnd * m_numDOFPhi, sizeof(glm::float32));
    m_pJacobianMatrix = &m_pLinearJacobianMatrix;
    m_pJacobianMatrix[0][1] = 1.f;
}


JacobianMatrix::~JacobianMatrix()
{
    if (m_pLinearJacobianMatrix)
        free(m_pLinearJacobianMatrix);
}

//void JacobianMatrix::computeJacobian(glm::vec4 e, JacobianKinematicJointSystem *pJointSys)
//{
//    glm::vec4 a, r;
//    glm::vec3 re3, a3;
//
//    //e = KinematicJoint::getEndEffectorWorldSpace();
//    // Fill up the matrix:
//    for (int i = 0; i < m_numDOFEnd; i++)
//    {
//        for (int j = 0; j < m_numDOFPhi; j++)
//        {
//            r = pJointSys->getRi(j);
//            a = pJointSys->getAi(j);
//            re3 = (e - r);
//            a3 = a;
//            glm::cross(a3, re3);
//        }
//    }
//}

void JacobianMatrix::invert(INVERSIONTYPE mode)
{
    switch (mode)
    {
    case PSUEDO:

        break;

    case TRANSPOSE:

        break;

    case TRUEINVERSE:

        break;

    default:
        // Do nothing
        break;
    }
}

void JacobianMatrix::transposeInversion()
{
}

void JacobianMatrix::psuedoInversion()
{
}

void JacobianMatrix::trueInversion()
{
    // TODO
}
