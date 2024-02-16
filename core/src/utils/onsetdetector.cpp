#include <iostream>
#include <vector>

class OnsetDetector {
private:
    int lastSample;
    int lastOnsetPosition;
    int debounceThreshold;

public:
    // Constructor with optional debounce threshold parameter
    OnsetDetector(int debounceThreshold = 0) // Default to 0 for no debounce effect
        : lastSample(0), lastOnsetPosition(-debounceThreshold), debounceThreshold(debounceThreshold) {}

    // Setter for debounceThreshold
    void setDebounceThreshold(int newThreshold) {
        debounceThreshold = newThreshold;
        lastOnsetPosition = -debounceThreshold; // Reset lastOnsetPosition based on new threshold
    }

    // Function to detect onsets in blocks of the signal
    std::vector<int> detectOnsetInBlock(const std::vector<int>& block, int blockStartIndex = 0) {
        std::vector<int> onsets;

        // Check if the first sample of the current block is an onset
        if (!block.empty() && block[0] == 1 && lastSample == 0) {
            int currentPosition = blockStartIndex;
            if (currentPosition - lastOnsetPosition >= debounceThreshold) {
                onsets.push_back(currentPosition);
                lastOnsetPosition = currentPosition;
            }
        }

        // Update lastSample with the first element before the loop to handle single-element blocks
        lastSample = block.empty() ? 0 : block[0];

        // Iterate through the current block to find onsets
        for (size_t i = 1; i < block.size(); ++i) {
            if (block[i] == 1 && block[i - 1] == 0) {
                int currentPosition = static_cast<int>(blockStartIndex + i);
                if (currentPosition - lastOnsetPosition >= debounceThreshold) {
                    onsets.push_back(currentPosition);
                    lastOnsetPosition = currentPosition;
                }
            }

            // Update lastSample for the next block
            lastSample = block[i];
        }

        return onsets;
    }
};

// int main() {
//     OnsetDetector detector;
//     detector.setDebounceThreshold(10); // Setting debounce threshold to 10

//     std::vector<int> block1 = {0, 1, 0, 0, 1, 0}; // Example first block
//     std::vector<int> block2 = {0, 1, 0, 1, 0, 1, 0, 1}; // Example second block

//     std::vector<int> onsetsBlock1 = detector.detectOnsetInBlock(block1);
//     std::vector<int> onsetsBlock2 = detector.detectOnsetInBlock(block2, block1.size()); // Continue indexing

//     std::cout << "Onset positions in block 1: ";
//     for (int pos : onsetsBlock1) {
//         std::cout << pos << " ";
//     }
//     std::cout << "\nOnset positions in block 2: ";
//     for (int pos : onsetsBlock2) {
//         std::cout << pos << " ";
//     }
//     std::cout << std::endl;

//     return 0;
// }
