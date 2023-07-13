#include "KinematicJointSystem.h"

///////////////////////////////////////////////////////////////
// KinematicJointSystem
///////////////////////////////////////////////////////////////
KinematicJointSystem::KinematicJointSystem(glm::vec4 rigidSupport, unsigned int numLinks)
    : m_rigidSupport(rigidSupport), m_numLinks(numLinks), m_maxLinks(32)
{
    initializeVertexPositions(rigidSupport, numLinks);
}

KinematicJointSystem::~KinematicJointSystem()
{
    if (m_vertices)
        free(m_vertices);
    if (m_indices)
        free(m_indices);
    if (m_normals)
        free(m_normals);
    if (m_jointMats)
        free(m_jointMats);

}

void KinematicJointSystem::initializeVertexPositions(glm::vec4 rigidSupport, unsigned int numLinks)
{
#if HARDSHADING
    m_numVertices = 24;
    m_numIndices = 0;
    { // Allocate Memory for the vertices indices joints and joint matrices
        m_vertices = (KinematicVertex *)calloc(m_numVertices, sizeof(KinematicVertex));
        if (!m_vertices)
            exit(EXIT_FAILURE);

        m_normals = (KinematicNormal *)calloc(m_numVertices, sizeof(KinematicNormal));
        if (!m_normals)
        {
            free(m_vertices);
            //free(m_indices);
            exit(EXIT_FAILURE);
        }

        m_jointMats = (glm::mat4 *)calloc(m_maxLinks, sizeof(glm::mat4));
        if (!m_jointMats)
        {
            free(m_vertices);
            //free(m_indices);
            free(m_normals);
            exit(EXIT_FAILURE);
        }

        /*m_jointAxisAngles = (CCD::CCDAxisAngle*)calloc(m_maxLinks, sizeof(CCD::CCDAxisAngle));
        if (!m_jointAxisAngles)
        {
            free(m_vertices);
            free(m_normals);
            free(m_jointList);
            exit(EXIT_FAILURE);
        }*/

        /*m_jointToWorldMats = (glm::mat4 *)calloc(m_maxLinks, sizeof(glm::mat4));
        if (!m_jointMats)
        {
            free(m_vertices);
            free(m_normals);
            free(m_jointList);
            free(m_jointMats);
            exit(EXIT_FAILURE);
        }

        m_worldToJointMats = (glm::mat4 *)calloc(m_maxLinks, sizeof(glm::mat4));
        if (!m_jointMats)
        {
            free(m_vertices);
            free(m_normals);
            free(m_jointList);
            free(m_jointMats);
            free(m_jointToWorldMats);
            exit(EXIT_FAILURE);
        }*/
    }
#else
    m_numVertices = 6;
    m_numIndices = 8;
    { // Allocate Memory for the vertices indices joints and joint matrices
        m_vertices = (KinematicVertex *)calloc(m_numVertices, sizeof(KinematicVertex));
        if (!m_vertices)
            exit(EXIT_FAILURE);

        m_indices = (KinematicIndex *)calloc(m_numIndices, sizeof(KinematicIndex));
        if (!m_indices)
        {
            free(m_vertices);
            exit(EXIT_FAILURE);
        }

        m_normals = (KinematicNormal *)calloc(m_numVertices, sizeof(KinematicNormal));
        if (!m_indices)
        {
            free(m_vertices);
            free(m_indices);
            exit(EXIT_FAILURE);
        }

        m_jointList = (KinematicJoint *)calloc(m_maxLinks, sizeof(KinematicJoint));
        if (!m_jointList)
        {
            free(m_vertices);
            free(m_indices);
            free(m_normals);
            exit(EXIT_FAILURE);
        }

        m_jointMats = (glm::mat4 *)calloc(m_maxLinks, sizeof(glm::mat4));
        if (!m_jointMats)
        {
            free(m_vertices);
            free(m_indices);
            free(m_normals);
            free(m_jointList);
            exit(EXIT_FAILURE);
        }

        /*m_jointToWorldMats = (glm::mat4 *)calloc(m_maxLinks, sizeof(glm::mat4));
        if (!m_jointMats)
        {
            free(m_vertices);
            free(m_normals);
            free(m_jointList);
            free(m_jointMats);
            exit(EXIT_FAILURE);
        }

        m_worldToJointMats = (glm::mat4 *)calloc(m_maxLinks, sizeof(glm::mat4));
        if (!m_jointMats)
        {
            free(m_vertices);
            free(m_normals);
            free(m_jointList);
            free(m_jointMats);
            free(m_jointToWorldMats);
            exit(EXIT_FAILURE);
        }*/

        m_jointRotationAngles = (glm::double1)calloc(m_maxLinks, sizeof(glm::double1));
        if (!m_jointRotationAngles)
        {
            free(m_vertices);
            free(m_normals);
            free(m_jointList);/*
            free(m_jointMats);
            free(m_jointToWorldMats);*/
            exit(EXIT_FAILURE);
        }
    }
#endif

    initVerticesIndicesAndNormals();

    // Generate initial matrices
    m_worldMat = glm::translate(
        glm::mat4(1.0),
        glm::vec3(rigidSupport.x, rigidSupport.y, rigidSupport.z)
    ); // Should be identity since rigid support is at 0

    glm::mat4 jointTransform = glm::translate(glm::mat4(1.0), glm::vec3(0.0, -1.0, 0.0));
    glm::quat jointQuaternion = glm::toQuat(glm::mat3(1.0));
    glm::mat4 transform = m_worldMat;

    m_jointMats[0] = glm::mat4(1.0);
    m_numLinks = 32;

    for (int i = 1; i < m_maxLinks; i++)
    {
        m_jointMats[i] = jointTransform;
    }

#if HARDSHADING
    m_endEffectorTarget = getWorldTransform(m_numLinks - 1) * m_vertices[8].position;
#else
    m_endEffectorTarget = getWorldTransform(m_numLinks - 1) * m_vertices[5];
#endif

}

