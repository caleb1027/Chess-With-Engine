#include "network.h"

Network::Network(int inputSize): hidden_layers{} {
    num_hidden_layers = 0;
    input_layer = std::make_shared<Layer>(inputSize, 0);
    output_layer = std::make_shared<Layer>(1, 1);
}

void Network::feedForward(Eigen::MatrixXd input) {

}

void Network::addLayer(int size) {
    if (num_hidden_layers == 0) {
        hidden_layers.push_back(std::make_shared<Layer>(input_layer->outputSize, size));
    }
    else {
        hidden_layers.push_back(std::make_shared<Layer>(hidden_layers[num_hidden_layers - 1]->outputSize, size));
    }
    num_hidden_layers++;
}
