#ifndef __ParticleFilter__particle__
#define __ParticleFilter__particle__

#include <vector>

class particle {
private:
    int dim;
    double weight;
    std::vector<double> x;
    
public:
    particle(int _dim);
    particle(std::vector<double>& _x,double _weight=0);
    ~particle();
    double GetWeight(){return weight;};
    std::vector<double>& GetPoint(){return x;};
    void SetPoint(std::vector<double>& _x);
    void SetWeight(double _weight) {weight = _weight;};
    void Print();
};

#endif