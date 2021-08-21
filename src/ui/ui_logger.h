#ifndef UI_LOGER_H
#define UI_LOGER_H
#ifdef WITH_QT_GUI
#include <QWidget>
#include <QString>
#include <QTextBrowser>

class UILogger : public QWidget
{
	Q_OBJECT
protected:
    UILogger();

public:
    ~UILogger();
    static UILogger *getInstance();

    QTextBrowser* getTxtBrowser();

    void log(const QString &qstr);

private:
    QTextBrowser *_txt_browser;
};

#define UILog UILogger::getInstance()->log

#endif // WITH_QT_GUI
#endif // UI_LOGER_H
