#include "sa.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

double schedule1(int iterations, double temperature_0, double cooling_rate) {

	double new_t = temperature_0 * pow(cooling_rate, iterations);

	if (temperature_0 > 0.005f && temperature_0 < 0.05f) {
		return temperature_0 * pow(cooling_rate * 150, iterations);
	}
	else if (temperature_0 > 0.0005f && temperature_0 < 0.005f) {
		return temperature_0 * pow(cooling_rate * 1000, iterations);
	}

	return new_t;
}

void sa(Cube *cube) {

	FILE *file = fopen("src/results/sa_result.csv", "w");

	if (file == NULL) {
		perror("Unable to open file");
		return;
	}

	fprintf(file, "Iteration,Heuristic Value,Temperature,Boltzmann Factor,Stuck,Time\n");
	int h_current, h_new;
	uint8_t u1, u2;
	int iterations = 1;
	int stuck = 0;
	double boltzman;

	int MAX_ITERATIONS = 100000000;

	double initial_temperature = 10.0;
	double temperature;
	double cooling_rate = 0.999999;

	uint8_t *linear_cube[TOTAL_VALUES];
	int index = 0;
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			for (int k = 0; k < SIZE; k++) {
				linear_cube[index++] = &cube->blocks[i][j][k].value;
			}
		}
	}

	h_current = calculate_heuristics(cube);

	clock_t start_time = clock();
	clock_t end_time;
	double elapsed_time;
	h_current = calculate_heuristics(cube);

	for (iterations = 1; iterations < MAX_ITERATIONS; iterations++) {

		temperature = schedule1(iterations, initial_temperature, cooling_rate);

		if (temperature < 1e-6) {
			printf("Near 0 (harusnya 0 sih cman sigh)\n");
			break;
		}
		// if (temperature == 0) {
		// 	printf("Harusya make ini supaya ngikutin kalo 0 stop. masalahny ini bakal lama banget wkkw\n");
		// 	break;
		// }

		u1 = rand() % TOTAL_VALUES;
		u2 = rand() % TOTAL_VALUES;

		while (u1 == u2) {
			u2 = rand() % TOTAL_VALUES;
		}

		swap(linear_cube[u1], linear_cube[u2]);

		h_new = calculate_heuristics(cube);

		int delta_h = h_new - h_current;
		boltzman = exp(delta_h / temperature);

		if (delta_h > 0) {

			h_current = h_new;
		}
		else {
			if (delta_h == 0) {
				stuck++;
			}
			double probability = exp(delta_h / temperature);

			//? bawah kalo misal si prob dingin gajelasny dipilih
			if (rand() / (double)RAND_MAX < probability) {

				h_current = h_new;
			}

			else {
				stuck++;
				swap(linear_cube[u1], linear_cube[u2]); //? ini kalo ga jdi ya ke default
			}
		}

		printf("Iteration %d: Heuristic value: %d, Temperature: %f\n", iterations, h_current, temperature);

		if (h_current == TOTAL_EDGES) {
			printf("Found a magic cube!\n");
			break;
		}
		end_time = clock();
		elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000;
		fprintf(file, "%d,%d,%f,%f,%d,%.4f\n", iterations, h_current, temperature, boltzman, stuck,
		        elapsed_time);
	}
	fclose(file);
}
