#include <iostream>
#include <vector>
#include <string>

#include "functions.h"

int main() {

    const unsigned int MAX_GENERATIONS = 5000;
    const unsigned int POPULATION_SIZE = 200;
	const unsigned int TOURNAMENT_SIZE = 5;	
    const double MUTATION_RATE = 0.01;
	
	//Both input and the capacity are taken from here (PO7): https://people.sc.fsu.edu/~jburkardt/datasets/knapsack_01/knapsack_01.html
	const std::string FILE_INPUT_NAME = "input.txt";
	const unsigned int CAPACITY = 170;

	std::vector<Item> items = readInput(FILE_INPUT_NAME);

	//This loop tests if items are in the vector
	//for (Item i : items) {
	//	std::cout << i.value << " " << i.weight <<std::endl;
	//}

    std::vector<std::vector<int>> population = initialize_population(POPULATION_SIZE, items);
    for (int generation = 0; generation < MAX_GENERATIONS; ++generation) {
        std::vector<std::vector<int>> new_population;
        while (new_population.size() < POPULATION_SIZE) {
            auto parent1 = tournament_selection(population, POPULATION_SIZE, TOURNAMENT_SIZE, items, CAPACITY);
            auto parent2 = tournament_selection(population, POPULATION_SIZE, TOURNAMENT_SIZE, items, CAPACITY);
            std::pair<std::vector<int>, std::vector<int>> children;
            children = crossover(parent1, parent2);
            mutate(children.first, MUTATION_RATE);
            mutate(children.second, MUTATION_RATE);
            new_population.push_back(children.first);
            new_population.push_back(children.second);
        }
        population = new_population;
    }
    std::vector<int> best_solution = findBest(population, items, CAPACITY);
    int best_value = calculateFitness(best_solution, items, CAPACITY);

    for (int i : best_solution)
        std::cout << i << " ";
    std::cout << "| " << best_value << std::endl;

	return 0;
}
