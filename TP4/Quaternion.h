/*
 * After multiples attempt to get it right, I will use another quaternion implementation
 * The one I will start with is : willperone's implementation available at https://github.com/MegaManSE/willperone
*/

#include <iostream>
#include <math.h>

using namespace std;

namespace quaternion {
	#define PI 3.14159265
	
	class Quaternion
	{
	private:

	public:
		float a;
		float i;
		float j;
		float k;

		// Should be called everytime values are updated
		void coerce()
		{
			float threshold = 0.0001f;
			
			if (a <= threshold && a >= -threshold) { this->a = 0; }
			if (i <= threshold && i >= -threshold) { this->i = 0; }
			if (j <= threshold && j >= -threshold) { this->j = 0; }
			if (k <= threshold && k >= -threshold) { this->k = 0; }
			
		}

		Quaternion() {
			a = 1;
			i = j = k = 0;
			coerce();
		}

		Quaternion(float a, float i, float j, float k) {
			this->a = a;
			this->i = i;
			this->j = j;
			this->k = k;		
			coerce();
		};

		
		Quaternion(float rot_x, float rot_y, float rot_z)
		{
			float axisX[] = {1,0,0};
			float axisY[] = {0,1,0};
			float axisZ[] = {0,0,1};

			Quaternion xrot = Quaternion(rot_x, axisX);
			Quaternion yrot = Quaternion(rot_y, axisY);
			Quaternion zrot = Quaternion(rot_z, axisZ);

			Quaternion rot = (xrot * yrot * zrot).Normalize();
			this->a = rot.a;
			this->i = rot.i;
			this->j = rot.j;
			this->k = rot.k;
			coerce();
		}
		
		/*
		Quaternion(float theta_x, float theta_y, float theta_z)
		{
			float cos_z_2 = cosf(0.5*theta_z);
			float cos_y_2 = cosf(0.5*theta_y);
			float cos_x_2 = cosf(0.5*theta_x);

			float sin_z_2 = sinf(0.5*theta_z);
			float sin_y_2 = sinf(0.5*theta_y);
			float sin_x_2 = sinf(0.5*theta_x);

			// and now compute quaternion
			a  = cos_z_2*cos_y_2*cos_x_2 + sin_z_2*sin_y_2*sin_x_2;
			i = cos_z_2*cos_y_2*sin_x_2 - sin_z_2*sin_y_2*cos_x_2;
			j = cos_z_2*sin_y_2*cos_x_2 + sin_z_2*cos_y_2*sin_x_2;
			k = sin_z_2*cos_y_2*cos_x_2 - cos_z_2*sin_y_2*sin_x_2;

			coerce();
		}
		*/
	
		Quaternion(float angle, const float* axis) {
			float theta = (angle/2) * PI/180.0f;
			a = cosf(theta);

			float sin_theta = sinf(theta);
			i = axis[0] * sin_theta;
			j = axis[1] * sin_theta;
			k = axis[2] * sin_theta;
			coerce();
		}
	
		Quaternion(const Quaternion & copy) {
			a = copy.a;
			i = copy.i;
			j = copy.j;
			k = copy.k;
		}

		~Quaternion(void) { 
		
		};
	
		float GetMagnitude() {
			float sqrA = a * a;
			float sqrB = i * i;
			float sqrC = j * j;
			float sqrD = k * k;

			return sqrt(sqrA + sqrB + sqrC + sqrD);
		}

		Quaternion operator +(const Quaternion & other);
		Quaternion operator -(const Quaternion & other);
		Quaternion operator *(const Quaternion & other);
		Quaternion operator *(const float f);
		bool operator ==(const Quaternion & other);
		bool operator !=(const Quaternion & other);
	
		void Rotate(float* vector);
		Quaternion Conjugate();
		Quaternion Normalize();
		
		float* toRotationMatrix() 
		{
			coerce();
			
			float a2 = a * a;
			float i2 = i * i;
			float j2 = j * j;
			float k2 = k * k;

			float* R = new float[16]();
			R[0] = a2 + i2 - j2 - k2;
			R[4] = 2*(i*j + a*k);
			R[8] = 2*(i*k - a*j);
			R[12] = 0;

			R[1] = 2*(i*j - a*k); 
			R[5] = a2 - i2 + j2 - k2;
			R[9] = 2*(a*i + j*k); 
			R[13] = 0;
			
			R[2] = 2*(a*j + i*k);
			R[6] = 2*(j*k - a*i);
			R[10] = a2 - i2 - j2 + k2;
			R[14] = 0;

			R[3] = 0;
			R[7] = 0;
			R[11] = 0;
			R[15] = 1;

			return R;
		};

		// output
		friend std::ostream & operator <<(std::ostream & out, const Quaternion & in) {
			out << "[";
			out << in.a << ", "
				<< in.i << ", "
				<< in.j << ", "
				<< in.k << "]";

			return out;
		}; 
	};

	Quaternion Quaternion::operator +(const Quaternion & other) {
		Quaternion retval(a + other.a, i + other.i, j + other.j, k + other.k);
		return retval;
	}

	Quaternion Quaternion::operator *(const Quaternion & q) {	
	
		// for easier computation
		float a1 = a;
		float b1 = i;
		float c1 = j;
		float d1 = k;

		float a2 = q.a;
		float b2 = q.i;
		float c2 = q.j;
		float d2 = q.k;
		
		Quaternion retval(
			a1*a2 - b1*b2 - c1*c2 - d1*d2,
			a1*b2 + b1*a2 + c1*d2 - d1*c2,
			a1*c2 - b1*d2 + c1*a1 + d1*b2,
			a1*d2 + b1*c2 - c1*b2 + d1*a2
			);

		return retval;
	}

	Quaternion Quaternion::operator *(const float f) {
		Quaternion retval(a * f, i * f, j * f, k * f);
		return retval;
	}

	bool Quaternion::operator ==(const Quaternion & other)
	{
		coerce();
		return (this->a == other.a) 
			&& (this->i == other.i)
			&& (this->j == other.j)
			&& (this->k == other.k);
	}
	bool Quaternion::operator !=(const Quaternion & other)
	{
		return !(*this == other);
	}

	Quaternion Quaternion::Normalize() {
		float norm = GetMagnitude();
		Quaternion retval(a/norm, i/norm, j/norm, k/norm);
		return retval;
	}

	// only for unit quaternion;
	Quaternion Quaternion::Conjugate() {
		coerce();
		return Quaternion(a, -i, -j, -k);	
	}

	void Quaternion::Rotate(float* vector) {
		coerce();
		// find the inverse
		float* v = vector;
		Quaternion inv = Conjugate();
		Quaternion quatVector = Quaternion(0, v[0], v[1], v[2]);
		Quaternion rotated = (*this) * quatVector;
		rotated = rotated * inv;

		v[0] = rotated.i;
		v[1] = rotated.j;
		v[2] = rotated.k;
	}
}