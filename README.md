The goal of this project is to create a simple 2D predator-prey simulation. In this simulation, the prey are the ants and the predators are the doodlebugs. These critters live in a world composed of a 20 x 20 grid of cells, where only one critter may occupy one cell at a time. The grid is enclosed, so a critter is not allowed to move off the edge of the world. Time is simulated in time steps. Each critter performs some action every time step. 

The ant and doodlebug move randomly in every step, and if the critter moves to an occupied grid or the wall, the critter will stay. If the ant survives three time steps, then at the end of the time step, the ant will breed at a random adjacent location anywhere around the particular ant, if there is no cell available, then no breeding occurs. The offspring cooldown for an ant is a 3-time step. 

The doodlebug is also able to move the same way as the ant does, and is able to reproduce after 8 steps. What differentiates the doodlebug and the ant is that the doodlebug is able to starve if they have not eaten any ants within three steps. 

This project highlights the use of Object-Oriented Programming, inheritance, and polymorphism.  
