#include "neuralnetwork.h"

double Sygma(double x)
{
    return 1/(1+exp(-x));
}

double D_Sygma(double x)
{
    return Sygma(x)*(1-Sygma(x));
}

double GetRandomFloat()
{
    return ( static_cast <double> (rand()) / static_cast <double> (RAND_MAX) );
}

double NeuralNetwork::Neuron::Akson::getSignal()
{
    if ( this->parentNeuron->isInput == true )
        return Sygma(this->parentNeuron->value);
    else
    {
        double x = parentNeuron->activate();
        return Sygma(x);
    }
}

double NeuralNetwork::Neuron::activate()
{
    if ( this->isInput == true )
    {
        return (this->value);
    } else {
        double result(0.);
        for(Neuron::Dendrit d : this->dendrits)
        {
            result += d.inputAkson->getSignal() * d.weight;
        }
        return result;
    }
}

void NeuralNetwork::SaveWeights(std::string path)
{
    std::ofstream file;
    file.open(path);

    for ( unsigned long layer = 1; layer < this->layers; layer++ )
    {
        for ( unsigned long i = 0; i < neurons[layer].size(); i++ )
        {
            for ( unsigned long a = 0; a < neurons[layer-1].size(); a++ )
            {
                file << neurons[layer][i].dendrits[a].weight << std::endl;
            }
        }
    }

    file.close();
    //std::cout << "[OK] Weights saved" << std::endl;
}

void NeuralNetwork::LoadWeights(std::string path)
{
    std::ifstream file;
    std::string line;
    file.open(path);

    if(file.is_open() && !file.eof())
    {
        for ( unsigned long layer = 1; layer < this->layers; layer++ )
        {
            for ( unsigned long i = 0; i < neurons[layer].size(); i++ )
            {
                for ( unsigned long a = 0; a < neurons[layer-1].size(); a++ )
                {
                    double w;
                    file >> w;
                    Neuron::Dendrit d;
                    d.weight = w;
                    d.inputAkson = &neurons[layer-1][a].akson;
                    this->neurons[layer][i].dendrits.push_back(d);
                }
            }
        }
    }
    file.close();
    //std::cout << "[OK] Weights loaded" << std::endl;
}

void NeuralNetwork::CreateWeights()
{
    for ( unsigned long layer = 1; layer < this->layers; layer++ )
    {
        for ( unsigned long i = 0; i < neurons[layer].size(); i++ )
        {
            for ( unsigned long a = 0; a < neurons[layer-1].size(); a++ )
            {
                Neuron::Dendrit d;
                d.weight = GetRandomFloat();
                d.inputAkson = &neurons[layer-1][a].akson;
                this->neurons[layer][i].dendrits.push_back(d);
            }
        }
    }
    //std::cout << "[OK] Weights created" << std::endl;
}

void NeuralNetwork::ShowWeights()
{
    for ( unsigned long layer = 1; layer <  this->layers; layer++)
    {
        std::cout << std::endl << "Layer[" << layer << "] : " << std::endl;
        for ( unsigned long i = 0; i < this->neurons[layer].size(); i++ )
        {
            for ( unsigned long a = 0; a < this->neurons[layer][i].dendrits.size(); a++ )
            {
                std::cout << this->neurons[layer][i].dendrits[a].weight << " ";
            }
        }
        std::cout << std::endl;
    }
}

std::vector<double> NeuralNetwork::GetDataFromImage(QImage* dataImage)
{
    int width = dataImage->width();
    int height = dataImage->height();

    std::vector<double> data;

    for ( int x = 0; x < width; x++ )
    {
        for ( int y = 0; y < height; y++ )
        {
            QColor color = dataImage->pixel(x,y);
            double middle = (color.red() + color.green() + color.blue()) / 3;
            //middle = middle > 127? 1 : 0;
            data.push_back(middle);
        }
    }
    return data;
}

