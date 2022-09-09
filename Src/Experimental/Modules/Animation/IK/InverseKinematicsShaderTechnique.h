#pragma once

// System includes
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <vector>

// OpenGL headers include
#include "./openGL_headers.h"

// Shader Relate Includes
#include "./ShaderTypeEnumerations.h"
#include "./InverseKinematicsBufferStructs.h"
#include "./KinematicJointSystem.h"

struct InverseKinematicsShaderTechnique
{
    InverseKinematicsShaderTechnique();
    ~InverseKinematicsShaderTechnique();

    // Load the shaders
    int instantiateShaderTechnique(const char ** paths, const SHADERTYPE * shaderTypes, int numShaders);
    int loadShaderFromFile(const char * shaderPath, GLuint &shaderID);

    // Generate the buffer objects
    int generateBufferObjects(KinematicJointSystem *pSystem);

    // Get the shader program
    GLuint getRenderProgram() { return m_renderProgramID; }
    GLuint getRenderVAO() { return m_renderVAO; }
    GLint getMVPUniformLocation() { return m_mvpLocation; }
    GLint getNormalUniformLocation() { return m_normalMatLocation; }

private:
    // Programs IDs
    GLuint                m_renderProgramID;

    // Shader IDs
    GLuint                m_vertShaderID;
    GLuint                m_fragShaderID;

    // Buffer Object ID's
    GLuint m_renderVAO;

    // Locations
    GLint m_mvpLocation;
    GLint m_normalMatLocation;

    GLuint m_vertexBuf;
    GLuint m_indexBuf;
    GLuint m_normalBuf;
};

