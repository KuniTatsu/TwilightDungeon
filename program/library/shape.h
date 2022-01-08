#pragma once
#include "vector.h"

namespace t2k {

	class Rect {
	public:
		Rect() {}
		Rect(const t2k::Vector3& p, float width, float height)
			:pos_(p)
			, width_(width)
			, height_(height)
		{}
		float width_;
		float height_;
		t2k::Vector3 pos_;
		inline float ltx() { return pos_.x - width_ * 0.5f; }
		inline float lty() { return pos_.y - height_ * 0.5f; }
		inline float rbx() { return pos_.x + width_ * 0.5f; }
		inline float rby() { return pos_.y + height_ * 0.5f; }
	};

}
