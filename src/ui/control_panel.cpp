#include "control_panel.h"
#ifdef WITH_QT_GUI
#include "cmd.h"
#include "ui_logger.h"
#include "ui_group_image.h"
#include "ui_group_camera.h"
#include "ui_group_enhance.h"
#include "ui_group_rectify.h"
#include "../image/frame_displayer.h"
#include "../vision_manager.h"
#include <QFont>

QTimer* ControlPanel::timer_imshow = new QTimer();
QTimer* ControlPanel::timer_vdshow = new QTimer();

ControlPanel::ControlPanel(QWidget *parent)
    : QWidget(parent)
{
    setupUI();

    connect(timer_imshow, &QTimer::timeout, this, &ControlPanel::onTimerImshow);
    connect(timer_vdshow, &QTimer::timeout, this, &ControlPanel::onTimerVdshow);
}


ControlPanel::~ControlPanel()
{
}


void ControlPanel::setupUI()
{
    /** Set the size the widget **/
    this->setWindowTitle("Vision Control Panel");
    this->resize(700, 400);
    /** Set the font style **/
    QFont font;
    font.setFamily(QStringLiteral("Arial"));
    font.setPointSize(9);
    this->setFont(font);
	//this->setStyleSheet(QString("background-color:rgb(30,30,30);color:rgb(240,240,240);"));
	

    /* Initialize others */
    QBoxLayout* vlayout = new QVBoxLayout();
    vlayout->setSpacing(20);
//	vlayout->setSizeConstraint(vlayout->SetFixedSize);

    /** CONTROL **/
    vlayout->addWidget(UIGroupImage::getInstance()->create());

    /** CAMERA **/
    vlayout->addWidget(UIGroupCamera::getInstance()->create());

    /** RECTIFY **/
    vlayout->addWidget(UIGroupRectify::getInstance()->create());

    /** ENHANCE **/
    vlayout->addWidget(UIGroupEnhance::getInstance()->create());

    
    /** LOGGOR **/
    QBoxLayout* hlayout = new QHBoxLayout();
    hlayout->setContentsMargins(10, 5, 10, 10);
    hlayout->setSpacing(10);
    hlayout->addWidget(UILogger::getInstance()->getTxtBrowser());
    hlayout->addLayout(vlayout);

    this->setLayout(hlayout);
}

void ControlPanel::onTimerImshow()
{
    VisionManager::getInstance()->showImage();
}

void ControlPanel::onTimerVdshow()
{
    if(!VisionManager::getInstance()->showVideo()){
        timer_vdshow->stop();
    }
}

#endif // WITH_QT_GUI
