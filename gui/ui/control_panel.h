#ifndef CONTROL_PANAL_H_LF
#define CONTROL_PANAL_H_LF
#include <QWidget>
#include <QTimer>

namespace gui{

class ControlPanel : public QWidget
{
    Q_OBJECT
public:
    ControlPanel(QWidget *parent = nullptr);
    ~ControlPanel();

    static QTimer* display_timer;
private:
    void setupUI();

private slots:
    void onTimerShow();
};

} // namespace::gui
#endif // CONTROL_PANAL_H_LF
