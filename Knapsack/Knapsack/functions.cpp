#include "functions.h"

#include <random>

//Reads a .txt file with available items listed
std::vector<Item> readInput(std::string file_input_name) {
    std::string line;
    std::vector<Item> resultVec;

    std::ifstream in(file_input_name);
    if (in.is_open()) {
        unsigned int temp_value, temp_weight;
        while (in >> temp_value >> temp_weight)
        {
            resultVec.push_back(Item{ temp_value, temp_weight });
        }
    }
    in.close();

    return resultVec;
}

//Calculates the value for a chromosome with specific genes in a knapsack
//  Chromosome: a vector of ints representing genes (items) that are either included (1) or not (0)
int calculateFitness(std::vector<int> &chromosome, std::vector<Item> &items, const unsigned int knapsack_capacity) {

    int total_value = 0;
    int total_weight = 0;

    for (size_t i = 0; i < chromosome.size(); ++i) {

        if (chromosome[i] == 1) { 

            total_value += items[i].value;
            total_weight += items[i].weight;
        }
    }

    //if the total weight exceeds the capacity, then the result is not valid, hence 0
    //...or you can imagine the knapsack is ripped under the mass that is too heavy
    //on your way to home, and you find out that you got nothing by the end, you greedy dummy
    if (total_weight > knapsack_capacity)
        return 0;  

    return total_value;
}

//Initializing a population randomly setting genes in chromosomes as 0 or 1
std::vector<std::vector<int>> initialize_population(const unsigned int population_size, std::vector<Item> &items) {

    //Pseudo random generator
    std::mt19937 gen(static_cast<long unsigned int>(std::time(0)));
    std::uniform_int_distribution<> dis(0, 1);

    std::vector<std::vector<int>> population( population_size, std::vector<int>(items.size()) );

    for (auto &chromosome : population) {
        for (auto &gene : chromosome) {
            gene = dis(gen);
        }
    }
    return population;
}

//Running a tournament, choosing the best chromosome
//  - Selecting a random chromosome from the selected population
//  - Calculating its fitness
//  - Iterating through other chromosomes, comparing the fitness scores of the first and the candidate
std::vector<int> tournament_selection(const std::vector<std::vector<int>>& population, const unsigned int tournament_size, std::vector<Item>& items, const unsigned int knapsack_capacity) {
    std::mt19937 gen(static_cast<long unsigned int>(std::time(0)));
    std::uniform_int_distribution<> dis(0, population.size() - 1);
    std::vector<int> best_chromosome = population[dis(gen)];
    int best_fitness = calculateFitness(best_chromosome, items, knapsack_capacity);
    for (int i = 1; i < tournament_size; ++i) {
        std::vector<int> candidate = population[dis(gen)];
        int candidate_fitness = calculateFitness(candidate, items, knapsack_capacity);
        if (candidate_fitness > best_fitness) {
            best_chromosome = candidate;
            best_fitness = candidate_fitness;
        }
    }
    return best_chromosome;
}

std::pair<std::vector<int>, std::vector<int>> crossover(const std::vector<int>& parent1, const std::vector<int>& parent2) {

    std::mt19937 gen(static_cast<long unsigned int>(std::time(0)));
    std::uniform_int_distribution<> dis(0, parent1.size() - 1);
    int crossover_point = dis(gen);

    std::vector<int> child1 = parent1;
    std::vector<int> child2 = parent2;

    for (int i = crossover_point; i < parent1.size(); ++i) {
        std::swap(child1[i], child2[i]);
    }
    return { child1, child2 };
}

void mutate(std::vector<int> &chromosome, const double mutation_rate) {
    std::mt19937 gen(static_cast<long unsigned int>(std::time(0)));
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (auto &gene : chromosome) {
        if (dis(gen) < mutation_rate) {
            gene = 1 - gene;
        }
    }
}

std::vector<int> findBest(const std::vector<std::vector<int>> &population, std::vector<Item> &items, const unsigned int knapsack_capacity) {
    int best_fitness = 0;
    std::vector<int> best_chromosome;
    for (const std::vector<int> & chromosome : population) {
        int fitness = calculateFitness(const_cast<std::vector<int>&>(chromosome), items, knapsack_capacity);
        if (fitness > best_fitness) {
            best_fitness = fitness;
            best_chromosome = chromosome;
        }
    }
    return best_chromosome;
}
