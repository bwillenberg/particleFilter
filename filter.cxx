#include "filter.h"
#include <random>
#include <iostream>

inline double sqr(double x) {return x*x;};

inline double laplacePDF(double x, double mu, double b) {
    return exp( -fabs( x-mu ) / b ) / (2.*b);
   // return exp( -sqr(x-mu) / 2. / b ) / sqrt(2.*M_PI *b);
};

template <typename T>
std::ostream& operator<<(std::ostream& os, std::vector<T> const& list) {
    typename std::vector<T>::const_iterator i = list.begin();
    
    os << "[ " << *i++ ;
    while ( i != list.end()) os << ", " << *i++;
    os << "]";
    return os;
}

filter::filter(int _dim, int _nParticle, std::string _inFile, std::string _outFile)
: dim(_dim), nParticle(_nParticle), inFile(_inFile), outFile(_outFile), measurement(NULL)
{
    myReader = new reader(_inFile);
    myWriter = new writer(_outFile);
    this->initialize();
    mean.assign(dim,0);
    covar.assign(dim, std::vector<double>(dim,0));
    var.assign(dim, 0);
    generator = std::default_random_engine();
    distNormal = std::normal_distribution<double>(0.,1.);
    distUniform = std::uniform_real_distribution<double>(0.,1.);
}

filter::~filter() {}

bool filter::initialize() {
    std::uniform_real_distribution<double> uniform(0.0,1.0);

    std::vector<double> position(dim,0.);
    for (int i=0; i<nParticle; ++i) {
        for( int j=0; j<dim; ++j)
            position[j] = uniform(generator);
        particles.push_back(new particle(dim));
        particles.back()->SetPoint(position);
        particles.back()->SetWeight(1./nParticle);
    }
    
    return true;
}

bool filter::resample() {
    std::cout << "resampling ..." << std::endl;
    std::vector<std::normal_distribution<double>> dists;
    for(int d=0; d<dim; ++d)
        dists.push_back(std::normal_distribution<double>(mean[d],sqrt(var[d])));
    
    for (auto p : particles) delete p;
    particles.clear();
    std::vector<double> x(dim,0.);
    for(int i=0; i<nParticle; ++i) {
        for(int d=0; d<dim; ++d)
            do {
                x[d] = dists[d](generator);
            } while (x[d]<0 || x[d]>1);
        particles.push_back(new particle(x,1./nParticle));
    }
    return true;
}

void filter::run() {
    int counter = 0;
    this->getMean();
    this->getVar();
    std::cout << mean << std::endl;
    std::cout << var << std::endl;
    
    while ((measurement = myReader->getLineVector())) {
        // move the particles according to fokker planck
        this->moveParticles();
        
        // calc the new weight for all particles
        double sumWeights=0.;
        for(auto p :  particles) {
            p->SetWeight(p->GetWeight() * likelihood(p->GetPoint()));
            sumWeights += p->GetWeight();
        }
        for(auto p : particles) {
            p->SetWeight(p->GetWeight()/sumWeights);
        }
            
        double size = this->getEffectiveSampleSize();
        if(size < 0.8*nParticle)
            this->resample();
        this->getMean();
        this->getVar();
        myWriter->writeVectorLine(mean);
        std::cout << counter++ << "\t" << size << std::endl;
        std::cout << mean << std::endl;
        std::cout << var << std::endl;
    }
}

std::vector<double>& filter::getMean() {
    mean.assign(dim, 0);
    for(int i=0; i<dim; ++i)
        for (auto p : particles)
            mean[i] += (p->GetPoint())[i] * p->GetWeight();
    return mean;
}

std::vector<std::vector<double> >& filter::getCoV() {
    covar.assign(dim,std::vector<double>(dim,0));
    for(int i=0; i<dim; ++i) {
        for(int j=0; j<dim; ++j) {
            for(auto p : particles) {
                covar[i][j] += (p->GetPoint())[i] * (p->GetPoint())[j];
            }
            covar[i][j] -= nParticle * mean[i]*mean[j];
            covar[i][j] /= nParticle - 1;
        }
    }
    return covar;
}

std::vector<double>& filter::getVar() {
    for(int i=0; i<dim; ++i) {
        var[i] = 0.;
        for(auto p:particles)
            var[i] += sqr(p->GetPoint()[i]-mean[i]) * p->GetWeight();
    }
    return var;
}

double filter::getEffectiveSampleSize() {
    double res = 0.;
    for(auto p:particles)
        res += sqr(p->GetWeight());
    return 1./res;
}

double filter::likelihood(std::vector<double> &pos) {
    // pos is the postion of the particle
    // the measurment is taken from *measurement
    // we assume a laplace distribution of the measurements
    double res = 1.;
    for(int d=0; d<dim; ++d)
        res *= laplacePDF(measurement->at(d), 0, pos.at(d));
    return res;
}

bool filter::moveParticles() {
    for(auto p : particles)
        fokkerPlanck(p->GetPoint());
    return 1;
}

bool filter::fokkerPlanck(std::vector<double> &pos) {
    double scaleFactor(0.01);
    double jumpProp(0.001);
    for(int d=0; d<pos.size(); ++d) {
        // random walk
        pos[d] = std::max(1.E-15,std::min(1.,pos[d] + scaleFactor * distNormal(generator)));
        
        // random jumps
        if(distUniform(generator) < jumpProp)
            pos[d] = distUniform(generator);
    }
    
    // pos updated successfully
    return true;
}