glm::mat4 KinematicJointSystem::getWorldTransform(int index)
{
    glm::mat4 worldTransform(1.0);

    if (index >= m_numLinks || index >= m_maxLinks)
        return worldTransform;

    for (int i = index; i >= 0; i--)
        worldTransform = m_jointMats[i] * worldTransform;

    return worldTransform;
}

void KinematicJointSystem::adjustLinks(int link)
{
    if (m_numLinks + link < 0)
        return;

    glm::mat4 boneMat = glm::translate(glm::mat4(1.f), glm::vec3(0.f, -1.f, 0.f));

    if (m_numLinks + link > m_maxLinks)
    {
        // Need to realloc
        m_maxLinks *= 2;

        m_jointMats = (glm::mat4 *)realloc(m_jointMats, m_maxLinks * sizeof(glm::mat4));
        for (int i = m_numLinks; i < m_maxLinks; i++)
        {
            m_jointMats[i] = boneMat;
        }
    }
    m_numLinks+=link;
}

void KinematicJointSystem::setTarget(glm::vec4 rayCast)
{
    m_endEffectorTarget = rayCast;
}

#if HARDSHADING
void KinematicJointSystem::initVerticesIndicesAndNormals()
{
    glm::vec4 posZ, negZ, posX, negY, negX, origin;
    glm::vec4 intermediaryVec;
    glm::vec3 aToB, aToC, normal;

    posZ = glm::vec4(0.0, -0.25f, 0.5f, 1.0f); // Zs
    negZ = glm::vec4(0.0, -0.25f, -0.5f, 1.0f);

    posX = glm::vec4(0.25f, -0.25f, 0.0, 1.0f);    // CounterClockwise starting at X+
    negY = glm::vec4(0.0, -1.0, 0.0, 1.0f);
    negX = glm::vec4(-0.25f, -0.25f, 0.0, 1.0f);
    origin = glm::vec4(0.0, 0.0, 0.0, 1.0f);

    {
        { m_vertices[0] = { negZ }; m_vertices[1] = { posX }; m_vertices[2] = { origin }; }
        { m_vertices[3] = { negZ }; m_vertices[4] = { origin }; m_vertices[5] = { negX }; }
        { m_vertices[6] = { negZ }; m_vertices[7] = { negX }; m_vertices[8] = { negY }; }
        { m_vertices[9] = { negZ }; m_vertices[10] = { negY }; m_vertices[11] = { posX }; }
        { m_vertices[12] = { posZ }; m_vertices[13] = { negX }; m_vertices[14] = { origin }; }
        { m_vertices[15] = { posZ }; m_vertices[16] = { origin }; m_vertices[17] = { posX }; }
        { m_vertices[18] = { posZ }; m_vertices[19] = { posX }; m_vertices[20] = { negY }; }
        { m_vertices[21] = { posZ }; m_vertices[22] = { negY }; m_vertices[23] = { negX }; }
    }

    for (int i = 0; i < 24; i += 3) {
        // Compute the triangle properties
        intermediaryVec =  m_vertices[i + 1].position - m_vertices[i].position;
        aToB = glm::vec3(intermediaryVec);

        intermediaryVec = m_vertices[i + 2].position - m_vertices[i].position;
        aToC = glm::vec3(intermediaryVec);

        normal = glm::normalize(glm::cross(aToC, aToB));

        m_normals[i] = m_normals[i + 1] = m_normals[i + 2] = { glm::vec4(normal, 0.0) };
    }
}
#else
inline void KinematicJointSystem::initVerticesIndicesAndNormals()
{
        m_vertices[0] = { glm::vec4(0.0, -0.25f, -0.5f, 1.0f) }; // Zs
        m_vertices[1] = { glm::vec4(0.0, -0.25f, 0.5f, 1.0f) };

        m_vertices[2] = { glm::vec4(0.25f, -0.25f, 0.0, 1.0f) };    // CounterClockwise starting at X+
        m_vertices[3] = { glm::vec4(0.0, 0.0, 0.0, 1.0f) };
        m_vertices[4] = { glm::vec4(-0.25f, -0.25f, 0.0, 1.0f) };
        m_vertices[5] = { glm::vec4(0.0, -1.0, 0.0, 1.0f) };

        // front
        m_indices[0] = { glm::int3(0, 2, 3) }; m_indices[1] = { glm::int3(0, 3, 4) };
        m_indices[2] = { glm::int3(0, 4, 5) }; m_indices[3] = { glm::int3(0, 5, 2) };

        // back
        m_indices[4] = { glm::int3(1, 4, 3) }; m_indices[5] = { glm::int3(1, 3, 2) };
        m_indices[6] = { glm::int3(1, 2, 5) }; m_indices[7] = { glm::int3(1, 5, 4) };

        // Normals
        m_normals[0] = { glm::vec4(0.0, 0.0, 1.0, 0.0f) }; // Zs
        m_normals[1] = { glm::vec4(0.0, 0.0, -1.0, 0.0f) };

        m_normals[2] = { glm::vec4(1.0, 0.0, 0.0, 0.0f) };    // CounterClockwise starting at X+
        m_normals[3] = { glm::vec4(0.0, -1.0, 0.0, 0.0f) };
        m_normals[4] = { glm::vec4(1.0, 0.0, 0.0, 0.0f) };
        m_normals[5] = { glm::vec4(0.0, 1.0, 0.0, 0.0f) };
}
#endif




