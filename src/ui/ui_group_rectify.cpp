#include "ui_group_rectify.h"
#ifdef WITH_QT_GUI
#include "ui_logger.h"
#include <QCheckBox>
#include <QPushButton>
#include <QLineEdit>
#include <QGroupBox>
#include <QFileDialog>
#include <QFileInfo>
#include "../def/cmd.h"
#include "../vision_manager.h"
// #include <libvisiongpu/gpu_algorithm_pipeline_manager.h>

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
    if (_chkBox->isChecked())
	{
        //pbtn_cam_param->setEnabled(true);
        //pbtn_view_cam_param->setEnabled(true);
        CMD::is_rectify = true;
        UILog(QString("Open rectification"));
	}
	else
	{
        //pbtn_cam_param->setEnabled(false);
        //pbtn_view_cam_param->setEnabled(false);
        CMD::is_rectify = false;
        UILog(QString("Close rectification"));
	}
}


void UIGroupRectify::onPushBtnCamParamClicked()
{
    QString cam_params_path = QFileDialog::getOpenFileName(this, "File Dialog", "../", "Camera Parameters File(*.yml *.csv *.sr)");
    if(cam_params_path.toStdString().empty()){
        return;
    }

    _lEdit_cam_params->setText(cam_params_path);

    UILog(QString("Load camera parameters: ") + cam_params_path);

    QFileInfo fileinfo(cam_params_path);
    QString filesuffix = fileinfo.suffix();

    if(VisionManager::getInstance()->loadCamParams(cam_params_path.toStdString())){
        _chkBox->setEnabled(true);
    }
}


#endif // WITH_QT_GUI
