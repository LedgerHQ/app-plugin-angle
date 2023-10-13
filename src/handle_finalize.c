#include "angle_plugin.h"

void handle_finalize(ethPluginFinalize_t *msg) {
    context_t *context = (context_t *) msg->pluginContext;

    msg->uiType = ETH_UI_TYPE_GENERIC;

    switch (context->selectorIndex) {
        case MINT:
            // amount, minStableAmount/minCollatAmount
            msg->numScreens = 2;
            // If the beneficiary is NOT the sender, we will need an additional screen to display
            // it.
            if (memcmp(msg->address, context->agToken_ctx.beneficiary, ADDRESS_LENGTH) != 0) {
                msg->numScreens += 1;
            }
            break;
        case BURN:
            // amount, minCollatAmount
            msg->numScreens = 2;
            // If the burner or the beneficiary is NOT the sender, we will need 2 additional screens
            // to display them.
            if (memcmp(msg->address, context->agToken_ctx.burner, ADDRESS_LENGTH) != 0 ||
                memcmp(msg->address, context->agToken_ctx.beneficiary, ADDRESS_LENGTH) != 0) {
                msg->numScreens += 2;
            }
            break;
        case SLP_DEPOSIT:
            // amount
            msg->numScreens = 1;
            // If the beneficiary is NOT the sender, we will need an additional screen to display
            // it.
            if (memcmp(msg->address, context->agToken_ctx.beneficiary, ADDRESS_LENGTH) != 0) {
                msg->numScreens += 1;
            }
            break;
        case SLP_WITHDRAW:
            // amount
            msg->numScreens = 1;
            // If the burner or the beneficiary is NOT the sender, we will need 2 additional screens
            // to display them.
            if (memcmp(msg->address, context->agToken_ctx.burner, ADDRESS_LENGTH) != 0 ||
                memcmp(msg->address, context->agToken_ctx.beneficiary, ADDRESS_LENGTH) != 0) {
                msg->numScreens += 2;
            }
            break;
        case OPEN_PERPETUAL:
            // amount, leverage, maxOracleRate, max_opening_fees
            msg->numScreens = 4;
            // If the beneficiary is NOT the sender, we will need an additional screen to display
            // it.
            if (memcmp(msg->address, context->perpetual_ctx.beneficiary, ADDRESS_LENGTH) != 0) {
                msg->numScreens += 1;
            }
            break;
        case ADD_TO_PERPETUAL:
            // perpetualID, amount
            msg->numScreens = 2;
            break;
        case GET_REWARD_PERPETUAL:
            // perpetualID
            msg->numScreens = 1;
            break;
        case CLOSE_PERPETUAL:
            // perpetualID
            msg->numScreens = 1;
            // If the beneficiary is NOT the sender, we will need an additional screen to display
            // it.
            if (memcmp(msg->address, context->perpetual_ctx.beneficiary, ADDRESS_LENGTH) != 0) {
                msg->numScreens = 2;
            }
            // if minCashOut != 0, let's display it
            if (cx_math_is_zero(context->perpetual_ctx.amount,
                                sizeof(context->perpetual_ctx.amount)) != true) {
                msg->numScreens = 3;
            }
            break;
        default:
            PRINTF("Selector Index not supported: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }

    msg->result = ETH_PLUGIN_RESULT_OK;
}
