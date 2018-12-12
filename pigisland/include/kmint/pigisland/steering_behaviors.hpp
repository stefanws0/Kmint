#ifndef KMINT_PIGISLAND_STEERING_BEHAVIORS_HPP
#define KMINT_PIGISLAND_STEERING_BEHAVIORS_HPP
#include "kmint/play.hpp"
#include "kmint/pigisland/pig.hpp"

namespace kmint {
	namespace pigisland {
		class steering_behaviors {
		public:
			explicit steering_behaviors(kmint::pigisland::pig* owner);
			math::vector2d seek(math::vector2d target_position) const;
			math::vector2d arrive(math::vector2d target_position, int deceleration) const;
			math::vector2d separation(const std::vector<pig*>& neighbors) const;
			math::vector2d alignment(const std::vector<pig*>& neighbors);
			math::vector2d cohesion(const std::vector<pig*>& neighbors);

		private:
			pig* owner_;
		};
	} // namespace pigisland
} // namespace kmint

#endif /* KMINT_PIGISLAND_STEERING_BEHAVIORS_HPP */
