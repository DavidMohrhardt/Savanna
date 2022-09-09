#include "./CCDFunctions.h"

#define MAXRADIANS 6.24828f
#define LOCALBASEPOS glm::vec4(0.0, 0.0, 0.0, 1.0)

namespace CCD
{
    inline glm::vec3 NormalizeWithExtendedPrecision(glm::vec3 normal)
    {
        glm::double1 x, y, z, magnitude;
        glm::vec3 extendedPrecisionsNormal;

        x = normal.x;
        y = normal.y;
        z = normal.z;

        magnitude = sqrt((x * x + y * y + z * z));
        x /= magnitude;
        y /= magnitude;
        z /= magnitude;

        extendedPrecisionsNormal.x = x;
        extendedPrecisionsNormal.y = y;
        extendedPrecisionsNormal.z = z;

        return extendedPrecisionsNormal;
    }

    /*Returns a float in radians for the rotation*/
    inline CCDAxisAngle SingleJointCCD(glm::vec4 jointTargetPosition, glm::vec4 jointEndEffectorPosition, glm::vec4 jointBase)
    {
        glm::vec3 baseToEnd, baseToTarget;
        baseToEnd = NormalizeWithExtendedPrecision(jointEndEffectorPosition - jointBase);
        baseToTarget = NormalizeWithExtendedPrecision(jointTargetPosition - jointBase);

        glm::double1 prod = glm::dot(baseToEnd, baseToTarget);
        glm::double1 arcCos = glm::acos(prod);

        glm::vec3 resultantVector = glm::cross(baseToEnd, baseToTarget);

        CCDAxisAngle data;

        data.m_thetaRadians = arcCos;
        data.m_rotationVector = resultantVector;
        data.m_quaternion = glm::quat(arcCos, resultantVector);

        return data;
    }


    int ComputeCCD(
        glm::mat4 * jointMatrices,
        glm::mat4 worldMat,
        glm::vec4 localSpaceEndEffector,
        glm::vec4 targetPosition,
        glm::float32 deltaTime,
        int numLinks
    )
    {
        CCDAxisAngle data;
        glm::mat4 worldToJointSpace, endEffectorToJoint;
        glm::vec4 jointSpaceTarget, jointSpaceEndEffector;

        // Work backwards
        for (int i = numLinks - 1; i >= 0; i--)
        {
            worldToJointSpace = endEffectorToJoint = glm::mat4(1.0);

            for (int j = numLinks - 1; j >= 0; j--)
            {
                if (j > i)
                    endEffectorToJoint = jointMatrices[j] * endEffectorToJoint;
                else
                    worldToJointSpace = jointMatrices[j] * worldToJointSpace;

            }

            worldToJointSpace = glm::inverse(worldToJointSpace);
            jointSpaceTarget = worldToJointSpace * targetPosition;
            jointSpaceEndEffector = endEffectorToJoint * localSpaceEndEffector;

            data = SingleJointCCD(jointSpaceTarget, jointSpaceEndEffector, LOCALBASEPOS);

            bool isNanBool = isnan(data.m_thetaRadians) || isnan(glm::length2(data.m_rotationVector));
            bool isZero = (data.m_thetaRadians == 0.f) || glm::length2(data.m_rotationVector) == 0;
            if (isNanBool || isZero)
                continue;

            jointMatrices[i] =
                glm::rotate(
                    jointMatrices[i],
                    glm::degrees(data.m_thetaRadians * deltaTime),
                    data.m_rotationVector
                );


            bool isnanMat;

            for (int j = 0; j < 4; j++)
                for(int k = 0; k < 4; k++)
                {
                    isnanMat = isnan(jointMatrices[i][j][k]);
                    SAVANNA_ASSERT(!isnanMat);
                }
        }

        return 1;
    }



}
