#include "rendering.h"
#include <thread>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <iostream>
#include <condition_variable>
#include "ShaderProgram.h"
#include "GLobject.h"

using namespace std;
static const float M_PI = 3.14159265358979323846f;

static int width, hight;
static float aspectRatio;

static std::mutex m;
static std::condition_variable cv;

static bool windowTerminated = false;
static keyCallback g_keyActionFunc;

static glm::mat4 P, V;
static glm::vec3 cameraObserver(0, 15, -30.0f), cameraCenter(0, 0, 0), cameraNose(0, 5.0f, 0);



static void renderWindow(GLFWwindow *window)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    shProgram->use();

    //iteracja po stworzonych obiektach 3D
    for(GLobject *obj: GLobject::m_models)
    {
        GLuint texture;
        glm::mat4 M;
        glm::vec2 *textCoords;
        glm::vec4 *vertices, *normals;
        int verticesCount;

        texture = obj->getTexture();
        M = obj->getModelMatrix();
        textCoords = obj->getTextCoord();
        vertices = obj->getVertices();
        normals = obj->getNormals();
        verticesCount = static_cast<int>(obj->getVerticesCount());


        glUniformMatrix4fv(shProgram->getUniform("P"), 1, false, glm::value_ptr(P));
        glUniformMatrix4fv(shProgram->getUniform("V"), 1, false, glm::value_ptr(V));
        glUniformMatrix4fv(shProgram->getUniform("M"), 1, false, glm::value_ptr(M));
        glUniform4f(shProgram->getUniform("lp_1"), -40, 20, -10, 1);
        glUniform4f(shProgram->getUniform("lp_2"), 40, 20, -10, 1);

        glUniform1i(shProgram->getAttribute("textureMap"), 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glEnableVertexAttribArray(shProgram->getAttribute("vertex"));
        glVertexAttribPointer(shProgram->getAttribute("vertex"), 4, GL_FLOAT, false, 0, vertices);

        glEnableVertexAttribArray(shProgram->getAttribute("normal"));
        glVertexAttribPointer(shProgram->getAttribute("normal"), 4, GL_FLOAT, false, 0, normals);

        glEnableVertexAttribArray(shProgram->getAttribute("texCoord0"));
        glVertexAttribPointer(shProgram->getAttribute("texCoord0"), 2, GL_FLOAT, false, 0, textCoords);

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

        glDrawArrays(GL_TRIANGLES , 0, verticesCount);

        glDisableVertexAttribArray(shProgram->getAttribute("vertex"));
        glDisableVertexAttribArray(shProgram->getAttribute("normal"));
        glDisableVertexAttribArray(shProgram->getAttribute("texCoord0"));
    }

    glfwSwapBuffers(window);
}

static void glKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    g_keyActionFunc(key, action);
}

static void glWindowResizeCallback(GLFWwindow *window, int newWidth, int newHight)
{
    glViewport(0, 0, newWidth, newHight);

    aspectRatio = static_cast<float>(newHight) / static_cast<float>(newWidth);
    width = newWidth;
    hight = newHight;
}

static void glErrorCallback(int error, const char *description)
{
    cerr << description << endl;
}

static void initRendering(GLFWwindow *window, keyCallback keyActionFunc, scrollCallback scrollActionFunc)
{
    glClearColor(0, 0, 0.5f, 1);
    glEnable(GL_DEPTH_TEST);

    glfwSetWindowSizeCallback(window, glWindowResizeCallback);
    glfwSetErrorCallback(glErrorCallback);
    glfwSetKeyCallback(window, glKeyCallback);
    glfwSetScrollCallback(window, scrollActionFunc);

    shProgram = new ShaderProgram;
}

static void freeApplication(GLFWwindow *window)
{
    glfwSetErrorCallback(nullptr);

    glfwDestroyWindow(window);
    glfwTerminate();
    delete shProgram;

    windowTerminated = true;
}

static void renderingLoop(string title, int l_width, int l_hight, callback createObjectsFunc, keyCallback keyActionFunc, scrollCallback scrollActionFunc)
{
    g_keyActionFunc = keyActionFunc;
    width = l_width;
    hight = l_hight;
    aspectRatio = static_cast<float>(l_hight) / static_cast<float>(l_width);

    if(!glfwInit())
    {
        cerr << "Cannot initialize GLFW" << endl;
        exit(EXIT_FAILURE);
    }
    else cout << "Initialize GLFW successfully" << endl;

    GLFWwindow *window;
    window = glfwCreateWindow(width, hight, title.c_str(), nullptr, nullptr);

    if(!window)
    {
        cerr << "Cannot initialize window" << endl;
        exit(EXIT_FAILURE);
    }
    else cout << "Initialize window successfully" << endl;

    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK)
    {
        cerr << "Cannot initialize GLEW" << endl;
        exit(EXIT_FAILURE);
    }
    else cout << "Initialize GLEW successfully" << endl;

    initRendering(window, keyActionFunc, scrollActionFunc);

    createObjectsFunc();

    cv.notify_all();


    P = glm::perspective(50.0f * M_PI/180.0f, aspectRatio, 1.0f, 500.0f);

    V = glm::lookAt(cameraObserver, cameraCenter, cameraNose);

    while(!glfwWindowShouldClose(window))
    {
        renderWindow(window);
        glfwPollEvents();
    }

    freeApplication(window);
}

void startOpenGLRenderingThread(string title, int l_width, int l_hight, callback createObjectsFunc, keyCallback keyActionFunc, scrollCallback scrollActionFunc)
{
    std::unique_lock<std::mutex> lk(m);
    new thread(renderingLoop, title, l_width, l_hight, createObjectsFunc, keyActionFunc, scrollActionFunc);
    cv.wait(lk);

    cout << "Rendering thread creating successfully" << endl;
}

void cameraZoomIn()
{
    cameraObserver = cameraCenter + (cameraObserver - cameraCenter) * 0.5f;
    V = glm::lookAt(cameraObserver, cameraCenter, cameraNose);
}

void cameraZoomOut()
{
    cameraObserver = cameraCenter + (cameraObserver - cameraCenter) * 2.0f;
    V = glm::lookAt(cameraObserver, cameraCenter, cameraNose);
}

bool isWindowTerminated()
{
    return windowTerminated;
}
