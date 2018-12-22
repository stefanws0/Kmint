#pragma once

#include "shark_state.hpp"

namespace kmint {
	namespace pigisland {

		class shark;

		class shark_state_tired : public shark_state
		{
			public:
				shark_state_tired(shark* s);
				virtual ~shark_state_tired();

				void perform();
		};

	} // namespace pigisland
} // namespace kmint