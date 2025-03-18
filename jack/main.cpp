#include <csignal>

#include "zerr.h"

using namespace zerr;
// spkrCfgFile
// "/Users/yangzeyu/Downloads/Zerr/configs/spkr_configs/circulation_8.yaml"
// zerrCfgFile
// "/Users/yangzeyu/Downloads/Zerr/configs/zerr_configs/preset1.yaml"
// ./run_zerr -z
// "/Users/yangzeyu/Downloads/Zerr/configs/zerr_configs/preset1.yaml" -s
// "/Users/yangzeyu/Downloads/Zerr/configs/spkr_configs/circulation_8.yaml"

Zerr* zerr_client;  // define the zerr_client ptr here to make handleSignal
                    // access it easier

void handleSignal(int signal) {
    std::cout << "Received keyboard interrupt. Stopping Zerr..." << std::endl;

    delete zerr_client;

    std::exit(signal);
}

int main(int argc, char* argv[]) {
    // std::signal(SIGINT, handleSignal);

    std::string zerrCfgFile;
    std::string spkrCfgFile;

    // Display each command-line argument.
    std::cout << "\nCommand-line arguments:\n";
    for (int i = 1; i < (argc - 1); i++) {
        if (std::string(argv[i]) == "-z") {
            std::cout << "zerr configs: " << " " << argv[i + 1] << std::endl;
            zerrCfgFile = argv[i + 1];
        }
        if (std::string(argv[i]) == "-s") {
            std::cout << "spkr configs: " << " " << argv[i + 1] << std::endl;
            spkrCfgFile = argv[i + 1];
        }
    }

    zerr_client = new Zerr(zerrCfgFile, spkrCfgFile);
    zerr_client->initialize();
    zerr_client->run();

    return 0;
}
