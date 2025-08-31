#pragma once
#include <filesystem>
#include <string>

#ifdef _WIN32
#include <windows.h>
#elif __APPLE__
#include <mach-o/dyld.h>
#elif __linux__
#include <unistd.h>
#endif

namespace fs = std::filesystem;

inline fs::path getExecutablePath() {
#ifdef _WIN32
    char buffer[MAX_PATH];
    DWORD len = GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    if (len == 0) {
        throw std::runtime_error("Failed to get executable path");
    }
    return fs::path(buffer).parent_path();
#elif __APPLE__
#include <mach-o/dyld.h>
    char buffer[1024];
    uint32_t size = sizeof(buffer);
    if (_NSGetExecutablePath(buffer, &size) != 0) {
        throw std::runtime_error("Buffer too small for executable path");
    }
    return fs::path(buffer).parent_path();
#elif __linux__
    char buffer[1024];
    ssize_t count = readlink("/proc/self/exe", buffer, sizeof(buffer));
    if (count == -1) {
        throw std::runtime_error("Failed to read /proc/self/exe");
    }
    return fs::path(std::string(buffer, count)).parent_path();
#else
    static_assert(false, "Unsupported platform");
#endif
}
