#include "ui_enhance_gamma.h"
#include "ui_logger.h"
#include "ui_global.h"
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QLabel>
GUI_NS_BEGIN
// #include <libvisiongpu/gpu_algorithm_pipeline_manager.h>

UIEnhanceGamma::UIEnhanceGamma()
{

}


UIEnhanceGamma::~UIEnhanceGamma()
{

}


UIEnhanceGamma* UIEnhanceGamma::getInstance()
{
	static UIEnhanceGamma ui_gamma;
	return &ui_gamma;
}


QBoxLayout* UIEnhanceGamma::create()
{
    if (!_hlayout)
	{
        _chkBox = new QCheckBox();
        _chkBox->setText("Gamma");
        _chkBox->setMinimumWidth(UI_CHECKBOX_WIDTH);
        _chkBox->setChecked(false);
        connect(_chkBox, &QCheckBox::stateChanged, this, &UIEnhanceGamma::onChkBoxGammaSelected);

		QLabel *lb_alpha = new QLabel();
		lb_alpha->setText("Alpha: ");

        _dspBox_alpha = new QDoubleSpinBox();
        _dspBox_alpha->setEnabled(false);
        _dspBox_alpha->setMaximum(1);
        _dspBox_alpha->setMinimum(0);
        _dspBox_alpha->setSingleStep(0.05);
        _dspBox_alpha->setFixedWidth(UI_SPINBOX_MAX_WIDTH);
        _dspBox_alpha->setValue(_prop.alpha);
        connect(_dspBox_alpha, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &UIEnhanceGamma::onDoubleSpinBoxAlphaValueChanged);

		QLabel *lb_ref_L = new QLabel();
		lb_ref_L->setText("Ref L: ");

        _dspBox_ref_L = new QDoubleSpinBox();
        _dspBox_ref_L->setEnabled(false);
        _dspBox_ref_L->setMaximum(1);
        _dspBox_ref_L->setMinimum(0);
        _dspBox_ref_L->setSingleStep(0.1);
        _dspBox_ref_L->setFixedWidth(UI_SPINBOX_MAX_WIDTH);
        _dspBox_ref_L->setValue(_prop.ref_L);
        connect(_dspBox_ref_L, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &UIEnhanceGamma::onDoubleSpinBoxRefLValueChanged);

        _hlayout = new QHBoxLayout();
        _hlayout->addWidget(_chkBox);
        _hlayout->addWidget(lb_alpha);
        _hlayout->addWidget(_dspBox_alpha);
        _hlayout->addWidget(lb_ref_L);
        _hlayout->addWidget(_dspBox_ref_L);
	}
    return _hlayout;
}


void UIEnhanceGamma::reset()
{
    _dspBox_alpha->setValue(_prop.alpha);
    _dspBox_ref_L->setValue(_prop.ref_L);
}


void UIEnhanceGamma::setProperty()
{
    float alpha = _dspBox_alpha->value();
    float ref_L = _dspBox_ref_L->value();

	UILogger::getInstance()->log(QString("Gamma: alpha = %1, reference L = %2").arg(alpha).arg(ref_L));

	//gpu::AlgoPipelineManager::getInstance()->setProperty(std::make_shared<gpu::GammaProperty>(alpha, ref_L));
}


void UIEnhanceGamma::onChkBoxGammaSelected()
{
    if (_chkBox->isChecked()) {
        _dspBox_alpha->setEnabled(true);
        _dspBox_ref_L->setEnabled(true);
		UILogger::getInstance()->log("Open gamma transformation.");
	}
	else {
        _dspBox_alpha->setEnabled(false);
        _dspBox_ref_L->setEnabled(false);
		UILogger::getInstance()->log("Close gamma transformation.");
	}
}


void UIEnhanceGamma::onDoubleSpinBoxAlphaValueChanged()
{
	setProperty();
}


void UIEnhanceGamma::onDoubleSpinBoxRefLValueChanged()
{
	setProperty();
}

GUI_NS_END
