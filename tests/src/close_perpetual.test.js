import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu, genericTx, nano_models, SPECULOS_ADDRESS, txFromEtherscan} from './test.fixture';
import { ethers } from "ethers";
import { parseUnits} from "ethers/lib/utils";

// Angle Protocol: Stable Master Front (proxy)
const contractAddr = "0x5efe48f8383921d950683c46b87e28e21dea9fb5";
const pluginName = "angle";
const abi_path = `../${pluginName}/abis/` + contractAddr + '.json';
const abi = require(abi_path);

// Reference transaction for this test:
// https://etherscan.io/tx/0xd796798e910c98206b30a97acce6ac01c0fa0fb069e69805abe9c516cc456aec

nano_models.forEach(function(model) {
  test('[Nano ' + model.letter + '] Close perpetual with sender != beneficiary', zemu(model, async (sim, eth) => {

  const serializedTx = txFromEtherscan("0x02f8d101078459682f00850fc292a81783059b10945efe48f8383921d950683c46b87e28e21dea9fb580b864e5234c7a00000000000000000000000000000000000000000000000000000000000000030000000000000000000000006aaf1da828370ebf05f4286ae17c63043e3be4000000000000000000000000000000000000000000000000000000000000000000c001a05ee9627f100414af264d377b34b9b794eacc14276a7a3d99f68970c4fd681063a0333dabe0adfd67daf81a239b607678e3ddd34fc58e756779899f687e4c203a3c");

  const tx = eth.signTransaction(
    "44'/60'/0'/0",
    serializedTx,
  );

  const right_clicks = model.letter === 'S' ? 9 : 5;

  // Wait for the application to actually load and parse the transaction
  await waitForAppScreen(sim);
  // Navigate the display by pressing the right button 6 times, then pressing both buttons to accept the transaction.
  await sim.navigateAndCompareSnapshots('.', model.name + '_close_perpetual_with_different_sender_and_beneficiary', [right_clicks, 0]);

  await tx;
  }));
});

nano_models.forEach(function(model) {
  test('[Nano ' + model.letter + '] Close perpetual with minCashOutAmount', zemu(model, async (sim, eth) => {
  const contract = new ethers.Contract(contractAddr, abi);

  const perpetualID = parseUnits("79", 'wei');;
  const to = SPECULOS_ADDRESS
  const minCashOutAmount = parseUnits("129026631104596981827926", 'wei');

  const {data} = await contract.populateTransaction.closePerpetual(perpetualID, to, minCashOutAmount);

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
    serializedTx,
  );

  const right_clicks = model.letter === 'S' ? 10 : 6;

  // Wait for the application to actually load and parse the transaction
  await waitForAppScreen(sim);
  // Navigate the display by pressing the right button 6 times, then pressing both buttons to accept the transaction.
  await sim.navigateAndCompareSnapshots('.', model.name + '_close_perpetual_with_minCashOutAmount', [right_clicks, 0]);

  await tx;
  }));
});
