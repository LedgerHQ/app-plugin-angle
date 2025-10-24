import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu, nano_models, txFromEtherscan} from './test.fixture';


// Reference transaction for this test:
// https://etherscan.io/tx/0xc5636c25c077652dd8e90943245f6fa81c568a595329d00302b41ba717f88755

nano_models.forEach(function(model) {
  test('[Nano ' + model.letter + '] Get reward of perpetual', zemu(model, async (sim, eth) => {

  const serializedTx = txFromEtherscan("0xf889338517bfac7c008305f4f0945efe48f8383921d950683c46b87e28e21dea9fb580a41c4b774b000000000000000000000000000000000000000000000000000000000000004e26a09f408ab644aff565c856e348bbc23d71d56b130b578acbd00869e1557b4e005ca00c03121086b95ad8f7bdcf3e0abc30499ba34018136753499dfe757c2c38da52");

  const tx = eth.signTransaction(
    "44'/60'/0'/0",
    serializedTx,
  );

  const right_clicks = model.letter === 'S' ? 4 : 4;

  // Wait for the application to actually load and parse the transaction
  await waitForAppScreen(sim);
  // Navigate the display by pressing the right button 5 times, then pressing both buttons to accept the transaction.
  await sim.navigateAndCompareSnapshots('.', model.name + '_get_reward_of_perpetual', [right_clicks, 0]);

  await tx;
  }));
});