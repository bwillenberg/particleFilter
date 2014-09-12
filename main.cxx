#include <iostream>
#include "particle.h"
#include "filter.h"


int main() {
    
    filter *myFilter = new filter(7, 1000, "../../measurement.txt", "result.txt");
    std::cout << "Starting Simulation..." << std::endl;
    myFilter->run();
    
    delete myFilter;
    
    return 1;
}