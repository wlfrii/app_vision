#ifndef UI_GROUP_RECTIFY_H_LF
#define UI_GROUP_RECTIFY_H_LF
#include "ui_base.h"

class QCheckBox;
class QPushButton;
class QLineEdit;
class QGroupBox;

GUI_NS_BEGIN

class UIGroupRectify : public UIWidgetBase
{
	Q_OBJECT
protected:
    UIGroupRectify();
public:
    ~UIGroupRectify();

    static UIGroupRectify* getInstance();

    QWidget* create() override;

private:
	void setProperty();

private slots:
	void onChkBoxRectifySelected();
    void onPushBtnCamParamClicked();

private:
    QGroupBox       *_gpBox_rectify;
    QCheckBox       *_chkBox;

    QPushButton		*_pbtn_cam_params;
    QLineEdit 		*_lEdit_cam_params;
};

GUI_NS_END
#endif // UI_GROUP_RECTIFY_H_LF
