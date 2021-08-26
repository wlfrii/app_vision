#include "ui_group_image.h"
#ifdef WITH_QT_GUI
#include <QString>
#include <QGroupBox>
#include <QCheckBox>
#include <QFileDialog>
#include <QPushButton>
#include "cmd.h"
#include "ui_logger.h"
#include "ui_global.h"
#include "../vision_manager.h"
#include "../image/frame_displayer.h"
#include "control_panel.h"

UIGroupImage::UIGroupImage()
{

}

UIGroupImage::~UIGroupImage()
{
}

UIGroupImage* UIGroupImage::getInstance()
{
    static UIGroupImage control_group;
    return &control_group;
}

QWidget* UIGroupImage::create()
{
    if (!_gpBox_image)
	{
        // Load image button
        _pbtn_load_L_image = new QPushButton();
        _pbtn_load_L_image->setText(tr("Load Left Image"));
        connect(_pbtn_load_L_image, &QPushButton::clicked, this, &UIGroupImage::onPushBtnLoadLeftImageClicked);

        _pbtn_load_R_image = new QPushButton();
        _pbtn_load_R_image->setText(tr("Load Right Image"));
        connect(_pbtn_load_R_image, &QPushButton::clicked, this, &UIGroupImage::onPushBtnLoadRightImageClicked);

        _pbtn_load_stereo_image = new QPushButton();
        _pbtn_load_stereo_image->setText(tr("Load Stereo Image"));
        connect(_pbtn_load_stereo_image, &QPushButton::clicked, this, &UIGroupImage::onPushBtnLoadStereoImageClicked);

        // --------
        QHBoxLayout* hlayout_image = new QHBoxLayout();
        UI_FORMAT_LAYOUT_MARGINS(hlayout_image);
        hlayout_image->addWidget(_pbtn_load_L_image);
        hlayout_image->addWidget(_pbtn_load_R_image);
        hlayout_image->addWidget(_pbtn_load_stereo_image);

        // Load video button
        _pbtn_load_L_video = new QPushButton();
        _pbtn_load_L_video->setText(tr("Load Left Video"));
        connect(_pbtn_load_L_video, &QPushButton::clicked, this, &UIGroupImage::onPushBtnLoadLeftVideoClicked);

        _pbtn_load_R_video = new QPushButton();
        _pbtn_load_R_video->setText(tr("Load Right Video"));
        connect(_pbtn_load_R_video, &QPushButton::clicked, this, &UIGroupImage::onPushBtnLoadRightVideoClicked);

        _pbtn_load_stereo_video = new QPushButton();
        _pbtn_load_stereo_video->setText(tr("Load Stereo Video"));
        connect(_pbtn_load_stereo_video, &QPushButton::clicked, this, &UIGroupImage::onPushBtnLoadStereoVideoClicked);

        // -------
        QHBoxLayout* hlayout_video = new QHBoxLayout();
        UI_FORMAT_LAYOUT_MARGINS(hlayout_video);
        hlayout_video->addWidget(_pbtn_load_L_video);
        hlayout_video->addWidget(_pbtn_load_R_video);
        hlayout_video->addWidget(_pbtn_load_stereo_video);

        // Close and Pause
        _pbtn_close = new QPushButton();
        _pbtn_close->setText("Close");
        connect(_pbtn_close, &QPushButton::clicked, this, &UIGroupImage::onPushBtnCloseClicked);
        hlayout_image->addWidget(_pbtn_close);

        _pbtn_pause = new QPushButton();
        _pbtn_pause->setText("Pause");
        connect(_pbtn_pause, &QPushButton::clicked, this, &UIGroupImage::onPushBtnPauseClicked);
        hlayout_video->addWidget(_pbtn_pause);

        // ------
        QVBoxLayout* vlayout = new QVBoxLayout();
        UI_FORMAT_LAYOUT_MARGINS(vlayout);
        vlayout->addLayout(hlayout_image);
        vlayout->addLayout(hlayout_video);

        _gpBox_image = new QGroupBox();
        _gpBox_image->setTitle(tr("Image Control"));
        _gpBox_image->setLayout(vlayout);
	}
    return _gpBox_image;
}

