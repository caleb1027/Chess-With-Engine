#ifndef __LAYER_H__
#define __LAYER_H__

#include <eigen3/Eigen/Dense>
#include <memory>
#include <iostream>

typedef double function(double x);


// Abstract Layer Class


class Layer {
    private:
        // rows = input, col = output
        Eigen::MatrixXd weights;
        Eigen::MatrixXd bias;
        int inputSize;
        int outputSize;
        double learningRate;
        std::shared_ptr <Layer> prevLayer;
        Eigen::MatrixXd activate(Eigen::MatrixXd input);

        static double relu(double z) {
            if (z > 0) return z;
            else return 0;
        }

        static double relu_deri(double z) {
            if (z > 0) return 1;
            else return 0;
        }


    public:
        // Input Layer Constructor
        Layer(int inputSize, int outputSize);
        // multiply weight matrix with input vector, add bias, and apply activation function
        Eigen::MatrixXd forward(Eigen::MatrixXd input);
};
#endif
