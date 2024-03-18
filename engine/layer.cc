#include "layer.h"


// Create layer with random weights
Layer::Layer(int inputSize, int outputSize): inputSize(inputSize), outputSize(outputSize), prevLayer(nullptr){
    // Output size is the number of neurons in the layer
    if(inputSize == 0) {
        weights = Eigen::MatrixXd::Random(outputSize, 1);
        bias = Eigen::MatrixXd::Random(outputSize, 1);
    }
    else {
        weights = Eigen::MatrixXd::Random(outputSize, inputSize);
        bias = Eigen::MatrixXd::Random(outputSize, 1);
    }
}

Layer::Layer(int numNeurons, std::shared_ptr<Layer> prevLayer): inputSize(prevLayer->outputSize), outputSize(numNeurons), prevLayer(prevLayer) {
    weights = Eigen::MatrixXd::Random(outputSize, inputSize);
    bias = Eigen::MatrixXd::Random(outputSize, 1);
}

void Layer::updateOutputSize(int outputSize) {
    this->outputSize = outputSize;
    weights = Eigen::MatrixXd::Random(outputSize, inputSize);
    bias = Eigen::MatrixXd::Random(outputSize, 1);
}

Eigen::MatrixXd Layer::activate(Eigen::MatrixXd input) {
    for (int i = 0; i < input.rows(); i++) {
        input(i, 0) = relu(input(i, 0));
    }
   return input;
}

void Layer::forward() {
    Eigen::MatrixXd prevOutput;
    if(prevLayer != nullptr) {
        prevOutput = prevLayer->output;
    } else {
        // input layer, take a as input
        prevOutput = a;
    }
    a = (weights * prevOutput) + bias;
    output = activate(a);
}

int main() {
    Layer l(3, 4);
    Eigen::MatrixXd input(3, 1);
    input << 1, 2, 3;
    l.a = input;
    l.forward();
    Eigen::MatrixXd output = l.output;
    std::cout << output << std::endl;
    return 0;
}
