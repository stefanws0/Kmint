#ifndef KMINT_PIGISLAND_SHARK_HPP
#define KMINT_PIGISLAND_SHARK_HPP

#include "kmint/map/map.hpp"
#include "kmint/play.hpp"
#include "kmint/primitives.hpp"

#include <vector>
#include <memory>

namespace kmint {
	namespace pigisland {

		class shark_state_wandering;
		class shark_state;

		class shark : public play::map_bound_actor {
			public:
				shark(kmint::map::map_graph &g);
				ui::drawable const &drawable() const override { return drawable_; }
				bool incorporeal() const override { return false; }
				scalar radius() const override { return 16.0; }
				void act(delta_time dt) override;
				bool is_tired();
				bool is_scared();
				bool has_route();
				bool is_at_resting_place();
				void add_pig(actor& p);
				void add_boat(actor& b);
				void move_random_node();
				void move_resting_place();
				bool wander();
				bool close_to_pig();
				void change_state(std::unique_ptr<shark_state> state);
				void move_to_target();
				void clear_route();
				void move();

			private:
				map::map_node* find_target_node();

			private:
				play::image_drawable drawable_;
				map::map_graph *map_;
				map::map_node const *resting_place_;
				std::vector<actor*> pigs_;
				actor* boat_;
				actor* target_;
				delta_time t_since_move_{};
				int totalSteps_;
				int wanderedSteps_;
				std::unique_ptr<std::vector<const map::map_node*>> route_;
				std::unique_ptr<shark_state> state_;
		};

	} // namespace pigisland
} // namespace kmint

#endif /* KMINT_PIGISLAND_SHARK_HPP */
