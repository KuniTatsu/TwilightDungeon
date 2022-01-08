#pragma once
#include <string>
#include "transform.h"

namespace t2k {

	class Font final {
	public:
		Font() {}
		~Font() {}
		int size_ = 20;
		int color_ = 0xFFFFFFFF;
		std::string str_;
		Transform transform_;

		void render();

		static void setFontSize(int size);
		static void drawStringEx(int x, int y, int color, char *_str, ...);
	};

}
