#ifndef UI_GLOBAL_H_LF



constexpr unsigned int UI_SPINBOX_MAX_WIDTH = 50;
constexpr unsigned int UI_CHECKBOX_WIDTH = 150;


#define UI_FORMAT_LAYOUT_MARGINS(ptr){      \
    ptr->setContentsMargins(2, 1, 2, 1);    \
    ptr->setSpacing(3);                     \
}



#endif // UI_GLOBAL_H_LF
