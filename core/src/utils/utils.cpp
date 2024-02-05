#include "utils.h"

namespace zerr{

std::string formatString(const char* format, ...) {
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

// template<typename T>
// std::string formatVector(std::vector<T> vector){
//     std::string formated = "";
//     for (size_t i = 0; i < vector.size(); ++i){
//         formated += formated + std::to_string(vector[i]);
//     }
// }


bool isEqualTo1(Param value, Param epsilon) {
    return std::abs(value - 1.0) < epsilon;
}


bool isEqualTo0(Param value, Param epsilon) {
    return std::abs(value) < epsilon;
}


template<typename T>
bool isInVec(T element, std::vector<T> vector){
    auto it = std::find(vector.begin(), vector.end(), element);
    return it == vector.end()?false:true;
}
// explicit instantiation required for PD
template bool isInVec<int>(int element, std::vector<int> vector);


Samples applyMovingAverage(const Samples& segment, int windowSize) {
    // Check if windowSize is valid
    if (windowSize <= 0) {
        throw std::invalid_argument("Window size must be greater than 0");
    }
    if (windowSize > segment.size()) {
        windowSize = segment.size();
    }

    int segmentSize = segment.size();
    Samples averagedSegment(segmentSize, 0.0);
    double windowSum = 0.0;
    // int windowElements = 0;

    // Initial windowSum and shortWindows
    for (int i = 0; i < windowSize; ++i) {
        windowSum += segment[i];
        averagedSegment[i] = windowSum / (i + 1);
    }

    // Apply moving average
    for (int i = windowSize; i < segmentSize; ++i) {
        // Add new element to the window
        if (i < segmentSize) {
            windowSum += segment[i];
            windowSum -= segment[i - windowSize];
        }

        averagedSegment[i] = windowSum / segmentSize;
    }

    return averagedSegment;
}

} // zerr





