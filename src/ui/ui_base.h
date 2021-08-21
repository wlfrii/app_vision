#ifndef UI_BASE_H
#define UI_BASE_H
#ifdef WITH_QT_GUI
#include <QWidget>
#include <QBoxLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>


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

#endif // WITH_QT_GUI
#endif // UI_BASE_H
