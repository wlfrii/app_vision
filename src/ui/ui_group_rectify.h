#ifndef UI_GROUP_RECTIFY_H_LF
#define UI_GROUP_RECTIFY_H_LF
#ifdef WITH_QT_GUI
#include "ui_base.h"

class QCheckBox;
class QPushButton;
class QLineEdit;
class QGroupBox;

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

#endif // WITH_QT_GUI
#endif // UI_GROUP_RECTIFY_H_LF
