#ifndef UI_BASE_H
#define UI_BASE_H
#include <QWidget>
#include <QBoxLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

#define GUI_NS_BEGIN namespace gui{
#define GUI_NS_END }

GUI_NS_BEGIN

class UIBase : public QWidget
{
	Q_OBJECT
protected:
	UIBase(QWidget *parent = nullptr);
	virtual ~UIBase();
};


class UIWidgetBase : public UIBase
{
	Q_OBJECT
protected:
	UIWidgetBase(QWidget *parent = nullptr);
	virtual ~UIWidgetBase();

public:
	virtual QWidget* create() = 0;
};


class UILayoutBase : public UIBase
{
	Q_OBJECT
protected:
	UILayoutBase(QWidget *parent = nullptr);
	virtual ~UILayoutBase();

public:
	virtual QBoxLayout* create() = 0;
};

GUI_NS_END
#endif // UI_BASE_H
