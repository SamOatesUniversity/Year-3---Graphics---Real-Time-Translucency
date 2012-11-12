#include <crtdbg.h>
#include <cstdlib>

#include "Window.hpp"
#include "../helper/MyController.hpp"

/*
*	\brief The main entry point of the application
*/
int main(
		int argc,						//!< Number of arguments passed in at startup 
		char *argv[]					//!< The arguments passed
	)
{
    // enable debug memory checks
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    std::shared_ptr<MyController> controller(new MyController());
    std::shared_ptr<tyga::Window> window = tyga::Window::mainWindow();
    window->setController(controller);

    const int windowWidth = 1024;
    const int windowHeight = 576;
    const int noofSamples = 4;

    if (window->open(windowWidth, windowHeight, noofSamples, true)) 
	{
        while (window->isVisible()) {
            window->update();
        }
        window->close();
    }

    // pause to display any console debug messages
    system("PAUSE");
    return 0;
}
