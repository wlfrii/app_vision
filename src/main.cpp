#include <string>
#include <memory>
#ifdef WITH_QT_GUI
#include "../gui/vision_gui.h"
#endif


int main(int argc, char *argv[])
{
#ifdef WITH_QT_GUI
    return 0;//gui::runVisionGUI(argc, argv);
#else
	system("pause");
    return 0;
#endif
}
