#include <ZmqWrapper.h>
#include <LogUtils.h>
#include "ConfigParser.h"

int main(int argc, char *argv[])
{

    OUT_STEP("Parse Configuration ...\n");
    ConfigParserPtr config = ConfigParser::GetConfigParserWithParams(argc, argv);
    if (!config)
    {
        handle_error("Failed to get configuration\n");
    }

    OUT_STEP("Preparing sockets ... \n");
    ContextPtr pContext(new context_t(config->GetIoThreadNumber()));
    if (!pContext)
    {
        handle_error("Failed to create ZMQ Context!\n");
    }

    return 0;
}
