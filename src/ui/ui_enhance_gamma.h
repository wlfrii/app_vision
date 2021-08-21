#ifndef UI_ENHANCE_GAMMA_H
#define UI_ENHANCE_GAMMA_H
#ifdef WITH_QT_GUI
#include "ui_base.h"

class QCheckBox;
class QDoubleSpinBox;

class UIEnhanceGamma : public UILayoutBase
{
	Q_OBJECT
protected:
    UIEnhanceGamma();
public:
	~UIEnhanceGamma();

	static UIEnhanceGamma* getInstance();

	QBoxLayout* create() override;
	void reset();

private:
	void setProperty();

private slots:
	void onChkBoxGammaSelected();
	void onDoubleSpinBoxAlphaValueChanged();
	void onDoubleSpinBoxRefLValueChanged();

private:
	struct {
		float alpha = 0.55;
		float ref_L = 0.5;
    }_prop;
    QHBoxLayout		*_hlayout;

    QCheckBox		*_chkBox;
    QDoubleSpinBox	*_dspBox_alpha;
    QDoubleSpinBox	*_dspBox_ref_L;
};

#endif // WITH_QT_GUI
#endif // UI_ENHANCE_GAMMA_H
