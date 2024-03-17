#include "layer.h"

// Create layer with random weights
Layer::Layer(int inputSize, int outputSize): inputSize(inputSize), outputSize(outputSize) {
    // Output size is the number of neurons in the layer
    weights = Eigen::MatrixXd::Random(outputSize, inputSize);
    bias = Eigen::MatrixXd::Random(outputSize, 1);
}

Eigen::MatrixXd Layer::activate(Eigen::MatrixXd input) {
    for (int i = 0; i < input.rows(); i++) {
        input(i, 0) = relu(input(i, 0));
    }
   return input;
}

Eigen::MatrixXd Layer::forward(Eigen::MatrixXd input) {
    Eigen::VectorXd z = (weights * input) + bias;
    return activate(z);
}

int main() {
    Layer l(3, 4);
    Eigen::MatrixXd input(3, 1);
    input << 1, 2, 3;
    Eigen::MatrixXd output = l.forward(input);
    std::cout << output << std::endl;
    return 0;
}
