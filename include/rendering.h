#ifndef RENDERING_H
#define RENDERING_H

#include <string>

struct GLFWwindow;

using callback = void(*)(void);
using keyCallback = void(*)(int, int);
using scrollCallback = void(*)(GLFWwindow*, double, double);

void startOpenGLRenderingThread(std::string title,
                                int width,
                                int hight,
                                callback createObjectsFunc,
                                keyCallback keyActionFunc,
                                scrollCallback scrollActionFunc);
bool isWindowTerminated();

void cameraZoomIn();
void cameraZoomOut();

#endif
