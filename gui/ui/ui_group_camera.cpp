#include "ui_group_camera.h"
#include "ui_logger.h"
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QCheckBox>
#include <QGroupBox>
#include "ui_global.h"
#include "../vision_gui.h"

GUI_NS_BEGIN

UIGroupCamera::UIGroupCamera()
{

}

UIGroupCamera::~UIGroupCamera()
{

}

UIGroupCamera* UIGroupCamera::getInstance()
{
    static UIGroupCamera ui_capture;
	return &ui_capture;
}

QWidget* UIGroupCamera::create()
{
    if (!_gpBox_camera)
    {
        // Camera
        _pBtn_open_camera = new QPushButton();
        _pBtn_open_camera->setText("Open Camera");
        connect(_pBtn_open_camera, &QPushButton::clicked, this,
                &UIGroupCamera::onPushBtnCameraClicked);

        // FPS
        _chkBox_show_fps = new QCheckBox();
        _chkBox_show_fps->setText(tr("Show FPS"));
        connect(_chkBox_show_fps, &QCheckBox::stateChanged, this,
                &UIGroupCamera::onChkBoxFPSSelected);

        // Capture button
        _pBtn_capture = new QPushButton();
        _pBtn_capture->setText(tr("Capture Image"));
        connect(_pBtn_capture, &QPushButton::clicked, this,
                &UIGroupCamera::onPushBtnCaptureClicked);

        // Capture number
        QLabel* lb_capture_num = new QLabel();
        lb_capture_num->setText(tr("Capture number:"));
        _spBox_capture_num = new QSpinBox();
        _spBox_capture_num->setRange(1, 12);
        _spBox_capture_num->setValue(1);
        _spBox_capture_num->setSingleStep(1);
        _spBox_capture_num->setFixedWidth(50);

        // ---------
        QHBoxLayout* hlayout_camera = new QHBoxLayout();
        UI_FORMAT_LAYOUT_MARGINS(hlayout_camera);
        hlayout_camera->addWidget(_pBtn_open_camera);
        hlayout_camera->addWidget(_chkBox_show_fps);
        hlayout_camera->addWidget(_pBtn_capture);
        hlayout_camera->addWidget(lb_capture_num);
        hlayout_camera->addWidget(_spBox_capture_num);

        // Saved name
        QString name_tip = "If no name specified, "
                "timestamp will be used as the file name";
        QLabel* lb_saved_name = new QLabel();
        lb_saved_name->setText(tr("Saved file name:"));
        lb_saved_name->setToolTip(name_tip);
        _lEdit_save_name = new QLineEdit();
        _lEdit_save_name->setMaximumWidth(80);
        _lEdit_save_name->setToolTip(name_tip);

        // Saved path
        QLabel* lb_saved_path = new QLabel();
        lb_saved_path->setText(tr("Saved path:"));
        _lEdit_save_path = new QLineEdit();
        _lEdit_save_path->setText("./capture/");

        // --------
        QHBoxLayout* hlayout_capture = new QHBoxLayout();
        UI_FORMAT_LAYOUT_MARGINS(hlayout_capture);
        hlayout_capture->addWidget(lb_saved_name);
        hlayout_capture->addWidget(_lEdit_save_name);
        hlayout_capture->addWidget(lb_saved_path);
        hlayout_capture->addWidget(_lEdit_save_path);

        // --------
        QVBoxLayout* vlayout = new QVBoxLayout();
        UI_FORMAT_LAYOUT_MARGINS(vlayout);
        vlayout->addLayout(hlayout_camera);
        vlayout->addLayout(hlayout_capture);

        // --------
        _gpBox_camera = new QGroupBox();
        _gpBox_camera->setTitle("Camera Control");
        _gpBox_camera->setLayout(vlayout);
	}
    return _gpBox_camera;
}

void UIGroupCamera::onPushBtnCameraClicked()
{
    static bool is_open_camera = false;
    if(_pBtn_open_camera->text() == "Open Camera")
    {
        is_open_camera = true;
        _pBtn_open_camera->setText("Close Camera");
    }
    else{
        is_open_camera = false;
        _pBtn_open_camera->setText("Open Camera");
    }
    CALLBACK(EVENT_CAMERA_STATUS, static_cast<void*>(&is_open_camera));
}

void UIGroupCamera::onChkBoxFPSSelected()
{
    static bool is_show_fps = _chkBox_show_fps->isChecked();
    if (_chkBox_show_fps->isChecked()) {
        UILogger::getInstance()->log(QString("Show FPS."));
        is_show_fps = true;
    }
    else {
        UILogger::getInstance()->log(QString("Close FPS."));
        is_show_fps = false;
    }
    CALLBACK(EVENT_CAM_FPS_STATUS, static_cast<void*>(&is_show_fps));
}

void UIGroupCamera::onPushBtnCaptureClicked()
{
    if (captureinfo.finished) {
        captureinfo.is_capture = true;
        captureinfo.save_num = _spBox_capture_num->value();
        captureinfo.save_name = _lEdit_save_name->text().toStdString();
        captureinfo.save_path = _lEdit_save_path->text().toStdString();
        captureinfo.finished = false;

        UILog(QString("Capturing %1 image...").arg(captureinfo.save_num));
        CALLBACK(EVENT_CAPTURE, nullptr);
	}
	else
        UILog("Please wait for current capturing.");
}

GUI_NS_END
