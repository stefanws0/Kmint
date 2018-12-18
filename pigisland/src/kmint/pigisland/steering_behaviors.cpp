#include "kmint/pigisland/steering_behaviors.hpp"
#include "kmint/pigisland/pig.hpp"
#include "kmint/pigisland/wall.hpp"
#include <cmath>
#include <utility>
#include "kmint/pigisland/matrix.hpp"

namespace kmint {
	namespace pigisland {
		steering_behaviors::steering_behaviors(pig* owner, std::vector<wall> walls ) : owner_ { owner }, walls_{
			std::move(walls)
		}
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
		 * \return 
		 */
		math::vector2d steering_behaviors::separation() const
		{
			const auto& neighbors = owner_->pigs();
			math::vector2d steering_force;

			for (const auto& neighbor : neighbors)
			{
				//make sure this agent isn't included in the calculations and that
				//the agent being examined is close enough.
				if ((neighbor != owner_) && neighbor->tag())
				{
					const auto to_agent = owner_->location() - neighbor->location();

					//scale the force inversely proportional to the agent's distance
					//from its neighbor.
					steering_force += math::normalize(to_agent)/ math::norm(to_agent);
				}
			}
			return steering_force * 10;
		};

		/**
		 * \brief 
		 * \return 
		 */
		math::vector2d steering_behaviors::alignment() const
		{
			const auto& neighbors = owner_->pigs();
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
		 * \return 
		 */
		math::vector2d steering_behaviors::cohesion() const
		{
			const auto& neighbors = owner_->pigs();
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

		inline bool line_intersection(math::vector2d a, math::vector2d b, math::vector2d c, math::vector2d d, double& dist, math::vector2d& point)
		{
			const double r_top = (a.y() - c.y()) * (d.x() - c.x()) - (a.x() - c.x()) * (d.y() - c.y());
			const double r_bot = (b.x() - a.x()) * (d.y() - c.y()) - (b.y() - a.y()) * (d.x() - c.x());

			const double s_top = (a.y() - c.y()) * (b.x() - a.x()) - (a.x() - c.x()) * (b.y() - a.y());
			const double s_bot = (b.x() - a.x()) * (d.y() - c.y()) - (b.y() - a.y()) * (d.x() - c.x());

			if ((r_bot == 0) || (s_bot == 0))
			{
				//lines are parallel
				return false;
			}

			const auto r = r_top / r_bot;
			const auto s = s_top / s_bot;

			if ((r > 0) && (r < 1) && (s > 0) && (s < 1))
			{
				dist = math::distance(a, b) * r;

				point = a + r * (b - a);

				return true;
			}

			else
			{
				dist = 0;

				return false;
			}
		}

		inline void rotate_around_origin(math::vector2d& v, const double ang)
		{
			//create a transformation matrix
			matrix2d mat;

			//
			mat.rotate(ang);

			//now transform the object's vertices
			mat.transform_vectors(v);
		}

		/**
		 * \brief 
		 * \param walls 
		 * \return 
		 */
		math::vector2d steering_behaviors::wall_avoidance(std::vector<wall>& walls)
		{
			create_feelers();

			auto distance_to_this_ip = 0.0;
			auto distance_to_closest_ip = std::numeric_limits<double>::max();

			auto closest_wall = -1;

			math::vector2d steering_force;
			math::vector2d point;
			math::vector2d closest_point;

			for (const auto feeler : feelers_)
			{
				for(auto w = 0; w < walls_.size(); w++)
				{
					if(line_intersection(
						owner_->location(),
						feeler,
						walls_.at(w).from(),
						walls_.at(w).to(),
						distance_to_this_ip,
						point))
					{
						if(distance_to_closest_ip > distance_to_this_ip)
						{
							distance_to_closest_ip = distance_to_this_ip;

							closest_wall = w;

							closest_point = point;
						}
					}
				}
				if(closest_wall >= 0 )
				{
					const auto overshoot = feeler - closest_point;
					steering_force = walls.at(closest_wall).normal() * math::norm(overshoot);
				}
			}

			return steering_force * 1000;
		}

		math::vector2d steering_behaviors::obstacle_avoidance(const std::vector<pig*> &obstacles)
		{
			return {0,0};
		}

		void steering_behaviors::enforce_non_penetration_constraint() const
		{
			auto entities = owner_->pigs();
			//iterate through all entities checking for any overlap of bounding radii
			for (const auto entity: entities)
			{
				//make sure we don't check against the individual
				if (entity == owner_) continue;
				//calculate the distance between the positions of the entities
				const auto to_entity = owner_->location() - entity->location();
				const double dist_from_each_other = math::norm(to_entity);
				//if this distance is smaller than the sum of their radii then this
				//entity must be moved away in the direction parallel to the
				//ToEntity vector
				const auto amount_of_over_lap = 8 + 8 - dist_from_each_other;
				if (amount_of_over_lap >= 0)
				{
					//move the entity a distance away equivalent to the amount of overlap.
					owner_->set_location(owner_->location() + (to_entity / dist_from_each_other) * amount_of_over_lap);
				}
			}//
		}

		void steering_behaviors::create_feelers()
		{
			feelers_.clear();
			//feeler pointing straight in front
			feelers_.push_back(owner_->location() + (10 * owner_->heading()));

			//feeler to left
			auto temp = owner_->heading();
			rotate_around_origin(temp, (3.14159 / 2) * 3.5f);
			feelers_.push_back(owner_->location() + (10 / 2.0f * temp));

			//feeler to right
			temp = owner_->heading();
			rotate_around_origin(temp, (3.14159 / 2) * 0.5f);
			feelers_.push_back(owner_->location() + (10 / 2.0f * temp));
		}

		math::vector2d steering_behaviors::calculate(const math::vector2d boat_location, const float boat_attraction, const math::vector2d shark_location, const float shark_attraction, const float p_cohesion, const float p_alignment, const float p_separation)
		{
			math::vector2d steering_force;

			steering_force += separation() * p_separation;
			steering_force += cohesion() * p_cohesion;
			steering_force += alignment() * p_alignment;
			steering_force += wall_avoidance(walls_);
			steering_force += arrive(boat_location, 2) * boat_attraction;
			steering_force += arrive(shark_location, 2) * shark_attraction; 
			enforce_non_penetration_constraint();

			return steering_force;
		}
	} // namespace pigisland
} // namespace kmint