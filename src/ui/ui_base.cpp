#include "ui_base.h"
#ifdef WITH_QT_GUI

UIBase::UIBase(QWidget *parent)
    : QWidget(parent)
{

}

UIBase::~UIBase()
{

}

UIWidgetBase::UIWidgetBase(QWidget *parent)
    : UIBase(parent)
{

}

UIWidgetBase::~UIWidgetBase()
{

}

UILayoutBase::UILayoutBase(QWidget *parent)
    : UIBase(parent)
{

}

UILayoutBase::~UILayoutBase()
{

}

#endif // WITH_QT_GUI