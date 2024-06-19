#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "structures.h"

std::vector<Item> readInput(std::string file_input_name);

int calculateFitness(std::vector<int>& chromosome, std::vector<Item>& items, const unsigned int knapsack_capacity);

std::vector<std::vector<int>> initialize_population(const unsigned int population_size, std::vector<Item> &items);

std::vector<int> tournament_selection
(
    const std::vector<std::vector<int>>& population,
    const unsigned int tournament_size,
    std::vector<Item>& items,
    const unsigned int knapsack_capacity
);

std::pair<std::vector<int>, std::vector<int>> crossover(const std::vector<int> &parent1, const std::vector<int> &parent2);

void mutate(std::vector<int> &chromosome, const double mutation_rate);

std::vector<int> findBest(const std::vector<std::vector<int>> &population, std::vector<Item>& items, const unsigned int knapsack_capacity);