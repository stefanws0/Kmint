#pragma once

#include "shark_state.hpp"

namespace kmint {
	namespace pigisland {

		class shark;

		class shark_state_scared : public shark_state
		{
			public:
				shark_state_scared(shark* s);
				virtual ~shark_state_scared();

				void perform();
		};

	} // namespace pigisland
} // namespace kmint