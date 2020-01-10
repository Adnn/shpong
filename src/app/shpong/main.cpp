#include <engine/Application.h>
#include <engine/Engine.h>
#include <engine/Timer.h>

#include "game.h"

constexpr int gWindowWidth{1280};
constexpr int gWindowHeight{1024};

int main(int argc, const char * argv[])
{
    try
    {
        ad::Application application("Shpong", gWindowWidth, gWindowHeight);

        ad::Timer timer{glfwGetTime(), 0.};
        std::unique_ptr<ad::Scene> scene = ad::setupScene(*application.mEngine);
        ad::MouseInput mouse;

        while(application.nextFrame())
        {
            {
                double x, y;
                glfwGetCursorPos(application.mWindow, &x, &y);
                bool isClicked =
                    glfwGetMouseButton(application.mWindow, GLFW_MOUSE_BUTTON_LEFT)==GLFW_PRESS;

                // Origin is top-left in glfw, and bottom-left in OpenGL: convert to OpenGL
                mouse.mPosition = {static_cast<GLfloat>(x),
                                   static_cast<GLfloat>(application.mEngine->getWindowSize().height() - y)};
                mouse.mClickEdge = (!mouse.mClick) && isClicked;
                mouse.mReleaseEdge = mouse.mClick && !isClicked;
                mouse.mClick = isClicked;
            }
            ad::updateScene(*scene, *application.mEngine, timer, mouse);
            ad::renderScene(*scene, *application.mEngine);

            timer.mark(glfwGetTime());
        }
    }
    catch(const std::exception & e)
    {
        std::cerr << "Exception:\n"
                  << e.what()
                  << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::exit(EXIT_SUCCESS);
}
