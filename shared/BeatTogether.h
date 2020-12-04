#pragma once

#include <string>

namespace BeatTogether {
    struct ServerOptions {
        std::string_view endpoint;
        int port;
    };

    ServerOptions& getGlobalServerOptions();
}
