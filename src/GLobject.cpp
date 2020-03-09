#include "GLobject.h"
#include "ObjectLoader.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <ctime>
#include <thread>
#include <algorithm>

ObjectLoader GLobject::m_loader;
std::vector<GLobject*> GLobject::m_models;

GLobject::GLobject(glm::vec3 position, std::string verticesPath, std::string texturePath)
{
    m_angleSpeed = m_distSpeed = 0;
    m_rotateAxis = m_moveAxis = glm::vec3();
    m_position = glm::vec3(0);

    m_rotateMatrix = glm::mat4(1.0f);
    m_translatePos = glm::mat4(1.0f);
    m_loader.loadOBJ(const_cast<char*>(verticesPath.c_str()), m_vertices, m_uvs, m_normals, m_colors);

    m_texture = m_loader.loadTexture(const_cast<char*>(texturePath.c_str()));
    teleport(position.x, position.y, position.z);

    m_models.push_back(this);

    std::cout << "Object " << this << " created!" << std::endl;
}

glm::mat4 GLobject::getModelMatrix()
{
    return m_translatePos;
}

GLuint GLobject::getTexture()
{
    return m_texture;
}

glm::vec4* GLobject::getVertices()
{
    return m_vertices.data();
}

glm::vec4* GLobject::getNormals()
{
    return m_normals.data();
}

glm::vec2* GLobject::getTextCoord()
{
    return m_uvs.data();
}

size_t GLobject::getVerticesCount()
{
    return m_vertices.size();
}

void GLobject::scale(float scaleValueX, float scaleValueY, float scaleValueZ)
{
    std::unique_lock<std::mutex> actionLock(m_actionPerforming);
    std::unique_lock<std::mutex> drawingLock(m_changingValues);

        glm::vec3 axis;

        axis = glm::vec3(scaleValueX, scaleValueY, scaleValueZ);

        m_translatePos = glm::scale(m_translatePos, axis);
}

void GLobject::teleport(float x, float y, float z)
{
    std::unique_lock<std::mutex> actionLock(m_actionPerforming);
    std::unique_lock<std::mutex> drawingLock(m_changingValues);

        glm::vec3 axis;

        axis = glm::vec3(x, y, z);
        m_translatePos = glm::translate(m_translatePos, glm::vec3(-m_position.x, -m_position.y, -m_position.z));
        m_translatePos = glm::translate(m_translatePos, axis);
}

void GLobject::doAnimation(double diffTime)
{
    std::unique_lock<std::mutex> lk(m_changingValues);

        float angle;
        double distance;

        angle = m_angleSpeed * diffTime;
        distance = m_distSpeed * diffTime;

        if(angle)
        {//poprawic
            m_translatePos = glm::translate(m_translatePos, glm::vec3(-m_position.x, -m_position.y, -m_position.z));
            m_translatePos = glm::rotate(m_translatePos, angle, m_rotateAxis);
            m_translatePos = glm::translate(m_translatePos, glm::vec3(m_position.x, m_position.y, m_position.z));
        }

        if(distance)//poprawic aby dzialalo tak jak bylo poprzednio
        m_translatePos = glm::translate(m_translatePos, m_moveAxis);
}

GLobject::~GLobject()
{
    auto it = std::find(m_models.begin(), m_models.end(), this);
    m_models.erase(it);

    std::cout << "Object " << this << " destroyed!" << std::endl;
}
