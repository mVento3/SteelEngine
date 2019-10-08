#pragma once

#include "glm\glm.hpp"
#include "glm\gtx\quaternion.hpp"

namespace SteelEngine {

    class Quaternion : public glm::quat
    {
    private:

    public:
        Quaternion() :
            glm::quat(1, 0, 0, 0)
        {

        }

        Quaternion(float x, float y, float z, float w) :
            glm::quat(w, x, y, z)
        {

        }

        Quaternion(const glm::quat& quaternion) :
            glm::quat(quaternion)
        {

        }

        ~Quaternion()
        {

        }

        inline glm::vec3 GetForward() const { return glm::rotate(*this, glm::vec3(0, 0, 1)); }
        inline glm::vec3 GetBackward() const { return glm::rotate(*this, glm::vec3(0, 0, -1)); }
        inline glm::vec3 GetUp() const { return glm::rotate(*this, glm::vec3(0, 1, 0)); }
        inline glm::vec3 GetDown() const { return glm::rotate(*this, glm::vec3(0, -1, 0)); }
        inline glm::vec3 GetRight() const { return glm::rotate(*this, glm::vec3(1, 0, 0)); }
        inline glm::vec3 GetLeft() const { return glm::rotate(*this, glm::vec3(-1, 0, 0)); }
    };

}