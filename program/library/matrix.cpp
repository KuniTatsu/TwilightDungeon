#include "matrix.h"

namespace t2k {

	Matrix& Matrix::operator = (DirectX::XMMATRIX& other) {
		DirectX::XMStoreFloat4x4(this, other);
		return *this;
	}

	Matrix Matrix::operator * (const Matrix& other) const {
		Matrix m1 = *this;
		Matrix m2 = other;
		return DirectX::XMMatrixMultiply(m1, m2);
	}

	Matrix& Matrix::operator *= (const Matrix& other) {
		Matrix m = other;
		*this = *this * m;
		return *this;
	}


}