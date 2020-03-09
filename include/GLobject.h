#ifndef GLOBJECT_H
#define GLOBJECT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <string>
#include <vector>
#include <mutex>
#include "rendering.h"

class ObjectLoader;
static void renderWindow(GLFWwindow *window);
static void renderingLoop(std::string title, int l_width, int l_hight, callback createObjectsFunc, keyCallback keyActionFunc, scrollCallback scrollActionFunc);


class GLobject
{
    public:
        glm::mat4 m_translatePos, m_rotateMatrix;
        glm::vec3 m_position;

        GLobject() = delete;
        GLobject(glm::vec3 position, std::string verticesPath, std::string texturePath);
        virtual ~GLobject();

        enum Rotate {DOWN, LEFT, RIGHT, UP};
        enum Move {BACKWARD, FORWARD, LEFTWARD, RIGHTWARD, UPWARD, DOWNWARD};

        void scale(float scaleValueX, float scaleValueY, float scaleValueZ);
        void teleport(float x, float y, float z);

    protected:
        static ObjectLoader m_loader;
        static std::vector<GLobject*> m_models;



        std::mutex m_changingValues, m_actionPerforming;
        double m_angleSpeed;
        double m_distSpeed;
        glm::vec3 m_rotateAxis, m_moveAxis;

        GLuint m_texture;
        std::vector<glm::vec4> m_vertices; //wspolrzedne wierzcholkow
        std::vector<glm::vec2> m_uvs; //wspolrzedne teksturowania
        std::vector<glm::vec4> m_normals; //wektory normalne
        std::vector<glm::vec4> m_colors; //kolory (wgl nieuzywane)

        //metody dla funkcji renderujacej
        glm::mat4 getModelMatrix();
        GLuint getTexture();
        glm::vec4* getVertices();
        glm::vec4* getNormals();
        glm::vec2* getTextCoord();
        size_t getVerticesCount();
        void doAnimation(double diffTime);


        friend void renderWindow(GLFWwindow *window);
        friend void renderingLoop(std::string title, int l_width, int l_hight, callback createObjectsFunc, keyCallback keyActionFunc, scrollCallback scrollActionFunc);
};

#endif // GLOBJECT_H
