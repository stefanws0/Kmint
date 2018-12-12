#include "kmint/pigisland/pig.hpp"
#include "kmint/math/angle.hpp"
#include "kmint/pigisland/resources.hpp"
#include "kmint/random.hpp"
#include <cmath>

namespace kmint {
	namespace pigisland {

		namespace {

			math::vector2d random_vector() {
				auto x = random_scalar(100, 924);
				auto y = random_scalar(50, 728);
				return { x, y };
			}
		} // namespace

		pig::pig(math::vector2d location, const int index)
			: free_roaming_actor{ random_vector() }, drawable_{ *this, pig_image() }
		{
			index_ = index;
			shark_attraction_ = float(random_int(-10, 10)) / 10;
			boat_attraction_ = float(random_int(-10, 10)) / 10;
			cohesion_ = float(random_int(0, 10)) / 10;
			separation_ = float(random_int(0, 10)) / 10;
			alignment_ = float(random_int(0, 10)) / 10;
			boarded_ = false;
			behaviors_ = std::make_unique<steering_behaviors>(this);
		}

		/**
	 * \brief
	 * \param location
	 * \param index
	 * \param shark_attraction
	 * \param boat_attraction
	 * \param cohesion
	 * \param separation
	 * \param alignment
	 */
		pig::pig(math::vector2d location, const int index, const float shark_attraction, const float boat_attraction, const float cohesion, const float separation,
			const float alignment)
			: free_roaming_actor{ random_vector() },
			index_{ index },
			boarded_{ false },
			shark_attraction_{ shark_attraction },
			boat_attraction_{ boat_attraction },
			cohesion_{ cohesion },
			separation_{ separation },
			alignment_{ alignment },
			drawable_{ *this, pig_image() }
		{
		}

		bool pig::operator!=(const pig& rhs) const
		{
			return (index_ != rhs.index_);
		}

		void pig::set_shark(actor& shark)
		{
			shark_ = &shark;
		}

		void pig::set_boat(actor& boat)
		{
			boat_ = &boat;
		}

		float pig::max_speed() const
		{
			return max_speed_;
		}

		math::vector2d pig::velocity() const
		{
			return velocity_;
		}

		bool pig::tag() const
		{
			return tag_;
		}

		void pig::act(delta_time dt) {
			auto boat_location = boat_->location();
			auto shark_location = shark_->location();
			auto pig_location = location();

			if (pig_location.x() == boat_location.x() && boat_location.y() == pig_location.y())
			{
				boarded_ = true;
			}
			if (boarded_)
			{
				pig_location.y(boat_location.y());
				pig_location.x(boat_location.x());
			}
			location(pig_location);

			free_roaming_actor::act(dt);
		}

	} // namespace pigisland

} // namespace kmint
