#pragma once
#include <list>
#include <unordered_map>
#include <time.h>

#define EPS 0.0001f
#define THRESH_MANIFOLD 0.001f

namespace Engine {

	class SupPoint {
	public:
		glm::vec3 mink_p;
		glm::vec3 sup_p;
		glm::vec3 sup_p2;

		void operator=(const SupPoint &s)
		{
			mink_p = s.mink_p;
			sup_p = s.sup_p;
			sup_p2 = s.sup_p2;
		}
		SupPoint() {}
	};

	glm::vec3 farthest_point(const std::vector<glm::vec3> &verts, const glm::vec3 &dir)
	{
		float dot;
		float max = glm::dot(verts[0], dir);
		glm::vec3 res = verts[0];
		unsigned int size = verts.size();

		for (unsigned int i = 1; i < size; i++) {
			dot = glm::dot(verts[i], dir);
			if (dot > max) {
				max = dot;
				res = verts[i];
			}
		}

		return res;
	}

	glm::vec3 triple_prod(const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c)
	{
		return glm::cross(glm::cross(a, b), c);
	}

	SupPoint supp_func(const Engine::Collider *obj1, const Engine::Collider *obj2, const glm::vec3 &dir)
	{
		SupPoint result;
		result.sup_p = obj1->FarthestPoint(dir);
		result.sup_p2 = obj2->FarthestPoint(-dir);
		result.mink_p = result.sup_p - result.sup_p2;
		return result;
	}


	class Simplex {
	public:
		SupPoint	pts[4];
		int			size;

		Simplex()
		{
			size = 0;
		}

		void add(SupPoint p)
		{
			size = size < 4 ? size + 1 : 4;
			pts[3] = pts[2];
			pts[2] = pts[1];
			pts[1] = pts[0];
			pts[0] = p;
		}

		void set(SupPoint a)
		{
			pts[0] = a;
			size = 1;
		}

		void set(SupPoint a, SupPoint b)
		{
			pts[0] = a;
			pts[1] = b;
			size = 2;
		}

		void set(SupPoint a, SupPoint b, SupPoint c)
		{
			pts[0] = a;
			pts[1] = b;
			pts[2] = c;
			size = 3;
		}

		void set(SupPoint a, SupPoint b, SupPoint c, SupPoint d)
		{
			pts[0] = a;
			pts[1] = b;
			pts[2] = c;
			pts[3] = d;
			size = 4;
		}
	};


	void triangle_case_simple(Simplex &simplex, glm::vec3 &direction)
	{
		glm::vec3 ab = simplex.pts[1].mink_p - simplex.pts[0].mink_p;
		glm::vec3 ac = simplex.pts[2].mink_p - simplex.pts[0].mink_p;
		glm::vec3 ao = -simplex.pts[0].mink_p;
		const glm::vec3 abc = glm::cross(ab, ac);

		if (glm::dot(glm::cross(ab, abc), ao) > 0) {
			simplex.set(simplex.pts[0], simplex.pts[1]);
			direction = triple_prod(ab, ao, ab);
		}
		else if (glm::dot(glm::cross(abc, ac), ao) > 0) {
			simplex.set(simplex.pts[0], simplex.pts[2]);
			direction = triple_prod(ac, ao, ac);
		}
		else {
			simplex.set(simplex.pts[0], simplex.pts[1], simplex.pts[2]);
			direction = abc;
		}
	}

	void triangle_case_complex(Simplex &simplex, glm::vec3 &direction)
	{
		const glm::vec3 ab = simplex.pts[1].mink_p - simplex.pts[0].mink_p;
		const glm::vec3 ac = simplex.pts[2].mink_p - simplex.pts[0].mink_p;
		const glm::vec3 ad = simplex.pts[3].mink_p - simplex.pts[0].mink_p;
		const glm::vec3 abc = glm::cross(ab, ac);
		const glm::vec3 ao = -simplex.pts[0].mink_p;

		if (glm::dot(glm::cross(ab, abc), ao) > 0) {
			simplex.set(simplex.pts[0], simplex.pts[1]);
			direction = triple_prod(ab, ao, ab);
		}
		else if (glm::dot(glm::cross(abc, ac), ao) > 0) {
			simplex.set(simplex.pts[0], simplex.pts[2]);
			direction = triple_prod(ac, ao, ac);

		}
		else if (glm::dot(abc, ao) > 0) {
			direction = abc;
		}
		else {
			simplex.set(simplex.pts[0], simplex.pts[2], simplex.pts[1]);
			direction = -abc;
		}
	}

	class SupportTriangle {
	public:
		SupPoint a, b, c;
		glm::vec3 normal;
		SupportTriangle(const SupPoint &_a, const SupPoint &_b, const SupPoint &_c)
		{
			a.mink_p = _a.mink_p;
			a.sup_p = _a.sup_p;
			a.sup_p2 = _a.sup_p2;

			b.mink_p = _b.mink_p;
			b.sup_p = _b.sup_p;
			b.sup_p2 = _b.sup_p2;

			c.mink_p = _c.mink_p;
			c.sup_p = _c.sup_p;
			c.sup_p2 = _c.sup_p2;
			normal = glm::normalize(glm::cross(b.mink_p - a.mink_p, c.mink_p - a.mink_p));
			//normal = glm::normalize(glm::cross(c.mink_p - a.mink_p, b.mink_p - a.mink_p));
		}
		SupportTriangle(void) {}
	};

