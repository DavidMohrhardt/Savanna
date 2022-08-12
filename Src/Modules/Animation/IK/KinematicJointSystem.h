#pragma once

// System Includes
#include <vector>

#include "InverseKinematicsBufferStructs.h"

#include "KinematicJoint.h"

// CCD and Jacobian Implementations
#include "JacobianMatrix.h"
#include "CCDFunctions.h"

#define NUMLINKS 3

struct KinematicJointSystem
{
    KinematicJointSystem(glm::vec4 rigidSupport, unsigned int numLinks);
    virtual ~KinematicJointSystem();

    virtual void calculateEndEffectorPosition(glm::float32 dt) = 0;

    glm::mat4 getWorldTransform(int index);

    KinematicVertex * getVerticesPtr() { return m_vertices; }
    KinematicIndex * getIndicesPtr() { return m_indices; }
    KinematicNormal * getNormalsPtr() { return m_normals; }

    void adjustLinks(int link);
    void setTarget(glm::vec4 rayCast);

    glm::vec4 getCurrentWorldTarget() { return m_endEffectorTarget; }
    glm::mat4 getTargetMatrix() { return glm::translate(glm::mat4(1.f), glm::vec3(m_endEffectorTarget)); }

    int getNumberOfVertices() { return m_numVertices; }
    int getNumberOfIndices() { return m_numIndices; }
    int getNumberOfJoints() { return m_numLinks; }

protected:
    KinematicVertex*    m_vertices;
    KinematicIndex*        m_indices;
    KinematicNormal*    m_normals;

    int                    m_numVertices;
    int                    m_numIndices;

    // Number of links
    unsigned int        m_numLinks;
    unsigned int        m_maxLinks;

    glm::vec4            m_rigidSupport;
    glm::mat4            m_worldMat; // left as Identity
    glm::mat4*            m_jointMats; // Remember that these are joint matrices!!! mW2 = mJ0 * mJ1 * mJ2

    glm::vec4            m_endEffectorTarget;

private:
    void initializeVertexPositions(glm::vec4 rigidSupport, unsigned int numLinks);
    void initVerticesIndicesAndNormals();
};

///////////////////////////////////////////////////////////////
// CCDKinematicJointSystem
///////////////////////////////////////////////////////////////
struct CCDKinematicJointSystem : public KinematicJointSystem
{
    CCDKinematicJointSystem(glm::vec4 rigidSupport, unsigned int numLinks);
    ~CCDKinematicJointSystem();

    void calculateEndEffectorPosition(glm::float32 dt);

private:

};

///////////////////////////////////////////////////////////////
// JacobianKinematicJointSystem
///////////////////////////////////////////////////////////////
struct JacobianKinematicJointSystem : public KinematicJointSystem
{
    JacobianKinematicJointSystem(glm::vec4 rigidSupport, unsigned int numLinks);
    ~JacobianKinematicJointSystem();

    void calculateEndEffectorPosition(glm::float32 dt);

private:
    // Member methods
    glm::vec4 getRi(int i);
    glm::vec4 getAi(int i);

    // MemberVariables
    JacobianMatrix *    m_pJacobianMat;

};

