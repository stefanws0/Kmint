#pragma once

#include "shark_state.hpp"

namespace kmint {
	namespace pigisland {

		class shark;

		class shark_state_attacking : public shark_state
		{
			public:
				shark_state_attacking(shark* s);
				virtual ~shark_state_attacking();

				void perform();
		};

	} // namespace pigisland
} // namespace kmint