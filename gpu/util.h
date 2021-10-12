#ifndef GPU_UTIL_H_LF
#define GPU_UTIL_H_LF
#include <cstdio>
#define GPU_NS_BEGIN namespace gpu{
#define GPU_NS_END }
GPU_NS_BEGIN

/** DEBUG = printf **/
#define DEBUG(fmt, ...) \
    printf("[%s][%s][%d] " fmt, __FILE__, __func__, __LINE__, ##__VA_ARGS__)

#define DELETE_POINT(ptr)   \
    if(ptr != nullptr){     \
        delete ptr;         \
        ptr = nullptr;      \
    }

GPU_NS_END
#endif // GPU_UTIL_H_LF
