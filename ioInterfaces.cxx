#include "ioInterfaces.h"
#include <sstream>
#include <iomanip>


ioInterface::ioInterface(std::string _fileName)
: fileName(_fileName) {}

ioInterface::~ioInterface() {}

reader::reader(std::string _fileName) : ioInterface(_fileName)
{
    file.open(fileName);
}

std::vector<double>* reader::getLineVector()
{
    // read one line
    if(!std::getline(file,line))
        return NULL;
    // input stringstream to read from the line
    std::istringstream stm(line);
        
    // create a vector containing inters in the line (left to right)
    using iterator = std::istream_iterator<double> ;
    std::vector<double> *seq = new std::vector<double> { iterator(stm), iterator() } ;
    
    // reverse the vector
    // std::reverse( std::begin(*seq), std::end(*seq) );
    
    return seq ; // and return it
}

reader::~reader()
{
    file.close();
}

writer::writer(std::string _fileName) : ioInterface(_fileName)
{
    file.open(fileName);
    file << std::fixed;
    file <<  std::setprecision(10);
}

bool writer::writeVectorLine(std::vector<double> &v) {
    for(auto vi : v)
        file << vi << "\t";
    file << std::endl;
    
    return true;
}

writer::~writer()
{
    file.close();
}