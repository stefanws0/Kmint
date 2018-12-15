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
  return {x, y};
}
} // namespace

pig::pig(math::vector2d location)
	: free_roaming_actor{ random_vector() }, drawable_{ *this, pig_image() }
{
	shark_attraction_ = float(random_int(-10, 10)) / 10;
	boat_attraction_ = float(random_int(-10, 10)) / 10;
	cohesion_ = float(random_int(0, 10)) / 10;
	separation_ = float(random_int(0, 10)) / 10;
	alignment_ = float(random_int(0, 10)) / 10;
	boarded_ = false;
}

	/**
 * \brief 
 * \param location 
 * \param shark_attraction 
 * \param boat_attraction 
 * \param cohesion 
 * \param separation 
 * \param alignment 
 */
pig::pig(math::vector2d location, const float shark_attraction, const float boat_attraction, const float cohesion, const float separation,
         float alignment)
	: free_roaming_actor{ random_vector() },
	boarded_{ false },
	shark_attraction_{ shark_attraction },
	boat_attraction_{ boat_attraction },
	cohesion_{ cohesion },
	separation_{ separation },
	alignment_{ alignment },
	drawable_{*this, pig_image()}
{
}

void pig::set_shark(actor& shark)
{
	shark_ = &shark;
}

void pig::set_boat(actor& boat)
{
	boat_ = &boat;
}

math::vector2d calculate_shark_direction(float shark_attraction, math::vector2d pig_location, math::vector2d shark_location)
{
	math::vector2d new_location = { pig_location.x(), pig_location.y() };
	const float speed = (1 * shark_attraction);
	if (shark_location.x() > pig_location.x())
	{
		if (shark_location.x() > pig_location.x() + speed)
		{
			new_location.x(new_location.x() + speed);
		}
		else
		{
			new_location.x(shark_location.x());
		}
	}
	else
	{
		if (shark_location.x() < pig_location.x() - speed)
		{
			new_location.x(new_location.x() - speed);
		}
		else
		{
			new_location.x(shark_location.x());
		}
	}

	if (shark_location.y() > pig_location.y())
	{
		if (shark_location.y() > pig_location.y() + speed)
		{
			new_location.y(new_location.y() + speed);
		}
		else
		{
			new_location.y(shark_location.y());
		}
	}
	else
	{
		if (shark_location.y() < pig_location.y() - speed)
		{
			new_location.y(new_location.y() - speed);
		}
		else
		{
			new_location.y(shark_location.y());
		}
	}
	return new_location;
}

void pig::act(delta_time dt) {
	auto boat_location = boat_->location();
	auto shark_location = shark_->location();
	auto pig_location = location();

	pig_location = calculate_shark_direction(shark_attraction_, pig_location, shark_location);
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
