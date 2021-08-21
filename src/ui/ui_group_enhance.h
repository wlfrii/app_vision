#ifndef UI_GROUP_ENHANCE_H_LF
#define UI_GROUP_ENHANCE_H_LF
#ifdef WITH_QT_GUI
#include "ui_base.h"

class QGroupBox;

class UIGroupEnhance : public UIWidgetBase
{
    Q_OBJECT
protected:
    UIGroupEnhance();
public:
    ~UIGroupEnhance();

    static UIGroupEnhance *getInstance();
    QWidget* create() override;

private slots:
	void onGroupBoxEnhanceSelected();

private:
    QGroupBox	*_gpBox_enhance;
};

#endif // WITH_QT_GUI
#endif // UI_GROUP_ENHANCE_H_LF
