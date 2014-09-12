#include "particle.h"
#include <cassert>
#include <iostream>
#include <iomanip>
#include <ostream>

using namespace std;

template<typename T>
ostream& operator<< (ostream& out, const vector<T> v) {
    int last = v.size() - 1;
    out << "[";
    for(int i = 0; i < last; i++)
        out << v[i] << ", ";
    out << v[last] << "]";
    return out;
}

particle::particle(int _dim)
: dim(_dim), weight(0)
{
    x.assign(dim, 0.);
}

particle::particle(std::vector<double>& _x, double _weight)
: dim(_x.size()), weight(_weight)
{
    x.assign(std::vector<double>::const_iterator(_x.begin()),
             std::vector<double>::const_iterator(_x.end()));
}

particle::~particle() {}

void particle::SetPoint(std::vector<double> &_x) {
    assert(dim == _x.size());
    x.assign(std::vector<double>::const_iterator(_x.begin()),
             std::vector<double>::const_iterator(_x.end()));
}

void particle::Print() {
    std::cout << std::fixed;
    std::cout << std::setprecision(5) << "w=" << weight << "\t";
    std::cout << std::setprecision(5) << "x=" << x << std::endl;
}