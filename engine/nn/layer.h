#ifndef __LAYER_H__
#define __LAYER_H__

#include <eigen3/Eigen/Dense>
#include <torch/torch.h>
#include <memory>
#include <iostream>

typedef double function(double x);

class Layer {
    friend class Network;
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
        // a = weights * input + bias
        Eigen::MatrixXd a;
        // output = activation(a)
        Eigen::MatrixXd output;

        // Input Layer Constructor
        Layer(int inputSize, int outputSize);
        Layer(int numNeurons, std::shared_ptr<Layer> prevLayer);
        // multiply weight matrix with input vector, add bias, and apply activation function
        void forward();
        void updateOutputSize(int outputSize);
        Eigen::MatrixXd getWeights() {
            return weights;
        }
};
#endif
