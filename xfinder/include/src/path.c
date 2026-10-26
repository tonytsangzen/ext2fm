#if defined(_WIN32)
#include <windows.h>
#elif defined(__linux__)
#include <unistd.h>
#include <linux/limits.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#include <limits.h>
#endif


const char* get_executable_path(void) {
    static char path[260] = {0};
    
    if (path[0] != '\0') {
        return path;
    }
    
#if defined(_WIN32)
    // Windows平台实现
    GetModuleFileNameA(NULL, path, sizeof(path));
    char *last_sep = strrchr(path, '\\');
    if (last_sep) {
            *last_sep = '\0';
    }
#elif defined(__linux__)
    // Linux平台实现
    ssize_t count = readlink("/proc/self/exe", path, sizeof(path) - 1);
    if (count != -1) {
        path[count] = '\0';
    } else {
        strcpy(path, "Unknown");
    }
    char *last_sep = strrchr(path, '/');
    if (last_sep) {
            *last_sep = '\0';
    }
#elif defined(__APPLE__)
    // macOS平台实现
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0) {
        // 解析符号链接获取真实路径
        char real_path[PATH_MAX];
        if (realpath(path, real_path) != 0) {
            strncpy(path, real_path, sizeof(path) - 1);
            path[sizeof(path) - 1] = '\0';
        }
    } else {
        strcpy(path, "Unknown");
    }
    char *last_sep = strrchr(path, '/');
    if (last_sep) {
            *last_sep = '\0';
    }
#else
    // 其他平台
    strcpy(path, "Unsupported platform");
#endif
    return path;
}
