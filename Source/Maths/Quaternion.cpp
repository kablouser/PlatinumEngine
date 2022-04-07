#include <Maths/Quaternion.h>


namespace PlatinumEngine
{
	namespace Maths
	{
		Quaternion::Quaternion(): x(0.f), y(0.f), z(0.f), w(0.f)
		{
		}
		Quaternion::Quaternion(float w, float x, float y, float z): x(x), y(y), z(z), w(w)
		{
		}
		Quaternion::Quaternion(float roll, float pitch, float yaw)
		{
			Quaternion q = EulerToQuaternion(Vec3(roll,pitch,yaw)*_DEG2RAD);
			x = q.x;
			y = q.y;
			z = q.z;
			w = q.w;
		}
		Quaternion::Quaternion(Vec3 euler)
		{
			Quaternion q = EulerToQuaternion(euler*_DEG2RAD);
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
			return QuaternionToEuler(Quaternion(w,x,y,z))*(180.f / _PI);
		}
		Vec4 Quaternion::ToVec4()
		{
			return Vec4(x,y,z,w);
		}
		std::string Quaternion::ToString()
		{
			return "W: "+std::to_string(w)+", X: "+std::to_string(x)+", Y: "+std::to_string(y)+", Z: "+std::to_string(z);
		}

		// Static Methods

		//Quaternion related functions

		//Sets the quaternion to be of unit length
		Quaternion Quaternion::Normalise(Quaternion &q)
		{
			float n = q.Length();
			float invN = 1.f/n;
			Quaternion r = q * invN;
			return r;
		}
		//Returns the conjugate of quaternion
		Quaternion Quaternion::Conjugate(Quaternion &q)
		{
			Quaternion r;
			r.x = q.x * -1.f;
			r.y = q.y * -1.f;
			r.z = q.z * -1.f;
			r.w = q.w;
			return r;
		}
		//Returns the inverse of quaternion
		Quaternion Quaternion::Inverse(Quaternion &q)
		{
			float n=q.Length();
			n=1.f/(n*n);
			Quaternion r = Conjugate(q) * n;
			return r;
		}
		//Returns the angle (radians) between 2 quaternions
		float Quaternion::Angle(Quaternion a, Quaternion b)
		{
			return 2 * acos(abs(glm::clamp(a.Dot(b),- 1.f,1.f)));
		}
		//Linear interpolation of 2 quaternions
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
		//Spherical interpolation of 2 quaternions
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

		//Functions related to Quaternion creation and conversion

