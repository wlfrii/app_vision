#include <string>
#include <memory>
#ifdef WITH_QT_GUI
#include <QApplication>
#include "./ui/control_panel.h"
#endif
#include "vision_manager.h"

int main(int argc, char *argv[])
{
//    for(int i = 0; i < 3; i++){
//        cv::namedWindow("Test");
//        cv::Mat image = cv::imread("../data/WechatIMG13.png");
//        cv::imshow("Test", image);
//        cv::waitKey(0);

//        cv::destroyWindow("Test");
//        printf("DestroyWindow\n");
//    }



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
