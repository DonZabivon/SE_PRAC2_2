#include <iostream>
#include <fstream>
using namespace std;

#include "exec.h"

// Выполнение программы из байт-кода
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cerr << "Usage: ./emu <program exe>" << endl;
        return -1;
    }

    ifstream in(argv[1], fstream::binary);
    if (!in)
    {
        cerr << "Can't open file " << argv[1] << endl;
        return -1;
    }

    in.seekg (0, in.end);
    size_t size = (size_t)in.tellg();
    in.seekg(0, in.beg);

    uint8_t *prog = new uint8_t[size];

    in.read(reinterpret_cast<char *>(prog), (std::streamsize)size);
    // В первых двух байтах точка входа
    uint16_t ep = *(uint16_t*)prog;
    execute(prog + 2, ep);

    delete[] prog;

    return 0;
}
