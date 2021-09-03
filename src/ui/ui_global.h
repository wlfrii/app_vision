#ifndef UI_GLOBAL_H_LF



constexpr unsigned int UI_SPINBOX_MAX_WIDTH = 50;
constexpr unsigned int UI_CHECKBOX_WIDTH = 150;


#define UI_FORMAT_LAYOUT_MARGINS(ptr)           \
    do {                                        \
        ptr->setContentsMargins(2, 1, 2, 1);    \
        ptr->setSpacing(3);                     \
    }while(0)

#define UI_CHECK_LOAD_PATH(path)                \
    do {                                        \
        if(path.toStdString().empty())          \
            return;                             \
    }while(0)

#endif // UI_GLOBAL_H_LF
