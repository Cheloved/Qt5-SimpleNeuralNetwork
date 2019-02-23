#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <QImage>
#include <iostream>
#include <random>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>

class NeuralNetwork
{
public:
    class Neuron
    {
    public:
        class Akson
        {
        public:
            double getSignal();
            Neuron* parentNeuron;
        };

        class Dendrit
        {
        public:
            double weight;
            Akson* inputAkson;
        };

        Neuron(double startValue, bool isInput);

        double activate();

        double d;
        double dw;
        double value;
        bool isInput;
        Akson akson;
        std::vector<Dendrit> dendrits;
    };
    NeuralNetwork(std::vector<int> l, std::vector<double> startData);
    NeuralNetwork(std::vector<int> l, QImage* dataImage);

    void CreateStructure();
    void ShowStructure();

    void CreateWeights();
    void SaveWeights(std::string savePath);
    void LoadWeights(std::string loadPath);
    void ShowWeights();


    std::vector<double> GetDataFromImage(QImage* dataImage);

    void Run();
    void Learn(int answer);
    int GetNetworkResult();

    std::vector< std::vector<Neuron> > neurons;

    double N = 1;
    std::vector<double> data;
    std::string WeightPath = "Weights.txt";
    unsigned long layers;
    std::vector<int> layersVector;
};

#endif // NEURALNETWORK_H
