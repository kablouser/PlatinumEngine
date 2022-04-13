#include <Maths/Quaternion.h>
#include <Maths/Common.h>
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

namespace PlatinumEngine
{
	namespace Maths
	{
		Quaternion::Quaternion():w(0.f), x(0.f), y(0.f), z(0.f)
		{
		}
		Quaternion::Quaternion(float w, float x, float y, float z): w(w), x(x), y(y), z(z)
		{
		}
		Quaternion::Quaternion(float pitch, float yaw, float roll)
		{
			Quaternion q = EulerToQuaternion(Vec3(pitch,yaw,roll)*Common::DEG2RAD);
			x = q.x;
			y = q.y;
			z = q.z;
			w = q.w;
		}
		Quaternion::Quaternion(Vec3 euler)
		{
			Quaternion q = EulerToQuaternion(euler*Common::DEG2RAD);
			x = q.x;
			y = q.y;
			z = q.z;
			w = q.w;
		}
		Quaternion::~Quaternion()
		{}

		// Operator Overloading

		void Quaternion::operator *=(const Quaternion &q)
		{
			float rw = w*q.w - x*q.x - y*q.y - z*q.z;
			float rx = w*q.x + x*q.w + y*q.z - z*q.y;
			float ry = w*q.y + y*q.w + z*q.x - x*q.z;
			float rz = w*q.z + z*q.w + x*q.y - y*q.x;
			w = rw; x = rx; y = ry; z = rz;
		}
		void Quaternion::operator *=(const float value)
		{
			w = w*value;
			x = x*value;
			y = y*value;
			z = z*value;
		}
		void Quaternion::operator *=(const int value)
		{
			w = w*value;
			x = x*value;
			y = y*value;
			z = z*value;
		}
		void Quaternion::operator +=(const Quaternion &q)
		{
			w += q.w;
			x += q.x;
			y += q.y;
			z += q.z;
		}
		void Quaternion::operator -=(const Quaternion &q)
		{
			w -= q.w;
			x -= q.x;
			y -= q.y;
			z -= q.z;
		}
		Quaternion Quaternion::operator *(const Quaternion &q)
		{
			Quaternion r;
			r.w = w*q.w - x*q.x - y*q.y - z*q.z;
			r.x = w*q.x + x*q.w + y*q.z - z*q.y;
			r.y = w*q.y + y*q.w + z*q.x - x*q.z;
			r.z = w*q.z + z*q.w + x*q.y - y*q.x;
			return r;
		}
		Quaternion Quaternion::operator *(const float value)
		{
			Quaternion r;
			r.w = w*value;
			r.x = x*value;
			r.y = y*value;
			r.z = z*value;
			return r;
		}
		Quaternion Quaternion::operator *(const int value)
		{
			Quaternion r;
			r.w = w*value;
			r.x = x*value;
			r.y = y*value;
			r.z = z*value;
			return r;
		}
		Vec3 Quaternion::operator*(const Vec3 v)
		{
			float rx = x * 2.f;
			float ry = y * 2.f;
			float rz = z * 2.f;
			float xx = x * rx;
			float yy = y * ry;
			float zz = z * rz;
			float xy = x * ry;
			float xz = x * rz;
			float yz = y * rz;
			float wx = w * rx;
			float wy = w * ry;
			float wz = w * rz;
			Vec3 r;
			r.x = (1.f - (yy + zz)) * v.x + (xy - wz)          * v.y + (xz + wy)          * v.z;
			r.y = (xy + wz)         * v.x + (1.f - (xx + zz))  * v.y + (yz - wx)          * v.z;
			r.z = (xz - wy)         * v.x + (yz + wx)          * v.y + (1.f - (xx + yy))  * v.z;
			return r;
		}
		Quaternion Quaternion::operator +(const Quaternion &q)
		{
			Quaternion r;
			r.w = w + q.w;
			r.x = x + q.x;
			r.y = y + q.y;
			r.z = z + q.z;
			return r;
		}
		Quaternion Quaternion::operator -(const Quaternion &q)
		{
			Quaternion r;
			r.w = w - q.w;
			r.x = x - q.x;
			r.y = y - q.y;
			r.z = z - q.z;
			return r;
		}
		void Quaternion::operator =(const Quaternion &q)
		{
			w = q.w;
			x = q.x;
			y = q.y;
			z = q.z;
		}
		bool Quaternion::operator ==(const Quaternion &q)
		{
			if(w==q.w && x==q.x && y==q.y && z==q.z)
				return true;
			return false;
		}
		float& Quaternion::operator[](int i)
		{
			switch(i)
			{
				case 0: return w;
				case 1: return x;
				case 2: return y;
				case 3: return z;
				default:
					throw std::out_of_range("Invalid Index");
			}
		}

