#include "ui_group_rectify.h"
#include "ui_logger.h"
#include "ui_global.h"
#include <QCheckBox>
#include <QPushButton>
#include <QLineEdit>
#include <QGroupBox>
#include <QFileDialog>
#include <QFileInfo>

// #include <libvisiongpu/gpu_algorithm_pipeline_manager.h>

GUI_NS_BEGIN

UIGroupRectify::UIGroupRectify()
{

}


UIGroupRectify::~UIGroupRectify()
{

}


UIGroupRectify* UIGroupRectify::getInstance()
{
    static UIGroupRectify ui_rectify;
	return &ui_rectify;
}


QWidget* UIGroupRectify::create()
{
    if (!_gpBox_rectify)
    {
        // Load camera parameters button
        _pbtn_cam_params = new QPushButton();
        _pbtn_cam_params->setText(tr("Load CamParams"));
        _pbtn_cam_params->setFixedWidth(90);
        connect(_pbtn_cam_params, &QPushButton::clicked, this, &UIGroupRectify::onPushBtnCamParamClicked);

        _lEdit_cam_params = new QLineEdit();
        _lEdit_cam_params->setReadOnly(true);
		
        // A switch for retification
        _chkBox = new QCheckBox();
        _chkBox->setText(tr("Do Rectify"));
        _chkBox->setChecked(false);
        _chkBox->setMinimumWidth(80);
        _chkBox->setEnabled(false);
        connect(_chkBox, &QCheckBox::stateChanged, this, &UIGroupRectify::onChkBoxRectifySelected);
		
        // Add a layout for camera parameters
        QHBoxLayout* hlayout = new QHBoxLayout();
        hlayout->setSpacing(2);
        hlayout->setContentsMargins(2, 1, 2, 1);
        hlayout->addWidget(_chkBox);
        hlayout->addWidget(_pbtn_cam_params);
        hlayout->addWidget(_lEdit_cam_params);

        // --------
        _gpBox_rectify = new QGroupBox();
        _gpBox_rectify->setTitle("Rectification Control");
        _gpBox_rectify->setLayout(hlayout);
	}
    return _gpBox_rectify;
}


void UIGroupRectify::setProperty()
{
	
    UILog(QString("Rectify: set map."));

	//gpu::AlgoPipelineManager::getInstance()->setProperty(std::make_shared<gpu::NonuniformProperty>(magnify, magnify0));
}


void UIGroupRectify::onChkBoxRectifySelected()
{
    static bool is_rectify = _chkBox->isChecked();
    if (_chkBox->isChecked())
	{
        is_rectify = true;
        UILog(QString("Open rectification"));
	}
	else
	{
        is_rectify = false;
        UILog(QString("Close rectification"));
	}
    CALLBACK(EVENT_RECTIFY_STATUS, static_cast<void*>(&is_rectify));
}


void UIGroupRectify::onPushBtnCamParamClicked()
{
    static QString cam_params_path;
    cam_params_path = QFileDialog::getOpenFileName(this, "File Dialog",
        cam_params_path.isEmpty() ? "../" : cam_params_path,
        "Camera Parameters File(*.yml *.csv *.sr)");
    if(cam_params_path.isEmpty()) return;

    _lEdit_cam_params->setText(cam_params_path);
    UILog("Load camera parameters: "+ cam_params_path);

    std::string path = cam_params_path.toStdString();
    CALLBACK(EVENT_LOAD_CAM_PARAMS, static_cast<void*>(&path));

    //if(0/*VisionManager::getInstance()->loadCamParams(cam_params_path.toStdString())*/){
    //    _chkBox->setEnabled(true);
    //}
}

GUI_NS_END
