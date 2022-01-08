#include "wrap.h"
#include "font.h"
#include "util.h"

namespace t2k {

	void Font::setFontSize(int size) {
		SetFontSize(size);
	}

	void Font::drawStringEx(int x, int y, int color, char *_str, ...) {

		char buff[255] = { 0 };
		va_list argptr;

		va_start(argptr, _str);
		vsprintf_s(buff, _str, argptr);
		va_end(argptr);

		DrawString(x, y, buff, color);

	}

	void Font::render() {
		SetFontSize(size_);
		drawStringEx((int)transform_.pos_.x, (int)transform_.pos_.y,
			color_, "%s", str_.c_str());
	}

}