#define CHECK_LOADED_IMAGE(image) { \
    if(image.empty()){                \
        UILogger::getInstance()->log(QString("No image loaded successfully."));\
        return;                     \
    }                               \
}
#define LOAD_IMAGE(image_path, image){                                  \
    image_path = QFileDialog::getOpenFileName(this, tr("File dialog"),  \
        image_path.isEmpty() ? "../" : image_path,                      \
        tr("Image Files(*bmp *png *jpg)"));                             \
    image = cv::imread(image_path.toStdString());                       \
    CHECK_LOADED_IMAGE(image);                                          \
    ControlPanel::timer_imshow->start(500);                             \
}

void UIGroupImage::onPushBtnLoadLeftImageClicked()
{
    static QString image_path;
    cv::Mat image;
    LOAD_IMAGE(image_path, image);
    UILogger::getInstance()->log(QString("Load left image from \"%1\".").arg(image_path));

    VisionManager::getInstance()->handleImage(image, vision::LEFT_CAMERA);
}

void UIGroupImage::onPushBtnLoadRightImageClicked()
{
    static QString image_path;
    cv::Mat image;
    LOAD_IMAGE(image_path, image);
    UILogger::getInstance()->log(QString("Load right image from \"%1\".").arg(image_path));

    VisionManager::getInstance()->handleImage(image, vision::RIGHT_CAMERA);
}

void UIGroupImage::onPushBtnLoadStereoImageClicked()
{
    static QString image_path;
    cv::Mat image;
    LOAD_IMAGE(image_path, image);
    UILogger::getInstance()->log(QString("Load stereo image from \"%1\".").arg(image_path));

    int width = image.cols;
    cv::Mat temp = image.colRange(0, width / 2);
    VisionManager::getInstance()->handleImage(temp, vision::LEFT_CAMERA);
    temp = image.colRange(width / 2, width);
    VisionManager::getInstance()->handleImage(temp, vision::RIGHT_CAMERA);
}

void UIGroupImage::onPushBtnLoadLeftVideoClicked()
{



}

void UIGroupImage::onPushBtnLoadRightVideoClicked()
{

}

void UIGroupImage::onPushBtnLoadStereoVideoClicked()
{
    static QString path;
    path = QFileDialog::getOpenFileName(this, tr("File dialog"),
        path.isEmpty() ? "../" : path,
        tr("Image Files(*mp4 *avi)"));
    if(VisionManager::getInstance()->handleVideo(path.toStdString())){
        ControlPanel::timer_vdshow->start(30);
    }
}

void UIGroupImage::onPushBtnCloseClicked()
{
    if(ControlPanel::timer_imshow->isActive()){
        ControlPanel::timer_imshow->stop();
        VisionManager::getInstance()->closeImageWindow();
    }

    if(ControlPanel::timer_vdshow->isActive()){
        ControlPanel::timer_vdshow->stop();
        VisionManager::getInstance()->closeVideoWindow();
    }
}

void UIGroupImage::onPushBtnPauseClicked()
{
    if(_pbtn_pause->isChecked()){
        _pbtn_pause->setText("Go on");
    }
    else{
        _pbtn_pause->setText("Pause");
    }
}

//void UIGroupImage::onPushBtnProcessClicked()
//{
//    static std::atomic<bool> flag(false);
//    bool ret = true;
//    while (!flag) {
//        //gpu::AlgoPipelineManager::getInstance()->process(image, image_processed, flag);
//        if (!ret) break;
//    }
//    if (!ret) {
//        UILogger::getInstance()->log(QString("Failed to process the image."));
//        return;
//    }
//    flag.store(false, std::memory_order_relaxed);
//    //::imshow(image, image_processed);
//}

#endif // WITH_QT_GUI
