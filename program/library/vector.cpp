#include "vector.h"
#include "quaternion.h"

namespace t2k {

	Vector3& Vector3::operator = (DirectX::XMVECTOR& other) { 
		DirectX::XMStoreFloat3(this, other); 
		return *this; 
	}
	Vector3& Vector3::operator = (const float other) {
		this->x = other;
		this->y = other;
		this->z = other;
		return *this;
	}
	Vector3 Vector3::operator * (const float other) const {
		Vector3 v = *this;
		return DirectX::XMVectorMultiply(v, { other, other, other }); 
	}
	Vector3 Vector3::operator * (const Vector3& other) const {
		Vector3 v1 = *this;
		Vector3 v2 = other;
		return DirectX::XMVectorMultiply(v1, v2); 
	}
	Vector3& Vector3::operator *= (const float other) {
		*this = *this * other; 
		return *this; 
	}
	Vector3& Vector3::operator *= (const Vector3& other) {
		Vector3 v = other;
		*this = *this * v; 
		return *this; 
	}
	Vector3 Vector3::operator / (const float other) const {
		Vector3 v1 = *this;
		return DirectX::XMVectorDivide(v1, { other, other, other }); 
	}
	Vector3 Vector3::operator / (const Vector3& other) const { 
		Vector3 v1 = *this;
		Vector3 v2 = other;
		return Vector3(DirectX::XMVectorDivide(v1, v2)); 
	}
	Vector3& Vector3::operator /= (const float other) {
		*this = *this / other; 
		return *this; 
	}
	Vector3& Vector3::operator /= (const Vector3& other) {
		*this = *this / other; 
		return *this; 
	}
	Vector3 Vector3::operator + (const Vector3& other) const {
		Vector3 v1 = *this;
		Vector3 v2 = other;
		return DirectX::XMVectorAdd(v1, v2); 
	}
	Vector3& Vector3::operator += (const Vector3& other) { 
		*this = *this + other; 
		return *this; 
	}
	Vector3 Vector3::operator - (const Vector3& other) const {
		Vector3 v1 = *this;
		Vector3 v2 = other;
		return DirectX::XMVectorSubtract(v1, v2); 
	}
	Vector3& Vector3::operator -= (const Vector3& other) { 
		*this = *this - other; 
		return *this; 
	}

	Vector3 Vector3::normalize(const Vector3 &v) {
		return DirectX::XMVector3Normalize(Vector3(v));
	}

	Vector3 Vector3::transformCoord(Vector3 &v, const Quaternion &q) { 
		return DirectX::XMVector3Rotate(v, Quaternion(q)); 
	}

	Vector3 Vector3::alongPlane(const t2k::Vector3& v, const t2k::Vector3& pn) {
		t2k::Vector3 c = t2k::Vector3::cross(v, pn);
		t2k::Vector3 along = t2k::Vector3::cross(pn, c);
		return along.normalize();
	}

}

