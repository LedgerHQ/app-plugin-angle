#include "angle_plugin.h"

// Sets the first screen to display.
void handle_query_contract_id(ethQueryContractID_t *msg) {
    context_t *context = (context_t *) msg->pluginContext;
    // msg->name will be the upper sentence displayed on the screen.
    // msg->version will be the lower sentence displayed on the screen.

    // For the first screen, display the plugin name.
    strlcpy(msg->name, PLUGIN_NAME, msg->nameLength);

    switch (context->selectorIndex) {
        case MINT:
            strlcpy(msg->version, "Mint", msg->versionLength);
            break;
        case BURN:
            strlcpy(msg->version, "Burn", msg->versionLength);
            break;
        case SLP_DEPOSIT:
            strlcpy(msg->version, "Deposit SLP", msg->versionLength);
            break;
        case SLP_WITHDRAW:
            strlcpy(msg->version, "Withdraw SLP", msg->versionLength);
            break;
        case OPEN_PERPETUAL:
            strlcpy(msg->version, "Open Perpetual", msg->versionLength);
            break;
        case ADD_TO_PERPETUAL:
            strlcpy(msg->version, "Add to Perpetual", msg->versionLength);
            break;
        case CLOSE_PERPETUAL:
            strlcpy(msg->version, "Close Perpetual", msg->versionLength);
            break;
        case GET_REWARD_PERPETUAL:
            strlcpy(msg->version, "Get Reward", msg->versionLength);
            break;
        default:
            PRINTF("Selector index: %d not supported\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    msg->result = ETH_PLUGIN_RESULT_OK;
}
