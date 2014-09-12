#ifndef __particleFilter__filter__
#define __particleFilter__filter__

#include "string"
#include "particle.h"
#include "ioInterfaces.h"
#include <random>

class filter {
private:
    int dim;
    int nParticle;
    std::string inFile, outFile;
    std::vector<particle*> particles;
    reader *myReader;
    writer *myWriter;
    
    std::default_random_engine generator;
    std::normal_distribution<double> distNormal;
    std::uniform_real_distribution<double> distUniform;
    
    std::vector<double> *measurement;
    std::vector<double> mean;
    std::vector<double> var;
    std::vector<std::vector<double>> covar;
    bool fokkerPlanck(std::vector<double> &pos);
    
    
public:
    filter(int _dim, int _nParticle, std::string _inFile, std::string _outFile);
    ~filter();
    bool initialize();
    bool resample();
    bool moveParticles();
    void run();
    std::vector<double>& getMean();
    std::vector<double>& getVar();
    std::vector<std::vector<double> >& getCoV();
    double getEffectiveSampleSize();
    double likelihood(std::vector<double> &pos);
};

#endif