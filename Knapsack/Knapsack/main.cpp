#include <iostream>
#include <vector>
#include <string>
#include <ctime>

#include "functions.h"

int main() {

    const unsigned int MAX_GENERATIONS = 7000;
    const unsigned int POPULATION_SIZE = 100;
	const unsigned int TOURNAMENT_SIZE = 10;	
    const double MUTATION_RATE = 0.9;
	
	//Both input and the capacity are taken from here (PO7): https://people.sc.fsu.edu/~jburkardt/datasets/knapsack_01/knapsack_01.html
	const std::string FILE_INPUT_NAME = "input.txt";
	const unsigned int CAPACITY = 750;

	std::vector<Item> items = readInput(FILE_INPUT_NAME);

	//This loop tests if items are in the vector
	//for (Item i : items) {
	//	std::cout << i.value << " " << i.weight <<std::endl;
	//}

    std::vector<std::vector<int>> population = initialize_population(POPULATION_SIZE, items);
    int best_value = 0;
    std::vector<int> best_solution;

    std::ofstream outfile("results.txt");
    if (!outfile.is_open()) {
        std::cerr << "Error: Unable to open the file for writing." << std::endl;
        return -1;
    }

    outfile << "Generation Best_Value" << std::endl;

    for (int generation = 0; generation < MAX_GENERATIONS; ++generation) {
        std::vector<std::vector<int>> new_population;

        //best_solution = findBest(population, items, CAPACITY);
        //new_population.push_back(best_solution);

        while (new_population.size() < POPULATION_SIZE) {
            std::vector<int> parent1 = tournament_selection(population, TOURNAMENT_SIZE, items, CAPACITY);
            std::vector<int> parent2 = tournament_selection(population, TOURNAMENT_SIZE, items, CAPACITY);
            std::pair<std::vector<int>, std::vector<int>> children = crossover(parent1, parent2);
            mutate(children.first, MUTATION_RATE);
            mutate(children.second, MUTATION_RATE);
            new_population.push_back(children.first);
            new_population.push_back(children.second);
        }

        population = new_population;

        std::vector<int> current_best_solution = findBest(population, items, CAPACITY);
        int current_best_value = calculateFitness(current_best_solution, items, CAPACITY);

        if (current_best_value > best_value) {
            best_value = current_best_value;
            best_solution = current_best_solution;
        }

        std::cout << "Generation " << generation << ": Best Value = " << best_value << std::endl;
        outfile << generation + 1 << " " << best_value << std::endl;
    }

    //std::vector<int> best_solution = findBest(population, items, CAPACITY);
    //int best_value = calculateFitness(best_solution, items, CAPACITY);

    for (int i : best_solution)
        std::cout << i << " ";
    std::cout << "| " << best_value << std::endl;

    outfile.close();

	return 0;
}
