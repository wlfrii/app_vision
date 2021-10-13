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

    //static QTimer* timer_imshow;
    //static QTimer* timer_vdshow;

private:
    void setupUI();


private slots:
    void onTimerImshow();
    void onTimerVdshow();
};

} // namespace::gui
#endif // CONTROL_PANAL_H_LF
