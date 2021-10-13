#include "ui_logger.h"
#include <QFont>

namespace gui{

UILogger::UILogger()
{
}


UILogger::~UILogger()
{
}


UILogger *UILogger::getInstance()
{
    static UILogger logger;
    return &logger;
}


QTextBrowser *UILogger::getTxtBrowser()
{
    if (!_txt_browser)
	{
        _txt_browser = new QTextBrowser();

		/** Set the font style **/
		QFont font;
		font.setFamily(QStringLiteral("Arial"));
		font.setPointSize(9);
		font.setItalic(true);
        _txt_browser->setFont(font);
        _txt_browser->setStyleSheet(QString("color:rgb(255,85,0);background-color:rgb(220,220,220)"));
	}
    return _txt_browser;
}


void UILogger::log(const QString &qstr)
{
    static unsigned int count = 0;
    QString tmp = QString("#%1. ").arg(++count) + qstr;
    _txt_browser->append(tmp);
}


} // namespace::gui
