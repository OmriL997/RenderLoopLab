#include "renderlooplab/application.hpp"
#include "renderlooplab/cli.hpp"
#include "renderlooplab/logging.hpp"

#include <cstdio>
#include <cstdlib>

int main(int argc, char* argv[])
{
    std::string cliError;
    auto result = rll::parseCli(argc, argv, cliError);

    if (!result.has_value()) {
        std::fprintf(stderr, "ERROR: %s\n", cliError.c_str());
        std::fprintf(stderr, "Run '%s --help' for usage.\n", argv[0]);
        return 1;
    }

    if (result->showHelp) {
        rll::printHelp(argv[0]);
        return 0;
    }

    rll::Application app(result->config);
    return app.run();
}
