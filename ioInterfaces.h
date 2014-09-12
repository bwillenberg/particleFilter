#ifndef __particleFilter__ioInterfaces__
#define __particleFilter__ioInterfaces__

#include <string>
#include <vector>
#include <fstream>

class ioInterface {
protected:
    std::string fileName;
public:
    ioInterface(std::string _fileName);
    ~ioInterface();
};

class reader : public ioInterface {
private:
    std::ifstream file;
    std::string line;
public:
    reader(std::string _fileName);
    ~reader();
    std::vector<double>* getLineVector();
};

class writer : public ioInterface {
private:
    std::ofstream file;
public:
    writer(std::string _fileName);
    ~writer();
    bool writeVectorLine(std::vector<double>& v);
};

#endif