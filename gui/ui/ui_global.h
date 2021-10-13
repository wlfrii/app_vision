#ifndef UI_GLOBAL_H_LF
#include "../vision_gui.h"

namespace gui{

constexpr unsigned int UI_SPINBOX_MAX_WIDTH = 50;
constexpr unsigned int UI_CHECKBOX_WIDTH = 150;

#define UI_FORMAT_LAYOUT_MARGINS(ptr)           \
    do {                                        \
        ptr->setContentsMargins(2, 1, 2, 1);    \
        ptr->setSpacing(3);                     \
    }while(0)


// The callback function for process all the GUI events.
void CALLBACK(unsigned char id, void* data);

} // namespace::gui
#endif // UI_GLOBAL_H_LF
