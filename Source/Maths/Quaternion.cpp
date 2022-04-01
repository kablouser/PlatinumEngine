#include <Maths/Quaternion.h>
namespace PlatinumEngine
{
	namespace Maths
	{
		Quaternion::Quaternion(): x(0.f), y(0.f), z(0.f), w(0.f)
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
				case 0: return x;
				case 1: return y;
				case 2: return z;
				case 3: return w;
				default:
					throw std::exception();
			}
		}

		// Public Methods

		void Quaternion::Set(float newX, float newY, float newZ, float newW)
		{
			x = newX;
			y = newY;
			z = newZ;
			w = newW;
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
			return QuatToEuler(Quaternion(x,y,z,w));
		}
		Vec4 Quaternion::ToVec4()
		{
			return Vec4(x,y,z,w);
		}
		std::string Quaternion::ToString()
		{
			return "X:"+std::to_string(x)+" Y:"+std::to_string(y)+" Z:"+std::to_string(z)+" W:"+std::to_string(w);
		}

		// Static Methods

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

		//Returns a rotation matrix from a Quaternion
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
		//Returns a Quaternion from a rotation matrix
		Quaternion Quaternion::MatrixToQuaternion(Mat4 matrix)
		{
			Quaternion q;
			float fTrace = matrix[0][0] + matrix[1][1] + matrix[2][2];
			float fRoot;

			if ( fTrace > 0.0f )
			{
				// |w| > 1/2, may as well choose w > 1/2
				fRoot = sqrt (fTrace + 1.0f);  // 2w
				q.w = 0.5f*fRoot;
				fRoot = 0.5f/fRoot;  // 1/(4w)
				q.x = (matrix[2][1] - matrix[1][2])*fRoot;
				q.y = (matrix[0][2] - matrix[2][0])*fRoot;
				q.z = (matrix[1][0] - matrix[0][1])*fRoot;
			}
			else
			{
				// |w| <= 1/2
				int s_iNext[] = { 1, 2, 0 };
				int i = 0;
				if ( matrix[1][1] > matrix[0][0] )
					i = 1;
				if ( matrix[2][2] > matrix[i][i] )
					i = 2;
				int j = s_iNext[i];
				int k = s_iNext[j];

				fRoot = sqrt (matrix[i][i] - matrix[j][j] - matrix[k][k] + 1.0f);
				//float* apkQuat[3] = { &q.x, &q.y, &q.z };
				//Assert (fRoot >= Vector3.kEpsilon);
				q[i] = 0.5f*fRoot;
				fRoot = 0.5f / fRoot;
				q.w = (matrix[k][j] - matrix[j][k]) * fRoot;
				q[j] = (matrix[j][i] + matrix[i][j])*fRoot;
				q[k] = (matrix[k][i] + matrix[i][k])*fRoot;
			}
			q = Normalise(q);
		}
		//Creates a Quaternion which rotates from fromDirection to toDirection
		Quaternion Quaternion::FromToRotation(Vec3 from, Vec3 to)
		{
			Mat4 m;
			m = FromToRotationMatrix(from, to);
			Quaternion q;
			q = MatrixToQuaternion(m);
			return q;
		}
		//Creates a Matrix which rotates from fromDirection to toDirection
		Mat4 Quaternion::FromToRotationMatrix(Vec3 from, Vec3 to)
		{
			Mat4 m;
			Vec3 v;
			float e,h;
			v = Maths::Cross(from, to);
			e = Maths::Dot(from, to);
			if(e>1.f-_eps)     /* "from" almost or equal to "to"-vector? */
			{
				m.SetIdentityMatrix();
				/* return identity */
			}
			else if(e<-1.f+_eps) /* "from" almost or equal to negated "to"? */
			{
				Vec3 up, left;
				float invlen;
				float fxx,fyy,fzz,fxy,fxz,fyz;
				float uxx,uyy,uzz,uxy,uxz,uyz;
				float lxx,lyy,lzz,lxy,lxz,lyz;
				/* left=CROSS(from, (1,0,0)) */
				left[0]=0.f; left[1]=from[2]; left[2]=-from[1];
				if(Maths::Dot(left,left)<_eps) /* was left=CROSS(from,(1,0,0)) a good choice? */
				{
					/* here we now that left = CROSS(from, (1,0,0)) will be a good choice */
					left[0]=-from[2]; left[1]=0.0f; left[2]=from[0];
				}
				/* normalize "left" */
				invlen=1.f/ sqrt(Maths::Dot(left,left));
				left[0]*=invlen;
				left[1]*=invlen;
				left[2]*=invlen;
				up = Maths::Cross(left, from);
				/* now we have a coordinate system, i.e., a basis;    */
				/* M=(from, up, left), and we want to rotate to:      */
				/* N=(-from, up, -left). This is done with the matrix:*/
				/* N*M^T where M^T is the transpose of M              */
				fxx=-from[0]*from[0]; fyy=-from[1]*from[1]; fzz=-from[2]*from[2];
				fxy=-from[0]*from[1]; fxz=-from[0]*from[2]; fyz=-from[1]*from[2];

				uxx=up[0]*up[0]; uyy=up[1]*up[1]; uzz=up[2]*up[2];
				uxy=up[0]*up[1]; uxz=up[0]*up[2]; uyz=up[1]*up[2];

				lxx=-left[0]*left[0]; lyy=-left[1]*left[1]; lzz=-left[2]*left[2];
				lxy=-left[0]*left[1]; lxz=-left[0]*left[2]; lyz=-left[1]*left[2];
				/* symmetric matrix */
				m[0][0]=fxx+uxx+lxx; m[0][1]=fxy+uxy+lxy; m[0][2]=fxz+uxz+lxz;
				m[1][0]=m[0][1];   m[1][1]=fyy+uyy+lyy; m[1][2]=fyz+uyz+lyz;
				m[2][0]=m[0][2];   m[2][1]=m[1][2];   m[2][2]=fzz+uzz+lzz;
			}
			else  /* the most common case, unless "from"="to", or "from"=-"to" */
			{
				/* ...otherwise use this hand optimized version (9 mults less) */
				float hvx,hvz,hvxy,hvxz,hvyz;
				h=(1.f-e)/Maths::Dot(v,v);
				hvx=h*v[0];
				hvz=h*v[2];
				hvxy=hvx*v[1];
				hvxz=hvx*v[2];
				hvyz=hvz*v[1];
				m[0][0]=e+hvx*v[0]; m[0][1]=hvxy-v[2];     m[0][2]=hvxz+v[1];
				m[1][0]=hvxy+v[2];  m[1][1]=e+h*v[1]*v[1]; m[1][2]=hvyz-v[0];
				m[2][0]=hvxz-v[1];  m[2][1]=hvyz+v[0];     m[2][2]=e+hvz*v[2];
			}
			m[0][3] = 0.f;
			m[1][3] = 0.f;
			m[2][3] = 0.f;
			m[3][0] = 0.f;		m[3][1] = 0.f;		m[3][2] = 0.f;		m[3][3] = 1.f;
		}
		//Creates a Quaternion with the specified forward and up directions
		Quaternion Quaternion::LookRotation(Vec3 forward, Vec3 up)
		{
			Quaternion q;
			if (!LookRotationToQuaternion (forward, up, q))
			{
				float mag = Maths::Length(forward);
				if (mag > _eps)
				{
					Mat4 m;
					m= FromToRotationMatrix(_forward, forward / mag);
					q = MatrixToQuaternion (m);
				}
			}
			return q;
		}
		Quaternion Quaternion::LookRotation(Vec3 forward)
		{
			return LookRotation(forward, _up);
		}

		//Private Methods

		//Returns a Quaternion if we can calculate
		bool Quaternion::LookRotationToQuaternion(Vec3 viewVec, Vec3 upVec, Quaternion& q)
		{
			Mat4 m;
			if (!LookRotationToMatrix(viewVec, upVec, m))
				return false;
			q = MatrixToQuaternion(m);
			return true;
		}
		//Returns a rotation matrix if we can calculate else return the indentity
		bool Quaternion::LookRotationToMatrix(Vec3 viewVec, Vec3 upVec, Mat4& m)
		{
			Vec3 z = viewVec;
			// compute u0
			float mag = Maths::Length(z);
			if (mag < _eps)
			{
				m.SetIdentityMatrix();
				return false;
			}
			z /= mag;

			Vec3 x = Maths::Cross (upVec, z);
			mag = Maths::Length(x);
			if (mag < _eps)
			{
				m.SetIdentityMatrix();
				return false;
			}
			x /= mag;

			Vec3 y = Maths::Cross (z, x);
			if (!Approximately(LengthSquared(y), 1.0f))
				return false;

			m = SetBasis (x, y, z);
			return true;
		}
		//Sets the first 3 columns with the 3 vectors respectively
		Mat4 Quaternion::SetBasis(Vec3 inX, Vec3 inY, Vec3 inZ)
		{
			Mat4 m;
			m[0][0] = inX[0];	m[0][1] = inY[0];	m[0][2] = inZ[0];	m[0][3] = 0.0f;
			m[1][0] = inX[1];	m[1][1] = inY[1];	m[1][2] = inZ[1];	m[1][3] = 0.0f;
			m[2][0] = inX[2];	m[2][1] = inY[2];	m[2][2] = inZ[2];	m[2][3] = 0.0f;
			m[3][0] = 0.0f;		m[3][1] = 0.0f;		m[3][2] = 0.0f;		m[3][3] = 1.0f;
			return m;
		}
		//Check how approximately close the values are
		bool Quaternion::Approximately(float a, float b)
		{
			return fabs(b - a) < fmax( 0.000001f * fmax(fabs(a), fabs(b)), _eps*8);
		}

		//Public static Variables

		Quaternion Quaternion::identity = Quaternion(0.f,0.f,0.f,1.f);

		//Private static Variables

		Vec3 Quaternion::_forward = Vec3(0.f,0.f,1.f);
		Vec3 Quaternion::_up = Vec3(0.f,1.f,0.f);
		Vec3 Quaternion::_right = Vec3(1.f,0.f,0.f);
		float Quaternion::_eps = 0.0000001f;
	}
}