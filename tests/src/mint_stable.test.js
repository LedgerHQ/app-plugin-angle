import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu, genericTx, nano_models, SPECULOS_ADDRESS, RANDOM_ADDRESS, txFromEtherscan} from './test.fixture';
import { ethers } from "ethers";
import { parseUnits} from "ethers/lib/utils";

// Angle Protocol: Stable Master Front (proxy)
const contractAddr = "0x5addc89785d75c86ab939e9e15bfbbb7fc086a87";
const pluginName = "angle";
const abi_path = `../${pluginName}/abis/` + contractAddr + '.json';
const abi = require(abi_path);


// Reference transaction for this test:
// https://etherscan.io/tx/0x89a319da4fe385d075d0fdeaee28b56012e3861ea72bbebea82190538c65abb6

nano_models.forEach(function(model) {
  test('[Nano ' + model.letter + '] Mint agEUR with USDC with sender != user', zemu(model, async (sim, eth) => {
  const contract = new ethers.Contract(contractAddr, abi);

  const amount = parseUnits("25213382881", 'wei');
  const user = RANDOM_ADDRESS;
  const poolManager = "0xe9f183fc656656f1f17af1f2b0df79b8ff9ad8ed";
  const minStableAmount = parseUnits("22069962301657650640274", 'wei');

  const {data} = await contract.populateTransaction.mint(amount, user, poolManager, minStableAmount);

  // Get the generic transaction template
  let unsignedTx = genericTx;
  // Modify `to` to make it interact with the contract
  unsignedTx.to = contractAddr;
  // Modify the attached data
  unsignedTx.data = data;

  // Create serializedTx and remove the "0x" prefix
  const serializedTx = ethers.utils.serializeTransaction(unsignedTx).slice(2);

  const tx = eth.signTransaction(
    "44'/60'/0'/0",
    serializedTx
  );

  const right_clicks = model.letter === 'S' ? 10 : 6;

  // Wait for the application to actually load and parse the transaction
  await waitForAppScreen(sim);
  // Navigate the display by pressing the right button 10 times, then pressing both buttons to accept the transaction.
  await sim.navigateAndCompareSnapshots('.', model.name + '_mint_ageur_usdc_with_different_sender_and_user', [right_clicks, 0]);

  await tx;
  }));
});

nano_models.forEach(function(model) {
  test('[Nano ' + model.letter + '] Mint agEUR with USDC with sender != user and ledger flag set', zemu(model, async (sim, eth) => {
  const contract = new ethers.Contract(contractAddr, abi);

  const amount = parseUnits("25213382881", 'wei');
  const user = RANDOM_ADDRESS;
  const poolManager = "0xe9f183fc656656f1f17af1f2b0df79b8ff9ad8ed";
  const minStableAmount = parseUnits("22069962301657650640274", 'wei');
  const ledger_flag = "AA";
  let {data} = await contract.populateTransaction.mint(amount, user, poolManager, minStableAmount);
  data = data + ledger_flag;

  // Get the generic transaction template
  let unsignedTx = genericTx;
  // Modify `to` to make it interact with the contract
  unsignedTx.to = contractAddr;
  // Modify the attached data
  unsignedTx.data = data;

  // Create serializedTx and remove the "0x" prefix
  const serializedTx = ethers.utils.serializeTransaction(unsignedTx).slice(2);

  const tx = eth.signTransaction(
    "44'/60'/0'/0",
    serializedTx
  );

  const right_clicks = model.letter === 'S' ? 10 : 6;

  // Wait for the application to actually load and parse the transaction
  await waitForAppScreen(sim);
  // Navigate the display by pressing the right button 10 times, then pressing both buttons to accept the transaction.
  await sim.navigateAndCompareSnapshots('.', model.name + '_mint_ageur_usdc_with_different_sender_and_user', [right_clicks, 0]);

  await tx;
  }));
});

nano_models.forEach(function(model) {
  test('[Nano ' + model.letter + '] Mint agEUR with USDC with sender == user', zemu(model, async (sim, eth) => {
  let raw_tx = "0x02f8f3018203f8843b9aca00850cab28e0bc8303c90e945addc89785d75c86ab939e9e15bfbbb7fc086a8780b884074ee44600000000000000000000000000000000000000000000000000000005ded5b0e10000000000000000000000006cd71d6cb7824add7c277f2ca99635d98f8b9248000000000000000000000000e9f183fc656656f1f17af1f2b0df79b8ff9ad8ed0000000000000000000000000000000000000000000004ac6a40f5af96bd3592c080a069fdc24b39343a2db06a432b9185d73eaffcbe7fc2be4bdf3e9d6ef3f32ddc91a056efe6f205331afd61c5dc99e591d4ddc5ab47e0a479a57519c5bc8762676da0";
  raw_tx = raw_tx.replaceAll("6cd71d6cb7824add7c277f2ca99635d98f8b9248", SPECULOS_ADDRESS.substring(2));
  const serializedTx = txFromEtherscan(raw_tx);

  const tx = eth.signTransaction(
    "44'/60'/0'/0",
    serializedTx
  );

  const right_clicks = model.letter === 'S' ? 9 : 5;

  // Wait for the application to actually load and parse the transaction
  await waitForAppScreen(sim);
  // Navigate the display by pressing the right button 7 times, then pressing both buttons to accept the transaction.
  await sim.navigateAndCompareSnapshots('.', model.name + '_mint_ageur_usdc_with_equal_sender_and_user', [right_clicks, 0]);

  await tx;
  }));
});