#ifndef KMINT_PIGISLAND_WALL_HPP
#define KMINT_PIGISLAND_WALL_HPP

#include "kmint/map/map.hpp"
#include "kmint/play.hpp"
#include "kmint/primitives.hpp"
namespace kmint {
	namespace pigisland {

		class wall {
		public:
			wall(math::vector2d from, math::vector2d to);
			math::vector2d from() const;
			math::vector2d to() const;
			math::vector2d normal() const;
			void calculate_normal();

		private:
			math::vector2d to_;
			math::vector2d from_;
			math::vector2d normal_;
		};

	} // namespace pigisland
} // namespace kmint

#endif /* KMINT_PIGISLAND_WALL_HPP */