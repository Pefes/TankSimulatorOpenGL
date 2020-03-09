#include "rendering.h"
#include "GLobject.h"
#include "GLtank.h"

static int actualPressedKey = -1;
static GLmodels::GLtank *tank;
static GLobject *myfloor;


static void loop()
{
    while(!isWindowTerminated())
    {
        switch(actualPressedKey)
        {
            case GLFW_KEY_LEFT:
                tank->turretLeft();
            break;

            case GLFW_KEY_RIGHT:
                tank->turretRight();
            break;

            case GLFW_KEY_UP:
                tank->barrelUp();
            break;

            case GLFW_KEY_DOWN:
                tank->barrelDown();
            break;

            case GLFW_KEY_W:
                tank->forward();
            break;

            case GLFW_KEY_S:
                tank->backward();
            break;

            case GLFW_KEY_A:
                tank->left();
            break;

            case GLFW_KEY_D:
                tank->right();
            break;

            case GLFW_KEY_SPACE:
                tank->fire();
            break;
        }
    }
}

//tutaj tworzysz obiekty na stercie, nie mozna jeszcze ich modyfikowac
static void createObjects()
{
    tank = new GLmodels::GLtank();

    //utworz kartony

    new GLobject(glm::vec3(20.0f + 5.0f, 0, 30.0f + 5.0f), "models/box/box.obj", "models/box/tex/Wooden Crate_Crate_BaseColor.png");
    new GLobject(glm::vec3(20.0f + 5.0f, 0, 30.0f - 5.0f), "models/box/box.obj", "models/box/tex/Wooden Crate_Crate_BaseColor.png");
    new GLobject(glm::vec3(20.0f - 5.0f, 0, 30.0f + 5.0f), "models/box/box.obj", "models/box/tex/Wooden Crate_Crate_BaseColor.png");
    new GLobject(glm::vec3(20.0f - 5.0f, 0, 30.0f - 5.0f), "models/box/box.obj", "models/box/tex/Wooden Crate_Crate_BaseColor.png");

    //trawa
    myfloor = new GLobject(glm::vec3(0, -10.0f, 0), "models/floor/floor.obj", "models/floor/green.png");

    //lampy
    new GLobject(glm::vec3(-40.0f, 0, -40.0f), "models/lamp/rv_lamp_post_3.obj", "models/lamp/tex.png");
    new GLobject(glm::vec3(40.0f, 0, -40.0f), "models/lamp/rv_lamp_post_3.obj", "models/lamp/tex.png");
}

//tylko wybrane klawisze moga modyfikowac zmienna aktualnie wcisnietego przycisku
static void keyAction(int key, int action)
{
    if(action == GLFW_PRESS)
    {
        switch(key)
        {
            case GLFW_KEY_LEFT:
            actualPressedKey = key;
            break;

            case GLFW_KEY_RIGHT:
            actualPressedKey = key;
            break;

            case GLFW_KEY_UP:
            actualPressedKey = key;
            break;

            case GLFW_KEY_DOWN:
            actualPressedKey = key;
            break;

            case GLFW_KEY_W:
            actualPressedKey = key;
            break;

            case GLFW_KEY_S:
            actualPressedKey = key;
            break;

            case GLFW_KEY_A:
            actualPressedKey = key;
            break;

            case GLFW_KEY_D:
            actualPressedKey = key;
            break;

            case GLFW_KEY_SPACE:
            actualPressedKey = key;
            break;
        }
    }
    else if(action == GLFW_RELEASE)
    {
        switch(key)
        {
            case GLFW_KEY_LEFT:
            actualPressedKey = -1;
            break;

            case GLFW_KEY_RIGHT:
            actualPressedKey = -1;
            break;

            case GLFW_KEY_UP:
            actualPressedKey = -1;
            break;

            case GLFW_KEY_DOWN:
            actualPressedKey = -1;
            break;

            case GLFW_KEY_W:
            actualPressedKey = -1;
            break;

            case GLFW_KEY_S:
            actualPressedKey = -1;
            break;

            case GLFW_KEY_A:
            actualPressedKey = -1;
            break;

            case GLFW_KEY_D:
            actualPressedKey = -1;
            break;

            case GLFW_KEY_SPACE:
            actualPressedKey = -1;
            break;
        }
    }
}

//przyblizanie i oddalanie skrola prowadzi do zoomu kamery
static void scrollAction(GLFWwindow *window, double xoffset, double yoffset)
{
    if(yoffset > 0)
        cameraZoomIn();
    else if(yoffset < 0)
        cameraZoomOut();
}

int main()
{
    //rysowanie dzieje sie wspolbieznie z akcjami, symulacjami itp..
    startOpenGLRenderingThread("TankSimulator", 1200, 800, createObjects, keyAction, scrollAction);

    loop();

    return 0;
}
