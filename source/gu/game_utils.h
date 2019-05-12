#ifndef GAME_UTILS_H
#define GAME_UTILS_H

#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "screen.h"

namespace gu
{

struct Config
{

    Config() : vsync(false), title("Game"), width(1600), height(900), showFPSInTitleBar(true), printOpenGLMessages(true) {}

    std::string title;
    int width, height;
    bool 
        vsync, 
        showFPSInTitleBar,
        printOpenGLMessages;
};

extern Config config;
extern GLFWwindow *window;
extern int width, height, widthPixels, heightPixels;

// initialize OpenGL, window, input and more. Returns false if initialization failed.
bool init(Config config);
void run();
void setScreen(Screen *screen);

}; // namespace gu

#endif