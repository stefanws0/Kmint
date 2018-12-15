#include "kmint/pigisland/matrix.hpp"

namespace kmint {
	namespace pigisland {
		
		void matrix2d::y1_x1(const double val){ matrix_.y1_x1_ = val; }
		void matrix2d::y1_x2(const double val){ matrix_.y1_x2_ = val; }
		void matrix2d::y1_x3(const double val){ matrix_.y1_x3_ = val; }
		void matrix2d::y2_x1(const double val){ matrix_.y2_x1_ = val; }
		void matrix2d::y2_x2(const double val){ matrix_.y2_x2_ = val; }
		void matrix2d::y2_x3(const double val){ matrix_.y2_x3_ = val; }
		void matrix2d::y3_x1(const double val){ matrix_.y3_x1_ = val; }
		void matrix2d::y3_x2(const double val){ matrix_.y3_x2_ = val; }
		void matrix2d::y3_x3(const double val){ matrix_.y3_x3_ = val; }

		//multiply two matrices together
		inline void matrix2d::matrix_multiply(matrix& matrix_in)
		{
			matrix2d::matrix mat_temp;

			//first row
			mat_temp.y1_x1_ = (matrix_.y1_x1_ * matrix_in.y1_x1_) + (matrix_.y1_x2_ * matrix_in.y2_x1_) + (matrix_.y1_x3_ * matrix_in.y3_x1_);
			mat_temp.y1_x2_ = (matrix_.y1_x1_ * matrix_in.y1_x2_) + (matrix_.y1_x2_ * matrix_in.y2_x2_) + (matrix_.y1_x3_ * matrix_in.y3_x2_);
			mat_temp.y1_x3_ = (matrix_.y1_x1_ * matrix_in.y1_x3_) + (matrix_.y1_x2_ * matrix_in.y2_x3_) + (matrix_.y1_x3_ * matrix_in.y3_x3_);

			//second
			mat_temp.y2_x1_ = (matrix_.y2_x1_ * matrix_in.y1_x1_) + (matrix_.y2_x2_ * matrix_in.y2_x1_) + (matrix_.y2_x3_ * matrix_in.y3_x1_);
			mat_temp.y2_x2_ = (matrix_.y2_x1_ * matrix_in.y1_x2_) + (matrix_.y2_x2_ * matrix_in.y2_x2_) + (matrix_.y2_x3_ * matrix_in.y3_x2_);
			mat_temp.y2_x3_ = (matrix_.y2_x1_ * matrix_in.y1_x3_) + (matrix_.y2_x2_ * matrix_in.y2_x3_) + (matrix_.y2_x3_ * matrix_in.y3_x3_);

			//third
			mat_temp.y3_x1_ = (matrix_.y3_x1_ * matrix_in.y1_x1_) + (matrix_.y3_x2_ * matrix_in.y2_x1_) + (matrix_.y3_x3_ * matrix_in.y3_x1_);
			mat_temp.y3_x2_ = (matrix_.y3_x1_ * matrix_in.y1_x2_) + (matrix_.y3_x2_ * matrix_in.y2_x2_) + (matrix_.y3_x3_ * matrix_in.y3_x2_);
			mat_temp.y3_x3_ = (matrix_.y3_x1_ * matrix_in.y1_x3_) + (matrix_.y3_x2_ * matrix_in.y2_x3_) + (matrix_.y3_x3_ * matrix_in.y3_x3_);

			matrix_ = mat_temp;
		}

		//applies a 2D transformation matrix to a std::vector of Vector2Ds
		void matrix2d::transform_vectors(std::vector<math::vector2d> &v_point) const
		{
			for (auto& i : v_point)
			{
				const auto temp_x = (matrix_.y1_x1_ * i.x()) + (matrix_.y2_x1_ * i.y()) + (matrix_.y3_x1_);

				const auto temp_y = (matrix_.y1_x2_ * i.x()) + (matrix_.y2_x2_ * i.y()) + (matrix_.y3_x2_);

				i.x(temp_x);

				i.y(temp_y);

			}
		}

		//applies a 2D transformation matrix to a single Vector2D
		void matrix2d::transform_vectors(math::vector2d &v_point) const
		{
			const auto temp_x = (matrix_.y1_x1_ * v_point.x()) + (matrix_.y2_x1_ * v_point.y()) + (matrix_.y3_x1_);

			const auto temp_y = (matrix_.y1_x2_ * v_point.x()) + (matrix_.y2_x2_ * v_point.y()) + (matrix_.y3_x2_);

			v_point.x(temp_x);

			v_point.y(temp_y);
		}



		//create an identity matrix
		void matrix2d::identify()
		{
			matrix_.y1_x1_ = 1; matrix_.y1_x2_ = 0; matrix_.y1_x3_ = 0;

			matrix_.y2_x1_ = 0; matrix_.y2_x2_ = 1; matrix_.y2_x3_ = 0;

			matrix_.y3_x1_ = 0; matrix_.y3_x2_ = 0; matrix_.y3_x3_ = 1;

		}

		//create a transformation matrix
		void matrix2d::translate(const double x, const double y)
		{
			matrix mat;

			mat.y1_x1_ = 1; mat.y1_x2_ = 0; mat.y1_x3_ = 0;

			mat.y2_x1_ = 0; mat.y2_x2_ = 1; mat.y2_x3_ = 0;

			mat.y3_x1_ = x;    mat.y3_x2_ = y;    mat.y3_x3_ = 1;

			//and multiply
			matrix_multiply(mat);
		}

		//create a scale matrix
		void matrix2d::scale(const double x_scale, const double y_scale)
		{
			matrix2d::matrix mat;

			mat.y1_x1_ = x_scale; mat.y1_x2_ = 0; mat.y1_x3_ = 0;

			mat.y2_x1_ = 0; mat.y2_x2_ = y_scale; mat.y2_x3_ = 0;

			mat.y3_x1_ = 0; mat.y3_x2_ = 0; mat.y3_x3_ = 1;

			//and multiply
			matrix_multiply(mat);
		}


		//create a rotation matrix
		void matrix2d::rotate(const double rot)
		{
			matrix2d::matrix mat;

			const auto sin = std::sin(rot);
			const auto cos = std::cos(rot);

			mat.y1_x1_ = cos;  mat.y1_x2_ = sin; mat.y1_x3_ = 0;

			mat.y2_x1_ = -sin; mat.y2_x2_ = cos; mat.y2_x3_ = 0;

			mat.y3_x1_ = 0; mat.y3_x2_ = 0; mat.y3_x3_ = 1;

			//and multiply
			matrix_multiply(mat);
		}


		//create a rotation matrix from a 2D vector
		void matrix2d::rotate(const math::vector2d &fwd, const math::vector2d &side)
		{
			matrix2d::matrix mat;

			mat.y1_x1_ = fwd.x();  mat.y1_x2_ = fwd.y(); mat.y1_x3_ = 0;

			mat.y2_x1_ = side.x(); mat.y2_x2_ = side.y(); mat.y2_x3_ = 0;

			mat.y3_x1_ = 0; mat.y3_x2_ = 0; mat.y3_x3_ = 1;

			//and multiply
			matrix_multiply(mat);
		}
	} // namespace pigisland
} // namespace kmint
