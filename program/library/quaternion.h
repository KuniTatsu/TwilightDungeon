#pragma once
#include <DirectXMath.h>
#include "matrix.h"
namespace t2k {
	class Vector3;
	class Matrix;
	class Quaternion final : public DirectX::XMFLOAT4 {
	public:
		Quaternion() : DirectX::XMFLOAT4(0, 0, 0, 1) {}
		Quaternion(DirectX::XMVECTOR& v) { DirectX::XMStoreFloat4(this, v); }

		//-----------------------------------------------------------------------------------------------------
		//
		// operator
		//
		inline operator DirectX::XMVECTOR() { return DirectX::XMLoadFloat4(this); }

		const Quaternion& operator = (DirectX::XMVECTOR& other) ;
		const Quaternion operator * (const Quaternion& other) const;
		const Quaternion& operator *= (const Quaternion& other) ;

		//-----------------------------------------------------------------------------------------------------
		//
		// method
		//
		Vector3 getEuler() const ;
		inline Matrix getMatrix() const { return DirectX::XMMatrixRotationQuaternion(Quaternion(*this)); }
		static Quaternion rotationAxis(Vector3& axis, const float rotate);
		static Quaternion subtract(Quaternion& q1, Quaternion& q2);

	};
} 
