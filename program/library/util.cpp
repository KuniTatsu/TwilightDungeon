#include <stdlib.h>
#include <vector>
#include "wrap.h"
#include "util.h"

namespace t2k {

	int spaceBit32(int n) {
		n = (n | (n << 8)) & 0x00ff00ff;
		n = (n | (n << 4)) & 0x0f0f0f0f;
		n = (n | (n << 2)) & 0x33333333;
		return (n | (n << 1)) & 0x55555555;
	}

	int countBit8(const uint8_t v) {
		uint8_t count = (v & 0x55) + ((v >> 1) & 0x55);
		count = (count & 0x33) + ((count >> 2) & 0x33);
		return (count & 0x0f) + ((count >> 4) & 0x0f);
	}

	int countBit16(const uint16_t v) {
		uint16_t count = (v & 0x5555) + ((v >> 1) & 0x5555);
		count = (count & 0x3333) + ((count >> 2) & 0x3333);
		count = (count & 0x0f0f) + ((count >> 4) & 0x0f0f);
		return (count & 0x00ff) + ((count >> 8) & 0x00ff);
	}

	int countBit32(const uint32_t v) {
		uint32_t count = (v & 0x55555555) + ((v >> 1) & 0x55555555);
		count = (count & 0x33333333) + ((count >> 2) & 0x33333333);
		count = (count & 0x0f0f0f0f) + ((count >> 4) & 0x0f0f0f0f);
		count = (count & 0x00ff00ff) + ((count >> 8) & 0x00ff00ff);
		return (count & 0x0000ffff) + ((count >> 16) & 0x0000ffff);
	}

	int countBit64(const uint64_t v) {
		uint64_t count = (v & 0x5555555555555555) + ((v >> 1) & 0x5555555555555555);
		count = (count & 0x3333333333333333) + ((count >> 2) & 0x3333333333333333);
		count = (count & 0x0f0f0f0f0f0f0f0f) + ((count >> 4) & 0x0f0f0f0f0f0f0f0f);
		count = (count & 0x00ff00ff00ff00ff) + ((count >> 8) & 0x00ff00ff00ff00ff);
		count = (count & 0x0000ffff0000ffff) + ((count >> 16) & 0x0000ffff0000ffff);
		return (int)((count & 0x00000000ffffffff) + ((count >> 32) & 0x00000000ffffffff));
	}

	bool mostBit8(const uint8_t value, int& out) {
		if (value == 0) return false;
		uint8_t v = value;
		v |= (v >> 1);
		v |= (v >> 2);
		v |= (v >> 4);
		out = countBit8(v) - 1;
		return true;
	}

	bool mostBit16(const uint16_t value, int& out) {
		if (value == 0) return false;
		uint16_t v = value;
		v |= (v >> 1);
		v |= (v >> 2);
		v |= (v >> 4);
		v |= (v >> 8);
		out = countBit16(v) - 1;
		return true;
	}

	bool mostBit32(const uint32_t value, int& out) {
		if (value == 0) return false;
		uint32_t v = value;
		v |= (v >> 1);
		v |= (v >> 2);
		v |= (v >> 4);
		v |= (v >> 8);
		v |= (v >> 16);
		out = countBit32(v) - 1;
		return true;
	}

	bool mostBit64(const uint64_t value, int& out) {
		if (value == 0) return false;
		uint64_t v = value;
		v |= (v >> 1);
		v |= (v >> 2);
		v |= (v >> 4);
		v |= (v >> 8);
		v |= (v >> 16);
		v |= (v >> 32);
		out = countBit64(v) - 1;
		return true;
	}

	bool leastBit8(const uint8_t value, int& out) {
		if (value == 0) return false;
		uint8_t v = value;
		v |= (v << 1);
		v |= (v << 2);
		v |= (v << 4);
		out = 8 - countBit8(v);
		return true;
	}

	bool leastBit16(const uint16_t value, int& out) {
		if (value == 0) return false;
		uint16_t v = value;
		v |= (v << 1);
		v |= (v << 2);
		v |= (v << 4);
		v |= (v << 8);
		out = 16 - countBit16(v);
		return true;
	}

	bool leastBit32(const uint32_t value, int& out) {
		if (value == 0) return false;
		uint32_t v = value;
		v |= (v << 1);
		v |= (v << 2);
		v |= (v << 4);
		v |= (v << 8);
		v |= (v << 16);
		out = 32 - countBit32(v);
		return true;
	}

	bool leastBit64(const uint64_t value, int& out) {
		if (value == 0) return false;
		uint64_t v = value;
		v |= (v << 1);
		v |= (v << 2);
		v |= (v << 4);
		v |= (v << 8);
		v |= (v << 16);
		v |= (v << 32);
		out = 64 - countBit64(v);
		return true;
	}


	std::vector<std::vector<std::string>> loadCsv(const std::string& file_path) {

		std::vector<std::vector<std::string>> ret;

		FILE* fp = nullptr;
		fopen_s(&fp, file_path.c_str(), "r");

		char buff[1024] = { 0 };
		while (fgets(buff, sizeof(buff), fp)) {
			std::string line = buff;
			std::vector<std::string> data;

			while (1) {
				size_t c = line.find(",");
				if (c == std::string::npos) {
					c = line.find("\n");
				}
				std::string s = line.substr(0, c);
				data.emplace_back(std::move(s));
				line = line.substr(c+1, line.length()-(c+1));
				if (line.empty() || line == "/n") break;
			}
			ret.emplace_back(std::move(data));
			memset(buff, 0, sizeof(buff));
		}

		fclose(fp);
		return std::move( ret );
	}


}

