#include "kmint/pigisland/shark.hpp"
#include "kmint/pigisland/node_algorithm.hpp"
#include "kmint/pigisland/resources.hpp"
#include "kmint/pigisland/pig.hpp"
#include "kmint/pigisland/shark_state.hpp"
#include "kmint/pigisland/shark_state_wandering.hpp"
#include "kmint/random.hpp"

#include <iostream>
#include <memory>
#include <cmath>

namespace kmint {
	namespace pigisland {

		shark::shark(kmint::map::map_graph &g) :
			play::map_bound_actor{ g, find_shark_resting_place(g) }, drawable_{ *this, shark_image() }, map_{ &g }, 
			resting_place_(&node()), totalSteps_{ 0 }, route_{ std::make_unique<std::vector<const map::map_node*>>() }, state_{ std::make_unique<shark_state_wandering>(this) }
		{
		}

		bool shark::is_tired()
		{
			if (totalSteps_ >= 100)
				return true;
			else
				return false;
		}

		bool shark::is_scared()
		{
			if (pow(boat_->location().x() - location().x(), 2) + pow(boat_->location().y() - location().y(), 2) < pow(50, 2)) // (x - center_x)^2 + (y - center_y)^2 < radius^2
				return true;
			else 
				return false;
		}

		bool shark::has_route()
		{
			if (route_->empty())
				return true;
			else
				return false;
		}

		bool shark::is_at_resting_place()
		{
			if (&node() == resting_place_)
				return true;
			else
				return false;
		}

		void shark::add_pig(actor& p)
		{
			pigs_.emplace_back(&p);
		}

		void shark::add_boat(actor& b)
		{
			boat_ = &b;
		}

		void shark::act(delta_time dt)
		{
			t_since_move_ += dt;
			if (to_seconds(t_since_move_) >= waiting_time(node()))
			{
				state_->perform();
				
				totalSteps_++;
				std::cout << totalSteps_ << std::endl;

				t_since_move_ = from_seconds(0);
			}
		}

		void shark::move_random_node()
		{
			node(random_adjacent_node(node()));
		}

		void shark::move_resting_place()
		{
			if (route_->empty())
				route_ = find_shortest_route(node(), *resting_place_);

			move();
		}

		bool shark::wander()
		{
			move_random_node();
			wanderedSteps_++;

			if (wanderedSteps_ >= 10)
			{
				wanderedSteps_ = 0;
				return true;
			}
			else
				return false;
		}

		bool shark::close_to_pig()
		{
			for (actor* p : pigs_)
				if (pow(p->location().x() - location().x(), 2) + pow(p->location().y() - location().y(), 2) < pow(100, 2)) // (x - center_x)^2 + (y - center_y)^2 < radius^2
				{
					target_ = p;
					return true;
				}

			target_ = nullptr;
			return false;
		}

		void shark::change_state(std::unique_ptr<shark_state> state)
		{
			state_ = std::move(state);
		}

		void shark::move_to_target()
		{
			map::map_node* targetNode = find_target_node();
			route_ = find_shortest_route(node(), *targetNode);
			move();
		}

		map::map_node* shark::find_target_node()
		{
			map::map_node* closest = nullptr;
			float distance = 0;
			
			for (auto i = map_->begin(); i != map_->end(); i++)
			{
				int temp_distance = sqrt(pow(abs(target_->location().x() - i->location().x()), 2) + pow(abs(target_->location().y() - i->location().y()), 2));

				if (!closest || temp_distance < distance)
				{
					closest = &(*i);
					distance = temp_distance;
				}
			}

			return closest;
		}

		void shark::clear_route()
		{
			route_->clear();
		}

		void shark::move()
		{
			if (!route_->empty())
			{
				const map::map_node* nextNode = *route_->begin();
				node(*nextNode);
				route_->erase(route_->begin());
			}
		}
	} // namespace pigisland
} // namespace kmint

