#include "math.h"

namespace t2k {

	int getSidesPointAndPlane(const Vector3& v, const Vector3& pn, const Vector3& pv)
	{
		Vector3 vec = v - pv;
		float a = vec.dot( pn );
		if (a > 0.0f) 		return 1;		// •\
		else if (a < 0.0f)	return -1;		// — 
		else 				return 0;		// •½–Êã
	}


	int getRegionPointAndRect(const Vector3& p, const Vector3& rp, const int rect_w, const int rect_h) {
		t2k::Vector3 v1 = t2k::Vector3(rect_h, rect_w, 0).normalize();
		t2k::Vector3 v2 = t2k::Vector3(rect_h, -rect_w, 0).normalize();
//		t2k::Vector3 vc1 = v1.cross(t2k::Vector3(0, 0, 1));
//		t2k::Vector3 vc2 = v2.cross(t2k::Vector3(0, 0, 1));
		t2k::Vector3 vc1 = v1;
		t2k::Vector3 vc2 = v2;

		int s1 = t2k::getSidesPointAndPlane(p, vc1, rp);
		int s2 = t2k::getSidesPointAndPlane(p, vc2, rp);
		if (s1 >= 0 && s2 >= 0) {
			return 1;
		}
		else if (s1 >= 0 && s2 <= 0) {
			return 2;
		}
		else if (s1 <= 0 && s2 >= 0) {
			return 0;
		}
		else {
			return 3;
		}
	}


	t2k::Vector3 getNearestRectPoint(const t2k::Vector3& rect_pos, float w, float h, const t2k::Vector3 &point) {
		t2k::Vector3 nearest(0, 0, 0);
		float hw = w * 0.5f;
		float hh = h * 0.5f;
		float l = rect_pos.x - hw;
		float r = rect_pos.x + hw;
		float t = rect_pos.y - hh;
		float b = rect_pos.y + hh;
		nearest.x = (point.x > l && point.x < r) ? point.x : (point.x > l) ? r : l;
		nearest.y = (point.y > t && point.y < b) ? point.y : (point.y > t) ? b : t;
		return nearest;
	}

}