#ifndef UI_ENHANCE_PROPERTY_H
#define UI_ENHANCE_PROPERTY_H
#ifdef WITH_QT_GUI
#include "ui_base.h"

class QCheckBox;
class QSlider;
class QSpinBox;

class UIEnhanceProperty : public UILayoutBase
{
	Q_OBJECT
protected:
    UIEnhanceProperty(QString name);

public:
    virtual ~UIEnhanceProperty();

public:
    QBoxLayout* create() override;
    void reset();

protected:
    void setProperty(int value);

private slots:
    void onChkBoxSaturationSelected();
    void onSliderValueChanged();
    void onSpinBoxValueChanged();

private:
    QString          _name;
    QHBoxLayout     *_hlayout;

    QCheckBox       *_chkBox;
    QSlider         *_slider;
    QSpinBox        *_spBox;
};


class UIEnhanceSaturation : public UIEnhanceProperty
{
protected:
    UIEnhanceSaturation()
        : UIEnhanceProperty("Saturation")
    {}

public:
    ~UIEnhanceSaturation(){}

    static UIEnhanceSaturation* getInstance();
};

class UIEnhanceContrast : public UIEnhanceProperty
{
protected:
    UIEnhanceContrast()
        : UIEnhanceProperty("Contrast")
    {}
public:
    ~UIEnhanceContrast() {}

    static UIEnhanceContrast* getInstance();
};

class UIEnhanceBrightness : public UIEnhanceProperty
{
protected:
    UIEnhanceBrightness()
        : UIEnhanceProperty("Brightness")
    {}
public:
    ~UIEnhanceBrightness() {}

    static UIEnhanceBrightness* getInstance();
};

#endif // WITH_QT_GUI
#endif // UI_ENHANCE_PROPERTY_H
