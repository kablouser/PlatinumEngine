#include <Maths/Quaternion.h>
namespace PlatinumEngine
{
	namespace Maths
	{
		Quaternion::Quaternion(): x(0.f), y(0.f), z(0.f), w(1.f)
		{
			identity.Set(0.f,0.f,0.f,1.f);
		}

		Quaternion::Quaternion(float x, float y, float z, float w): x(x), y(y), z(z), w(w)
		{
			identity.Set(0.f,0.f,0.f,1.f);
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
			float t0 = 2. * (w*x + y*z);
			float t1 = 1.f - 2.f * (x*x + y*y);
			float roll = atan2(t0, t1);
			float t2 = 2.f * (w*y - z*x);
			t2 = t2>1.f?1.f:t2;
			t2 = t2<-1.f?-1.f:t2;
			float pitch = asin(t2);
			float t3 = 2.f * (w*z + x*y);
			float t4 = 1.f - 2.f * (y*y + z*z);
			float yaw = atan2(t3, t4);
			Vec3 v(yaw, pitch, roll);
			return v;
		}

		// Static Methods

		static Quaternion Normalise(Quaternion &q)
		{
			float n = q.Norm();
			float invN = 1.f/n;
			Quaternion r = q * invN;
			return r;
		}

		static Quaternion Conjugate(Quaternion &q)
		{
			Quaternion r;
			r.x = q.x * -1.f;
			r.y = q.y * -1.f;
			r.z = q.z * -1.f;
			r.w = q.w;
			return r;
		}

		static Quaternion Inverse(Quaternion &q)
		{
			float n=q.Norm();
			n=1.f/(n*n);
			Quaternion r = Conjugate(q) * n;
			return r;
		}

		static float Angle(Quaternion a, Quaternion b)
		{
			return 2 * acos(abs(glm::clamp(a.Dot(b),- 1.f,1.f)));
		}

		static Quaternion Slerp(Quaternion a, Quaternion b, float t)
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

		static Vec3 QuatToEuler(Quaternion q)
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

		static Quaternion EulerToQuat(Vec3 euler)
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

		static Quaternion AngleAxis(Vec3 axis, float angle)
		{
			float halfAngle = angle * .5f;
			float s = sin(halfAngle);
			Quaternion q;
			q.x = axis.x * s;
			q.y = axis.y * s;
			q.z = axis.z * s;
			q.w = cos(halfAngle);
			return q;
		}
	}
}