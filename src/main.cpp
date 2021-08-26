#include <string>
#include <memory>
#ifdef WITH_QT_GUI
#include <QApplication>
#include "./ui/control_panel.h"
#endif


int main(int argc, char *argv[])
{
#ifdef WITH_QT_GUI
	QApplication app(argc, argv);
	ControlPanel *panel = new ControlPanel();
	panel->show();

	return app.exec();
#else
	system("pause");
    return 0;
#endif
}
