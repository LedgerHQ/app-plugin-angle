import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu, nano_models, SPECULOS_ADDRESS, txFromEtherscan} from './test.fixture';

// Reference transaction for this test:
// https://etherscan.io/tx/0x3d824aec5ee7597e6f4e7678d9962cad97f7a6ae20fb8db77c4cc3bde68f8889

nano_models.forEach(function(model) {
  test('[Nano ' + model.letter + '] Deposit SLP collateral with sender != user', zemu(model, async (sim, eth) => {

  const serializedTx = txFromEtherscan("0xf8cc8204f5850a3e9ab80083027f73945addc89785d75c86ab939e9e15bfbbb7fc086a8780b8642e2d2984000000000000000000000000000000000000000000000a968163f0a57b40000000000000000000000000000067bc76e8fd78cc59594c9f43c643ea7cafa4866900000000000000000000000053b981389cfc5dcda2dc2e903147b5dd0e985f4426a03f3a3d9e51906b05a073271fa7e4ed22095c2543caac9928446d11b56e431f52a0401e7a7e87bc6c8aed48faf6acb6421b989ae6f49658ffd95900894e48840af6");

  const tx = eth.signTransaction(
    "44'/60'/0'/0",
    serializedTx,
  );

  const right_clicks = model.letter === 'S' ? 7 : 5;

  // Wait for the application to actually load and parse the transaction
  await waitForAppScreen(sim);
  // Navigate the display by pressing the right button 17 times, then pressing both buttons to accept the transaction.
  await sim.navigateAndCompareSnapshots('.', model.name + '_deposit_slp_collateral_with_different_sender_and_user', [right_clicks, 0]);

  await tx;
  }));
});

nano_models.forEach(function(model) {
  test('[Nano ' + model.letter + '] Deposit SLP collateral with sender == user', zemu(model, async (sim, eth) => {
  let raw_tx = "0xf8cc8204f5850a3e9ab80083027f73945addc89785d75c86ab939e9e15bfbbb7fc086a8780b8642e2d2984000000000000000000000000000000000000000000000a968163f0a57b40000000000000000000000000000067bc76e8fd78cc59594c9f43c643ea7cafa4866900000000000000000000000053b981389cfc5dcda2dc2e903147b5dd0e985f4426a03f3a3d9e51906b05a073271fa7e4ed22095c2543caac9928446d11b56e431f52a0401e7a7e87bc6c8aed48faf6acb6421b989ae6f49658ffd95900894e48840af6";
  raw_tx = raw_tx.replace("67bc76e8fd78cc59594c9f43c643ea7cafa48669", SPECULOS_ADDRESS.substring(2));
  const serializedTx = txFromEtherscan(raw_tx);

  const tx = eth.signTransaction(
    "44'/60'/0'/0",
    serializedTx,
  );

  const right_clicks = model.letter === 'S' ? 4 : 4;

  // Wait for the application to actually load and parse the transaction
  await waitForAppScreen(sim);
  // Navigate the display by pressing the right button 14 times, then pressing both buttons to accept the transaction.
  await sim.navigateAndCompareSnapshots('.', model.name + '_deposit_slp_collateral_with_equal_sender_and_user', [right_clicks, 0]);

  await tx;
  }));
});
