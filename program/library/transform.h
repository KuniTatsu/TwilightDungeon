#pragma once
#include "vector.h"
#include "matrix.h"
#include "quaternion.h"

namespace t2k {

	class Transform {
	public :
		Transform(){}
		virtual ~Transform(){}
		Vector3 pos_ = Vector3(0, 0, 0);
		Vector3 scl_ = Vector3(1, 1, 1);
		Quaternion rot_;

	};

}
