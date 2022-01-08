#pragma once
#include "shape.h"

namespace t2k {
	class BoundingRect : public Rect {
	public:
		static constexpr int FLG_CORRECT_LEFT = 1 << 0;
		static constexpr int FLG_CORRECT_RIGHT = 1 << 1;
		static constexpr int FLG_CORRECT_UP = 1 << 2;
		static constexpr int FLG_CORRECT_DOWN = 1 << 3;
		int flg_prev_correct_dir_ = 0;
		t2k::Vector3 prev_pos_;
		BoundingRect(const t2k::Vector3& p, float width, float height) : Rect::Rect(p, width, height) {}
	};
}

