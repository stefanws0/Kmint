#include "kmint/pigisland/steering_behaviors.hpp"
#include "kmint/pigisland/pig.hpp"
#include <cmath>

namespace kmint {
	namespace pigisland {
		steering_behaviors::steering_behaviors(pig* owner) : owner_ { owner }
		{
		}

		/**
		 * \brief 
		 * \param target_position 
		 * \return 
		 */
		math::vector2d steering_behaviors::seek(const math::vector2d target_position) const
		{
			const auto desired_velocity = math::normalize(target_position - owner_->location())
				* owner_->max_speed();
			return (desired_velocity - owner_->velocity());
		}

		/**
		 * \brief 
		 * \param target_position 
		 * \param deceleration 
		 * \return 
		 */
		math::vector2d steering_behaviors::arrive(const math::vector2d target_position, const int deceleration) const
		{
			auto to_target = target_position - owner_->location();

			//calculate the distance to the target position
			const double dist = sqrt((to_target.x() * to_target.x()) + (to_target.y() * to_target.y()));

			if (dist > 0)
			{
				//because Deceleration is enumerated as an int, this value is required
				//to provide fine tweaking of the deceleration.
				const auto deceleration_tweaker = 0.3;

				//calculate the speed required to reach the target given the desired
				//deceleration
				auto speed = dist / (double(deceleration) * deceleration_tweaker);

				//make sure the velocity does not exceed the max
				speed = std::min(speed, double(owner_->max_speed()));

				//from here proceed just like Seek except we don't need to normalize
				//the ToTarget vector because we have already gone to the trouble
				//of calculating its length: dist.
				const auto desired_velocity = to_target * speed / dist;

				return (desired_velocity - owner_->velocity());
			}
			return math::vector2d(0, 0);
		}

		/**
		 * \brief 
		 * \param neighbors 
		 * \return 
		 */
		math::vector2d steering_behaviors::separation(const std::vector<pig*>& neighbors) const
		{
			math::vector2d steering_force;

			for (const auto& neighbor : neighbors)
			{
				//make sure this agent isn't included in the calculations and that
				//the agent being examined is close enough.
				if ((neighbor != owner_) && neighbor->tag())
				{
					auto to_agent = owner_->location() - neighbor->location();

					//scale the force inversely proportional to the agent's distance
					//from its neighbor.
					steering_force += math::normalize(to_agent)/ sqrt(to_agent.x() * to_agent.x() + to_agent.y() * to_agent.y());
				}
			}
			return steering_force;
		};

		/**
		 * \brief 
		 * \param p_pig 
		 * \param neighbors 
		 * \return 
		 */
		math::vector2d steering_behaviors::alignment(const std::vector<pig*>& neighbors)
		{
			//used to record the average heading of the neighbors
			math::vector2d average_heading;
			//used to count the number of vehicles in the neighborhood
			auto neighbor_count = 0;
			//iterate through all the tagged vehicles and sum their heading vectors
			for (const auto& neighbor : neighbors)
			{
				//make sure *this* agent isn't included in the calculations and that
				//the agent being examined is close enough
				if ((neighbor != owner_) && neighbor->tag())
				{
					average_heading += neighbor->heading();
					++neighbor_count;
				}
			}
			//if the neighborhood contained one or more vehicles, average their
			//heading vectors.
			if (neighbor_count > 0)
			{
				average_heading /= double(neighbor_count);
				average_heading -= owner_->heading();
			}
			return average_heading;
		}

		/**
		 * \brief 
		 * \param neighbors 
		 * \return 
		 */
		math::vector2d steering_behaviors::cohesion(const std::vector<pig*>& neighbors)
		{
			math::vector2d center_of_mass;
			math::vector2d steering_force;
			auto neighbor_count = 0;

			for (const auto& neighbor : neighbors)
			{
				if((neighbor != owner_) && neighbor->tag())
				{
					center_of_mass += neighbor->location();

					neighbor_count++;
				}
			}

			if(neighbor_count > 0)
			{
				center_of_mass /= double(neighbor_count);

				steering_force = seek(center_of_mass);
			}

			return steering_force;
		}
	} // namespace pigisland
} // namespace kmint