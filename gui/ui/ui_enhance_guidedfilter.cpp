#include "ui_enhance_guidedfilter.h"
#include "ui_logger.h"
#include "ui_global.h"
// #include <libvisiongpu/gpu_algorithm_pipeline_manager.h>
#include <QSlider>
#include <QCheckBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QSpacerItem>
#include <QLabel>
GUI_NS_BEGIN

UIEnhanceGuidedFilter::UIEnhanceGuidedFilter()
{
	init = { 0.2, 16, 4 };
}

UIEnhanceGuidedFilter::~UIEnhanceGuidedFilter()
{
}

UIEnhanceGuidedFilter* UIEnhanceGuidedFilter::getInstance()
{
	static UIEnhanceGuidedFilter ui_guided_filter;
	return &ui_guided_filter;
}


QBoxLayout* UIEnhanceGuidedFilter::create()
{
    if (!_vlayout)
	{
        _chkBox = new QCheckBox();
        _chkBox->setText("Guided Filter");
        _chkBox->setChecked(false);
        _chkBox->setFixedWidth(UI_CHECKBOX_WIDTH);
        connect(_chkBox, &QCheckBox::stateChanged, this, &UIEnhanceGuidedFilter::onChkBoxGuidedFilterSelected);

        _slider = new QSlider();
        _slider->setMaximum(100);
        _slider->setMinimum(0);
        _slider->setValue(0);
        _slider->setSingleStep(1);
        _slider->setOrientation(Qt::Horizontal);
        _slider->setEnabled(false);
        connect(_slider, &QSlider::valueChanged, this, &UIEnhanceGuidedFilter::onSliderValueChanged);

        _dspBox_eps = new QDoubleSpinBox();
        _dspBox_eps->setMaximum(2);
        _dspBox_eps->setMinimum(0);
        _dspBox_eps->setValue(0);
        _dspBox_eps->setSingleStep(0.02);
        _dspBox_eps->setFixedWidth(UI_SPINBOX_MAX_WIDTH);
        _dspBox_eps->setEnabled(false);
        connect(_dspBox_eps, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &UIEnhanceGuidedFilter::onDSpinBoxValueChanged);

		QHBoxLayout *hlayout1 = new QHBoxLayout();
        hlayout1->addWidget(_chkBox);
        hlayout1->addWidget(_slider);
        hlayout1->addWidget(_dspBox_eps);

        _spacer = new QSpacerItem(50, 18, QSizePolicy::Expanding);
		

		QLabel* lb_radius = new QLabel();
		lb_radius->setText("Filter radius: ");
        _spBox_radius = new QSpinBox();
        _spBox_radius->setFixedWidth(UI_SPINBOX_MAX_WIDTH);
        _spBox_radius->setValue(init.radius);

		QLabel* lb_scale = new QLabel();
		lb_scale->setText("Downsampling scale: ");
        _spBox_scale = new QSpinBox();
        _spBox_scale->setFixedWidth(UI_SPINBOX_MAX_WIDTH);
        _spBox_scale->setValue(init.scale);

		QHBoxLayout *hlayout2 = new QHBoxLayout();
        hlayout2->addSpacerItem(_spacer);
		hlayout2->addWidget(lb_radius);
        hlayout2->addWidget(_spBox_radius);
		hlayout2->addWidget(lb_scale);
        hlayout2->addWidget(_spBox_scale);

        _vlayout = new QVBoxLayout();
        _vlayout->addLayout(hlayout1);
        _vlayout->addLayout(hlayout2);
	}
    return _vlayout;
}


void UIEnhanceGuidedFilter::reset()
{
    _slider->setValue(50);
    _dspBox_eps->setValue(init.eps);
    _spBox_radius->setValue(init.radius);
    _spBox_scale->setValue(init.scale);
}

void UIEnhanceGuidedFilter::setProperty()
{
    double eps = _dspBox_eps->value();
    uint radius = _spBox_radius->value();
    uint scale = _spBox_scale->value();

	UILogger::getInstance()->log(QString("Guided filter: eps = %1, raidus = %2, scale = %3").arg(eps).arg(radius).arg(scale));

	//gpu::AlgoPipelineManager::getInstance()->setProperty(std::make_shared<gpu::ImageAdjustProperty>(eps, radius, scale));
}

void UIEnhanceGuidedFilter::onChkBoxGuidedFilterSelected()
{
    if (_chkBox->isChecked()) {
        _slider->setEnabled(true);
        _dspBox_eps->setEnabled(true);
		UILogger::getInstance()->log("Open guided flter.");
	}
	else {
        _slider->setEnabled(false);
        _dspBox_eps->setEnabled(false);
		UILogger::getInstance()->log("Close guided flter.");
	}
}

void UIEnhanceGuidedFilter::onSliderValueChanged()
{
    int val = _slider->value();
    _dspBox_eps->setValue(float(val) / 50);
}

void UIEnhanceGuidedFilter::onDSpinBoxValueChanged()
{
    double val = _dspBox_eps->value();
    _slider->setValue(round(val*50));
	setProperty();
}

GUI_NS_END
