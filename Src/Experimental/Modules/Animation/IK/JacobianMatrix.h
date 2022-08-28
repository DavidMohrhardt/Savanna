#pragma once

#include "glm/ext.hpp"

#include "./KinematicJoint.h"

enum INVERSIONTYPE
{
    PSUEDO = 0,
    TRANSPOSE,
    TRUEINVERSE
};

struct JacobianMatrix
{
    JacobianMatrix();
    JacobianMatrix(int numDOFE, int numDOFPhi);

    ~JacobianMatrix();

    // Member Methods
    inline void resetMatrix()
    {
        memset(m_pLinearJacobianMatrix, 0, sizeof(glm::float32) * m_numDOFEnd * m_numDOFPhi);
    }

    //void computeJacobian(glm::vec4 e, JacobianKinematicJointSystem * pJointSys);

    void invert(INVERSIONTYPE mode);

    void transposeInversion();
    void psuedoInversion();
    void trueInversion();

private:
    glm::mat2 *                m_pJacobian2Mat;
    glm::mat3 *                m_pJacobian3Mat;
    glm::mat4 *                m_pJacobian4Mat;

    glm::float32 *            m_pLinearJacobianMatrix;
    glm::float32 **            m_pJacobianMatrix;

    int                        m_numDOFEnd;
    int                        m_numDOFPhi;
};

