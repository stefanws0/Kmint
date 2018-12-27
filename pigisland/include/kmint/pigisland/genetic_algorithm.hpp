#ifndef KMINT_PIGISLAND_GENETIC_ALGORITHM_HPP
#define KMINT_PIGISLAND_GENETIC_ALGORITHM_HPP
#include <vector>
#include "pig.hpp"

namespace kmint {
	namespace pigisland {

		class genetic_algorithm
		{
		public:
			std::vector<pig> genetic_algo(std::vector<pig*> current_population);
		private:
			pig* selection(std::vector<pig*> current_population, const int total_fitness);
			void crossover(pig& father, pig& mother);

		};
	} // namespace pigisland
} // namespace kmint

#endif /* KMINT_PIGISLAND_GENETIC_ALGORITHM_HPP */