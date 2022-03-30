#include <Maths/Quaternion.h>
namespace PlatinumEngine
{
	namespace Maths
	{
		Quaternion::Quaternion(): x(0.f), y(0.f), z(0.f), w(1.f)
		{
		}

		Quaternion::Quaternion(float x, float y, float z, float w): x(x), y(y), z(z), w(w)
		{
		}

		Quaternion::Quaternion(float yaw, float pitch, float roll)
		{
			Quaternion q = EulerToQuat(Vec3(yaw,pitch,roll));
			x = q.x;
			y = q.y;
			z = q.z;
			w = q.w;
		}

		Quaternion::Quaternion(Vec3 euler)
		{
			Quaternion q = EulerToQuat(euler);
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
			w = w*q.w - x*q.x - y*q.y - z*q.z;
			x = w*q.x + x*q.w + y*q.z - z*q.y;
			y = w*q.y + y*q.w + z*q.x - x*q.z;
			z = w*q.z + z*q.w + x*q.y - y*q.x;
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

		// Public Methods

		void Quaternion::Set(float newX, float newY, float newZ, float newW)
		{
			x = newX;
			y = newY;
			z = newZ;
			w = newW;
		}

		float Quaternion::Norm()
		{
			return sqrt(x*x + y*y + z*z + w*w);
		}

		float Quaternion::Dot(const Quaternion& q)
		{
			return (x*q.x + y*q.y + z*q.z + w*q.w);
		}

		Vec3 Quaternion::EulerAngles()
		{
			return QuatToEuler(Quaternion(x,y,z,w));
		}

		Vec4 Quaternion::ToVec4()
		{
			return Vec4(x,y,z,w);
		}

		std::string Quaternion::ToString()
		{
			return "{X:"+std::to_string(x)+" Y:"+std::to_string(y)+" Z:"+std::to_string(z)+" W:"+std::to_string(w)+"}";
		}

		// Static Methods

		Quaternion Quaternion::Normalise(Quaternion &q)
		{
			float n = q.Norm();
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
			float n=q.Norm();
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
			float num = t;
			float num2 = 1.f - num;
			Quaternion q;
			float num5 = (((a.x * b.x) + (a.y * b.y)) + (a.z * b.z)) + (a.w * b.w);
			if (num5 >= 0.f)
			{
				q.x = (num2 * a.x) + (num * b.x);
				q.y = (num2 * a.y) + (num * b.y);
				q.z = (num2 * a.z) + (num * b.z);
				q.w = (num2 * a.w) + (num * b.w);
			}
			else
			{
				q.x = (num2 * a.x) - (num * b.x);
				q.y = (num2 * a.y) - (num * b.y);
				q.z = (num2 * a.z) - (num * b.z);
				q.w = (num2 * a.w) - (num * b.w);
			}
			float num4 = (((q.x * q.x) + (q.y * q.y)) + (q.z * q.z)) + (q.w * q.w);
			float num3 = 1.f / sqrt(num4);
			q.x *= num3;
			q.y *= num3;
			q.z *= num3;
			q.w *= num3;
			return q;
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
			if ( sqrSinHalfTheta <= std::numeric_limits<float>::epsilon())
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

		Vec3 Quaternion::QuatToEuler(Quaternion q)
		{
			float t0 = 2. * (q.w*q.x + q.y*q.z);
			float t1 = 1.f - 2.f * (q.x*q.x + q.y*q.y);
			float roll = atan2(t0, t1);
			float t2 = 2.f * (q.w*q.y - q.z*q.x);
			t2 = t2>1.f?1.f:t2;
			t2 = t2<-1.f?-1.f:t2;
			float pitch = asin(t2);
			float t3 = 2.f * (q.w*q.z + q.x*q.y);
			float t4 = 1.f - 2.f * (q.y*q.y + q.z*q.z);
			float yaw = atan2(t3, t4);
			Vec3 v(yaw, pitch, roll);
			return v;
		}

		Quaternion Quaternion::EulerToQuat(Vec3 euler)
		{
			float yaw = euler.x/2.f, pitch = euler.y/2.f, roll = euler.z/2.f;
			float sinroll = sin(roll), sinpitch = sin(pitch), sinyaw = sin(yaw);
			float cosroll = cos(roll), cospitch = cos(pitch), cosyaw = cos(yaw);
			float qx = sinroll * cospitch * cosyaw - cosroll * sinpitch * sinyaw;
			float qy = cosroll * sinpitch * cosyaw + sinroll * cospitch * sinyaw;
			float qz = cosroll * cospitch * sinyaw - sinroll * sinpitch * cosyaw;
			float qw = cosroll * cospitch * cosyaw + sinroll * sinpitch * sinyaw;
			return Quaternion(qx, qy, qz, qw);
		}

		Quaternion Quaternion::AngleAxis(Vec3 axis, float angle)
		{
			float halfAngle = angle * .5f;
			float s = sin(halfAngle);
			Quaternion q = Quaternion(axis.x * s, axis.y * s, axis.z * s, cos(halfAngle));
			return q;
		}

		Quaternion Quaternion::RotationMatrix(Mat4 matrix)
		{
			Quaternion q;
			float root, half;
			float scale = matrix[0][0] + matrix[1][1] + matrix[2][2];

			if (scale > 0.0f)
			{
				root = sqrt(scale + 1.0f);
				q.w = root * 0.5f;
				root = 0.5f / root;

				q.x = (matrix[1][2] - matrix[2][1]) * root;
				q.y = (matrix[2][0] - matrix[0][2]) * root;
				q.z = (matrix[0][1] - matrix[1][0]) * root;

				return q;
			}
			if ((matrix[0][0] >= matrix[1][1]) && (matrix[0][0] >= matrix[2][2]))
			{
				root = sqrt(1.0f + matrix[0][0] - matrix[1][1] - matrix[2][2]);
				half = 0.5f / root;

				q.x = 0.5f * root;
				q.y = (matrix[0][1] + matrix[1][0]) * half;
				q.z = (matrix[0][2] + matrix[2][0]) * half;
				q.w = (matrix[1][2] - matrix[2][1]) * half;

				return q;
			}
			if (matrix[1][1] > matrix[2][2])
			{
				root = sqrt(1.0f + matrix[1][1] - matrix[0][0] - matrix[2][2]);
				half = 0.5f / root;

				q.x = (matrix[1][0] + matrix[0][1]) * half;
				q.y = 0.5f * root;
				q.z = (matrix[2][1] + matrix[1][2]) * half;
				q.w = (matrix[2][0] - matrix[0][2]) * half;

				return q;
			}
			root = sqrt(1.0f + matrix[2][2] - matrix[0][0] - matrix[1][1]);
			half = 0.5f / root;

			q.x = (matrix[2][0] + matrix[0][2]) * half;
			q.y = (matrix[2][1] + matrix[1][2]) * half;
			q.z = 0.5f * root;
			q.w = (matrix[0][1] - matrix[1][0]) * half;

			return q;
		}

		Mat4 Quaternion::QuaternionToMatrix(Quaternion q)
		{
			Mat4 m;
			// Precalculate coordinate products
			float x = q.x * 2.0F;
			float y = q.y * 2.0F;
			float z = q.z * 2.0F;
			float xx = q.x * x;
			float yy = q.y * y;
			float zz = q.z * z;
			float xy = q.x * y;
			float xz = q.x * z;
			float yz = q.y * z;
			float wx = q.w * x;
			float wy = q.w * y;
			float wz = q.w * z;

			// Calculate 3x3 matrix from orthonormal basis
			m[0][0] = 1.0f - (yy + zz);
			m[0][1] = xy + wz;
			m[0][2] = xz - wy;
			m[0][3] = 0.0F;
			m[1][0] = xy - wz;
			m[1][1] = 1.0f - (xx + zz);
			m[1][2] = yz + wx;
			m[1][3] = 0.0F;
			m[2][0] = xz + wy;
			m[2][1] = yz - wx;
			m[2][2] = 1.0f - (xx + yy);
			m[2][3] = 0.0F;
			m[3][0] = 0.0F;
			m[3][1] = 0.0F;
			m[3][2] = 0.0F;
			m[3][3] = 1.0F;
			return m;
		}

		Quaternion Quaternion::identity = Quaternion();
	}
}