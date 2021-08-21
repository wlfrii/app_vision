#include "ui_group_camera.h"
#ifdef WITH_QT_GUI
#include "ui_logger.h"
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QCheckBox>
#include <QGroupBox>
#include "cmd.h"
#include "ui_global.h"

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

        // FPS
        _chkBox_show_fps = new QCheckBox();
        _chkBox_show_fps->setText(tr("Show FPS"));
        connect(_chkBox_show_fps, &QCheckBox::stateChanged, this, &UIGroupCamera::onChkBoxFPSSelected);

        // Capture button
        _pBtn_capture = new QPushButton();
        _pBtn_capture->setText(tr("Capture Image"));
        connect(_pBtn_capture, &QPushButton::clicked, this, &UIGroupCamera::onPushBtnCaptureClicked);

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
        QLabel* lb_saved_name = new QLabel();
        lb_saved_name->setText(tr("Saved file name:"));
        _lEdit_save_name = new QLineEdit();
        _lEdit_save_name->setMaximumWidth(80);

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

void UIGroupCamera::onChkBoxFPSSelected()
{
    if (_chkBox_show_fps->isChecked()) {
        UILogger::getInstance()->log(QString("Show FPS."));
        CMD::is_show_fps = true;
    }
    else {
        UILogger::getInstance()->log(QString("Close FPS."));
        CMD::is_show_fps = false;
    }
}

void UIGroupCamera::onPushBtnCaptureClicked()
{
	if (CMD::capture.finished) {
		CMD::capture.is_take_photo = true;
        CMD::capture.save_num = _spBox_capture_num->value();
        CMD::capture.save_name = _lEdit_save_name->text().toStdString();
        CMD::capture.save_path = _lEdit_save_path->text().toStdString();
		CMD::capture.finished = false;

		UILogger::getInstance()->log(QString("Capturing %1 image...").arg(CMD::capture.save_num));
	}
	else
		UILogger::getInstance()->log(QString("Please wait for current capturing."));
}

#endif // WITH_QT_GUI
