#ifndef KMINT_PIGISLAND_PIG_HPP
#define KMINT_PIGISLAND_PIG_HPP
#include "kmint/play.hpp"
#include <memory>


namespace kmint {
	namespace pigisland {

		class wall;
		class shark;
		class boat;
		class steering_behaviors;

		class pig : public play::free_roaming_actor {
		public:
			explicit pig(math::vector2d location, const int index, const std::vector<wall>& walls);
			pig(math::vector2d location,
				int index,
				float shark_attraction,
				float boat_attraction,
				float cohesion,
			    float separation,
			    float alignment);
			bool operator!=(const pig& rhs) const;
			void set_shark(shark& shark);
			const ui::drawable &drawable() const override { return drawable_; }
			void move(math::vector2d delta) { location(location() + delta); }
			void set_boat(boat& boat);
			float max_speed() const;
			math::vector2d velocity() const;
			void velocity(math::vector2d velocity);
			bool tag() const;
			void act(delta_time dt) override;
			bool perceptive() const override { return true; }
			scalar range_of_perception() const override { return 30.0f; }

		private:
			int index_;
			actor* shark_;
			actor* boat_;
			std::unique_ptr<steering_behaviors> behaviors_;
			bool tag_;
			bool boarded_;
			std::vector<wall> walls_;
			math::vector2d velocity_;
			math::vector2d heading_;
			math::vector2d side_;
			float mass_;
			float max_speed_;
			float max_force_;
			float max_turn_rate;
			float shark_attraction_;
			float boat_attraction_;
			float cohesion_;
			float separation_;
			float alignment_;
			play::image_drawable drawable_;
		};

	} // namespace pigisland
} // namespace kmint

#endif /* KMINT_PIGISLAND_PIG_HPP */
