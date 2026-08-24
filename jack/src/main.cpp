/**
 * @file main.cpp
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief Entry point of the Zerr* standalone JACK client
 * @date 2026-08-19
 *
 * @copyright Copyright (c) 2023-2026
 */
#include <csignal>
#include <cstring>
#include <exception>
#include <iostream>
#include <string>

#include "featurebank.h"
#include "zerr_jack.h"

namespace {

/**
 * The signal handler needs a way back to the running client, and a handler takes
 * no user data. One client per process, so a file-scope pointer is the whole
 * mechanism; it is only ever read to set an atomic flag.
 */
zerr::ZerrJack* runningClient = nullptr;

void handleSignal(int)
{
    if (runningClient) {
        runningClient->requestStop();
    }
}

void printUsage()
{
    std::cout << "Usage: zerr_jack [options] <config.yaml>\n"
                 "\n"
                 "Runs the Zerr* spatialization pipeline as a JACK client: one audio input\n"
                 "port, one output port per speaker in the configured speaker array.\n"
                 "\n"
                 "Options:\n"
                 "  -n, --name <name>     Override the JACK client name from the config\n"
                 "  -a, --auto-connect    Connect to the first physical capture/playback ports\n"
                 "      --list-features   Print the feature names the core supports and exit\n"
                 "  -h, --help            Show this message and exit\n"
                 "\n"
                 "See jack/config/example.yaml for an annotated client config.\n";
}

} // namespace

int main(int argc, char** argv)
{
    std::string configPath;
    std::string nameOverride;
    bool autoConnect = false;

    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            printUsage();
            return 0;
        }
        if (arg == "--list-features") {
            zerr::FeatureBank bank;
            bank.print_all_features();
            return 0;
        }
        if (arg == "-a" || arg == "--auto-connect") {
            autoConnect = true;
            continue;
        }
        if (arg == "-n" || arg == "--name") {
            if (i + 1 >= argc) {
                std::cerr << "zerr_jack: " << arg << " needs a value." << std::endl;
                return 1;
            }
            nameOverride = argv[++i];
            continue;
        }
        if (!arg.empty() && arg[0] == '-') {
            std::cerr << "zerr_jack: unknown option '" << arg << "'." << std::endl;
            printUsage();
            return 1;
        }
        if (!configPath.empty()) {
            std::cerr << "zerr_jack: more than one config file given." << std::endl;
            return 1;
        }
        configPath = arg;
    }

    if (configPath.empty()) {
        printUsage();
        return 1;
    }

    zerr::ClientConfig config;
    try {
        config = zerr::ClientConfig::fromFile(configPath);
    }
    catch (const std::exception& e) {
        std::cerr << "zerr_jack: " << e.what() << std::endl;
        return 1;
    }

    if (!nameOverride.empty()) {
        config.clientName = nameOverride;
    }
    if (autoConnect) {
        config.autoConnect = true;
    }

    zerr::ZerrJack zerrJack(config);
    if (!zerrJack.initialize()) {
        return 1;
    }

    // Installed only once the client is ready, so Ctrl+C before that still gets
    // the default behaviour rather than being swallowed by a no-op handler.
    runningClient = &zerrJack;
    std::signal(SIGINT, handleSignal);
    std::signal(SIGTERM, handleSignal);

    const bool clean = zerrJack.run();

    runningClient = nullptr;
    std::cout << "zerr_jack: stopped." << std::endl;

    return clean ? 0 : 1;
}