		// Public Methods

		void Quaternion::Set(float newW, float newX, float newY, float newZ)
		{
			w = newW;
			x = newX;
			y = newY;
			z = newZ;
		}
		float Quaternion::Length()
		{
			return sqrt(x*x + y*y + z*z + w*w);
		}
		float Quaternion::Dot(const Quaternion& q)
		{
			return (x*q.x + y*q.y + z*q.z + w*q.w);
		}
		Vec3 Quaternion::EulerAngles()
		{
			return QuaternionToEuler(Quaternion(w,x,y,z))*Common::RAD2DEG;
		}
		Vec4 Quaternion::ToVec4()
		{
			return Vec4(w,x,y,z);
		}

		// Static Methods

		//Quaternion related functions

		Quaternion Quaternion::Normalise(Quaternion &q)
		{
			float n = q.Length();
			float invN = 1.f/n;
			Quaternion r = q * invN;
			return r;
		}

		Quaternion Quaternion::Conjugate(Quaternion &q)
		{
			Quaternion r;
			r.x = q.x * -1.f;
			r.y = q.y * -1.f;
			r.z = q.z * -1.f;
			r.w = q.w;
			return r;
		}

		Quaternion Quaternion::Inverse(Quaternion &q)
		{
			float n=q.Length();
			n=1.f/(n*n);
			Quaternion r = Conjugate(q) * n;
			return r;
		}

		float Quaternion::Angle(Quaternion a, Quaternion b)
		{
			return 2 * acos(abs(glm::clamp(a.Dot(b),- 1.f,1.f)));
		}

		Quaternion Quaternion::Lerp(Quaternion a, Quaternion b, float t)
		{
			float invt = 1.f - t;
			Quaternion q;
			if (a.Dot(b) >= 0.f)
			{
				q.x = (invt * a.x) + (t * b.x);
				q.y = (invt * a.y) + (t * b.y);
				q.z = (invt * a.z) + (t * b.z);
				q.w = (invt * a.w) + (t * b.w);
			}
			else
			{
				q.x = (invt * a.x) - (t * b.x);
				q.y = (invt * a.y) - (t * b.y);
				q.z = (invt * a.z) - (t * b.z);
				q.w = (invt * a.w) - (t * b.w);
			}
			return Normalise(q);
		}

		Quaternion Quaternion::Slerp(Quaternion a, Quaternion b, float t)
		{
			if ( t == 0 ) return a;
			if ( t == 1 ) return b;

			Quaternion r;
			float cosHalfTheta = a.Dot(b);
			if ( cosHalfTheta < 0 )
			{
				r.w = - b.w;
				r.x = - b.x;
				r.y = - b.y;
				r.z = - b.z;
				cosHalfTheta = - cosHalfTheta;
			}
			else
			{
				r = b;
			}
			if ( cosHalfTheta >= 1.0 )
			{
				r = a;
				return r;
			}

			const float sqrSinHalfTheta = 1.f - cosHalfTheta * cosHalfTheta;
			if ( sqrSinHalfTheta <= Common::EPS)
			{
				const float s = 1.f - t;
				r.w = s * a.w + t * r.w;
				r.x = s * a.x + t * r.x;
				r.y = s * a.y + t * r.y;
				r.z = s * a.z + t * r.z;
				r = Normalise(r);
				return r;
			}

			const float sinHalfTheta = sqrt( sqrSinHalfTheta );
			const float halfTheta = atan2( sinHalfTheta, cosHalfTheta );
			const float ratioA = sin( ( 1.f - t ) * halfTheta ) / sinHalfTheta,
						ratioB = sin( t * halfTheta ) / sinHalfTheta;

			r.w = ( a.w * ratioA + r.w * ratioB );
			r.x = ( a.x * ratioA + r.x * ratioB );
			r.y = ( a.y * ratioA + r.y * ratioB );
			r.z = ( a.z * ratioA + r.z * ratioB );
			return r;
		}

