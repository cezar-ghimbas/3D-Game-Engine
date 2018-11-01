#pragma once

namespace Engine {

	class Vertex {
	public:

		Vertex() {}
		Vertex(float _x, float _y, float _z, float _n_x, float _n_y, float _n_z)
		{
			x = _x;
			y = _y;
			z = _z;

			nx = _n_x;
			ny = _n_y;
			nz = _n_z;
		}

		Vertex(float _x, float _y, float _z, float _n_x, float _n_y, float _n_z, float _t_x, float _t_y)
		{
			x = _x;
			y = _y;
			z = _z;

			nx = _n_x;
			ny = _n_y;
			nz = _n_z;

			tx = _t_x;
			ty = _t_y;
		}

		Vertex *operator=(Vertex v)
		{
			x = v.x;
			y = v.x;
			z = v.z;

			nx = v.nx;
			ny = v.ny;
			nz = v.nz;
			return this;
		}

		float x, y, z;
		float nx, ny, nz;
		float tx, ty;
	};

}