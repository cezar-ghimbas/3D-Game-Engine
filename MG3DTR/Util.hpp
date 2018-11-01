#pragma once
#include <vector>
#include <iostream>

#define PI_FLOAT	3.14159265
#define DEG			180
#define N_COLS		12

namespace physics {
	class Util {
	public:
		static void trans_mult(std::vector<glm::vec4> *verts, glm::vec3 delta)
		{
			glm::mat4 trans_mat = glm::translate(glm::mat4(1.0f), delta);
			unsigned int size = verts->size();
			for (unsigned int i = 0; i < size; i++) {
				verts->at(i) = trans_mat * verts->at(i);
			}
		}

		static void trans_mult(std::vector<glm::vec3> *verts, glm::vec3 delta)
		{
			glm::mat4 trans_mat = glm::translate(glm::mat4(1.0f), delta);
			unsigned int size = verts->size();
			for (unsigned int i = 0; i < size; i++) {
				verts->at(i) = glm::vec3(trans_mat * glm::vec4(verts->at(i), 1));
			}
		}

		static std::vector<glm::vec3> *trans_mult_ret(std::vector<glm::vec3> *verts, glm::vec3 delta)
		{
			glm::mat4 trans_mat = glm::translate(glm::mat4(1.0f), delta);
			std::vector<glm::vec3> *ret = new std::vector<glm::vec3>();
			unsigned int size = verts->size();
			for (unsigned int i = 0; i < size; i++) {
				ret->push_back(glm::vec3(trans_mat * glm::vec4(verts->at(i), 1)));
			}

			return ret;
		}

		static void trans_mult(std::vector<glm::vec4> *verts, unsigned int start, 
				unsigned int end, glm::vec3 delta)
		{
			glm::mat4 trans_mat = glm::translate(glm::mat4(1.0f), delta);
			for (unsigned int i = start; i < end; i++) {
				verts->at(i) = trans_mat * verts->at(i);
			}
		}

		static void rot_mult(std::vector<glm::vec4> *verts, float angle, glm::vec3 axis)
		{
			unsigned int size = verts->size();
			glm::mat4 rot_mat = glm::rotate(glm::mat4(1.0f), angle, axis);
			for (unsigned int i = 0; i < size; i++) {
				verts->at(i) = rot_mat * verts->at(i);
			}
		}

		static void rot_mult(std::vector<glm::vec3> *verts, float angle, glm::vec3 axis)
		{
			unsigned int size = verts->size();
			glm::mat4 rot_mat = glm::rotate(glm::mat4(1.0f), angle, axis);
			for (unsigned int i = 0; i < size; i++) {
				verts->at(i) = glm::vec3(rot_mat * glm::vec4(verts->at(i), 1));
			}
		}

		static void rot_mult_ctr(std::vector<glm::vec4> *verts, float angle, glm::vec3 axis)
		{
			glm::vec3 centre = get_centre(*verts);
			trans_mult(verts, -centre);
			rot_mult(verts, angle, axis);
			trans_mult(verts, centre);
		}


		static void rot_mult_ctr(std::vector<glm::vec3> *verts, float angle, glm::vec3 axis)
		{
			glm::vec3 centre = get_centre(*verts);
			trans_mult(verts, -centre);
			rot_mult(verts, angle, axis);
			trans_mult(verts, centre);
		}

		static glm::vec3 get_centre(std::vector<glm::vec4> verts)
		{
			glm::vec3 centre = glm::vec3();
			unsigned int size = verts.size();

			for (unsigned int i = 0; i < size; i++) {
				centre.x += verts[i].x;
				centre.y += verts[i].y;
				centre.z += verts[i].z;
			}

			centre.x /= size;
			centre.y /= size;
			centre.z /= size;

			return centre;
		}

		static glm::vec3 get_centre(std::vector<glm::vec3> verts)
		{
			glm::vec3 centre = glm::vec3();
			unsigned int size = verts.size();

			for (unsigned int i = 0; i < size; i++) {
				centre.x += verts[i].x;
				centre.y += verts[i].y;
				centre.z += verts[i].z;
			}

			centre.x /= size;
			centre.y /= size;
			centre.z /= size;

			return centre;
		}

		static float vec_angle(glm::vec4 v1, glm::vec4 v2)
		{
			glm::vec3 v1_n = glm::normalize(glm::vec3(v1));
			glm::vec3 v2_n = glm::normalize(glm::vec3(v2));
			return (float) (DEG / PI_FLOAT) * acos(glm::dot(v1_n, v2_n));
		}

		static void scale_mult(std::vector<glm::vec4> *verts, glm::vec3 amount)
		{
			glm::mat4 scale_mat = glm::scale(glm::mat4(1.0f), amount);
			unsigned int size = verts->size();
			for (unsigned int i = 0; i < size; i++) {
				verts->at(i) = scale_mat * verts->at(i);
			}
		}

