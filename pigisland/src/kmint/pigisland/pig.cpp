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

void pig::act(delta_time dt) {
  free_roaming_actor::act(dt);
}
} // namespace pigisland

} // namespace kmint
