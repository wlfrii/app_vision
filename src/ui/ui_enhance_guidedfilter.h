#ifndef UI_ENHANCE_GUIDEDFILTER_H
#define UI_ENHANCE_GUIDEDFILTER_H
#ifdef WITH_QT_GUI
#include "ui_base.h"

class QCheckBox;
class QSlider;
class QDoubleSpinBox;
class QSpinBox;
class QSpacerItem;

class UIEnhanceGuidedFilter : public UILayoutBase
{
	Q_OBJECT
protected:
    UIEnhanceGuidedFilter();

public:
	~UIEnhanceGuidedFilter();
	static UIEnhanceGuidedFilter* getInstance();
	
	QBoxLayout* create() override;
	void reset();

private:
	void setProperty();

private slots:
	void onChkBoxGuidedFilterSelected();
	void onSliderValueChanged();
	void onDSpinBoxValueChanged();

private:
	struct {
		float eps;
		uchar radius;
		uchar scale;
	}init;
    QVBoxLayout		*_vlayout;

    QCheckBox		*_chkBox;
    QSlider			*_slider;
    QDoubleSpinBox	*_dspBox_eps;
    QSpinBox		*_spBox_radius;
    QSpinBox		*_spBox_scale;
    QSpacerItem		*_spacer;
};

#endif // WITH_QT_GUI
#endif // UI_ENHANCE_GUIDEDFILTER_H
