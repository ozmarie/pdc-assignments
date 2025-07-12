#define PROB_SPORE_TO_HYPHAE 0.5 // Probability that SPORE -> YOUNG
#define PROB_MUSHROOM 0.8 // Probability that MATURING -> MUSHROOM (else OLDER)
#define PROB_SPREAD 0.25  // Probability that EMPTY -> SPORE (from neighboring YOUNG)

// For the current index into the flattened grid, apply the mushroom simulation rules.
void apply_rules(double randN, int *grid, int *newGrid, int id, int w) {
    // Implementing the Mushroom Simulation Rules
    if (grid[id] == 0) {  // EMPTY, probSpread to SPORE
        int neighbors[8] = {
            grid[id + (w + 2)], //south
            grid[id - (w + 2)], //north
            grid[id + 1],       // east
            grid[id - 1],       // west
            grid[id + (w + 3)], // southeast
            grid[id - (w + 3)], // northwest
            grid[id - (w + 1)], // northeast
            grid[id + (w + 1)]};// southwest
        for (int k = 0; k < 8; k++) {  // this should be okay sequential in the parallel version since it is only 8 elements

            if (neighbors[k] == 1 && randN < PROB_SPREAD) {
                newGrid[id] = 1;
                break;  // break out of the inner k loop
            } else {
                newGrid[id] = 0;
            }
        }  // (ELSE EMPTY, no change)
    } else if (grid[id] == 1) {  // SPORE, probSporeToHyphae to YOUNG

        if (randN < PROB_SPORE_TO_HYPHAE) {
            newGrid[id] = 2;
        } else {
            newGrid[id] = 1;
        }
    } else if (grid[id] == 2) {  // YOUNG -> MATURING
        newGrid[id] = 3;
    } else if (grid[id] == 3) {  // MATURING, probMushroom to MUSHROOM else OLDER
        if (randN < PROB_MUSHROOM) {
            newGrid[id] = 4;  // MUSHROOM
        } else {
            newGrid[id] = 5;  // OLDER
        }
    } else if (grid[id] == 4 || grid[id] == 5) {  // MUSHROOM or OLDER -> DECAYING
        newGrid[id] = 6;
    } else if (grid[id] == 6) {  // DECAYING -> DEAD1
        newGrid[id] = 7;
    } else if (grid[id] == 7) {  // DEAD1 ->     DEAD2
        newGrid[id] = 8;
    } else if (grid[id] == 8) {  // DEAD2 -> INERT
        newGrid[id] = 9;
    }
 
}