	glm::vec3 barycentric_coords(const SupportTriangle &t, const glm::vec3 &p)
	{
		glm::vec3 res;

		glm::vec3 v0 = t.b.mink_p - t.a.mink_p;
		glm::vec3 v1 = t.c.mink_p - t.a.mink_p;
		glm::vec3 v2 = p - t.a.mink_p;

		float d00 = dot(v0, v0);
		float d01 = dot(v0, v1);
		float d11 = dot(v1, v1);
		float d20 = dot(v2, v0);
		float d21 = dot(v2, v1);
		float denom = d00 * d11 - d01 * d01;

		res.y = (d11 * d20 - d01 * d21) / denom;
		res.z = (d00 * d21 - d01 * d20) / denom;
		res.x = 1.0f - res.y - res.z;

		return res;

	}


	bool aabb_collision_test(std::pair<glm::vec3, glm::vec3> aabb1, std::pair<glm::vec3, glm::vec3> aabb2, const std::vector<glm::vec3>& aabbVerts1, const std::vector<glm::vec3>& aabbVerts2)
	{
		glm::vec3 lower1 = aabbVerts1[0];
		glm::vec3 upper1 = aabbVerts1[6];

		glm::vec3 lower2 = aabbVerts2[0];
		glm::vec3 upper2 = aabbVerts2[6];

		for (unsigned int i = 0; i < 8; i++) {
			if (aabbVerts1[i].x >= lower2.x && aabbVerts1[i].x <= upper2.x &&
				aabbVerts1[i].y >= lower2.y && aabbVerts1[i].y <= upper2.y &&
				aabbVerts1[i].z >= lower2.z && aabbVerts1[i].z <= upper2.z) {
				return true;
			}
		}

		for (unsigned int i = 0; i < 8; i++) {
			if (aabbVerts2[i].x >= lower1.x && aabbVerts2[i].x <= upper1.x &&
				aabbVerts2[i].y >= lower1.y && aabbVerts2[i].y <= upper1.y &&
				aabbVerts2[i].z >= lower1.z && aabbVerts2[i].z <= upper1.z) {
				return true;
			}
		}

		return false;
	}

	bool GJK(Engine::Collider* obj1, Engine::Collider *obj2) {
		glm::vec3 a, b, c, direction, ao, ab, ac, ad;

		Simplex simplex;
		SupPoint supp_point;

		glm::vec3 position1 = obj1->gameObject->GetTransform()->position;
		glm::vec3 position2 = obj2->gameObject->GetTransform()->position;

		direction = glm::vec3(1, 0, 0);
		supp_point = supp_func(obj1, obj2,  direction);
		if (fabs(glm::dot(direction, supp_point.mink_p)) >= glm::length(supp_point.mink_p) * 0.8f) {
			direction = glm::vec3(0, 1, 0);
			supp_point = supp_func(obj1, obj2, direction);
		}

		//Alternative version
		//direction = position1 - position2;
		//supp_point = supp_func (vertices1, vertices2, direction);

		simplex.add(supp_point);

		direction = -supp_point.mink_p;

		for (int num_iter = 50; num_iter >= 0; num_iter--) {
			supp_point = supp_func(obj1, obj2, direction);
			if (glm::dot(supp_point.mink_p, direction) < 0) {
				return false;
			}

			simplex.add(supp_point);

			if (simplex.size == 2) {
				ao = -simplex.pts[0].mink_p;
				glm::vec3 ab = simplex.pts[1].mink_p - simplex.pts[0].mink_p;
				direction = triple_prod(ab, ao, ab);
				continue;
			}
			if (simplex.size == 3) {
				triangle_case_complex(simplex, direction);
				continue;

			}
			else if (simplex.size == 4) {
				glm::vec3 ab = simplex.pts[1].mink_p - simplex.pts[0].mink_p;
				glm::vec3 ac = simplex.pts[2].mink_p - simplex.pts[0].mink_p;
				glm::vec3 ad = simplex.pts[3].mink_p - simplex.pts[0].mink_p;
				ao = -simplex.pts[0].mink_p;

				if (glm::dot(glm::cross(ab, ac), ao) > 0) {
					triangle_case_simple(simplex, direction);
					continue;
				}

				if (glm::dot(glm::cross(ac, ad), ao) > 0) {
					simplex.set(simplex.pts[0], simplex.pts[2], simplex.pts[3]);
					triangle_case_simple(simplex, direction);
					continue;
				}

				if (glm::dot(glm::cross(ad, ab), ao) > 0) {
					simplex.set(simplex.pts[0], simplex.pts[3], simplex.pts[1]);
					triangle_case_simple(simplex, direction);
					continue;
				}

				return true;

			}
		}

		return false;
	}


}