#include "ui_group_image.h"
#include <QString>
#include <QGroupBox>
#include <QCheckBox>
#include <QFileDialog>
#include <QPushButton>
#include <QImage>
#include <QTimer>
#include "ui_logger.h"
#include "ui_global.h"
#include "../display/displayer.h"

#include <QLabel>

GUI_NS_BEGIN

extern Displayer* gui_displayer;
extern QTimer gui_display_timer;

UIGroupImage::UIGroupImage()
    : _data_path("")
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
        connect(_pbtn_load_L_image, &QPushButton::clicked, this,
                &UIGroupImage::onPushBtnLoadLeftImageClicked);

        _pbtn_load_R_image = new QPushButton();
        _pbtn_load_R_image->setText(tr("Load Right Image"));
        connect(_pbtn_load_R_image, &QPushButton::clicked, this,
                &UIGroupImage::onPushBtnLoadRightImageClicked);

        _pbtn_load_stereo_image = new QPushButton();
        _pbtn_load_stereo_image->setText(tr("Load Stereo Image"));
        connect(_pbtn_load_stereo_image, &QPushButton::clicked, this,
                &UIGroupImage::onPushBtnLoadStereoImageClicked);

        // --------
        QHBoxLayout* hlayout_image = new QHBoxLayout();
        UI_FORMAT_LAYOUT_MARGINS(hlayout_image);
        hlayout_image->addWidget(_pbtn_load_L_image);
        hlayout_image->addWidget(_pbtn_load_R_image);
        hlayout_image->addWidget(_pbtn_load_stereo_image);

        // Load video button
        _pbtn_load_L_video = new QPushButton();
        _pbtn_load_L_video->setText(tr("Load Left Video"));
        connect(_pbtn_load_L_video, &QPushButton::clicked, this,
                &UIGroupImage::onPushBtnLoadLeftVideoClicked);

        _pbtn_load_R_video = new QPushButton();
        _pbtn_load_R_video->setText(tr("Load Right Video"));
        connect(_pbtn_load_R_video, &QPushButton::clicked, this,
                &UIGroupImage::onPushBtnLoadRightVideoClicked);

        _pbtn_load_stereo_video = new QPushButton();
        _pbtn_load_stereo_video->setText(tr("Load Stereo Video"));
        connect(_pbtn_load_stereo_video, &QPushButton::clicked, this,
                &UIGroupImage::onPushBtnLoadStereoVideoClicked);

        // -------
        QHBoxLayout* hlayout_video = new QHBoxLayout();
        UI_FORMAT_LAYOUT_MARGINS(hlayout_video);
        hlayout_video->addWidget(_pbtn_load_L_video);
        hlayout_video->addWidget(_pbtn_load_R_video);
        hlayout_video->addWidget(_pbtn_load_stereo_video);

        // Close and Pause
        _pbtn_close = new QPushButton();
        _pbtn_close->setText("Close");
        connect(_pbtn_close, &QPushButton::clicked, this,
                &UIGroupImage::onPushBtnCloseClicked);
        hlayout_image->addWidget(_pbtn_close);

        _pbtn_pause = new QPushButton();
        _pbtn_pause->setText("Pause");
        connect(_pbtn_pause, &QPushButton::clicked, this,
                &UIGroupImage::onPushBtnPauseClicked);
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


#define LOAD(path, type, token)                                         \
    QString filter = QString(#type) == "image" ?                        \
        "Image Files(*bmp *png *jpg)" : "Video Files(*avi *mp4)";       \
    path = QFileDialog::getOpenFileName(this, tr("File dialog"),        \
        path.isEmpty() ? "../" : path, filter);                         \
    if(!path.isEmpty()){                                                \
        QString qstr = "Load "#token" "#type" from \""+path+"\".";      \
        UILog(qstr);                                                    \
    }



void UIGroupImage::onPushBtnLoadLeftImageClicked()
{
    LOAD(_data_path, image, left);
    QImage image;
    if(!image.load(_data_path)){
        UILog("Failed load image.\n");
        return;
    }
    gui_displayer->updateImage(image, false);
    START_DISPLAY();
}

void UIGroupImage::onPushBtnLoadRightImageClicked()
{
    LOAD(_data_path, image, right);
    QImage image;
    if(!image.load(_data_path)){
        UILog("Failed load image.\n");
        return;
    }
    gui_displayer->updateImage(image, true);
    START_DISPLAY();
}

void UIGroupImage::onPushBtnLoadStereoImageClicked()
{
    LOAD(_data_path, image, stereo);
    QImage image;
    if(!image.load(_data_path)){
        UILog("Failed load image.\n");
        return;
    }
    QImage left, right;
    auto rect = image.rect();
    auto w = rect.width() / 2;
    left = image.copy(rect.x(), rect.y(), w, rect.height());
    right = image.copy(rect.x()+w, rect.y(), w, rect.height());
    gui_displayer->updateImage(left, false);
    gui_displayer->updateImage(right, true);
    START_DISPLAY();
}

void UIGroupImage::onPushBtnLoadLeftVideoClicked()
{
    LOAD(_data_path, video, left);
    CALLBACK(EVENT_LOAD_LEFT_VIDEO, static_cast<void*>(&_data_path));
    START_DISPLAY();
}

void UIGroupImage::onPushBtnLoadRightVideoClicked()
{
    LOAD(_data_path, video, right);
    CALLBACK(EVENT_LOAD_RIGHT_VIDEO, static_cast<void*>(&_data_path));
    START_DISPLAY();
}

void UIGroupImage::onPushBtnLoadStereoVideoClicked()
{
    LOAD(_data_path, video, stereo);
    CALLBACK(EVENT_LOAD_STEREO_VIDEO, static_cast<void*>(&_data_path));
    START_DISPLAY();
}


void UIGroupImage::onPushBtnCloseClicked()
{
    _pbtn_pause->setText("Pause");
    STOP_DISPLAY();
    gui_displayer->close();
}

void UIGroupImage::onPushBtnPauseClicked()
{
    if(_pbtn_pause->text() == "Pause"){
        CALLBACK(EVENT_PAUSE_DISPLAY, nullptr);
        _pbtn_pause->setText("Go on");
        STOP_DISPLAY();
    }
    else{
        CALLBACK(EVENT_GOON_DISPLAY, nullptr);
        _pbtn_pause->setText("Pause");
        START_DISPLAY();
    }
}

GUI_NS_END
