#include <iostream>
#include <fstream>
using namespace std;

#include "compiler.h"
#include "exec.h"

// Интерпретатор: читает текстовую программу из файла и сразу выполняет ее
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cerr << "Usage: ./emu <program text>" << endl;
        return -1;
    }

    ifstream in(argv[1]);
    if (!in)
    {
        cerr << "Can't open file " << argv[1] << endl;
        return -1;
    }

    vector<uint8_t> prog = compile(in);
    // В первых двух байтах точка входа
    uint16_t ep = *(uint16_t *)prog.data();
    execute(prog.data() + 2, ep);

    return 0;
}