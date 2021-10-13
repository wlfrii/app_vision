#ifndef UI_LOGER_H
#define UI_LOGER_H
#include <QWidget>
#include <QString>
#include <QTextBrowser>
#include <string>

namespace gui{

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

} // namespace::gui
#endif // UI_LOGER_H
