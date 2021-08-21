#ifndef UI_GROUP_IMAGE_H_LF
#define UI_GROUP_IMAGE_H_LF
#ifdef WITH_QT_GUI
#include "ui_base.h"

class QGroupBox;
class QCheckBox;
class QPushButton;

class UIGroupImage : public UIWidgetBase
{
    Q_OBJECT

protected:
    UIGroupImage();
public:
    ~UIGroupImage();

    static UIGroupImage* getInstance();
    QWidget* create() override;

private slots:
    void onPushBtnLoadLeftImageClicked();
    void onPushBtnLoadRightImageClicked();
    void onPushBtnLoadStereoImageClicked();

    void onPushBtnLoadLeftVideoClicked();
    void onPushBtnLoadRightVideoClicked();
    void onPushBtnLoadStereoVideoClicked();

    void onPushBtnCloseClicked();
    void onPushBtnPauseClicked();

private:
    QGroupBox       *_gpBox_image;

    QPushButton		*_pbtn_load_L_image;
    QPushButton		*_pbtn_load_R_image;
    QPushButton		*_pbtn_load_stereo_image;

    QPushButton		*_pbtn_load_L_video;
    QPushButton		*_pbtn_load_R_video;
    QPushButton		*_pbtn_load_stereo_video;

    QPushButton		*_pbtn_close;
    QPushButton     *_pbtn_pause;
};

#endif // WITH_QT_GUI
#endif // UI_GROUP_IMAGE_H_LF
