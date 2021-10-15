#ifndef VISION_UTIL_H_LF
#define VISION_UTIL_H_LF
#include <vector>
#include <array>
#include <string>
// For make dir
#ifdef _WIN64
#include <io.h>     ////@func int access(const char *_Filename, int _AccessMode)
#include <direct.h> ////@func int mkdir(const char *_Path), int rmdir(const char *_Path)
#else // (defined __linux__) || (defined __APPLE__)
#include <stdio.h>  ////@func int access(const char *_Filename, int _AccessMode)
#include <unistd.h>
#include <sys/stat.h> ////@func int mkdir(const char *pathname, mode_t mode), int rmdir(const char *_Path)
#endif

namespace util {

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


template<typename Tp>
void delete_ptr(Tp* ptr)
{
	if (ptr) {
        delete ptr;
		ptr = nullptr;
	}
}

template<typename Tp>
void delete_ptr(Tp* ptr[])
{
    if (ptr) {
        delete[] ptr;
        ptr = nullptr;
    }
}

template<typename Tp>
void delete_ptr(std::vector<Tp*>& ptrs)
{
	for (auto &ptr : ptrs) {
		delete_ptr<Tp>(ptr);
	}
}

template<size_t N, typename Tp>
void delete_ptr(std::array<Tp*, N>& ptrs)
{
    for (auto &ptr : ptrs) {
        delete_ptr<Tp>(ptr);
    }
}

} // namespace::util

/* Delete a pointer */
#define DELETE_PIONTER(ptr)	\
    if(ptr != nullptr) {	\
        delete ptr;			\
        ptr = nullptr;		\
    }
/* Delete a pointer array*/
#define DELETE_PIONTER_ARRAY(ptr)	\
    if(ptr != nullptr) {            \
        delete[] ptr;               \
        ptr = nullptr;              \
    }
/* Delete a array of pointer */
#define DELETE_ARRAY_PIONTER(ptr, count)	\
    for(int i = 0; i < count; i++){     	\
		DELETE_PIONTER(ptr[i])				\
	}


void LOG(const std::string&);

#endif // VISION_UTIL_H_LF