		static void scale_mult_ctr(std::vector<glm::vec4> *verts, glm::vec3 amount)
		{
			glm::vec3 centre = get_centre(*verts);
			trans_mult(verts, -centre);
			scale_mult(verts, amount);
			trans_mult(verts, centre);
		}

		static void norm_mult(std::vector<glm::vec4> *verts)
		{
			unsigned int size = verts->size();
			for (unsigned int i = 0; i < size; i++)
				verts->at(i) = glm::normalize(verts->at(i));
		}

		static void print_vec(const glm::vec3 &v)
		{
			std::cout << v.x << " " << v.y << " " << v.z << "\n";
		}

		static void print_vec(const std::vector<glm::vec3> &v)
		{
			//std::cout << v.x << " " << v.y << " " << v.z << "\n";
			for (unsigned int i = 0; i < v.size(); i++) {
				print_vec(v[i]);
			}
		}

		static glm::quat quat_from_angle_axis(const float &angle, const glm::vec3 &axis)
		{
			glm::quat res = glm::quat(
				cos(glm::radians(angle / 2)),
				axis.x * sin(glm::radians(angle / 2)),
				axis.y * sin(glm::radians(angle / 2)),
				axis.z * sin(glm::radians(angle / 2))
			);
			return res;
		}

		static glm::vec3 to_global(glm::vec3 local_p, glm::quat orientation, glm::vec3 position) {
			 return orientation * local_p + position;
		}

		~Util(void)
		{
		}

		class mat {
			
		public:
			mat() {}
			mat(unsigned int nr, unsigned int nc)
			{
				m = new float[nr * nc];
				n_rows = nr;
				n_cols = nc;
			}
			mat(float val, unsigned int nr, unsigned int nc)
			{
				m = new float[nr * nc];
				n_rows = nr;
				n_cols = nc;

				for (unsigned int i = 0; i < nr * nc; i++) {
					m[i] = val;
				}

			}
			mat(float *_m, unsigned int nr, unsigned int nc)
			{
				m = new float[nr * nc];
				n_rows = nr;
				n_cols = nc;
				for (unsigned int i = 0; i < nr * nc; i++) {
					m[i] = _m[i];
				}
			}

			mat operator*(mat a)
			{
				mat res = mat(0.0f, n_rows, a.n_cols);
				//exterior -> m
				//interior -> a

				for (unsigned int i = 0; i < n_rows; i++) {
					for (unsigned int j = 0; j < a.n_cols; j++) {
						for (unsigned int k = 0; k < a.n_rows; k++) {
							//res[i * n_cols + j] = res[i * n_cols + j] + (m[i * n_cols + k] * a[k * a.n_cols + j]);
							res[i * a.n_cols + j] = res[i * a.n_cols + j] + (m[i * n_cols + k] * a[k * a.n_cols + j]);
							//std::cout << "----------------------------\n";
						}
					}
				}

				return res;
			}

			mat operator+(mat a)
			{
				mat res = mat(n_rows, n_cols);

				for (unsigned int i = 0; i < n_rows; i++) {
					for (unsigned int j = 0; j < n_cols; j++) {
						res[i * n_cols + j] = m[i * n_cols + j] +  a[i * n_cols + j];
					}
				}

				return res;
			}

			mat operator*(float val)
			{
				mat res = mat(n_rows, n_cols);

				for (unsigned int i = 0; i < n_rows; i++) {
					for (unsigned int j = 0; j < n_cols; j++) {
						res[i * n_cols + j] = m[i * n_cols + j] * val;
					}
				}

				return res;
			}


			/*float* &operator*()
			{
				return m;
			}*/

			float &operator[](unsigned int i)
			{
				return m[i];
			}


			/*mat12 operator=( int index)
			{

				return *this;
			}*/

			unsigned int get_n_cols()
			{
				return n_cols;
			}

			unsigned int get_n_rows()
			{
				return n_rows;
			}

			mat transpose()
			{
				mat res = mat(n_cols, n_rows);

				for (unsigned int i = 0; i < n_rows; i++) {
					for (unsigned int j = 0; j < n_cols; j++) {
						res[j * n_rows + i] = m[i * n_cols + j];
					}
				}

				return res;
			}

			std::ostream &operator<<(std::ostream &out)
			{
				for (unsigned int i = 0; i < n_rows; i++) {
					for (unsigned int j = 0; j < n_cols; j++) {
						out << m[i * n_cols + j] << " ";
					}
					out << "\n";
				}
			}

			void print()
			{
				for (unsigned int i = 0; i < n_rows; i++) {
					for (unsigned int j = 0; j < n_cols; j++) {
						std::cout << m[i * n_cols + j] << " ";
					}
					std::cout << "\n";
				}
				std::cout << "\n";
			}

		private:
			float *m;
			unsigned int n_cols, n_rows;
		};

		class vec12 {
			
		};
	};
}