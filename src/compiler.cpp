#include "compiler.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <cassert>
using namespace std;

#include "command.h"

// Структура для хранения команды в промежуточном формате,
// с текстовыми метками
struct CommandText
{
    Opcode opcode;
    uint16_t addr;
    uint8_t reg;
    int32_t value;
    string label;

    CommandText(Opcode opcode, uint16_t addr) : opcode(opcode), addr(addr) {}
};

// Функция для разбора программы и формирования промежуточного кода
static void parse(istream &in, vector<CommandText> &commands, unordered_map<string, uint16_t> &labels, uint16_t &pc, uint16_t &ep)
{
    pc = 0; // program counter
    ep = (uint16_t) -1; // entry point
    
    string line;
    while (getline(in, line))
    {
        istringstream iss(line);
        string first;
        iss >> first;
        if (first.empty() || (first.front() == ';' || first.front() == '/'))
        {
            // comment
            continue;
        }

        if (first.back() == ':')
        {
            // метка
            first.pop_back();
            if (labels.count(first) > 0)
            {
                throw runtime_error("Label " + first + " redeclared.");
            }
            // Запоминание адреса метки
            labels.insert(make_pair(first, pc));
            first = "";
            iss >> first;
        }

        if (first.empty()) continue;

        if (first.front() == ';' || first.front() == '/')
        {
            // comment
            continue;
        }        

        if (first == "BEGIN")
        {
            // Запоминаем точку входа
            ep = pc;
            commands.emplace_back(Opcode::BEGIN, pc);
            pc += 1;
            continue;
        }

        if (first == "END")
        {
            commands.emplace_back(Opcode::END, pc);
            pc += 1;
            continue;
        }

        if (first == "POP")
        {
            commands.emplace_back(Opcode::POP, pc);
            pc += 1;
            continue;
        }

        if (first == "ADD")
        {
            commands.emplace_back(Opcode::ADD, pc);
            pc += 1;
            continue;
        }

        if (first == "SUB")
        {
            commands.emplace_back(Opcode::SUB, pc);
            pc += 1;
            continue;
        }

        if (first == "MUL")
        {
            commands.emplace_back(Opcode::MUL, pc);
            pc += 1;
            continue;
        }

        if (first == "DIV")
        {
            commands.emplace_back(Opcode::DIV, pc);
            pc += 1;
            continue;
        }

        if (first == "OUT")
        {
            commands.emplace_back(Opcode::OUT, pc);
            pc += 1;
            continue;
        }

        if (first == "IN")
        {
            commands.emplace_back(Opcode::IN, pc);
            pc += 1;
            continue;
        }

        if (first == "RET")
        {
            commands.emplace_back(Opcode::RET, pc);
            pc += 1;
            continue;
        }

        if (first == "PUSH")
        {
            commands.emplace_back(Opcode::PUSH, pc);
            string value;
            iss >> value;
            commands.back().value = stoi(value);
            pc += 5;
            continue;
        }

        if (first == "PUSHR")
        {
            commands.emplace_back(Opcode::PUSHR, pc);
            string reg;
            iss >> reg;
            if (reg.front() != 'R')
            {
                throw runtime_error("Invalid register");
            }
            int reg_idx = stoi(reg.substr(1));
            if (reg_idx < 0 || reg_idx > 255)
            {
                throw runtime_error("Invalid register");
            }
            commands.back().reg = (uint8_t)reg_idx;
            pc += 2;
            continue;
        }

        if (first == "POPR")
        {
            commands.emplace_back(Opcode::POPR, pc);
            string reg;
            iss >> reg;
            if (reg.front() != 'R')
            {
                throw runtime_error("Invalid register");
            }
            int reg_idx = stoi(reg.substr(1));
            if (reg_idx < 0 || reg_idx > 255)
            {
                throw runtime_error("Invalid register");
            }
            commands.back().reg = (uint8_t)reg_idx;
            pc += 2;
            continue;
        }

        if (first == "JMP")
        {
            commands.emplace_back(Opcode::JMP, pc);
            string label;
            iss >> label;
            commands.back().label = label;
            pc += 3;
            continue;
        }

        if (first == "JEQ")
        {
            commands.emplace_back(Opcode::JEQ, pc);
            string label;
            iss >> label;
            commands.back().label = label;
            pc += 3;
            continue;
        }

        if (first == "JNE")
        {
            commands.emplace_back(Opcode::JNE, pc);
            string label;
            iss >> label;
            commands.back().label = label;
            pc += 3;
            continue;
        }

        if (first == "JA")
        {
            commands.emplace_back(Opcode::JA, pc);
            string label;
            iss >> label;
            commands.back().label = label;
            pc += 3;
            continue;
        }

        if (first == "JAE")
        {
            commands.emplace_back(Opcode::JAE, pc);
            string label;
            iss >> label;
            commands.back().label = label;
            pc += 3;
            continue;
        }

        if (first == "JB")
        {
            commands.emplace_back(Opcode::JB, pc);
            string label;
            iss >> label;
            commands.back().label = label;
            pc += 3;
            continue;
        }

        if (first == "JBE")
        {
            commands.emplace_back(Opcode::JBE, pc);
            string label;
            iss >> label;
            commands.back().label = label;
            pc += 3;
            continue;
        }

        if (first == "CALL")
        {
            commands.emplace_back(Opcode::CALL, pc);
            string label;
            iss >> label;
            commands.back().label = label;
            pc += 3;
            continue;
        }

        throw runtime_error("Unknown command.");
    }

    // Максимальный размер программы не более 65536
    assert(pc < 65536);
}

