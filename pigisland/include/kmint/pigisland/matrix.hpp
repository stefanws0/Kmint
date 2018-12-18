#ifndef KMINT_PIGISLAND_MATRIX_HPP
#define KMINT_PIGISLAND_MATRIX_HPP
#include "kmint/play.hpp"

namespace kmint {
	namespace pigisland {
		class matrix2d
		{
		private:
			struct matrix
			{
				double y1_x1_, y1_x2_, y1_x3_;
				double y2_x1_, y2_x2_, y2_x3_;
				double y3_x1_, y3_x2_, y3_x3_;

				matrix()
				{
					y1_x1_ = 0.0; y1_x2_ = 0.0; y1_x3_ = 0.0;
					y2_x1_ = 0.0; y2_x2_ = 0.0; y2_x3_ = 0.0;
					y3_x1_ = 0.0; y3_x2_ = 0.0; y3_x3_ = 0.0;
				}
			};

			matrix matrix_;

			inline void matrix_multiply(matrix &matrix_in);
		public:

			matrix2d()
			{
				identify();
			}

			//create an identity matrix
			void identify();

			//create a transformation matrix
			void translate(double x, double y);

			//create a scale matrix
			void scale(double x_scale, double y_scale);

			//create a rotation matrix
			void rotate(double rot);

			//create a rotation matrix from a fwd and side 2D vector
			void rotate(const math::vector2d &fwd, const math::vector2d &side);

			//applies a transformation matrix to a std::vector of points
			void transform_vectors(std::vector<math::vector2d> & v_point) const;

			//applies a transformation matrix to a point
			void transform_vectors(math::vector2d &v_point) const;

			//accessors to the matrix elements
			void y1_x1(double val);
			void y1_x2(double val);
			void y1_x3(double val);

			void y2_x1(double val);
			void y2_x2(double val);
			void y2_x3(double val);

			void y3_x1(double val);
			void y3_x2(double val);
			void y3_x3(double val);
		};
	} // namespace pigisland
} // namespace kmint

#endif /* KMINT_PIGISLAND_BOAT_HPP */
