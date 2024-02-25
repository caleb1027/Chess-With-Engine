#ifndef __LAYER_H__
#define __LAYER_H__

#include <eigen3/Eigen/Dense>
#include <memory>

typedef double function(double x);

double relu(double z) {
	if (z > 0) return z;
	else return 0;
}

double relu_deri(double z) {
	if (z > 0) return 1;
	else return 0;
}

// Abstract Layer Class


class Layer {
    private:
        // rows = input, col = output
        Eigen::MatrixXd weights;
        Eigen::MatrixXd bias;
        int inputSize;
        int outputSize;
        std::shared_ptr<function> activation;
        std::shared_ptr<function> derivative;
        std::shared_ptr <Layer> prevLayer;

        Eigen::VectorXd activate(Eigen::VectorXd input);

    public:
        Layer(int inputSize, int outputSize);
        // multiply weight matrix with input vector, add bias, and apply activation function
        Eigen::VectorXd forward(Eigen::MatrixXd input);
};
#endif