		//Functions related to Quaternion creation and conversion

		Quaternion Quaternion::AngleAxis(Vec3 axis, float angle)
		{
			float halfAngle = angle * .5f;
			float s = sin(halfAngle);
			Quaternion q = Quaternion(cos(halfAngle), axis.x * s, axis.y * s, axis.z * s);
			return q;
		}

		Vec3 Quaternion::QuaternionToEuler(Quaternion q)
		{
			glm::vec3 angles = glm::eulerAngles(glm::quat(q.w,q.x,q.y,q.z));
			return Vec3(angles.x,angles.y, angles.z) * Common::RAD2DEG;
		}

		Quaternion Quaternion::EulerToQuaternion(Vec3 euler)
		{
			glm::quat q{euler * Common::DEG2RAD};
			return Quaternion(q.w,q.x,q.y,q.z);
		}

		Mat4 Quaternion::QuaternionToMatrix(Quaternion q)
		{
			glm::mat4 matrix = glm::mat4_cast(glm::quat(q.w,q.x,q.y,q.z));
			Mat4 m;
			m[0][0] = matrix[0][0]; m[0][1] = matrix[0][1]; m[0][2] = matrix[0][2]; m[0][3] = matrix[0][3];
			m[1][0] = matrix[1][0]; m[1][1] = matrix[1][1]; m[1][2] = matrix[1][2]; m[1][3] = matrix[1][3];
			m[2][0] = matrix[2][0]; m[2][1] = matrix[2][1]; m[2][2] = matrix[2][2]; m[2][3] = matrix[2][3];
			m[3][0] = matrix[3][0]; m[3][1] = matrix[3][1]; m[3][2] = matrix[3][2]; m[3][3] = matrix[3][3];
			return m;
		}

		Quaternion Quaternion::MatrixToQuaternion(Mat4 matrix)
		{
			glm::mat4 m;
			m[0][0] = matrix[0][0]; m[0][1] = matrix[0][1]; m[0][2] = matrix[0][2]; m[0][3] = matrix[0][3];
			m[1][0] = matrix[1][0]; m[1][1] = matrix[1][1]; m[1][2] = matrix[1][2]; m[1][3] = matrix[1][3];
			m[2][0] = matrix[2][0]; m[2][1] = matrix[2][1]; m[2][2] = matrix[2][2]; m[2][3] = matrix[2][3];
			m[3][0] = matrix[3][0]; m[3][1] = matrix[3][1]; m[3][2] = matrix[3][2]; m[3][3] = matrix[3][3];
			glm::quat q = glm::quat_cast(m);
			return Quaternion(q.w,q.x,q.y,q.z);
		}

		Vec3 Quaternion::MatrixToEuler(Mat4 matrix)
		{
			Quaternion q = MatrixToQuaternion(matrix);
			return QuaternionToEuler(q);
		}

		Mat4 Quaternion::EulerToMatrix(Vec3 euler)
		{
			return QuaternionToMatrix(EulerToQuaternion(euler));
		}

		Quaternion Quaternion::FromToRotation(Vec3 from, Vec3 to)
		{
			glm::quat q = glm::rotation(from,to);
			return Quaternion(q.w,q.x,q.y,q.z);
		}

		Quaternion Quaternion::LookRotation(Vec3 forward, Vec3 up)
		{
			Vec3 x = forward;
			Vec3 z = Cross(forward,up);
			Vec3 y = Cross(z,forward);
			Mat4 m;
			m[0][0] = x[0];	m[0][1] = x[1];	m[0][2] = x[2];	m[0][3] = 0.0f;
			m[1][0] = y[0];	m[1][1] = y[1];	m[1][2] = y[2];	m[1][3] = 0.0f;
			m[2][0] = z[0];	m[2][1] = z[1];	m[2][2] = z[2];	m[2][3] = 0.0f;
			m[3][0] = 0.0f;	m[3][1] = 0.0f;	m[3][2] = 0.0f;	m[3][3] = 1.0f;
			return MatrixToQuaternion(m);
		}

		//Output operator overloading

		std::ostream& operator <<(std::ostream& os, Quaternion& q)
		{
			os <<q.w<<", "<<q.x<<", "<<q.y<<", "<<q.z;
			return os;
		}

		//Public static Variables

		Quaternion Quaternion::identity = Quaternion(1.f,0.f,0.f,0.f);
	}

}