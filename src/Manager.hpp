#pragma once
#include "Cube.hpp"
#include "Solver.hpp"
#include <unordered_map>

class Manager
{
public:
    Manager();
    void start();
    
private:
    Cube cube;

    void load_cube(const std::string&);
    void save_cube(const std::string&);
    void perform_solution();

    std::unordered_map<Rotation, std::string> turns_to_strings; 
    std::unordered_map<std::string, Rotation> strings_to_turns;
    std::list<Rotation> parse_turns(const std::string&);
};