#include "util.h"
#include "vector.h"
#include "quaternion.h"

namespace t2k {

	const Quaternion& Quaternion::operator = (DirectX::XMVECTOR& other) {
		DirectX::XMStoreFloat4(this, other); 
		return *this; 
	}

	const Quaternion Quaternion::operator * (const Quaternion& other) const {
		Quaternion q1 = *this;
		Quaternion q2 = other;
		return DirectX::XMQuaternionMultiply(q1, q2);
	}
	const Quaternion& Quaternion::operator *= (const Quaternion& other) {
		Quaternion q = other;
		*this = DirectX::XMQuaternionMultiply(*this, q);
		return *this;
	}


	Quaternion Quaternion::rotationAxis(Vector3& axis, const float rotate) { 
		return DirectX::XMQuaternionRotationAxis(axis, rotate); 
	}

	Quaternion Quaternion::subtract(Quaternion& q1, Quaternion& q2) {
		DirectX::XMVECTOR q0i = DirectX::XMQuaternionInverse(q1);
		DirectX::XMVECTOR qd = DirectX::XMQuaternionMultiply(q0i, q2);
		if (qd.m128_f32[3] < 0) {
			qd.m128_f32[0] *= -1.0f;
			qd.m128_f32[1] *= -1.0f;
			qd.m128_f32[2] *= -1.0f;
			qd.m128_f32[3] *= -1.0f;
		}
		return qd;
	}

	Vector3 Quaternion::getEuler() const {

		float x2 = x * x;
		float y2 = y * y;
		float z2 = z * z;

		float xy = x * y;
		float xz = x * z;
		float yz = y * z;
		float wx = w * x;
		float wy = w * y;
		float wz = w * z;

		// 1 - 2y^2 - 2z^2
		float m00 = 1.f - (2.f * y2) - (2.f * z2);

		// 2xy + 2wz
		float m01 = (2.f * xy) + (2.f * wz);

		// 2xy - 2wz
		float m10 = (2.f * xy) - (2.f * wz);

		// 1 - 2x^2 - 2z^2
		float m11 = 1.f - (2.f * x2) - (2.f * z2);

		// 2xz + 2wy
		float m20 = (2.f * xz) + (2.f * wy);

		// 2yz+2wx
		float m21 = (2.f * yz) - (2.f * wx);

		// 1 - 2x^2 - 2y^2
		float m22 = 1.f - (2.f * x2) - (2.f * y2);


		float tx, ty, tz;

		if ( fabsf(m21-1.0f) < FLT_EPSILON )
		{
			tx = PI / 2.f;
			ty = 0;
			tz = atan2f( m10, m00 );
		}
		else if ( fabsf(m21+1.0f) < FLT_EPSILON )
		{
			tx = -PI / 2.0f;
			ty = 0;
			tz = atan2f(m10, m00);
		}
		else
		{
			tx = asinf(-m21);
			ty = atan2f(m20, m22);
			tz = atan2f(m01, m11);
		}

		return Vector3(tx, ty, tz);

	}


}