///////////////////////////////////////////////////////////////
// CCDKinematicJointSystem;
///////////////////////////////////////////////////////////////
CCDKinematicJointSystem::CCDKinematicJointSystem(glm::vec4 rigidSupport, unsigned int numLinks)
    : KinematicJointSystem(rigidSupport, numLinks)
{
}

CCDKinematicJointSystem::~CCDKinematicJointSystem()
{
}

void CCDKinematicJointSystem::calculateEndEffectorPosition(glm::float32 dt)
{
    glm::vec4 endEffectorPosition(0.0, -1.0, 0.0, 1.0);

    for (int i = 0; i < m_numLinks; i++)
    {
        endEffectorPosition = m_jointMats[i] * endEffectorPosition;
    }

    if ((endEffectorPosition - m_endEffectorTarget).length() > 0.5)
        CCD::ComputeCCD(
            m_jointMats,
            m_worldMat,
#if HARDSHADING
            m_vertices[8].position,
#else
            m_vertices[5].position,
#endif
            m_endEffectorTarget,
            dt,
            m_numLinks
        );
}



///////////////////////////////////////////////////////////////
// JacobianKinematicJointSystem
//
//    Unfinished
///////////////////////////////////////////////////////////////
JacobianKinematicJointSystem::JacobianKinematicJointSystem(glm::vec4 rigidSupport, unsigned int numLinks)
    : KinematicJointSystem(rigidSupport, numLinks)
{
}

JacobianKinematicJointSystem::~JacobianKinematicJointSystem()
{
}

glm::vec4 JacobianKinematicJointSystem::getRi(int index)
{
    glm::vec4 r;
    glm::mat4 jointTransform;

    //r = m_jointList[index].getBasePosition();

    for (int i = index; i >= 0; i--)
    {
        jointTransform *= m_jointMats[i];
    }

    r = jointTransform * r;

    return r;
}

glm::vec4 JacobianKinematicJointSystem::getAi(int index)
{
    glm::vec4 a(0.0, 0.0, -1.0, 0.0);
    glm::mat4 jointTransform;

    for (int i = index; i >= 0; i--)
    {
        jointTransform *= m_jointMats[i];
    }

    a = jointTransform * a;

    return a;
}

void JacobianKinematicJointSystem::calculateEndEffectorPosition(glm::float32 dt)
{
    if (!m_pJacobianMat)
        m_pJacobianMat = new JacobianMatrix(3, NUMLINKS);
    else
        m_pJacobianMat->resetMatrix();

    //m_pJacobianMat->computeJacobian(m_jointList);
}

