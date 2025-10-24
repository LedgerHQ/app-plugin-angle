import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu, nano_models, txFromEtherscan} from './test.fixture';

// Reference transaction for this test:
// https://etherscan.io/tx/0xf79714e53b93b2aabdc5e29e068da2baa7f5bc7ca938835c4bb9b5b20acb75aa

nano_models.forEach(function(model) {
  test('[Nano ' + model.letter + '] Add collateral to perpetual', zemu(model, async (sim, eth) => {

    const serializedTx = txFromEtherscan("0xf8ab81b4851a4f532a008305784f945efe48f8383921d950683c46b87e28e21dea9fb580b84424d83b79000000000000000000000000000000000000000000000000000000000000004400000000000000000000000000000000000000000000000000000002540be40026a08944a4d630584bd26b25d7957b081b47814013b42b5980d201d86a81a070f7daa058c1f8842a6d6262e39aa82d13c0a259326dfd00058ea60377ce8b2ca7a235c2");

    const tx = eth.signTransaction(
      "44'/60'/0'/0",
      serializedTx,
    );

    const right_clicks = model.letter === 'S' ? 5 : 5;

    // Wait for the application to actually load and parse the transaction
    await waitForAppScreen(sim);
    // Navigate the display by pressing the right button 5 times, then pressing both buttons to accept the transaction.
    await sim.navigateAndCompareSnapshots('.', model.name + '_add_collateral_to_perpetual', [right_clicks, 0]);

    await tx;
  }));
});
