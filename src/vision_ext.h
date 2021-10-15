#ifndef VISION_GUI_EVENT_H_LF
#define VISION_GUI_EVENT_H_LF

/**
 * @brief For initialize gui event if WITH_QT_GUI
 */
void activateGUIEvent();

/**
 * @brief For initialize gpu pipeline if WITH_CUDA
 */
void initGPUPipeline();

#endif // VISION_GUI_EVENT_H_LF
