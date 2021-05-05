#pragma once
#include "Cube.hpp"
#include <list>
#include <vector>
#include <unordered_map>

class Solver
{
public:
    Solver() = default;
    Solver(const Cube& cube);
    std::list<Rotation> solution();
    
private:
    Cube cube;

    std::list<Rotation> improve_group(size_t cur_group);
    std::pair<std::list<Rotation>, bool> dls(Cube, size_t, std::list<Rotation>, size_t);
    std::vector<std::vector<Rotation>> turns_for_group;
    
    bool next_turn_valid(Rotation, Rotation);
};
