#include "InverseKinematicsScene.h"

InverseKinematicsScene::InverseKinematicsScene()
{
    m_jointSystem =
        new CCDKinematicJointSystem(glm::vec4(0.f, 0.f, 0.f, 1.f), DEFAULTNUMLINKS);
}


InverseKinematicsScene::~InverseKinematicsScene()
{
    delete m_jointSystem;
}

void InverseKinematicsScene::setAttractorPlaneZ(int direction)
{
    if (direction == 1)
    {
        if (m_zPlanePosition < 30.f)
            m_zPlanePosition++;
        else
            return;
    }
    else
    {
        if (m_zPlanePosition > -30.f)
            m_zPlanePosition--;
        else
            return;
    }
    glm::vec4 target = m_jointSystem->getCurrentWorldTarget();
    target.z = m_zPlanePosition;
    m_jointSystem->setTarget(target);
}

void InverseKinematicsScene::setSimulationSpeed(int speed)
{
    if (m_deltaTimeState == MANUAL)
    {
        if (speed == 1)
            m_deltaTime *= 10;
        else
            m_deltaTime *= 0.1f;
    }
}

void InverseKinematicsScene::setNumberOfLinks(int link)
{
    m_jointSystem->adjustLinks(link);
}

void InverseKinematicsScene::placeTarget(double screenX, double screenY)
{
    glm::vec4 viewport(0.f, 0.f, (float)m_resolutionW, (float)m_resolutionH);
    glm::vec3 screenPos(screenX, viewport.w - screenY, 0.f);
    glm::vec3 unproject = glm::unProject(screenPos, m_view, m_projection, viewport);
    glm::vec3 rayCast = unproject - glm::vec3(0.f, 0.f, -50.f);

    float scalar = (-50.0 - m_zPlanePosition) / rayCast.z;

    rayCast *= scalar;
    rayCast = -rayCast;
    rayCast.z = m_zPlanePosition;

    m_jointSystem->setTarget(glm::vec4(rayCast, 1.f));
}

int InverseKinematicsScene::initScene(float FoV, float aspectRatio, int resolutionWidth, int resolutionHeight)
{
    // Set the resolution
    m_resolutionW = resolutionWidth;
    m_resolutionH = resolutionHeight;

    // Generate the MVP matrix
    {
        glm::vec3 cameraPos(0.f, 0.f, -50.f);
        glm::vec3 cameraTarget(0.f, 0.f, 0.f);
        glm::vec3 upVec(0.f, 1.f, 0.f);

        m_view = glm::lookAt(cameraPos, cameraTarget, upVec);

        // Projection
        m_projection = glm::perspective(
            glm::radians(FoV),
            aspectRatio,
            0.1f,
            100.f
        );

        m_viewProjMat = m_projection * m_view;
    }

    // Initialize Shader technique
    // Compile Shader Techniques
    {
        char vertPath[] = VERT_PATH;
        char fragPath[] = FRAG_PATH;

        const char * pPaths[] = { vertPath, fragPath };
        const SHADERTYPE pTypes[] = { SHADERTYPE::VERTEX, SHADERTYPE::FRAGMENT };

        if (m_shaderTech.instantiateShaderTechnique(pPaths, pTypes, 2) == GL_FALSE)
        {
            exit(EXIT_FAILURE);
        }

        m_shaderTech.generateBufferObjects(m_jointSystem);
    }

    // Set the States
    {
        m_deltaTimeState = MANUAL;
        m_deltaTime = 0.00001f;
    }

    m_zPlanePosition = 0.f;

    // Initialize the last frame to the current time in clock ticks
    m_beginTime = m_lastFrame = clock();

    return 1;
}

void InverseKinematicsScene::display()
{
    glm::mat4 currentModelMat;

    // Set the clock
    m_currentFrame = clock();
    if (m_deltaTimeState == MANUAL)
    {
        if (m_deltaTime > 2.f)
            m_deltaTime = 2.f;
    }
    else {
        m_deltaTime = (float)((m_currentFrame - m_lastFrame) / CLOCKS_PER_SEC);

        // Don't let the simulation explode
        if (m_deltaTime > 0.01f)
            m_deltaTime = 0.01f;
        if (m_deltaTime <= 0.00000001f)
            m_deltaTime = 0.00000001f;
    }
    // Set the last frame to the current frame time
    m_lastFrame = m_currentFrame;

    // Perform the geometry draw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(.2, .2, .2, 1.0);
    glUseProgram(m_shaderTech.getRenderProgram());
    glBindVertexArray(m_shaderTech.getRenderVAO());
    //glBindBuffer(GL_ARRAY_BUFFER, m_shaderTech.getNormalBuffer());
    glDisable(GL_CULL_FACE);

    m_jointSystem->calculateEndEffectorPosition(m_deltaTime);

    // Compute the current transformation matrix
    for (int i = 0; i < m_jointSystem->getNumberOfJoints(); i++)
    {
        // Update the MVP matrix
        currentModelMat = m_jointSystem->getWorldTransform(i);
        m_normalMatrix = glm::transpose(glm::inverse(m_view * currentModelMat));
        m_modelViewProjection = m_viewProjMat * currentModelMat;

        glUniformMatrix4fv(m_shaderTech.getMVPUniformLocation(), 1, GL_FALSE, &(m_modelViewProjection[0][0]));
        glUniformMatrix4fv(m_shaderTech.getNormalUniformLocation(), 1, GL_FALSE, &(m_normalMatrix[0][0]));
#if HARDSHADING
        glDrawArrays(GL_TRIANGLES, 0, m_jointSystem->getNumberOfVertices());
#else
        // Do Index drawing
#endif
    }

    // Draw Target
    currentModelMat = m_jointSystem->getTargetMatrix();
    m_normalMatrix = glm::transpose(glm::inverse(m_view * currentModelMat));
    m_modelViewProjection = m_viewProjMat * currentModelMat;

    glUniformMatrix4fv(m_shaderTech.getMVPUniformLocation(), 1, GL_FALSE, &(m_modelViewProjection[0][0]));
    glUniformMatrix4fv(m_shaderTech.getNormalUniformLocation(), 1, GL_FALSE, &(m_normalMatrix[0][0]));
#if HARDSHADING
    glDrawArrays(GL_TRIANGLES, 0, m_jointSystem->getNumberOfVertices());
#else
    // Do Index drawing
#endif
}
