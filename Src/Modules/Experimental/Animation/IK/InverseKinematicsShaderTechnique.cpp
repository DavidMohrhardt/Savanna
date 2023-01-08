#include "InverseKinematicsShaderTechnique.h"

InverseKinematicsShaderTechnique::InverseKinematicsShaderTechnique()
{
}

InverseKinematicsShaderTechnique::~InverseKinematicsShaderTechnique()
{
}

/** Instantiate the shader techniques, adapted from the cited tutorial for general purpose
* Citation: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
*
*/
int InverseKinematicsShaderTechnique::instantiateShaderTechnique(const char ** paths, const SHADERTYPE * shaderTypes, int numShaders)
{
    // Perform Compilation
    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile the shaders
    for (int i = 0; i < numShaders; i++)
    {
        switch (shaderTypes[i])
        {
        case VERTEX:
            m_vertShaderID = glCreateShader(GL_VERTEX_SHADER);
            if (!loadShaderFromFile(paths[i], m_vertShaderID))
                return 0;
            break;

        case FRAGMENT:
            m_fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
            if (!loadShaderFromFile(paths[i], m_fragShaderID))
                return 0;
            break;

        default:
            // Do nothing as there should be no default
            break;
        }
    }

    // Create the program
    m_renderProgramID = glCreateProgram();

    glAttachShader(m_renderProgramID, m_vertShaderID);
    glAttachShader(m_renderProgramID, m_fragShaderID);

    // Link the program
    glLinkProgram(m_renderProgramID);

    // Check the program
    glGetProgramiv(m_renderProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(m_renderProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetProgramInfoLog(m_renderProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }

    m_mvpLocation = glGetUniformLocation(m_renderProgramID, "mvp");
    m_normalMatLocation = glGetUniformLocation(m_renderProgramID, "normalMat");

    // Detach the shader objects just incase some changes need to be made at compile time
    glDetachShader(m_renderProgramID, m_vertShaderID);
    glDetachShader(m_renderProgramID, m_fragShaderID);

    // Delete the shader objects
    glDeleteShader(m_vertShaderID);
    glDeleteShader(m_fragShaderID);

    return Result;
}

/** Loads a shader from a file, adapted from the cited tutorial for general purpose
* Citation: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
*
*/
int InverseKinematicsShaderTechnique::loadShaderFromFile(const char * shaderPath, GLuint &shaderID)
{
    // Load and compile
    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Get the actual strings for the shaders
    std::string shaderCode;
    std::ifstream shaderFileStream(shaderPath, std::ios::in);
    if (shaderFileStream.is_open())
    {
        std::stringstream sstr;
        sstr << shaderFileStream.rdbuf();
        shaderCode = sstr.str();
        shaderFileStream.close();
    }
    else
    {
        printf("Could not open path %s!\n", shaderPath);
        return 0;
    }

    // Compile Shader code
    {
        printf("Compiling shader : %s\n", shaderPath);
        char const * VertSourcePointer = shaderCode.c_str();
        glShaderSource(shaderID, 1, &VertSourcePointer, NULL);
        glCompileShader(shaderID);

        // Check Shader
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0) {
            std::vector<char> VertShaderErrorMessage(InfoLogLength + 1);
            glGetShaderInfoLog(shaderID, InfoLogLength, NULL, &VertShaderErrorMessage[0]);
            printf("%s\n", &VertShaderErrorMessage[0]);

            return 0;
        }
    }

    return shaderID;
}

/** Generate the Shader Technique's buffer objects
*        This is a one time call
*/
int InverseKinematicsShaderTechnique::generateBufferObjects(KinematicJointSystem * pSystem)
{
    KinematicVertex *pVerts = pSystem->getVerticesPtr();
    KinematicNormal *pNormals = pSystem->getNormalsPtr();

    glGenVertexArrays(1, &m_renderVAO);
    glBindVertexArray(m_renderVAO);

    // Get the basic CPU side vertices and indices
    glGenBuffers(1, &m_vertexBuf);
    glGenBuffers(1, &m_normalBuf);

    glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuf);
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(KinematicVertex) * pSystem->getNumberOfVertices(),
            &pVerts[0],
            GL_STATIC_DRAW
        );
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, m_normalBuf);
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(KinematicNormal) * pSystem->getNumberOfVertices(),
            &pNormals[0],
            GL_STATIC_DRAW
        );
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, NULL);

#if HARDSHADING
    // No need for an index buffer
#else
    glGenBuffers(1, &m_indexBuf);
    glBindBuffer(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(KinematicIndex) * pSystem->getNumberOfIndices(),
        pSystem->getIndicesPtr(),
        GL_STATIC_DRAW
    );
#endif



    return 1;
}
