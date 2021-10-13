#include "../vision_gui.h"
#include "ui_global.h"
#include "ui_logger.h"
#include <QApplication>
#include "control_panel.h"
#include <functional>

namespace gui {

int runVisionGUI(int argc, char* argv[])
{
    QApplication app(argc, argv);
    ControlPanel *panel = new ControlPanel();
    panel->show();

    return app.exec();
}

std::function<void(uint8_t, void*)> _callbackfunc;
void registerEventCallback(Callback callbackfunc)
{
    _callbackfunc = callbackfunc;
}

void log(const std::string& msg)
{
    UILog(QString::fromStdString(msg));
}

CaptureInfo captureinfo = { false, 1, "", "./capture", true };


void CALLBACK(unsigned char id, void* data)
{
    if(_callbackfunc){
        _callbackfunc(id, data);
    } else {
        UILog("No callback registered.");
    }
}

} // namespace::gui

