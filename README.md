# Rubic's cube solver

This project was written for learning purposes. It performs Thistlethwaite’s Algorithm based on Groups Theory.

## Architecture of Cube

The very first question we need to ask ourselves is "How to handle cube states and rotations effectively?".\
Well, there are 2 ways:
1. **Naive**: store cube as 54 integers, `int cube[6][3][3]` for example. It's human-friendly but quite slow.
2. **BitWISE**: Cube's sticker can be 1 of 6 colors, so 4 bits of memory is enough to store it. \
Using Higher Math formulas we can figure out that cube's side is just 4-bytes integer `uint32_t` and cube is actually `uint32_t sides[6]`\
This way provides ability to handle rotations on the bits level which is really fast and, suddenly, easier for coding (see `Cube::rotate()` for example).

```
Now every side is uint32_t:
4 bits per square - 0000 0000 0000 0000 0000 0000 0000 0000
indices of sides -- 0    1    2    3    4    5    6    7    
```

That's how it looks on cube (x is index of side).
![Flat view](https://github.com/thermo911/RubicsCubeSolver/blob/master/images/cube.png)

## Algorithm

There are lots of existing optimal algorithms, however most of them are hard to implement. That is why I selected Thistlethwaite’s. \
It's understandable, simple and elegant, based on dividing solving problem to smaller problems which is easier to solve.  \
You can read good [explanation](https://medium.com/@benjamin.botto/implementing-an-optimal-rubiks-cube-solver-using-korf-s-algorithm-bf750b332cf9) of main idea, but
keep in mind that there are some mistakes in it, so I'm going to describe some moments too.

### Groups checking
* **G0** - all possible states of cube. There are no moves could take cube out of it.  
* **G1** - subgroup of **G0**.
  * All cube's edges are correctly oriented. 
To check if cube is in **G1**, use [that](http://cube.rider.biz/zz.php?p=eoline) method.
* **G2** - subgroup of **G0**.
  * Corner squares on up and down sides are colored with up or down colors.
  * Squares placed between left and right slices cannot be of left or right colors.
* **G3** - subgroup of **G2** and it is a bit tricky.
  * From that group cube can be solved only by 180-degrees moves.
To learn how to check this group you can look my `Cube::on_group3()`, `Cube::get_corner_index()`, `Cube::check_parity()`.
* **G4** - solved cube.

### Moves to improve group
1. **G0** -> **G1**: all possible moves.
2. **G1** -> **G2**: all possible moves excluding F, F', B, B'.
3. **G2** -> **G3**: doubled moves (L2, R2, ...) + U, U', D, D'.
4. **G3** -> **G4**: only doubled moves.

### Pseudocode
```
current = 0                     // current group
while not solved():
    if on_group(current + 1):
        current += 1
        continue
    else:
        improve_group(group)    // increase group using moves desribed earlier 
```
`Solver::improve_group()` method uses [IDDFS](https://en.wikipedia.org/wiki/Iterative_deepening_depth-first_search#:~:text=In%20computer%20science%2C%20iterative%20deepening,until%20the%20goal%20is%20found) algorithm to iterate through moves combinations. 
It finds shortest path from one group to another like BFS, but takes only O(n) memory, where n - length of combination.
