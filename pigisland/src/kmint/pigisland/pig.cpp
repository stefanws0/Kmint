#include "kmint/pigisland/pig.hpp"
#include "kmint/math/angle.hpp"
#include "kmint/pigisland/resources.hpp"
#include "kmint/pigisland/wall.hpp"
#include "kmint/pigisland/steering_behaviors.hpp"
#include "kmint/pigisland/boat.hpp"
#include "kmint/pigisland/shark.hpp"
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

		pig::pig(math::vector2d location, const int index, const std::vector<pigisland::wall>& walls)
			: free_roaming_actor{ random_vector() }, walls_{walls}, drawable_{ *this, pig_image() }
		{
			index_ = index;
			shark_attraction_ = float(random_int(-10, 10)) / 10;
			boat_attraction_ = float(random_int(-10, 10)) / 10;
			cohesion_ = float(random_int(0, 10)) / 10;
			separation_ = float(random_int(0, 10)) / 10;
			alignment_ = float(random_int(0, 10)) / 10;
			boarded_ = false;
			behaviors_ = std::make_unique<steering_behaviors>(this, walls);
			max_speed_ = 10;
			mass_ = 10;
			velocity_ = { 1, 1 };
			heading_ = math::normalize(velocity_);
			side_ = math::perp(heading_);
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

		bool pig::operator!=(const pig& rhs) const { return (index_ != rhs.index_); }
		void pig::set_location(math::vector2d p_location) { location(p_location); }
		void pig::set_shark(shark& shark) { shark_ = &shark; }
		void pig::set_boat(boat& boat) { boat_ = &boat; }
		void pig::set_pigs(std::vector<pig*> pigs) { pigs_ = pigs; }
		std::vector<pig*> pig::pigs() const { return pigs_; }
		float pig::max_speed() const { return max_speed_; }
		math::vector2d pig::velocity() const { return velocity_; }
		bool pig::tag() const { return tag_; }
		void pig::tag(bool value) { tag_ = value; }

		void pig::velocity(const math::vector2d velocity)
		{
			auto l = sqrt((velocity.x() * velocity.x()) + (velocity.y() * velocity.y()));
			if (l <= max_speed())
			{
				velocity_ = velocity;
			}
			else
			{
				velocity_ = velocity / l * max_speed();
			}

		}

		int pig::calculate_fitness() const
		{
			auto fitness = 0;
				fitness = fitness + boat_attraction_ * 20;
				fitness = fitness + shark_attraction_ * -20;
			if(boarded_)
			{
				fitness = fitness + 50;
			}
			fitness = fitness + cohesion_ * 10;
			fitness = fitness + alignment_ * 10;
			fitness = fitness + separation_ * -10;
			return fitness;
		}

		void pig::tag_neighbors()
		{
			const auto radius = 32.0;
			//iterate through all entities checking for range
			for (auto current_pig : pigs_)
			{
				//first clear any current tag
				current_pig->tag(false);
				const auto to = current_pig->location() - this->location();
				//the bounding radius of the other is taken into account by adding it
				//to the range
				const auto range = radius + radius;
				//if entity within range, tag for further consideration. (working in
				//distance-squared space to avoid sqrts)
				if ((current_pig != this) && (math::norm2(to) < range * range))
				{
					current_pig->tag(true);
				}
			}//next entity
		}

		void pig::act(delta_time dt) {
			tag_neighbors();
			const auto steering_force = behaviors_->calculate(boat_->location(), boat_attraction_, shark_->location(), shark_attraction_, cohesion_, alignment_, separation_);

			const auto acceleration = steering_force / mass_;

			// times delta time
			velocity(velocity_ += acceleration * dt.count() / 1000000000);

			// times delta time
			location(location() += velocity() * dt.count() / 1000000000);

			if (math::norm2(velocity_) > 0.00000001)
			{
				heading_ = math::normalize(velocity_);
				side_ = math::perp(heading_);
			}
			//check boundaries

			free_roaming_actor::act(dt);
		}

	} // namespace pigisland

} // namespace kmint
