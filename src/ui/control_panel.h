#ifndef CONTROL_PANAL_H_LF
#define CONTROL_PANAL_H_LF
#ifdef WITH_QT_GUI
#include <QWidget>
#include <QTimer>


class ControlPanel : public QWidget
{
    Q_OBJECT
public:
    ControlPanel(QWidget *parent = nullptr);
    ~ControlPanel();

    static QTimer* timer_imshow;
    static QTimer* timer_vdshow;

private:
    void setupUI();


private slots:
    void onTimerImshow();
    void onTimerVdshow();
};

#endif // WITH_QT_GUI
#endif // CONTROL_PANAL_H_LF
