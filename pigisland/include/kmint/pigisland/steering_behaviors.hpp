#ifndef KMINT_PIGISLAND_STEERING_BEHAVIORS_HPP
#define KMINT_PIGISLAND_STEERING_BEHAVIORS_HPP
#include "kmint/play.hpp"


namespace kmint {
	namespace pigisland {

		class pig;
		class wall;
		class matrix;

		class steering_behaviors {
		public:
			steering_behaviors(pig* owner, std::vector<wall> walls);
			math::vector2d seek(math::vector2d target_position) const;
			math::vector2d arrive(math::vector2d target_position, int deceleration) const;
			math::vector2d separation(const std::vector<pig*>& neighbors) const;
			math::vector2d alignment(const std::vector<pig*>& neighbors);
			math::vector2d cohesion(const std::vector<pig*>& neighbors);
			math::vector2d wall_avoidance(std::vector<wall>& walls);
			math::vector2d obstacle_avoidance(const std::vector<pig*>& obstacles);
			void create_feelers();
			math::vector2d calculate(math::vector2d boat_location, float boat_attraction, math::vector2d shark_location,
			                         float shark_attraction);

		private:
			pig* owner_;
			std::vector<wall> walls_;
			std::vector<math::vector2d> feelers_;
		};
	} // namespace pigisland
} // namespace kmint

#endif /* KMINT_PIGISLAND_STEERING_BEHAVIORS_HPP */
