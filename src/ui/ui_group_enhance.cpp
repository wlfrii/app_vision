#include "ui_group_enhance.h"
#ifdef WITH_QT_GUI
#include <QString>
#include <QGroupBox>
#include <QCheckBox>
#include "cmd.h"
#include "ui_logger.h"
#include "ui_enhance_property.h"
#include "ui_enhance_guidedfilter.h"
#include "ui_enhance_gamma.h"
#include "ui_enhance_nonuniform.h"
#include "ui_global.h"

UIGroupEnhance::UIGroupEnhance()
{

}

UIGroupEnhance::~UIGroupEnhance()
{
}

UIGroupEnhance *UIGroupEnhance::getInstance()
{
    static UIGroupEnhance enhance_group;
    return &enhance_group;
}

QWidget *UIGroupEnhance::create()
{
    if(!_gpBox_enhance){
        QBoxLayout* vlayout_enhance = new QVBoxLayout(this);
        UI_FORMAT_LAYOUT_MARGINS(vlayout_enhance);

        // Enhance Group: Image property
        vlayout_enhance->addLayout(UIEnhanceSaturation::getInstance()->create());
        vlayout_enhance->addLayout(UIEnhanceContrast::getInstance()->create());
        vlayout_enhance->addLayout(UIEnhanceBrightness::getInstance()->create());

        // Enhance Group: Guided filter
        vlayout_enhance->addLayout(UIEnhanceGuidedFilter::getInstance()->create());

        // Enhance Group: Gamma
        vlayout_enhance->addLayout(UIEnhanceGamma::getInstance()->create());

        // Enhance Group: Nonuniform
        vlayout_enhance->addLayout(UIEnhanceNonuniform::getInstance()->create());

        _gpBox_enhance = new QGroupBox();
        connect(_gpBox_enhance, &QGroupBox::clicked, this, &UIGroupEnhance::onGroupBoxEnhanceSelected);
        _gpBox_enhance->setTitle(tr("Enhancement Control"));
        _gpBox_enhance->setCheckable(false);
        _gpBox_enhance->setLayout(vlayout_enhance);
    }
    return _gpBox_enhance;
}


void UIGroupEnhance::onGroupBoxEnhanceSelected()
{
    if (_gpBox_enhance->isChecked()) {
		UILogger::getInstance()->log(QString("Open to image enhancement mode."));
		CMD::is_enhance = true;
	}
	else {
		UILogger::getInstance()->log(QString("Close image enhancement mode."));
		CMD::is_enhance = false;
	}
}

#endif // WITH_QT_GUI
