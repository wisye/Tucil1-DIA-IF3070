#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Genetic_Algorithm/ga.h"
#include "Hill-climbing_with_Sideways_Move/hcsm.h"
#include "Random_Restart_Hill-climbing/rrhc.h"
#include "Simulated_Annealing/sa.h"
#include "Steepest_Ascent_Hill-climbing/sahc.h"
#include "Stochastic_Hill-climbing/shc.h"
#include "cube.h"
#include "visualize_cube.h"

int main() {
	Cube *cube = (Cube *)malloc(sizeof(Cube));
	if (cube == NULL) return 1;

	init_cube(cube);

	char choice;
	puts("1. Steepest ascent hill climbing, its variant, and simulated annealing\n2. Genetic algorithm");
	scanf("%c", &choice);

	if(choice == '1') visualize_cube(cube);
	else ga(cube);

	free(cube);
	return 0;
}