		//Creates a quaternion with the given axis and angle values
		Quaternion Quaternion::AngleAxis(Vec3 axis, float angle)
		{
			float halfAngle = angle * .5f;
			float s = sin(halfAngle);
			Quaternion q = Quaternion(cos(halfAngle), axis.x * s, axis.y * s, axis.z * s);
			return q;
		}
		//Returns a Quaternion from Vec3 euler angles
		Vec3 Quaternion::QuaternionToEuler(Quaternion q)
		{
			Mat4 m = QuaternionToMatrix(q);
			Vec3 v = MatrixToEuler (m);
			return v;
		}
		//Returns a Vec3 (XYZ ORDER) from a Quaternion
		Quaternion Quaternion::EulerToQuaternion(Vec3 euler)
		{
			float roll = euler.x/2.f, pitch = euler.y/2.f, yaw = euler.z/2.f;
			float sinroll = sin(roll), sinpitch = sin(pitch), sinyaw = sin(yaw);
			float cosroll = cos(roll), cospitch = cos(pitch), cosyaw = cos(yaw);
			float qx = sinroll * cospitch * cosyaw + cosroll * sinpitch * sinyaw;
			float qy = cosroll * sinpitch * cosyaw - sinroll * cospitch * sinyaw;
			float qz = cosroll * cospitch * sinyaw + sinroll * sinpitch * cosyaw;
			float qw = cosroll * cospitch * cosyaw - sinroll * sinpitch * sinyaw;
			return Quaternion(qw, qx, qy, qz);
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
			m[0][0] = 1.f - (yy + zz);
			m[1][0] = xy + wz;
			m[2][0] = xz - wy;
			m[3][0] = 0.f;
			m[0][1] = xy - wz;
			m[1][1] = 1.f - (xx + zz);
			m[2][1] = yz + wx;
			m[3][1] = 0.f;
			m[0][2] = xz + wy;
			m[1][2] = yz - wx;
			m[2][2] = 1.f - (xx + yy);
			m[3][2] = 0.f;
			m[0][3] = 0.f;
			m[1][3] = 0.f;
			m[2][3] = 0.f;
			m[3][3] = 1.f;
			return m;
		}
		//Returns a Quaternion from a rotation matrix
		Quaternion Quaternion::MatrixToQuaternion(Mat4 matrix)
		{
			Quaternion q;
			float tr = matrix[0][0] + matrix[1][1] + matrix[2][2];
			if (tr > 0) {
				float S = sqrt(tr+1.0) * 2; // S=4*qw
				q.w = 0.25 * S;
				q.x = (matrix[2][1] - matrix[1][2]) / S;
				q.y = (matrix[0][2] - matrix[2][0]) / S;
				q.z = (matrix[1][0] - matrix[0][1]) / S;
			}
			else if ((matrix[0][0] > matrix[1][1])&(matrix[0][0] > matrix[2][2]))
			{
				float S = sqrt(1.0 + matrix[0][0] - matrix[1][1] - matrix[2][2]) * 2; // S=4*qx
				q.w = (matrix[2][1] - matrix[1][2]) / S;
				q.x = 0.25 * S;
				q.y = (matrix[0][1] + matrix[1][0]) / S;
				q.z = (matrix[0][2] + matrix[2][0]) / S;
			}
			else if (matrix[1][1] > matrix[2][2])
			{
				float S = sqrt(1.0 + matrix[1][1] - matrix[0][0] - matrix[2][2]) * 2; // S=4*qy
				q.w = (matrix[0][2] - matrix[2][0]) / S;
				q.x = (matrix[0][1] + matrix[1][0]) / S;
				q.y = 0.25 * S;
				q.z = (matrix[1][2] + matrix[2][1]) / S;
			}
			else
			{
				float S = sqrt(1.0 + matrix[2][2] - matrix[0][0] - matrix[1][1]) * 2; // S=4*qz
				q.w = (matrix[1][0] - matrix[0][1]) / S;
				q.x = (matrix[0][2] + matrix[2][0]) / S;
				q.y = (matrix[1][2] + matrix[2][1]) / S;
				q.z = 0.25 * S;
			}
			return Normalise(q);
		}
		//Returns a Vec3 (XYZ ORDER) from a rotation matrix
		Vec3 Quaternion::MatrixToEuler(Mat4 matrix)
		{
			Vec3 v;
			if ( matrix[0][2] < 0.999F )
			{
				if ( matrix[0][2] > -0.999F )
				{
					v.y = asin(matrix[0][2]);
					v.x = atan2(-matrix[1][2], matrix[2][2]);
					v.z = atan2(-matrix[0][1], matrix[0][0]);
					MakePositive(v);
				}
				else
				{
					v.y = -_PI * 0.5F;
					v.x = -atan2(matrix[1][0], matrix[1][1]);
					v.z = 0.0F;
					MakePositive(v);
				}
			}
			else
			{
				v.y = _PI * 0.5F;
				v.x = atan2(matrix[1][0],matrix[1][1]);
				v.z = 0.0F;
				MakePositive(v);
			}
			return v;
		}
		//Returns a rotation matrix from Vec3 euler angles
		Mat4 Quaternion::EulerToMatrix(Vec3 euler)
		{
			return QuaternionToMatrix(EulerToQuaternion(euler));
		}
		//Creates a Quaternion which rotates from fromDirection to toDirection
		Quaternion Quaternion::FromToRotation(Vec3 from, Vec3 to)
		{
			Mat4 m;
			m = FromToRotationMatrix(from, to);
			Quaternion q;
			q = MatrixToQuaternion(m);
			std::cout<<"ROT: "<<q.ToString()<<"\n";
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
			if(e>(1.f-_eps))     //"from" almost or equal to "to"-vector?
				m.SetIdentityMatrix(); //return identity
			else if(e<(-1.f+_eps)) // "from" almost or equal to negated "to"?
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
			return m;
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
		//Creates a Quaternion with the specified forward direction
		Quaternion Quaternion::LookRotation(Vec3 forward)
		{
			return LookRotation(forward, _up);
		}

		//Private Methods (Helper functions)

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
		//Makes any negative angles positive
		void Quaternion::MakePositive(Vec3& euler)
		{
			const float negativeFlip = -0.0001F;
			const float positiveFlip = (_PI * 2.0F) - 0.0001F;
			if (euler.x < negativeFlip)
				euler.x += 2.0f * _PI;
			else if (euler.x > positiveFlip)
				euler.x -= 2.0f * _PI;
			if (euler.y < negativeFlip)
				euler.y += 2.0f * _PI;
			else if (euler.y > positiveFlip)
				euler.y -= 2.0f * _PI;
			if (euler.z < negativeFlip)
				euler.z += 2.0f * _PI;
			else if (euler.z > positiveFlip)
				euler.z -= 2.0f * _PI;
		}

		//Public static Variables

		Quaternion Quaternion::identity = Quaternion(1.f,0.f,0.f,0.f);

		//Private static Variables

		Vec3 Quaternion::_forward = Vec3(0.f,0.f,1.f);
		Vec3 Quaternion::_up = Vec3(0.f,1.f,0.f);
		Vec3 Quaternion::_right = Vec3(1.f,0.f,0.f);
		float Quaternion::_eps = 0.0000001f;
		float Quaternion::_PI = 3.1415926535897932f;
		float Quaternion::_DEG2RAD = _PI / 180.f;
		float Quaternion::_RAD2DEG = 180.f / _PI;
	}

}