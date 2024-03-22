#include "exec.h"

#include <iostream>
#include <vector>
using namespace std;

#include "command.h"
#include <stack.h>

// Функция для выполнения программы из байт-кода
void execute(uint8_t *prog, uint16_t ep)
{    
    // Program counter
    int32_t pc = ep;
    // Program stack
    Stack<int32_t> stack;
    // CPU registers
    vector<int> registers = vector<int>(256, 0);

    int32_t value1;
    int32_t value2;
    int32_t addr;
    uint8_t reg;

    while (true)
    {
        Opcode opcode = static_cast<Opcode>(prog[pc]);
        switch (opcode)
        {
        case Opcode::BEGIN:
            pc += 1;
            continue;

        case Opcode::END:            
            pc += 1;
            return;

        case Opcode::POP:
            pc += 1;
            stack.pop();
            continue;

        case Opcode::ADD:
            pc += 1;
            stack.pop(value1);
            stack.pop(value2);
            value1 += value2;
            stack.push(value1);
            continue;

        case Opcode::SUB:
            pc += 1;
            stack.pop(value1);
            stack.pop(value2);
            value1 -= value2;
            stack.push(value1);
            continue;

        case Opcode::MUL:
            pc += 1;
            stack.pop(value1);
            stack.pop(value2);
            value1 *= value2;
            stack.push(value1);
            continue;

        case Opcode::DIV:
            pc += 1;
            stack.pop(value1);
            stack.pop(value2);
            value1 /= value2;
            stack.push(value1);
            continue;

        case Opcode::OUT:
            pc += 1;
            stack.pop(value1);
            cout << value1 << endl;
            continue;

        case Opcode::IN:
            pc += 1;
            cin >> value1;
            stack.push(value1);
            continue;

        case Opcode::RET:
            stack.pop(addr);
            pc = addr;
            continue;

        case Opcode::PUSH:
            value1 = *(int32_t *)(&prog[pc + 1]);
            pc += 5;
            stack.push(value1);
            continue;

        case Opcode::PUSHR:
            reg = prog[pc + 1];
            pc += 2;
            stack.push(registers[reg]);
            continue;

        case Opcode::POPR:
            reg = prog[pc + 1];
            pc += 2;
            stack.pop(registers[reg]);
            continue;

        case Opcode::JMP:
            addr = *(uint16_t *)(&prog[pc + 1]);
            pc = addr;
            continue;

        case Opcode::JEQ:
            addr = *(uint16_t *)(&prog[pc + 1]);
            stack.pop(value1);
            stack.pop(value2);
            if (value1 == value2)
            {
                pc = addr;
            }
            else
            {
                pc += 3;
            }
            continue;

        case Opcode::JNE:
            addr = *(uint16_t *)(&prog[pc + 1]);
            stack.pop(value1);
            stack.pop(value2);
            if (value1 != value2)
            {
                pc = addr;
            }
            else
            {
                pc += 3;
            }
            continue;

        case Opcode::JA:
            addr = *(uint16_t *)(&prog[pc + 1]);
            stack.pop(value1);
            stack.pop(value2);
            if (value1 > value2)
            {
                pc = addr;
            }
            else
            {
                pc += 3;
            }
            continue;

        case Opcode::JAE:
            addr = *(uint16_t *)(&prog[pc + 1]);
            stack.pop(value1);
            stack.pop(value2);
            if (value1 >= value2)
            {
                pc = addr;
            }
            else
            {
                pc += 3;
            }
            continue;

        case Opcode::JB:
            addr = *(uint16_t *)(&prog[pc + 1]);
            stack.pop(value1);
            stack.pop(value2);
            if (value1 < value2)
            {
                pc = addr;
            }
            else
            {
                pc += 3;
            }
            continue;

        case Opcode::JBE:
            addr = *(uint16_t *)(&prog[pc + 1]);
            stack.pop(value1);
            stack.pop(value2);
            if (value1 <= value2)
            {
                pc = addr;
            }
            else
            {
                pc += 3;
            }
            continue;

        case Opcode::CALL:
            addr = *(uint16_t *)(&prog[pc + 1]);
            stack.push(pc + 3);
            pc = addr;
            continue;

        default:
            break;
        }
    }
}