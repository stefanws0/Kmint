#include "kmint/pigisland/shark_state_scared.hpp"
#include "kmint/pigisland/shark_state_tired.hpp"
#include "kmint/pigisland/shark_state_attacking.hpp"
#include "kmint/pigisland/shark_state_wandering.hpp"
#include "kmint/pigisland/shark_state.hpp"
#include "kmint/pigisland/shark.hpp"

namespace kmint {
	namespace pigisland {

		shark_state_scared::shark_state_scared(shark* s) : shark_state(s)
		{
			shark_->clear_route();
		}

		shark_state_scared::~shark_state_scared()
		{
		}

		void shark_state_scared::perform()
		{
			if (shark_->is_tired())
			{
				shark_->change_state(std::make_unique<shark_state_tired>(shark_));
				return;
			}

			if (shark_->wander())
			{
				shark_->change_state(std::make_unique<shark_state_wandering>(shark_));
				return;
			}
		}
	} // namespace pigisland
} // namespace kmint