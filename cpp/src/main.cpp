#include <iostream>
#include <memory>
#include <csignal>
#include <cstring>
#include <app/application.hh>

static std::unique_ptr<cfd::Application> g_app;

void signal_handler(int signal) {
    std::cerr << "\nReceived signal " << signal << ", shutting down...\n";
    g_app.reset();
    exit(signal);
}

int main(int argc, char** argv) {
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    try {
        g_app = std::make_unique<cfd::Application>();
        g_app->initialize(argc, argv);
        g_app->run();
        g_app->shutdown();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
