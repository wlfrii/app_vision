#include <QGuiApplication>
#include <QScreen>
#include <QLabel>
#include <QImage>
#include <QPainter>
#include "displayer.h"
#include <cstdio>

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
}

Displayer::~Displayer()
{
    if(_window){
        delete _window;
        _window = nullptr;
    }
}

void Displayer::updateImage(const QImage& image, bool is_right)
{
    _images[is_right] = image;
}

void Displayer::show()
{
    if(_images[0].isNull() && _images[1].isNull())
        return;

    _window->display_label->clear();

    if(_mode == Displayer::DISPLAY_2D){
        if(_images[0].isNull())
            return;
        _window->display_label->setPixmap(QPixmap::fromImage(_images[0].copy()));
    }
    else{
        auto& tmp = _images[0].isNull() ? _images[1] : _images[0];
        int width = tmp.width();
        int height = tmp.height();
        QPixmap pixmap(width * 2, height);
        QPainter p(&pixmap);
        p.fillRect(pixmap.rect(), QBrush("#000000"));
        for(int i = 0; i < 2; i++){
            if(!_images[i].isNull()){
                p.drawImage(QRect(0 + i*width, 0, width, height), _images[i]);
            }
        }
        QPixmap out = pixmap.scaled(_window->width, _window->height);
        _window->display_label->setPixmap(out);
    }
    _window->display_label->show();
}

void Displayer::setDisplayMode(Displayer::DisplayMode mode)
{
    _mode = mode;
}

void Displayer::setFullScreen()
{
    _window->toFullScreen();
}

void Displayer::close()
{
    _window->display_label->close();
    _window->display_label->clear();
    for(int i = 0; i < 2; i++){
        _images[i] = QImage();
    }
}

} // namespace::gui
