#include "Solver.hpp"
#include <iostream>
#include <queue>
#include <windows.h>
#include <unordered_map>

Solver::Solver(const Cube& cube) 
{
    this->cube = cube;
    turns_for_group.resize(4);

    turns_for_group[0] = {
        L, R, F, B, U, D, 
        Lp, Rp, Fp, Bp, Up, Dp, 
        L2, R2, F2, B2, U2, D2
    };

    turns_for_group[1] = {
        L, R, U, D, 
        Lp, Rp, Up, Dp, 
        L2, R2, F2, B2, U2, D2
    };

    turns_for_group[2] = {
        U, D, Up, Dp, 
        L2, R2, F2, B2, U2, D2
    };

    turns_for_group[3] = { L2, R2, F2, B2, U2, D2 };
}


bool Solver::next_turn_valid(Rotation last, Rotation next)
{
    if (last < next)
        std::swap(last, next);
    return (std::abs((int)last) % std::abs((int)next)) != 0;
}

// IDDFS algorithm
std::list<Rotation> Solver::improve_group(size_t cur_group) 
{
    std::list<Rotation> result;
    if (cube.on_group(cur_group + 1))
        return result; 
    for (size_t depth = 0; depth <= 17; ++depth) {
        std::cout << ". ";
        auto [found, remaining] = dls(cube, cur_group, std::list<Rotation>(), depth);
        if (!found.empty()) {
            result = found;
            break;
        } else if (!remaining) {
            return std::list<Rotation>();
        }
    }
    std::cout << std::endl;
    return result;
}

std::pair<std::list<Rotation>, bool> Solver::dls(Cube c, size_t cur_group, std::list<Rotation> cur_rotation, size_t depth) 
{
    if (depth == 0) {
        c.combo_move(cur_rotation);
        if (c.on_group(cur_group + 1)) {
            return { cur_rotation, true };
        } else {
            return { std::list<Rotation>(), true };
        }
    }

    bool any_remaining = false;
    for (auto x: turns_for_group[cur_group]) {
        std::list<Rotation> new_rotation{ cur_rotation };
        if (!cur_rotation.empty()) {
            if (next_turn_valid(cur_rotation.back(), x)) {
                new_rotation.push_back(x);
            } else {
                continue;
            }
        } else {
            new_rotation.push_back(x);
        }
        auto [found, remaining] = dls(c, cur_group, new_rotation, depth - 1);
        if (!found.empty()) 
            return { found, true };
        if (remaining)
            any_remaining = true;
    }
    return {std::list<Rotation>(), any_remaining};
}

std::list<Rotation> Solver::solution()
{
    std::list<Rotation> answer;
    size_t cur_group = 0;
    std::cout << "solution: 0%\n";
    while (cur_group < 4) {
        if (cube.on_group(cur_group + 1)) {
            cur_group++;
            continue;
        }
        // std::cout << "cur_group " << cur_group << std::endl;
        auto seq = improve_group(cur_group); 
        Cube temp{ cube };
        temp.combo_move(seq);
        if (temp.on_group(cur_group + 1)) {
            cur_group++;
            cube.combo_move(seq);
            // std::cout << "[CUR_GROUP]: " << cur_group << std::endl << cube <<std::endl;
            for (auto x: seq) {
                // std::cout << turns[x] << ' ';
                answer.push_back(x);
            }
            std::cout << "\nsolution: " << cur_group * 25 << "%\n";
        }      
    } 
    return answer;
}