// Функция для формирования байт-кода из массива промежуточных команд
static vector<uint8_t> make_bytecode(uint16_t pc, uint16_t ep, vector<CommandText> &commands, unordered_map<string, uint16_t> &labels)
{    
    uint8_t prog[65536]; // массив байт-кода

    for (auto &cmd: commands)
    {
        switch (cmd.opcode)
        {
        case Opcode::BEGIN:
            prog[cmd.addr] = static_cast<uint8_t>(Opcode::BEGIN);
            continue;

        case Opcode::END:
            prog[cmd.addr] = static_cast<uint8_t>(Opcode::END);
            continue;

        case Opcode::POP:
            prog[cmd.addr] = static_cast<uint8_t>(Opcode::POP);
            continue;
        
        case Opcode::ADD:
            prog[cmd.addr] = static_cast<uint8_t>(Opcode::ADD);
            continue;
        
        case Opcode::SUB:
            prog[cmd.addr] = static_cast<uint8_t>(Opcode::SUB);
            continue;
        
        case Opcode::MUL:
            prog[cmd.addr] = static_cast<uint8_t>(Opcode::MUL);
            continue;
        
        case Opcode::DIV:
            prog[cmd.addr] = static_cast<uint8_t>(Opcode::DIV);
            continue;

        case Opcode::OUT:
            prog[cmd.addr] = static_cast<uint8_t>(Opcode::OUT);
            continue;
        
        case Opcode::IN:
            prog[cmd.addr] = static_cast<uint8_t>(Opcode::IN);
            continue;
        
        case Opcode::RET:
            prog[cmd.addr] = static_cast<uint8_t>(Opcode::RET);
            continue;
        
        case Opcode::PUSH:
            prog[cmd.addr] = static_cast<uint8_t>(Opcode::PUSH);
            *(int32_t *)(&prog[cmd.addr + 1]) = cmd.value;
            continue;
        
        case Opcode::PUSHR:
            prog[cmd.addr] = static_cast<uint8_t>(Opcode::PUSHR);
            prog[cmd.addr + 1] = cmd.reg;
            continue;
        
        case Opcode::POPR:
            prog[cmd.addr] = static_cast<uint8_t>(Opcode::POPR);
            prog[cmd.addr + 1] = cmd.reg;
            continue;

        // Все команды перехода извлекают адрес метки из карты меток
        case Opcode::JMP:
            prog[cmd.addr] = static_cast<uint8_t>(Opcode::JMP);
            if (labels.count(cmd.label) == 0)
            {
                throw runtime_error("Undeclared label " + cmd.label);
            }
            *(uint16_t *)(&prog[cmd.addr + 1]) = labels[cmd.label];
            continue;
        
        case Opcode::JEQ:
            prog[cmd.addr] = static_cast<uint8_t>(Opcode::JEQ);
            if (labels.count(cmd.label) == 0)
            {
                throw runtime_error("Undeclared label " + cmd.label);
            }
            *(uint16_t*)(&prog[cmd.addr + 1]) = labels[cmd.label];
            continue;
        
        case Opcode::JNE:
            prog[cmd.addr] = static_cast<uint8_t>(Opcode::JNE);
            if (labels.count(cmd.label) == 0)
            {
                throw runtime_error("Undeclared label " + cmd.label);
            }
            *(uint16_t*)(&prog[cmd.addr + 1]) = labels[cmd.label];
            continue;
        
        case Opcode::JA:
            prog[cmd.addr] = static_cast<uint8_t>(Opcode::JA);
            if (labels.count(cmd.label) == 0)
            {
                throw runtime_error("Undeclared label " + cmd.label);
            }
            *(uint16_t*)(&prog[cmd.addr + 1]) = labels[cmd.label];
            continue;
        
        case Opcode::JAE:
            prog[cmd.addr] = static_cast<uint8_t>(Opcode::JAE);
            if (labels.count(cmd.label) == 0)
            {
                throw runtime_error("Undeclared label " + cmd.label);
            }
            *(uint16_t*)(&prog[cmd.addr + 1]) = labels[cmd.label];
            continue;
        
        case Opcode::JB:
            prog[cmd.addr] = static_cast<uint8_t>(Opcode::JB);
            if (labels.count(cmd.label) == 0)
            {
                throw runtime_error("Undeclared label " + cmd.label);
            }
            *(uint16_t*)(&prog[cmd.addr + 1]) = labels[cmd.label];
            continue;
        
        case Opcode::JBE:
            prog[cmd.addr] = static_cast<uint8_t>(Opcode::JBE);
            if (labels.count(cmd.label) == 0)
            {
                throw runtime_error("Undeclared label " + cmd.label);
            }
            *(uint16_t*)(&prog[cmd.addr + 1]) = labels[cmd.label];
            continue;
        
        case Opcode::CALL:
            prog[cmd.addr] = static_cast<uint8_t>(Opcode::CALL);
            if (labels.count(cmd.label) == 0)
            {
                throw runtime_error("Undeclared label " + cmd.label);
            }
            *(uint16_t*)(&prog[cmd.addr + 1]) = labels[cmd.label];
            continue;
        
        default:
            break;
        }
    }

    vector<uint8_t> byte_code;

    uint8_t *pep = (uint8_t *)&ep;

    byte_code.insert(byte_code.end(), pep, pep + 2);

    byte_code.insert(byte_code.end(), prog, prog + pc);

    return byte_code;
}

// Функция для компиляции текстовой программы
vector<uint8_t> compile(istream &in)
{
    uint16_t pc; // размер программы    
    uint16_t ep; // точка входа (адресс инструкции BEGIN)
    vector<CommandText> commands; // массив промежуточных комманд
    unordered_map<string, uint16_t> labels; // карта меток

    parse(in, commands, labels, pc, ep);

    if (ep == (uint16_t)-1)
    {
        throw runtime_error("No entry point");
    }

    return make_bytecode(pc, ep, commands, labels);
}