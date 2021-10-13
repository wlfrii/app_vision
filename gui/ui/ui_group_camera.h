#ifndef UI_GROUP_CAMERA_H_LF
#define UI_GROUP_CAMERA_H_LF
#include "ui_base.h"

class QGroupBox;
class QPushButton;
class QSpinBox;
class QLineEdit;
class QCheckBox;

GUI_NS_BEGIN

class UIGroupCamera : public UIWidgetBase
{
	Q_OBJECT
protected:
    UIGroupCamera();

public:
    ~UIGroupCamera();

    static UIGroupCamera* getInstance();
    QWidget* create() override;

private slots:
    void onPushBtnCameraClicked();
    void onChkBoxFPSSelected();
	void onPushBtnCaptureClicked();

private:
    QGroupBox		*_gpBox_camera;

    QPushButton     *_pBtn_open_camera;
    QCheckBox       *_chkBox_show_fps;

	// Take photos
    QPushButton     *_pBtn_capture;
    QSpinBox        *_spBox_capture_num;

    QLineEdit       *_lEdit_save_path;
    QLineEdit		*_lEdit_save_name;
};

GUI_NS_END
#endif // UI_GROUP_CAMERA_H_LF
