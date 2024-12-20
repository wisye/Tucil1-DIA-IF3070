#include "shc.h"
#include <stdbool.h>
#include <time.h>

void shc(Cube *cube) {

	FILE *file = fopen("src/results/shc_result.csv", "w");

	if (file == NULL) {
		perror("Unable to open file");
		return;
	}

	fprintf(file, "Iteration,Heuristic Value,Time\n");

	int h_current, h_new;
	uint8_t u1, u2;
	int iterations = 0;
	int MAX_ITERATIONS = 10000;

	uint8_t *linear_cube[TOTAL_VALUES];
	int index = 0;
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			for (int k = 0; k < SIZE; k++) {
				linear_cube[index++] = &cube->blocks[i][j][k].value;
			}
		}
	}

	clock_t start_time = clock();
	clock_t end_time;
	double elapsed_time;
	h_current = calculate_heuristics(cube);

	for (int i = 1; i <= MAX_ITERATIONS; i++) {

		u1 = rand() % TOTAL_VALUES;
		u2 = rand() % TOTAL_VALUES;

		while (u1 == u2) {
			u2 = rand() % TOTAL_VALUES;
		}
		swap(linear_cube[u1], linear_cube[u2]);

		h_new = calculate_heuristics(cube);

		if (h_new > h_current) {
			h_current = h_new;
		}
		else {
			swap(linear_cube[u1], linear_cube[u2]);
		}

		iterations++;
		unflatten_cube2(linear_cube);
		printf("Iteration %d: Improved heuristic to %d\n", iterations, h_current);

		if (h_current == TOTAL_EDGES) {
			printf("Found a magic cube!\n");
			break;
		}

		end_time = clock();
		elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000;
		fprintf(file, "%d,%d,%.4f\n", iterations, h_current, elapsed_time);
	}
	fclose(file);
}