#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <vector>
#include <memory>
#include <eigen3/Eigen/Dense>
#include "layer.h"

class Network {
    private:
        int num_hidden_layers;
        std::shared_ptr<Layer> output_layer;
        std::shared_ptr<Layer> input_layer;
        void feedForward(Eigen::MatrixXd input);
        void backProp(Eigen::MatrixXd input, Eigen::MatrixXd output);

    public:
        std::vector <std::shared_ptr<Layer>> hidden_layers;
        Network(int inputSize);
        void train();
        double predict(Eigen::MatrixXd input);
        void addLayer(int size);
};
#endif
