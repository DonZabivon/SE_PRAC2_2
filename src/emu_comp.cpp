#include <iostream>
#include <fstream>
using namespace std;

#include "compiler.h"

// Компиляция текстовой программы в байт-код
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cerr << "Usage: ./emu_compiler <program text> <program exe>" << endl;
        return -1;
    }

    ifstream in(argv[1]);
    if (!in)
    {
        cerr << "Can't open file " << argv[1] << endl;
        return -1;
    }

    vector<uint8_t> prog = compile(in);
    
    ofstream out(argv[2], fstream::binary);
    if (!out)
    {
        cerr << "Can't create file " << argv[2] << endl;
        return -1;
    }
    out.write(reinterpret_cast<const char *>(prog.data()), (std::streamsize)prog.size());

    return 0;
}
