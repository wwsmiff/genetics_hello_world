#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <array>
#include <climits>

#include "utils.h"

const std::string TARGET = "Hello, World!";
const uint32_t POPULATION_SIZE = 2048;
const uint32_t ITERATIONS = 163844;
const float ELITE_RATE = 0.10f;
const uint32_t STARTING_CHAR = 32;
const uint32_t ENDING_CHAR = 122;

struct outcome
{
	outcome(const outcome &other) 
		:str(other.str),
		rank(other.rank)
	{}

	outcome(const std::string &_str, uint32_t _rank)
		:str(_str),
		rank(_rank)
	{}
	
	outcome() {}
	
	std::string str = "";
	uint32_t rank = 0;
};

std::vector<outcome> population_vec;
std::vector<outcome> buffer;

void init_population(std::vector<outcome> &population, std::vector<outcome> &buffer)
{
	for(size_t i = 0; i < POPULATION_SIZE; ++i)
	{
		std::string tmp = "";

		for(size_t i = 0; i < TARGET.size(); i++)
			tmp.push_back(static_cast<char>(utils::generate_random(STARTING_CHAR, ENDING_CHAR)));

		population.emplace_back(tmp, 0);
	}
	
	buffer.resize(POPULATION_SIZE);
}

void calculate_rank(std::vector<outcome> &population)
{
	
	for(size_t i = 0; i < POPULATION_SIZE; ++i)
	{
		uint32_t rank = 0;

		for(size_t j = 0; j < TARGET.size(); ++j)
		{
			rank += abs(int32_t(population[i].str[j] - TARGET[j]));
		}
		
		population[i].rank = rank;
	}
}

void sort_by_rank(std::vector<outcome> &population)
{
	std::sort(population.begin(), population.end(), [] (const outcome &a, const outcome &b) {return a.rank < b.rank;});
}

void elites(std::vector<outcome> &population, std::vector<outcome> &buffer, int32_t elite_size)
{
	for(int32_t i = 0; i < elite_size; ++i)
	{
		buffer[i].str = population[i].str;
		buffer[i].rank = population[i].rank;
	}
}

void mutate(outcome &member)
{
	size_t target_size = TARGET.size();
	int32_t random_pos = utils::generate_random(0, target_size);
	char delta = utils::generate_random(STARTING_CHAR, ENDING_CHAR);
	member.str[random_pos] = delta;
}

void mate(std::vector<outcome> &population, std::vector<outcome> &buffer)
{
	int32_t esize = POPULATION_SIZE * ELITE_RATE;
	size_t target_size = TARGET.size();
	int32_t random_pos = 0;
	int32_t i1 = 0, i2 = 0;

	elites(population, buffer, esize);

	for(size_t i = esize; i < POPULATION_SIZE; ++i)
	{
		i1 = utils::generate_random(0, POPULATION_SIZE / 2);
		i2 = utils::generate_random(0, POPULATION_SIZE / 2);
		random_pos = utils::generate_random(0, target_size);

		buffer[i].str = population[i1].str.substr(0, random_pos) +
						population[i2].str.substr(random_pos, esize - random_pos);

		if(utils::generate_random(0, INT_MAX) < INT_MAX * 0.1) mutate(buffer[i]);
	}
}

inline void swap(std::vector<outcome> &population, std::vector<outcome> &buffer)
{ 
	std::vector<outcome> temp = population; 
	population = buffer; 
	buffer = temp; 
}

int main(void)
{
	init_population(population_vec, buffer);

	for(uint32_t i = 0; i < ITERATIONS; ++i)
	{
		std::cerr << "Generation: " << i << '\r';
		calculate_rank(population_vec);
		sort_by_rank(population_vec);
		
		std::cout << population_vec[0].str << ": " << population_vec[0].rank << " | " << "Generation: " << i << std::endl;

		if(population_vec[0].rank == 0) break;

		mate(population_vec, buffer);
		swap(population_vec, buffer);
	}

	std::cerr << "\nDone!" << std::endl;

	return EXIT_SUCCESS;
}
