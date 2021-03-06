#include <iostream>
#include <math.h>

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

		Quaternion() {
			a = 1;
			i = j = k = 0;
		}

		Quaternion(float a, float i, float j, float k) {
			this->a = a;
			this->i = i;
			this->j = j;
			this->k = k;
		};

		Quaternion(float rot_x, float rot_y, float rot_z)
		{
			float cos_z_2 = cosf(0.5*rot_z);
			float cos_y_2 = cosf(0.5*rot_y);
			float cos_x_2 = cosf(0.5*rot_x);

			float sin_z_2 = sinf(0.5*rot_z);
			float sin_y_2 = sinf(0.5*rot_y);
			float sin_x_2 = sinf(0.5*rot_x);

			a = cos_z_2*cos_y_2*cos_x_2 + sin_z_2*sin_y_2*sin_x_2;
			i = cos_z_2*cos_y_2*sin_x_2 - sin_z_2*sin_y_2*cos_x_2;
			j = cos_z_2*sin_y_2*cos_x_2 + sin_z_2*cos_y_2*sin_x_2;
			k = sin_z_2*cos_y_2*cos_x_2 - cos_z_2*sin_y_2*sin_x_2;
		}
	
		Quaternion(float angle, const float* axis) {
			float theta = angle * PI / 360;
			a = cos(theta);

			float sin_theta = sin(theta);
			i = axis[0] * sin_theta;
			j = axis[1] * sin_theta;
			k = axis[2] * sin_theta;
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
		float* toRotationMatrix() {
			float* R = new float[16]();
			
			R[ 0] = 1.0f  - 2*j*j  - 2*k*k;
			R[ 4] = 2*i*j + 2*a*k;
			R[ 8] = 2*i*k - 2*a*j;
			R[12] = 0;

			R[ 1] = 2*i*j - 2*a*k; 
			R[ 5] = 1.0f  - 2*i*i  - 2*k*k;
			R[ 9] = 2*j*k + 2*a*i; 
			R[13] = 0;
			
			R[ 2] = 2*i*k - 2*a*j;
			R[ 6] = 2*j*k - 2*a*i;
			R[10] = 1.0f  - 2*i*i  - 2*j*j;
			R[14] = 0;

			R[ 3] = 0;
			R[ 7] = 0;
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

	Quaternion Quaternion::operator *(const Quaternion & other) {		

		Quaternion retval(
			a * other.a - i * other.i - j * other.j - k * other.k,
			(a * other.i + i * other.a + j * other.k - k * other.j),
			(a * other.j - i * other.k + j * other.a + k * other.i),
			(a * other.k + i * other.j - j * other.i + k * other.a)
			);

		return retval;
	}

	Quaternion Quaternion::operator *(const float f) {
		Quaternion retval(a * f, i * f, j * f, k * f);
		return retval;
	}

	bool Quaternion::operator ==(const Quaternion & other)
	{
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
		return Quaternion(a, -i, -j, -k);	
	}

	void Quaternion::Rotate(float* vector) {
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