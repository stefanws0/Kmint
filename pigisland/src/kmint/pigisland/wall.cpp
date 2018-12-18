#include "kmint/pigisland/wall.hpp"

namespace kmint {
	namespace pigisland {
		wall::wall(math::vector2d from, math::vector2d to) : from_{ from }, to_{ to }{ calculate_normal(); }

		math::vector2d wall::from() const {return from_;}
		math::vector2d wall::to() const { return to_; }
		math::vector2d wall::normal() const { return normal_;  }

		void wall::calculate_normal()
		{
			auto temp = math::normalize(to_ - from_);

			normal_.x(-temp.y());
			normal_.y(temp.x());
		}
	} // namespace pigisland
} // namespace kmint
