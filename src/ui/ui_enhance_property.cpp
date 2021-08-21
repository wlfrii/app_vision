#include "ui_enhance_property.h"
#ifdef WITH_QT_GUI
#include "ui_logger.h"
#include "ui_global.h"
// #include <libvisiongpu/gpu_algorithm_pipeline_manager.h>
#include <QCheckBox>
#include <QSlider>
#include <QSpinBox>

UIEnhanceProperty::UIEnhanceProperty(QString name)
    : _name(name)
{

}

UIEnhanceProperty::~UIEnhanceProperty()
{
}

QBoxLayout *UIEnhanceProperty::create()
{
    if(!_hlayout)
    {
        _hlayout = new QHBoxLayout();

        _chkBox = new QCheckBox();
        _chkBox->setText(_name);
        _chkBox->setChecked(false);
        connect(_chkBox, &QCheckBox::stateChanged, this, &UIEnhanceProperty::onChkBoxSaturationSelected);
        _chkBox->setFixedWidth(UI_CHECKBOX_WIDTH);

        _slider = new QSlider();
        _slider->setMaximum(100);
        _slider->setMinimum(0);
        _slider->setValue(50);
        _slider->setSingleStep(1);
        _slider->setOrientation(Qt::Horizontal);
//        slider->setMaximumWidth(200);
        _slider->setEnabled(false);
        connect(_slider, &QSlider::valueChanged, this, &UIEnhanceProperty::onSliderValueChanged);

        _spBox = new QSpinBox();
        _spBox->setMaximum(100);
        _spBox->setMinimum(0);
        _spBox->setValue(50);
        _spBox->setSingleStep(1);
        _spBox->setFixedWidth(UI_SPINBOX_MAX_WIDTH);
        _spBox->setEnabled(false);
        connect(_spBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &UIEnhanceProperty::onSpinBoxValueChanged);

        _hlayout->addWidget(_chkBox);
        _hlayout->addWidget(_slider);
        _hlayout->addWidget(_spBox);
    }
    return _hlayout;
}

void UIEnhanceProperty::reset()
{
    _slider->setValue(50);
    _spBox->setValue(50);
}

void UIEnhanceProperty::setProperty(int value)
{
    static char satur = 50, contr = 50, bright = 50;
    if(_name == "Saturation"){
        satur = value;
    }else if(_name == "Contrast"){
        contr = value;
    }else{ // name = brightness
        bright = value;
    }
    UILogger::getInstance()->log(_name + QString(" set to %1").arg(value));
    
    //gpu::AlgoPipelineManager::getInstance()->setProperty(std::make_shared<gpu::ImageAdjustProperty>(satur, contr, bright));
}

void UIEnhanceProperty::onChkBoxSaturationSelected()
{
    if(_chkBox->isChecked()){
        _slider->setEnabled(true);
        _spBox->setEnabled(true);
        UILogger::getInstance()->log("Open " + _name + ".");
    }
    else{
        _slider->setEnabled(false);
        _spBox->setEnabled(false);
        UILogger::getInstance()->log("Close " + _name + ".");
    }
}

void UIEnhanceProperty::onSliderValueChanged()
{
    int val = _slider->value();
    _spBox->setValue(val);
}

void UIEnhanceProperty::onSpinBoxValueChanged()
{
    int val = _spBox->value();
    _slider->setValue(val);
    UIEnhanceProperty::setProperty(val);
}


UIEnhanceSaturation* UIEnhanceSaturation::getInstance()
{
    static UIEnhanceSaturation ui_saturation;
    return &ui_saturation;
}

UIEnhanceContrast* UIEnhanceContrast::getInstance()
{
    static UIEnhanceContrast ui_contrast;
    return &ui_contrast;
}

UIEnhanceBrightness* UIEnhanceBrightness::getInstance()
{
    static UIEnhanceBrightness ui_brightness;
    return &ui_brightness;
}

#endif // WITH_QT_GUI
