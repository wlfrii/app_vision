#include "ui_base.h"
GUI_NS_BEGIN

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

GUI_NS_END