void NeuralNetwork::Run()
{
    for ( unsigned long layer = 0; layer < this->layers; layer++ )
        for ( unsigned long i = 0; i < neurons[layer].size(); i++ )
            neurons[layer][i].value = neurons[layer][i].activate();
}

NeuralNetwork::Neuron::Neuron(double startValue, bool isInput)
{
    this->value = startValue;
    this->isInput = isInput;
    this->akson.parentNeuron = this;
}

void NeuralNetwork::CreateStructure()
{
    srand (static_cast <unsigned> (time(nullptr)));
    for ( unsigned long layer = 0; layer < this->layers; layer++ )
    {
        std::vector<Neuron> neuronLayer;
        for ( int i = 0; i < layersVector[layer]; i++ )
        {
            if ( layer == 0 )
                neuronLayer.push_back( Neuron( data[static_cast<unsigned long>(i)], true ) );
            else {
                neuronLayer.push_back( Neuron( 0, false ) );
            }
        }
        this->neurons.push_back(neuronLayer);
    }
}

void NeuralNetwork::ShowStructure()
{
    for ( unsigned long layer = 0; layer < this->layers; layer++ )
    {
        std::cout << std::endl << "Layer[" << layer << "] : " << std::endl;
        for ( unsigned long i = 0; i < this->neurons[layer].size(); i++ ) { std::cout << this->neurons[layer][i].value << " "; }
        std::cout << std::endl;
    }
}

int NeuralNetwork::GetNetworkResult()
{
    int digit = 0;
    double maxValue = this->neurons.back()[0].value;
    for ( unsigned long i = 1; i < this->neurons.back().size(); i++ )
    {
        if ( neurons.back()[i].value > maxValue )
        {
            maxValue = neurons.back()[i].value;
            digit = static_cast<int>(i);
        }
    }
    return digit;
}

std::vector<int> GenerateAnswer(int answer)
{
    std::vector<int> ans;
    for ( int i = 0; i < 10; i++ )
    {
        if ( i == answer )
            ans.push_back(10);
        else
            ans.push_back(0);
    }
    return ans;
}

void NeuralNetwork::Learn(int answer)
{
    std::vector<int> answeres = GenerateAnswer(answer);
    Run();
    if ( GetNetworkResult() == answer )
    {
        std::cout << "Network has been learned!" << std::endl;
        this->SaveWeights(this->WeightPath);
        return;
    }
    for ( unsigned long i = 0; i < this->neurons.back().size(); i++ )
    {
        this->neurons.back()[i].d = answeres[i] - neurons.back()[i].value;
    }
    for ( unsigned long layer = this->layers-2; layer > 0; --layer )
    {
        for ( unsigned long i = 0; i < this->neurons[layer].size(); i++ )
        {
            double _d = 0;
            for ( unsigned long a = 0; a < this->neurons[layer+1].size(); a++ )
            {
                _d += this->neurons[layer+1][a].d * this->neurons[layer+1][a].dendrits[i].weight;
            }
            this->neurons[layer][i].d = _d;
        }
    }

    for ( unsigned long layer = 1; layer < this->layers; layer++ )
    {
        for ( unsigned long i = 0; i < this->neurons[layer].size(); i++ )
        {
            for ( unsigned long a = 0; a < this->neurons[layer-1].size(); a++ )
            {
                double _dw = this->N * this->neurons[layer][i].d * D_Sygma(this->neurons[layer][i].value) * this->neurons[layer-1][a].value;
                this->neurons[layer][i].dendrits[a].weight += _dw;
            }
        }
    }
    this->SaveWeights(this->WeightPath);
    //std::cout << "[OK] Study example ready" << std::endl;
}

NeuralNetwork::NeuralNetwork(std::vector<int> l, QImage* dataImage)
{
    this->layersVector = l;
    this->layers = l.size();
    this->data = this->GetDataFromImage(dataImage);

    this->CreateStructure();
}

NeuralNetwork::NeuralNetwork(std::vector<int> l, std::vector<double> startData)
{
    this->layersVector = l;
    this->layers = l.size();
    this->data = startData;

    this->CreateStructure();

}
