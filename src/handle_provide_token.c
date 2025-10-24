#include "angle_plugin.h"

// We don't manipulate erc20 directly but use PoolManagers instead, so we skip this step
void handle_provide_token(ethPluginProvideInfo_t *msg) {
    msg->result = ETH_PLUGIN_RESULT_OK;
}
