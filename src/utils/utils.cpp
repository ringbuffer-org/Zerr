#include "utils.h"

namespace zerr{

std::string zerr::formatString(const char* format, ...) {
    va_list args;
    va_start(args, format);

    // Get the required size for the formatted string
    int size = vsnprintf(nullptr, 0, format, args);
    va_end(args);

    if (size <= 0) {
        // Invalid format or error
        return "";
    }

    // Allocate memory for the formatted string
    std::vector<char> buffer(size + 1); // +1 for null-terminator

    va_start(args, format);
    vsnprintf(buffer.data(), size + 1, format, args);
    va_end(args);

    return std::string(buffer.data());
}

} // zerr





