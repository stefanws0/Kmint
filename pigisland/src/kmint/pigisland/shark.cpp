#include "kmint/pigisland/shark.hpp"
#include "kmint/pigisland/node_algorithm.hpp"
#include "kmint/pigisland/resources.hpp"
#include "kmint/pigisland/pig.hpp"
#include "kmint/pigisland/shark_state.hpp"
#include "kmint/random.hpp"

#include <iostream>
#include <memory>
#include <cmath>

namespace kmint {
	namespace pigisland {

		shark::shark(kmint::map::map_graph &g) :
			play::map_bound_actor{ g, find_shark_resting_place(g) }, drawable_{ *this, shark_image() }, map_{ &g }, 
			resting_place_(&node()), totalSteps_{ 0 }, route_{ nullptr }, state_{ std::make_unique<shark_state>() }
		{
		}

		void shark::add_pig(actor& p)
		{
			pigs_.emplace_back(&p);
		}

		void shark::act(delta_time dt)
		{
			t_since_move_ += dt;
			if (to_seconds(t_since_move_) >= waiting_time(node()))
			{
				//node(random_adjacent_node(node()));
				if (route_ == nullptr)
				{
					node(*map_->begin());
					route_ = find_shortest_route(node(), *resting_place_);
				}	
				else if (route_->empty())
				{
					route_ = nullptr;
				}	
				else
				{
					const map::map_node* nextNode = *route_->begin();
					node(*nextNode);
					route_->erase(route_->begin());
				}
				
				totalSteps_++;
				std::cout << totalSteps_ << std::endl;

				t_since_move_ = from_seconds(0);
			}

			actor* foundPig = nullptr;

			for (actor* p : pigs_)
			{
				if (pow(p->location().x() - location().x(), 2) + pow(p->location().y() - location().y(), 2) < pow(100, 2)) // (x - center_x)^2 + (y - center_y)^2 < radius^2
				{
					foundPig = p;
				}
			}

		}
	} // namespace pigisland
} // namespace kmint
