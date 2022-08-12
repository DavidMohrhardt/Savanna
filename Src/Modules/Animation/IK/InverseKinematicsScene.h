#pragma once

#include <ctime>

#include "./InverseKinematicsBufferStructs.h"
#include "./InverseKinematicsShaderTechnique.h"
#include "./KinematicJointSystem.h"

// Include the particle system state enum
#include "ParticleSystemStateEnums.h"

#ifndef VERT_PATH
#define VERT_PATH "./gpuPrograms/IKVertShader.glsl"
#endif
#ifndef FRAG_PATH
#define FRAG_PATH "./gpuPrograms/IKFragShader.glsl"
#endif

#define DEFAULTNUMLINKS 3

struct InverseKinematicsScene
{
    InverseKinematicsScene();
    ~InverseKinematicsScene();

    // Input Related functions
    void setAttractorPlaneZ(int direction);
    void setSimulationSpeed(int speed);
    void setNumberOfLinks(int link);
    void placeTarget(double screenX, double screenY);

    int initScene(float FoV, float aspectRatio, int resolutionWidth, int resolutionHeight);

    void display();

private:
    // A collection of data needed to run the simulation
    InverseKinematicsShaderTechnique        m_shaderTech;

    // The Kinematic Joints
    KinematicJointSystem*                    m_jointSystem;

    // Universal Matrices
    glm::mat4                                m_modelViewProjection;
    glm::mat4                                m_normalMatrix;
    glm::mat4                                m_viewProjMat;
    glm::mat4                                m_view;
    glm::mat4                                m_projection;

    // Plane positioning
    glm::float32                            m_zPlanePosition;

    // Clock related data
    clock_t                                    m_beginTime;
    clock_t                                    m_lastFrame;
    clock_t                                    m_currentFrame;
    glm::float32                            m_deltaTime;

    // State Related ID's
    TIMESTATE                                m_deltaTimeState;

    // Additional items
    int                                        m_resolutionW;
    int                                        m_resolutionH;
};

