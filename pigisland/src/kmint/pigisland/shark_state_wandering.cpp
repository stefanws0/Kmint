#include "kmint/pigisland/shark_state_scared.hpp"
#include "kmint/pigisland/shark_state_wandering.hpp"
#include "kmint/pigisland/shark_state_attacking.hpp"
#include "kmint/pigisland/shark_state_tired.hpp"
#include "kmint/pigisland/shark_state.hpp"
#include "kmint/pigisland/shark.hpp"

namespace kmint {
	namespace pigisland {

		shark_state_wandering::shark_state_wandering(shark* s) : shark_state(s)
		{
			shark_->clear_route();
		}

		shark_state_wandering::~shark_state_wandering()
		{
		}

		void shark_state_wandering::perform()
		{
			if (shark_->is_tired())
			{
				shark_->change_state(std::make_unique<shark_state_tired>(shark_));
				return;
			}
				
			if (shark_->is_scared())
			{
				shark_->change_state(std::make_unique<shark_state_scared>(shark_));
				return;
			}
				
			if (shark_->close_to_pig())
			{
				shark_->change_state(std::make_unique<shark_state_attacking>(shark_));
				return;
			}		

			shark_->move_random_node();
		}
	} // namespace pigisland
} // namespace kmint