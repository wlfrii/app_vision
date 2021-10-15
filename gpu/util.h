#ifndef GPU_UTIL_H_LF
#define GPU_UTIL_H_LF

/** DEBUG = printf **/
#include <cstdio>
#define DEBUG(fmt, ...) \
    printf("[%s][%s][%d] " fmt, __FILE__, __func__, __LINE__, ##__VA_ARGS__)

#define LOG(fmt, ...) \
    printf(fmt, ##__VA_ARGS__)

#define DELETE_POINT(ptr)   \
    if(ptr != nullptr){     \
        delete ptr;         \
        ptr = nullptr;      \
    }

/** For make dir **/
#include <string>
#ifdef _WIN64
#include <io.h>     ////@func int access(const char *_Filename, int _AccessMode)
#include <direct.h> ////@func int mkdir(const char *_Path), int rmdir(const char *_Path)
#else // (defined __linux__) || (defined __APPLE__)
#include <stdio.h>  ////@func int access(const char *_Filename, int _AccessMode)
#include <unistd.h>
#include <sys/stat.h> ////@func int mkdir(const char *pathname, mode_t mode), int rmdir(const char *_Path)
#endif

/** For timer **/
#include <ctime>
#include <chrono>

#define GPU_NS_BEGIN namespace gpu{
#define GPU_NS_END }
GPU_NS_BEGIN

namespace util{
/**
 * @brief Make a dir if the dir is not exist.
 */
inline void MAKE_DIR(const std::string& folder)
{
#if (defined _WIN64)
    if(_access(folder.c_str(), 0) == -1)
        _mkdir(folder.c_str());
#else // (defined __linux__) || (defined __APPLE__)
    if(access(folder.c_str(), 0) == -1)
        mkdir(folder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
}

/**
 * @brief Return current time string.
 */
inline ::std::string getCurrentTimeStr()
{
    time_t timep;
    time(&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y%m%d_%H%M%S", localtime(&timep));

    return std::string(tmp);
}
} // namespace::util
GPU_NS_END
#endif // GPU_UTIL_H_LF
