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

private:
    void setupUI();


private slots:
    void onTimerImshow();


public:
    static QTimer* timer_imshow;
};

#endif // WITH_QT_GUI
#endif // CONTROL_PANAL_H_LF
