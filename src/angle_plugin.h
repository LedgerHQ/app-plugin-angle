#pragma once

#include "eth_internals.h"
#include "eth_plugin_interface.h"
#include <string.h>

// Number of selectors defined in this plugin. Should match the enum `selector_t`.
// EDIT THIS: Put in the number of selectors your plugin is going to support.
#define NUM_SELECTORS 8

// Name of the plugin.
// EDIT THIS: Replace with your plugin name.
#define PLUGIN_NAME "Angle"

// Enumeration of the different selectors possible.
// Should follow the exact same order as the array declared in main.c
// EDIT THIS: Change the naming (`selector_t`), and add your selector names.
typedef enum {
    MINT = 0,
    BURN,
    SLP_DEPOSIT,
    SLP_WITHDRAW,
    OPEN_PERPETUAL,
    ADD_TO_PERPETUAL,
    CLOSE_PERPETUAL,
    GET_REWARD_PERPETUAL
} selector_t;

// Enumeration used to parse the smart contract data.
// EDIT THIS: Adapt the parameter names here.
typedef enum {
    AMOUNT = 0,
    BURNER,
    POOL_MANAGER,
    MIN_RECEIVED_AMOUNT,
    BENEFICIARY,
    MARGIN,
    COMMITTED_AMOUNT,
    MAX_ORACLE_RATE,
    MIN_NET_MARGIN,
    PERPETUAL_ID,
    FLAGS_PARAM,
    UNEXPECTED_PARAMETER,
} parameter;

extern const uint8_t *const ANGLE_SELECTORS[NUM_SELECTORS];

// STABLE METHODS + SLP
typedef struct agToken_ctx_t {
    uint8_t amount[INT256_LENGTH];
    uint8_t burner[ADDRESS_LENGTH];
    // 'minStableAmount' or 'minCollatAmount'
    uint8_t min_amount_received[INT256_LENGTH];
    // 'user' or 'dest'
    uint8_t beneficiary[ADDRESS_LENGTH];
    int16_t poolManagerIndex;
} agToken_ctx_t;

// PERPETUAL METHODS
typedef struct perpetual_ctx_t {
    uint8_t perpetualID[INT256_LENGTH];
    // either 'owner' or 'to'
    uint8_t beneficiary[ADDRESS_LENGTH];
    // either 'margin' or 'minCashOutAmount'
    uint8_t amount[INT256_LENGTH];
    union {
        uint8_t committedAmount[INT256_LENGTH];
        uint8_t max_opening_fees[INT256_LENGTH];  // 'margin' - 'minNetMargin'
    };
    uint8_t maxOracleRate[INT256_LENGTH];
    uint16_t leverage;  // leverage = ('committedAmount' + 'minNetMargin') / 'minNetMargin'
} perpetual_ctx_t;

// Shared global memory with Ethereum app. Must be at most 5 * 32 bytes.
// EDIT THIS: This struct is used by your plugin to save the parameters you parse. You
// will need to adapt this struct to your plugin.
typedef struct context_t {
    union {
        agToken_ctx_t agToken_ctx;
        perpetual_ctx_t perpetual_ctx;
    };

    // For parsing data.
    uint8_t next_param;  // Set to be the next param we expect to parse.
    uint16_t offset;     // Offset at which the array or struct starts.
    bool go_to_offset;   // If set, will force the parsing to iterate through parameters until
                         // `offset` is reached.

    // For both parsing and display.
    selector_t selectorIndex;
} context_t;

// Piece of code that will check that the above structure is not bigger than 5 * 32. Do not remove
// this check.
_Static_assert(sizeof(context_t) <= 5 * 32, "Structure of parameters too big.");

typedef struct manager_contract_t {
    uint8_t address[ADDRESS_LENGTH];
    char *agToken_ticker;
    uint8_t agToken_decimals;
    char *collateral_ticker;
    uint8_t collateral_decimals;
} manager_contract_t;

typedef manager_contract_t pool_manager_t;
typedef manager_contract_t perpetual_manager_t;

// clang-format off
static const pool_manager_t POOL_MANAGERS[] = {
    {{0xe9, 0xf1, 0x83, 0xfc, 0x65, 0x66, 0x56, 0xf1, 0xf1, 0x7a, 0xf1, 0xf2, 0xb0, 0xdf, 0x79, 0xb8, 0xff, 0x9a, 0xd8, 0xed}, "agEUR", 18, "USDC", 6 },
    {{0xc9, 0xda, 0xab, 0xc6, 0x77, 0xf3, 0xd1, 0x30, 0x10, 0x06, 0xe7, 0x23, 0xbd, 0x21, 0xc6, 0x0b, 0xe5, 0x7a, 0x59, 0x15}, "agEUR", 18, "DAI", 18 },
    {{0x53, 0xb9, 0x81, 0x38, 0x9c, 0xfc, 0x5d, 0xcd, 0xa2, 0xdc, 0x2e, 0x90, 0x31, 0x47, 0xb5, 0xdd, 0x0e, 0x98, 0x5f, 0x44}, "agEUR", 18, "FEI", 18 },
    {{0x6b, 0x4e, 0xe7, 0x35, 0x24, 0x06, 0x70, 0x70, 0x03, 0xbc, 0x6f, 0x6b, 0x96, 0x59, 0x5f, 0xd3, 0x59, 0x25, 0xaf, 0x48}, "agEUR", 18, "FRAX", 18 },
};

static const perpetual_manager_t PERPETUAL_MANAGERS[] = {
    {{0x5e, 0xfe, 0x48, 0xf8, 0x38, 0x39, 0x21, 0xd9, 0x50, 0x68, 0x3c, 0x46, 0xb8, 0x7e, 0x28, 0xe2, 0x1d, 0xea, 0x9f, 0xb5}, "agEUR", 18, "USDC", 6 },
    {{0xfc, 0x8f, 0x9e, 0xef, 0xC5, 0xFC, 0xe1, 0xD9, 0xdA, 0xcE, 0x2B, 0x0a, 0x11, 0xA1, 0xe1, 0x84, 0x38, 0x17, 0x87, 0xC4}, "agEUR", 18, "DAI", 18 },
    {{0x98, 0xfd, 0xbc, 0x54, 0x97, 0x59, 0x9e, 0xff, 0x83, 0x09, 0x23, 0xea, 0x1e, 0xe1, 0x52, 0xad, 0xb9, 0xa4, 0xce, 0xa5}, "agEUR", 18, "FEI", 18 },
    {{0x41, 0x21, 0xa2, 0x58, 0x67, 0x4e, 0x50, 0x7c, 0x99, 0x0c, 0xdf, 0x39, 0x0f, 0x74, 0xd4, 0xef, 0x27, 0x59, 0x21, 0x14}, "agEUR", 18, "FRAX", 18 },
};
// clang-format on

#define NUMBER_OF_POOL_MANAGERS      (sizeof(POOL_MANAGERS) / sizeof(manager_contract_t))
#define NUMBER_OF_PERPETUAL_MANAGERS (sizeof(PERPETUAL_MANAGERS) / sizeof(manager_contract_t))
#define MANAGER_CONTRACT_NOT_FOUND   -1

#define MAX_LEVERAGE_DISPLAYABLE 0xFFFF

void handle_provide_parameter(void *parameters);
void handle_query_contract_ui(void *parameters);
void handle_init_contract(void *parameters);
void handle_finalize(void *parameters);
void handle_provide_token(void *parameters);
void handle_query_contract_id(void *parameters);
