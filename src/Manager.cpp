#include "Manager.hpp"
#include <iostream>
#include <fstream>
#include <list>
#include <sstream>

Manager::Manager()
{
    turns_to_strings = { 
        {U, "U"}, {Up, "U\'"}, {L, "L"}, {Lp, "L\'"}, {F, "F"}, {Fp, "F\'"}, 
        {R, "R"}, {Rp, "R\'"}, {B, "B"}, {Bp, "B\'"}, {D, "D"}, {Dp, "D\'"},
        {U2, "U2"}, {L2, "L2"}, {F2, "F2"}, {R2, "R2"}, {B2, "B2"}, {D2, "D2"} 
    };

    strings_to_turns = { 
        {"U", U}, {"U\'", Up}, {"L", B}, {"L\'", Lp}, {"F", F}, {"F\'", Fp}, 
        {"R", R}, {"R\'", Rp}, {"B", B}, {"B\'", Bp}, {"D", D}, {"D\'", Dp},
        {"U2", U2}, {"L2", L2}, {"F2", F2}, {"R2", R2}, {"B2", B2}, {"D2", D2} 
    };
}

void Manager::start()
{
    std::cout << "Welcome!\n";
    std::string command1, command2;
    while (true) 
    {
        std::cout << ">>> ";
        std::cin >> command1;
        if (command1 == "load") {
            std::cin >> command2;
            load_cube(command2);

        } else if (command1 == "save") {
            std::cin >> command2;
            save_cube(command2);

        } else if (command1 == "print") {
            std::cout << cube << std::endl;

        } else if (command1 == "new") {
            cube = Cube();
            std::cout << cube << std::endl;

        } else if (command1 == "rotate") {
            std::getline(std::cin, command2);
            auto seq = parse_turns(command2);
            cube.combo_move(seq);
            std::cout << cube << std::endl;
            
        } else if (command1 == "solve") {
            perform_solution();
        } else if (command1 == "exit") {
            std::cout << "logout\n";
            break;
        }
    }
}

void Manager::load_cube(const std::string& filename)
{
    std::ifstream in(filename);
    if (!in.good()) {
        in.close();
        std::cout << "file \"" << filename << "\" not found\n";
        return;
    }
    uint32_t sides[6];
    for (size_t i = 0; i < 6; ++i) {
        in >> sides[i];
    }
    cube = Cube { sides };
    in.close();
}

void Manager::save_cube(const std::string& filename)
{
    std::ofstream out(filename);
    for (size_t i = 0; i < 6; ++i) {
        out << cube.get_side(i) << std::endl;
    }
    out.close();
}

std::list<Rotation> Manager::parse_turns(const std::string& str)
{
    std::stringstream ss;
    std::list<Rotation> seq;
    ss << str;
    std::string turn;
    while (ss >> turn) {
        seq.push_back(strings_to_turns[turn]);
    }
    return seq;
}

void Manager::perform_solution()
{
    Solver solver = Solver{ cube };
    std::list<Rotation> seq = solver.solution();
    for (auto x: seq) {
        std::cout << turns_to_strings[x] << ' ';
    }
    cube.combo_move(seq);
    std::cout << std::endl << cube << std::endl;
}