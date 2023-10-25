#include <stdbool.h>
#include "angle_plugin.h"
#include "helpers.h"

static bool handle_agToken_display(ethQueryContractUI_t *msg, context_t *context) {
    agToken_ctx_t *agToken_ctx = &context->agToken_ctx;
    bool ret = false;

    if (context->selectorIndex == MINT || context->selectorIndex == BURN) {
        switch (msg->screenIndex) {
            case 0:
                ret = set_amount_ui(msg,
                                    "Send",
                                    agToken_ctx->amount,
                                    sizeof(agToken_ctx->amount),
                                    POOL_MANAGERS,
                                    NUMBER_OF_POOL_MANAGERS,
                                    agToken_ctx->poolManagerIndex,
                                    context->selectorIndex == MINT);
                break;
            case 1:
                ret = set_amount_ui(msg,
                                    "Receive Min.",
                                    agToken_ctx->min_amount_received,
                                    sizeof(agToken_ctx->min_amount_received),
                                    POOL_MANAGERS,
                                    NUMBER_OF_POOL_MANAGERS,
                                    agToken_ctx->poolManagerIndex,
                                    context->selectorIndex == BURN);
                break;
            case 2:
                // optional
                ret = set_address_ui(msg, "Beneficiary", agToken_ctx->beneficiary);
                break;
            case 3:
                // optional
                ret = set_address_ui(msg, "Burner", agToken_ctx->burner);
                break;
            default:
                PRINTF("Received an invalid screenIndex\n");
        }
    }
    // SLP deposit/withdraw
    else {
        switch (msg->screenIndex) {
            case 0:
                ret = set_amount_ui(msg,
                                    context->selectorIndex == SLP_DEPOSIT ? "Send" : "Withdraw",
                                    agToken_ctx->amount,
                                    sizeof(agToken_ctx->amount),
                                    POOL_MANAGERS,
                                    NUMBER_OF_POOL_MANAGERS,
                                    agToken_ctx->poolManagerIndex,
                                    true);
                break;
            case 1:
                // optional
                ret = set_address_ui(msg, "Beneficiary", agToken_ctx->beneficiary);
                break;
            case 2:
                // optional
                ret = set_address_ui(msg, "Burner", agToken_ctx->burner);
                break;
            default:
                PRINTF("Received an invalid screenIndex\n");
        }
    }
    return ret;
}
// amount, leverage, maxOracleRate, max_opening_fees
static bool handle_perpetual_display(ethQueryContractUI_t *msg, context_t *context) {
    perpetual_ctx_t *perpetual_ctx = &context->perpetual_ctx;
    uint8_t *contract_addr = msg->pluginSharedRO->txContent->destination;
    int16_t manager_contract_index =
        get_manager_contract_index(contract_addr, PERPETUAL_MANAGERS, NUMBER_OF_PERPETUAL_MANAGERS);
    bool ret = false;

    if (context->selectorIndex == OPEN_PERPETUAL) {
        switch (msg->screenIndex) {
            case 0:
                ret = set_amount_ui(msg,
                                    "Send",
                                    perpetual_ctx->amount,
                                    sizeof(perpetual_ctx->amount),
                                    PERPETUAL_MANAGERS,
                                    NUMBER_OF_PERPETUAL_MANAGERS,
                                    manager_contract_index,
                                    true);
                break;
            case 1: {
                if (perpetual_ctx->leverage == MAX_LEVERAGE_DISPLAYABLE) {
                    ret = set_message_ui(msg, "Warning", "leverage > 65535");
                } else {
                    // convert context leverage to big endian byte array
                    uint8_t leverage_BE[2] = {perpetual_ctx->leverage >> 8,
                                              perpetual_ctx->leverage & 0xFF};
                    ret = set_integer_ui(msg, "Leverage", leverage_BE, sizeof(leverage_BE));
                }
            } break;
            case 2:
                ret = set_amount_ui(msg,
                                    "Max Oracle Rate",
                                    perpetual_ctx->maxOracleRate,
                                    sizeof(perpetual_ctx->maxOracleRate),
                                    PERPETUAL_MANAGERS,
                                    NUMBER_OF_PERPETUAL_MANAGERS,
                                    manager_contract_index,
                                    false);
                break;
            case 3:
                ret = set_amount_ui(msg,
                                    "Opening Fees",
                                    perpetual_ctx->max_opening_fees,
                                    sizeof(perpetual_ctx->max_opening_fees),
                                    PERPETUAL_MANAGERS,
                                    NUMBER_OF_PERPETUAL_MANAGERS,
                                    manager_contract_index,
                                    true);
                break;
            case 4:
                // optional
                ret = set_address_ui(msg, "Beneficiary", perpetual_ctx->beneficiary);
                break;
            default:
                PRINTF("Received an invalid screenIndex\n");
        }
    } else if (context->selectorIndex == CLOSE_PERPETUAL) {
        switch (msg->screenIndex) {
            case 0:
                ret = set_integer_ui(msg,
                                     "Perpetual ID",
                                     perpetual_ctx->perpetualID,
                                     sizeof(perpetual_ctx->perpetualID));
                break;
            case 1:
                // optional
                ret = set_address_ui(msg, "Beneficiary", perpetual_ctx->beneficiary);
                break;
            case 2:
                // optional
                ret = set_amount_ui(msg,
                                    "Receive Min.",
                                    perpetual_ctx->amount,
                                    sizeof(perpetual_ctx->amount),
                                    PERPETUAL_MANAGERS,
                                    NUMBER_OF_PERPETUAL_MANAGERS,
                                    manager_contract_index,
                                    true);
                break;
            default:
                PRINTF("Received an invalid screenIndex\n");
        }
    } else {
        switch (msg->screenIndex) {
            case 0:
                ret = set_integer_ui(msg,
                                     "Perpetual ID",
                                     perpetual_ctx->perpetualID,
                                     sizeof(perpetual_ctx->perpetualID));
                break;
            case 1:
                ret = set_amount_ui(msg,
                                    "Send",
                                    perpetual_ctx->amount,
                                    sizeof(perpetual_ctx->amount),
                                    PERPETUAL_MANAGERS,
                                    NUMBER_OF_PERPETUAL_MANAGERS,
                                    manager_contract_index,
                                    true);
                break;
            default:
                PRINTF("Received an invalid screenIndex\n");
        }
    }
    return ret;
}

void handle_query_contract_ui(ethQueryContractUI_t *msg) {
    context_t *context = (context_t *) msg->pluginContext;
    bool ret = false;

    // msg->title is the upper line displayed on the device.
    // msg->msg is the lower line displayed on the device.

    // Clean the display fields.
    memset(msg->title, 0, msg->titleLength);
    memset(msg->msg, 0, msg->msgLength);

    switch (context->selectorIndex) {
        case MINT:
        case BURN:
        case SLP_DEPOSIT:
        case SLP_WITHDRAW:
            ret = handle_agToken_display(msg, context);
            break;
        case OPEN_PERPETUAL:
        case ADD_TO_PERPETUAL:
        case CLOSE_PERPETUAL:
        case GET_REWARD_PERPETUAL:
            ret = handle_perpetual_display(msg, context);
            break;
        default:
            PRINTF("Selector index: %d not supported\n", context->selectorIndex);
    }
    msg->result = ret ? ETH_PLUGIN_RESULT_OK : ETH_PLUGIN_RESULT_ERROR;
}
