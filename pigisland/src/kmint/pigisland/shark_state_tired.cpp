#include "kmint/pigisland/shark_state_tired.hpp"
#include "kmint/pigisland/shark_state_attacking.hpp"
#include "kmint/pigisland/shark_state_wandering.hpp"
#include "kmint/pigisland/shark_state.hpp"
#include "kmint/pigisland/shark.hpp"

namespace kmint {
	namespace pigisland {

		shark_state_tired::shark_state_tired(shark* s) : shark_state(s)
		{
			shark_->clear_route();
		}

		shark_state_tired::~shark_state_tired()
		{
		}

		void shark_state_tired::perform()
		{
			shark_->move_resting_place();

			if (shark_->is_at_resting_place())
			{
				shark_->change_state(std::make_unique<shark_state_wandering>(shark_));
				return;
			}
				
		}
	} // namespace pigisland
} // namespace kmint