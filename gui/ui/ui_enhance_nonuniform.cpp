#include "ui_enhance_nonuniform.h"
#include "ui_logger.h"
#include "ui_global.h"
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QLabel>
// #include <libvisiongpu/gpu_algorithm_pipeline_manager.h>
GUI_NS_BEGIN

UIEnhanceNonuniform::UIEnhanceNonuniform()
{

}


UIEnhanceNonuniform::~UIEnhanceNonuniform()
{

}


UIEnhanceNonuniform* UIEnhanceNonuniform::getInstance()
{
	static UIEnhanceNonuniform ui_nonuniform;
	return &ui_nonuniform;
}


QBoxLayout* UIEnhanceNonuniform::create()
{
    if (!_hlayout)
	{
        _hlayout = new QHBoxLayout();

        _chkBox = new QCheckBox();
        _chkBox->setText("Nonuniform");
        _chkBox->setMinimumWidth(UI_CHECKBOX_WIDTH);
        _chkBox->setChecked(false);
        connect(_chkBox, &QCheckBox::stateChanged, this, &UIEnhanceNonuniform::onChkBoxNonuniformSelected);

		QLabel *lb_magnify = new QLabel();
		lb_magnify->setText("Magnify: ");

        _dspBox_magnify = new QDoubleSpinBox();
        _dspBox_magnify->setMaximum(3.0);
        _dspBox_magnify->setMinimum(1.5);
        _dspBox_magnify->setValue(_prop.magnify);
        _dspBox_magnify->setSingleStep(0.1);
        _dspBox_magnify->setFixedWidth(UI_SPINBOX_MAX_WIDTH);
        _dspBox_magnify->setEnabled(false);
        connect(_dspBox_magnify, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &UIEnhanceNonuniform::onDoubleSpinBoxMagnifyValueChanged);

		QLabel *lb_magnify0 = new QLabel();
		lb_magnify0->setText("Magnify0: ");

        _dspBox_magnify0 = new QDoubleSpinBox();
        _dspBox_magnify0->setMaximum(1.3);
        _dspBox_magnify0->setMinimum(0.7);
        _dspBox_magnify0->setValue(_prop.magnify0);
        _dspBox_magnify0->setSingleStep(0.05);
        _dspBox_magnify0->setFixedWidth(UI_SPINBOX_MAX_WIDTH);
        _dspBox_magnify0->setValue(_prop.magnify0);
        _dspBox_magnify0->setEnabled(false);
        connect(_dspBox_magnify0, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &UIEnhanceNonuniform::onDoubleSpinBoxMagnify0ValueChanged);

		
        _hlayout->addWidget(_chkBox);
        _hlayout->addWidget(lb_magnify);
        _hlayout->addWidget(_dspBox_magnify);
        _hlayout->addWidget(lb_magnify0);
        _hlayout->addWidget(_dspBox_magnify0);
	}
    return _hlayout;
}


void UIEnhanceNonuniform::reset()
{

}


void UIEnhanceNonuniform::setProperty()
{
    float magnify = _dspBox_magnify->value();
    float magnify0 = _dspBox_magnify0->value();

	UILogger::getInstance()->log(QString("Nonuniform: magnify = %1, magnify0 = %2").arg(magnify).arg(magnify0));

	//gpu::AlgoPipelineManager::getInstance()->setProperty(std::make_shared<gpu::NonuniformProperty>(magnify, magnify0));
}


void UIEnhanceNonuniform::onChkBoxNonuniformSelected()
{
    if (_chkBox->isChecked()) {
        _dspBox_magnify->setEnabled(true);
        _dspBox_magnify0->setEnabled(true);
		UILogger::getInstance()->log("Open nonuniform adjutment.");
	}
	else {
        _dspBox_magnify->setEnabled(false);
        _dspBox_magnify0->setEnabled(false);
		UILogger::getInstance()->log("Close nonuniform adjutment.");
	}
}


void UIEnhanceNonuniform::onDoubleSpinBoxMagnifyValueChanged()
{
	setProperty();
}


void UIEnhanceNonuniform::onDoubleSpinBoxMagnify0ValueChanged()
{
	setProperty();
}

GUI_NS_END
