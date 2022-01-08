#pragma once
#include <DirectXMath.h>
#include "vector.h"

namespace t2k{

	class Matrix final : public DirectX::XMFLOAT4X4 {
	public:
		Matrix() : 
			XMFLOAT4X4(
				1,0,0,0,
				0,1,0,0,
				0,0,1,0,
				0,0,0,1)
		{}
		Matrix(DirectX::XMMATRIX& m) { DirectX::XMStoreFloat4x4(this, m); }
		Matrix(DirectX::XMFLOAT4X4& m) : DirectX::XMFLOAT4X4(m) {}


		//-----------------------------------------------------------------------------------------------------
		//
		// operator
		//
		inline operator DirectX::XMMATRIX() { return DirectX::XMLoadFloat4x4(&DirectX::XMFLOAT4X4(*this)); }
		Matrix& operator = (DirectX::XMMATRIX& other);
		Matrix operator * (const Matrix& other) const;
		Matrix& operator *= (const Matrix& other);

	
		//-----------------------------------------------------------------------------------------------------
		//
		// method
		//
		inline void inTranslation( const float x, const float y, const float z ) { *this = DirectX::XMMatrixTranslation( x, y, z ); }
		inline void inScaling( const float x, const float y, const float z ) { *this = DirectX::XMMatrixScaling(x, y, z); }
		inline void inRotationAxis( Vector3 &v, const float angle ) { *this = DirectX::XMMatrixRotationAxis(v, angle ); }
		inline void inInverse( Matrix& m ) { *this = DirectX::XMMatrixInverse(nullptr, m); }
		inline void inTranspose( Matrix& m ) { *this = DirectX::XMMatrixTranspose( m ); }
		inline Matrix inverse() const { return DirectX::XMMatrixInverse(nullptr, Matrix(*this)); }

		static inline Matrix createTranslation(const float x, const float y, const float z){ return DirectX::XMMatrixTranslation(x, y, z); }
		static inline Matrix createScaling(const float x, const float y, const float z) { return DirectX::XMMatrixScaling(x, y, z); }
		static inline Matrix createRotationAxis(Vector3& v, const float angle) { return DirectX::XMMatrixRotationAxis(v, angle); }
		static inline Matrix createInverse(Matrix& m) { return DirectX::XMMatrixInverse(nullptr, m);}
		static inline Matrix createTranspose(Matrix& m) { return  DirectX::XMMatrixTranspose(m); }

	};

}
