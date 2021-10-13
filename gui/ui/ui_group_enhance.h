#ifndef UI_GROUP_ENHANCE_H_LF
#define UI_GROUP_ENHANCE_H_LF
#include "ui_base.h"

class QGroupBox;

GUI_NS_BEGIN

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

GUI_NS_END
#endif // UI_GROUP_ENHANCE_H_LF
