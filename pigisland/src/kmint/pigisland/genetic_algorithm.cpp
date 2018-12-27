#include "kmint/pigisland/genetic_algorithm.hpp"
#include "kmint/random.hpp"
#include "kmint/pigisland/pig.hpp"

namespace kmint {
	namespace pigisland {

		pig* genetic_algorithm::selection(std::vector<pig*> current_population, const int total_fitness)
		{
			std::sort(current_population.begin(), current_population.end(), [](pig const &a, pig const &b) {return a.calculate_fitness() < b.calculate_fitness(); });
			const auto random_number = random_int(0, total_fitness);
			auto offset = 0.0;
			auto pick = 0;

			for(auto i = 0; i < int(current_population.size()); i++)
			{
				offset += current_population.at(i)->calculate_fitness();
				if (random_number < offset)
				{
					pick = i;
					break;
				}
			}
			return current_population.at(pick);
		};

		void genetic_algorithm::crossover(pig & father, pig & mother)
		{
			auto& child_1 = std::unique_ptr<pig*>();
			auto& child_2 = std::unique_ptr<pig*>();
		}

	} // namespace pigisland
} // namespace kmint