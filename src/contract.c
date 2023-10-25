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

#include <stdint.h>
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
