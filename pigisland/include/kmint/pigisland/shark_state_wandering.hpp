#pragma once

#include "shark_state.hpp"

namespace kmint {
	namespace pigisland {

		class shark;

		class shark_state_wandering : public shark_state
		{
			public:
				shark_state_wandering(shark* s);
				virtual ~shark_state_wandering();

				void perform();
		};

	} // namespace pigisland
} // namespace kmint