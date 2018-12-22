#pragma once

#include "kmint/pigisland/shark.hpp"

namespace kmint {
	namespace pigisland {

		class shark_state
		{
			public:
				shark_state(shark* shark);
				virtual ~shark_state();

				virtual void perform() = 0;

			protected:
				shark* shark_;
		};

	} // namespace pigisland
} // namespace kmint