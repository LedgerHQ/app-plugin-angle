/*******************************************************************************
 *   Ethereum 2 Deposit Application
 *   (c) 2020 Ledger
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 ********************************************************************************/

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "os.h"
#include "cx.h"

#include "angle_plugin.h"

// List of selectors supported by this plugin.
// EDIT THIS: Adapt the variable names and change the `0x` values to match your selectors.
static const uint8_t MINT_SELECTOR[SELECTOR_SIZE] = {0x07, 0x4e, 0xe4, 0x46};
static const uint8_t BURN_SELECTOR[SELECTOR_SIZE] = {0x6d, 0x10, 0x42, 0x16};
static const uint8_t SLP_DEPOSIT_SELECTOR[SELECTOR_SIZE] = {0x2e, 0x2d, 0x29, 0x84};
static const uint8_t SLP_WITHDRAW_SELECTOR[SELECTOR_SIZE] = {0x23, 0xe1, 0x03, 0xa8};
static const uint8_t OPEN_PERPETUAL_SELECTOR[SELECTOR_SIZE] = {0xa6, 0x2b, 0x2a, 0x3d};
static const uint8_t ADD_TO_PERPETUAL_SELECTOR[SELECTOR_SIZE] = {0x24, 0xd8, 0x3b, 0x79};
static const uint8_t CLOSE_PERPETUAL_SELECTOR[SELECTOR_SIZE] = {0xe5, 0x23, 0x4c, 0x7a};
static const uint8_t GET_REWARD_PERPETUAL_SELECTOR[SELECTOR_SIZE] = {0x1c, 0x4b, 0x77, 0x4b};

// Array of all the different angle selectors. Make sure this follows the same order as the
// enum defined in `angle_plugin.h`
// EDIT THIS: Use the names of the array declared above.
const uint8_t *const ANGLE_SELECTORS[NUM_SELECTORS] = {MINT_SELECTOR,
                                                       BURN_SELECTOR,
                                                       SLP_DEPOSIT_SELECTOR,
                                                       SLP_WITHDRAW_SELECTOR,
                                                       OPEN_PERPETUAL_SELECTOR,
                                                       ADD_TO_PERPETUAL_SELECTOR,
                                                       CLOSE_PERPETUAL_SELECTOR,
                                                       GET_REWARD_PERPETUAL_SELECTOR};

// Function to dispatch calls from the ethereum app.
void dispatch_plugin_calls(int message, void *parameters) {
    switch (message) {
        case ETH_PLUGIN_INIT_CONTRACT:
            handle_init_contract(parameters);
            break;
        case ETH_PLUGIN_PROVIDE_PARAMETER:
            handle_provide_parameter(parameters);
            break;
        case ETH_PLUGIN_FINALIZE:
            handle_finalize(parameters);
            break;
        case ETH_PLUGIN_PROVIDE_INFO:
            handle_provide_token(parameters);
            break;
        case ETH_PLUGIN_QUERY_CONTRACT_ID:
            handle_query_contract_id(parameters);
            break;
        case ETH_PLUGIN_QUERY_CONTRACT_UI:
            handle_query_contract_ui(parameters);
            break;
        default:
            PRINTF("Unhandled message %d\n", message);
            break;
    }
}

// Calls the ethereum app.
void call_app_ethereum() {
    unsigned int libcall_params[3];
    libcall_params[0] = (unsigned int) "Ethereum";
    libcall_params[1] = 0x100;
    libcall_params[2] = RUN_APPLICATION;
    os_lib_call((unsigned int *) &libcall_params);
}

void handle_query_ui_exception(unsigned int *args) {
    switch (args[0]) {
        case ETH_PLUGIN_QUERY_CONTRACT_UI:
            ((ethQueryContractUI_t *) args[1])->result = ETH_PLUGIN_RESULT_ERROR;
            break;
        default:
            break;
    }
}

// Weird low-level black magic. No need to edit this.
__attribute__((section(".boot"))) int main(int arg0) {
    // Exit critical section
    __asm volatile("cpsie i");

    // Ensure exception will work as planned
    os_boot();

    // Try catch block. Please read the docs for more information on how to use those!
    BEGIN_TRY {
        TRY {
            // Low-level black magic.
            check_api_level(CX_COMPAT_APILEVEL);

            // Check if we are called from the dashboard.
            if (!arg0) {
                // Called from dashboard, launch Ethereum app
                call_app_ethereum();
                return 0;
            } else {
                // Not called from dashboard: called from the ethereum app!
                unsigned int *args = (unsigned int *) arg0;

                // If `ETH_PLUGIN_CHECK_PRESENCE` is set, this means the caller is just trying to
                // know whether this app exists or not. We can skip `dispatch_plugin_calls`.
                if (args[0] != ETH_PLUGIN_CHECK_PRESENCE) {
                    dispatch_plugin_calls(args[0], (void *) args[1]);
                }
            }
        }
        CATCH_OTHER(e) {
            switch (e) {
                // These exceptions are only generated on handle_query_contract_ui()
                case 0x6502:
                case EXCEPTION_OVERFLOW:
                    handle_query_ui_exception((unsigned int *) arg0);
                    break;
                default:
                    break;
            }
            PRINTF("Exception 0x%x caught\n", e);
        }
        FINALLY {
            // Call `os_lib_end`, go back to the ethereum app.
            os_lib_end();
        }
    }
    END_TRY;

    // Will not get reached.
    return 0;
}
