#include <renderer/Error.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>

#include "paragame.h"

constexpr int gWindowWidth{1280};
constexpr int gWindowHeight{1024};

constexpr int gGLVersionMajor{4};
constexpr int gGLVersionMinor{1};


static void error_callback(int error, const char* description)
{
    std::cerr << "GLFW error: " << description << std::endl;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void windowsSize_callback(GLFWwindow * window, int width, int height)
{
    ad::Engine * engine = static_cast<ad::Engine *>(glfwGetWindowUserPointer(window));
    engine->callbackWindowSize(width, height);
}

int main(void)
{
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
    {
        throw std::runtime_error("Unable to initialize glfw");
    }
    Guard glfwInitGuard{glfwTerminate};

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gGLVersionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gGLVersionMinor);

    // macOS requirement (as well as not going above to OpenGL 4.1)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Only show the window after its size callback is set
    // so we cannot miss notifications
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    auto window = guard(glfwCreateWindow(gWindowWidth,
                                         gWindowHeight,
                                         "Shpong",
                                         NULL,
                                         NULL),
                        glfwDestroyWindow);
    if (!window)
    {
        throw std::runtime_error("Unable to initialize window or context");
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();

    ad::Engine engine;
    glfwSetWindowUserPointer(window, &engine);
    // Explicitly call it, because it is used to complete the engine setup
    {
        // Get the size, because the hints might not be satisfied
        // (yet not invoking the size callback)
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        windowsSize_callback(window, width, height);
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowSizeCallback(window, windowsSize_callback);

    glfwShowWindow(window);

    // VSync
    glfwSwapInterval(1);

    if (!GLAD_GL_KHR_debug)
    {
        std::cerr << "Debug output is not available."
                  << " Please run on a decent platform for debugging."
                  << std::endl;
    }
    else
    {
        ad::enableDebugOutput();
    }

    std::unique_ptr<ad::Scene> scene = ad::setupScene(engine);
    ad::Timer timer{glfwGetTime(), 0.};
    ad::MouseInput mouse;

    while(!glfwWindowShouldClose(window))
    {
        {
            double x, y;
            glfwGetCursorPos(window, &x, &y);
            bool isClicked = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)==GLFW_PRESS;

            // Origin is top-left in glfw, and bottom-left in OpenGL: convert to OpenGL
            mouse.mPosition = {x, engine.getWindowSize().height() - y};
            mouse.mClickEdge = (!mouse.mClick) && isClicked;
            mouse.mClick = isClicked;
        }

        ad::updateScene(*scene, engine, timer, mouse);
        ad::renderScene(*scene, engine);

        glfwSwapBuffers(window);
        glfwPollEvents();

        timer.mark(glfwGetTime());
    }

    std::exit(EXIT_SUCCESS);
}
