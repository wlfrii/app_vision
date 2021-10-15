#include <QGuiApplication>
#include <QScreen>
#include <QLabel>
#include <QImage>
#include <QPainter>
#include "../vision_gui.h"

namespace gui {
class Window
{
public:
    Window(uint16_t win_width, uint16_t win_height)
        : width(win_width)
        , height(win_height)
    {
        display_label = new QLabel();
        QRect rect = QGuiApplication::screens()[0]->geometry();
        display_label->move(rect.x(), rect.y());
        display_label->resize(width, height);
        display_label->show();
        display_label = nullptr;
    }
    ~Window()
    {
        if(display_label){
            delete display_label;
            display_label = nullptr;
        }
    }

    void toFullScreen()
    {
        QScreen* screen = QGuiApplication::screens()[1];
        if(screen){
            QRect rect = screen->geometry();
            display_label->move(QPoint(rect.x(), rect.y()));
            display_label->showFullScreen();
        }
    }

    uint16_t width;
    uint16_t height;

    QLabel* display_label;
};

Displayer::Displayer(uint16_t win_width, uint16_t win_height, DisplayMode mode)
    : _window(new Window(win_width, win_height))
    , _mode(mode)
{
    _images[0] = nullptr;
    _images[1] = nullptr;
}

Displayer::~Displayer()
{
    if(_window){
        delete _window;
        _window = nullptr;
    }
}

void Displayer::updateImage(uint8_t *data, uint16_t im_w, uint16_t im_h,
                            uint8_t im_c, bool is_right)
{
    if(!data) return;

    auto fmt = QImage::Format_BGR888;
    if(im_c == 4) fmt = QImage::Format_ARGB32;
    _images[is_right] = new QImage(data, im_w, im_h, fmt);
}

void Displayer::show()
{
    _window->display_label->clear();

    if(_mode == Displayer::DISPLAY_2D){
        _window->display_label->setPixmap(QPixmap::fromImage(_images[0]->copy()));
    }
    else{
        int width = _images[0]->width();
        int height = _images[0]->height();
        QPixmap pixmap(width * 2, height);
        QPainter p(&pixmap);
        p.fillRect(pixmap.rect(), QBrush("#000000"));
        for(int i = 0; i < 2; i++){
            if(_images[i] != nullptr){
                p.drawImage(QRect(0, 0, width, height), *_images[i]);
            }
        }
        QPixmap out = pixmap.scaled(_window->width, _window->height);
        _window->display_label->setPixmap(out);
    }
}

void Displayer::setFullScreen()
{
    _window->toFullScreen();
}

} // namespace::gui
