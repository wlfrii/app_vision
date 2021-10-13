#include "../vision_gui.h"
#include "gui_event.h"

int main(int argc, char* argv[])
{
    gui::registerEventCallback(onGUIEvent);
    return gui::runVisionGUI(argc, argv);
}