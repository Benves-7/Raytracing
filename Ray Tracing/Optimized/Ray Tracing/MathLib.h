#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <string>
#include <math.h>

#pragma region Vector2D
class Vector2D
{
public:

	Vector2D();
	Vector2D(float x, float y);

	~Vector2D();

	void Set(float x, float y);
	void SetX(float x);
	void SetY(float y);
	inline float GetX() { return cord[0]; }
	inline float GetY() { return cord[1]; }
	inline void Print() const { printf("%0.1f : %0.1f\n", cord[0], cord[1]); }
	float Norm();

	float operator*(Vector2D rhs);
	void operator*(float rhs);
	Vector2D operator+(Vector2D& rhs);
	Vector2D operator-(Vector2D& rhs);
	void operator=(Vector2D& rhs);
	bool operator==(Vector2D& rhs);
	bool operator<(Vector2D& rhs);
	bool operator<=(Vector2D& rhs);
	bool operator>(Vector2D& rhs);
	bool operator>=(Vector2D& rhs);
	bool operator!=(Vector2D& rhs);
	float& operator[] (const int x);
	float operator[] (const int x) const;


private:

	float cord[2];

};
#pragma endregion

#pragma region Matrix2D
class Matrix2D
{
public:

	//Konstructorer
	Matrix2D();												// skapar en identitetsmatris
	Matrix2D(float x1, float y1, float x2, float y2);		// skapar en matris med fyra v�rden
	Matrix2D(Vector2D x, Vector2D y);							// skapar en matris med tv� vectorer (sparas p� samma s�tt..)
	Matrix2D(const Matrix2D& a);							// skapar en kopia av matrisen.
	Matrix2D(float degrees);

	~Matrix2D();

	//Funktioner
	void Set(float x1, float y1, float x2, float y2);		// s�tter v�rden med fyra v�rden
	void Set(Vector2D x, Vector2D y);							// s�tter v�rden med tv� vectorer
	void Set(float degrees);
	void Rot(float degrees);
	void Transponat();

	inline float GetX1() const { return matrix[0][0]; };	// return row 1 coloum 1
	inline float GetY1() const { return matrix[0][1]; };	// return row 1 coloum 2
	inline float GetX2() const { return matrix[1][0]; };	// return row 2 coloum 1
	inline float GetY2() const { return matrix[1][1]; };	// return row 2 coloum 2
	inline void Print() const { printf("\n Matrix = %0.1f : %0.1f \n          %0.1f : %0.1f\n", matrix[0][0], matrix[0][1], matrix[1][0], matrix[1][1]); }

	//Operatorer
	void operator=(Matrix2D& rhs);							//kolla (matrix = matrix) = kopia med sammam v�rden. 
	void operator*(float rhs);								//kolla (matrix * 4)
	Vector2D operator*(Vector2D rhs);							//kolla (matrix * vector)
	Matrix2D operator*(Matrix2D rhs);

private:

	float matrix[2][2];

};
#pragma endregion

#pragma region Vec3
class vec3
{
public:
	vec3() 
	{ 
		e[0] = 0;
		e[1] = 0;
		e[2] = 0;
	}
    inline vec3(float e0, float e1, float e2) {e[0] = e0; e[1] = e1; e[2] = e2;}
	inline void set(float x, float y, float z) {
		this->e[0] = x;
		this->e[1] = y;
		this->e[2] = z;
	}
    inline float x() const {return e[0];}
    inline float y() const {return e[1];}
    inline float z() const {return e[2];}
    inline float r() const {return e[0];}
    inline float g() const {return e[1];}
    inline float b() const {return e[2];}

    inline const vec3& operator+() const { return *this; }
    inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    inline float operator[](int i) const { return e[i]; }
    inline float& operator[](int i) { return e[i]; };

	inline vec3& operator+=(const vec3& v) {
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	};
	inline vec3& operator-=(const vec3& v) {
		e[0] -= v.e[0];
		e[1] -= v.e[1];
		e[2] -= v.e[2];
		return *this;
	};
	inline vec3& operator*=(const vec3& v) {
		e[0] *= v.e[0];
		e[1] *= v.e[1];
		e[2] *= v.e[2];
		return *this;
	};
	inline vec3& operator/=(const vec3& v) {
		e[0] /= v.e[0];
		e[1] /= v.e[1];
		e[2] /= v.e[2];
		return *this;
	};
	inline vec3& operator*=(const float t) {
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	};
	inline vec3& operator/=(const float t) {
		float k = 1.0f / t;
		e[0] *= k;
		e[1] *= k;
		e[2] *= k;
		return *this;
	};

    inline float length() const { 
		return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]); }
    inline float squared_length() const { 
		return e[0]*e[0] + e[1]*e[1] + e[2]*e[2]; }
	inline void make_unit_vector() {
		float k = 1.0f / length();
		e[0] *= k; e[1] *= k; e[2] *= k;
	}

    float e[3];
};

inline std::istream& operator>>(std::istream& is, const vec3& t) {
	//is >> t.e[0] >> t.e[1] >> t.e[2];
	//return is;
}
inline std::ostream& operator<<(std::ostream& os, const vec3& t) {
	os << t.e[0] << " " << t.e[1] << " " << t.e[2];
	return os;
}

inline vec3 operator+(const vec3& v1, const vec3& v2) {
	return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}
inline vec3 operator-(const vec3& v1, const vec3& v2) {
	return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}
inline vec3 operator*(const vec3& v1, const vec3& v2) {
	return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}
inline vec3 operator/(const vec3& v1, const vec3& v2) {
	return vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}
inline vec3 operator*(float t, const vec3& v) {
	return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}
inline vec3 operator*(const vec3& v, float t) {
	return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}
inline vec3 operator/(float t, const vec3& v) {
	return vec3(v.e[0] / t, v.e[1] / t, v.e[2] / t);
}
inline vec3 operator/(const vec3& v, float t) {
	return vec3(v.e[0] / t, v.e[1] / t, v.e[2] / t);
}
inline float dot(const vec3& v1, const vec3& v2) {
	return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}
inline vec3 cross(const vec3& v1, const vec3& v2) {
	return vec3( (v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1]),
			   (-(v1.e[0] * v2.e[2] - v1.e[2] * v2.e[0])),
				 (v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]));
}
inline vec3 unit_vector(vec3 v) {
	return v / v.length();
}


float randomFloat() {
	float temp = (float)rand() / RAND_MAX;
	return temp;
}
float randomFloatboth() {
	float temp1 = (float)rand() / RAND_MAX;
	float temp2 = (float)rand() / RAND_MAX;
	if (temp2 < 0.5)
	{
		temp1 = -temp1;
	}
	return temp1;
}

#pragma endregion

#pragma region Matrix3D
class Matrix3D
{
public:

	//Konstructorer
	Matrix3D();
	Matrix3D(float x1, float y1, float x2, float y2);
	Matrix3D(Vector2D x, Vector2D y);
	Matrix3D(const Matrix2D& a);
	Matrix3D(float degrees);

	~Matrix3D();

	//Funktioner
	void Set(float x1, float y1, float x2, float y2);		// s�tter v�rden med fyra v�rden
	void Set(Vector2D x, Vector2D y);							// s�tter v�rden med tv� vectorer
	void Set(float degrees);
	void SetSize(float sizeX, float sizeY);
	void Rot(float degrees);
	void Transponat();

	void SetPosition(Vector2D v);
	Vector2D GetPosition();
	void TranslationMatrix(Vector2D v);

	inline float GetX1()		const { return matrix[0][0]; };	// return row 1 coloum 1
	inline float GetY1()		const { return matrix[0][1]; };	// return row 1 coloum 2
	inline float GetZ1()		const { return matrix[0][2]; };	// return row 1 coloum 3
	inline float GetX2()		const { return matrix[1][0]; };	// return row 2 coloum 1
	inline float GetY2()		const { return matrix[1][1]; };	// return row 2 coloum 2
	inline float GetZ2()		const { return matrix[1][2]; };	// return row 2 coloum 3
	inline float GetX3()		const { return matrix[2][0]; };	// return row 3 coloum 1
	inline float GetY3()		const { return matrix[2][1]; };	// return row 3 coloum 2
	inline float GetZ3()		const { return matrix[2][2]; };	// return row 3 coloum 3
	inline float GetSizeX()		const { return matrix[2][0]; };
	inline float GetSizeY()		const { return matrix[2][1]; };
	inline float GetRadius()	const { return matrix[2][2]; };
	inline void Print()			const { printf("\n Matrix = %0.1f : %0.1f \n          %0.1f : %0.1f \n Position X = %0.1f \n          Y = %0.1f", matrix[0][0], matrix[0][1], matrix[1][0], matrix[1][1], matrix[0][2], matrix[1][2]); }
	//Operatorer
	Vector2D operator* (Vector2D& rhs);

private:

	float matrix[3][3];

};
#pragma endregion

#pragma region Vector4D
class Vector4D {
public:

    inline explicit Vector4D(float x = 0, float y = 0, float z = 0, float w = 1) { cord[0] = x, cord[1] = y, cord[2] = z, cord[3] = w; }
    inline Vector4D(Vector4D* v1, float f) { cord[0] = v1->x() / f; cord[1] = v1->y() / f; cord[2] = v1->z() / f; cord[3] = v1->w() / f; }

    inline ~Vector4D() {}

    inline void Set(float x, float y, float z, float w)
    {
        this->cord[0] = x; this->cord[1] = y; this->cord[2] = z; this->cord[3] = w;
    }

    void SetX(float x);

    void SetY(float y);

    void SetZ(float z);

    void SetW(float w);

    inline float GetX()  const { return cord[0]; }

    inline float GetY() const { return cord[1]; }

    inline float GetZ() const { return cord[2]; }

    inline float GetW() const { return cord[3]; }

    inline float *GetPointer() { return cord; }

    inline float r() const { return cord[0]; }

    inline float g() const { return cord[1]; }

    inline float b() const { return cord[2]; }

    inline float a() const { return cord[3]; }

    inline float x() const { return cord[0]; }

    inline float y() const { return cord[1]; }

    inline float z() const { return cord[2]; }

    inline float w() const { return cord[3]; }


    inline void Print() const { printf("%0.1f : %0.1f : %0.1f : %0.1f\n", cord[0], cord[1], cord[2], cord[3]); }

    inline Vector4D inverse() const
    {
        return Vector4D(-this->cord[0], -this->cord[1], -this->cord[2]);
    }
	inline Vector4D Normalize()
	{
	    if (this->cord[0] == 0 && this->cord[1] == 0 && this->cord[0] == 0)
            return Vector4D(0,0,0);
        float length = 1.0f / this->Length();
        return Vector4D((this->cord[0] * length), (this->cord[1] * length), (this->cord[2] * length), 1.0f);
    }
    inline void addTo(Vector4D v) {
        this->cord[0] += v.cord[0];
        this->cord[1] += v.cord[1];
        this->cord[2] += v.cord[2];
    }
    inline void divide(float f) {
        this->cord[0] = this->x() / f; this->cord[1] = this->y() / f; this->cord[2] = this->z() / f; this->cord[3] = this->w() / f;
    }
    inline static Vector4D unit_vector(Vector4D const& v)
    {
        if (v.x() == 0 && v.y() == 0 && v.z() == 0)
            return Vector4D(0,0,0);
        float length = 1.0f / v.Length();
        return Vector4D((v.x() * length), (v.y() * length), (v.z() * length), 1.0f);
    }
	float Length()  const
	{
	    return sqrt((this->cord[0] * this->cord[0]) + (this->cord[1] * this->cord[1]) + (this->cord[2] * this->cord[2]));
	}
	inline float squared_length() const {
        return sqrt(cord[0]*cord[0] + cord[1]*cord[1] + cord[2]*cord[2]);
	}
    inline static Vector4D cross(const Vector4D lhs, const Vector4D rhs)
    {
        Vector4D temp((lhs.cord[1] * rhs.cord[2] - lhs.cord[2] * rhs.cord[1]), (lhs.cord[2] * rhs.cord[0] - lhs.cord[0] * rhs.cord[2]), (lhs.cord[0] * rhs.cord[1] - lhs.cord[1] * rhs.cord[0]), 1.0f);
        return temp;
    }
	inline float Dot(const Vector4D& rhs)const
    {
        return ((this->cord[0] * rhs.cord[0]) + (this->cord[1] * rhs.cord[1]) + (this->cord[2] * rhs.cord[2]));
    }

	//float operator*(const Vector4D rhs);
	inline Vector4D operator*(float t) const
	{
	    return Vector4D(t*this->cord[0], t*this->cord[1], t*this->cord[2], this->cord[3] );
	}
	inline Vector4D operator*(Vector4D rhs) {
        return Vector4D(this->x() * rhs.x(),this->y() * rhs.y(),this->z() * rhs.z() );
	}
	inline Vector4D operator/(const float t) const
	{
        return Vector4D(this->cord[0]/t, this->cord[1]/t, this->cord[2]/t);
	}
	inline Vector4D operator/=(const float t) {
	    float temp = 1.0f/t;

	    this->cord[0] *= temp;
	    this->cord[1] *= temp;
	    this->cord[2] *= temp;
	}
	Vector4D operator&&(Vector4D rhs);	//Cross Multi  "&&"  may change

	inline Vector4D operator+(const Vector4D& rhs) const
	{
        Vector4D temp((this->cord[0] + rhs.cord[0]), (this->cord[1] + rhs.cord[1]), (this->cord[2] + rhs.cord[2]));
        return temp;
    }	//Addition
	inline Vector4D operator-(const Vector4D& rhs) const
	{
        Vector4D temp((this->cord[0] - rhs.cord[0]), (this->cord[1] - rhs.cord[1]), (this->cord[2] - rhs.cord[2]));
        return temp;
    }	//Subtraction
	inline Vector4D operator+=(const Vector4D& v){
	    this->cord[0] += v.cord[0];
        this->cord[1] += v.cord[1];
        this->cord[2] += v.cord[2];
	}
	void operator-=(Vector4D rhs);
	float& operator[] (const int x);
	float operator[] (const int x) const;

private:
	float cord[4];
};
#pragma endregion

#pragma region Matrix4D
class Matrix4D
{

public:

	/// Konstruktorer

	Matrix4D(float x1 = 1, float y1 = 0, float z1 = 0, float w1 = 0, float x2 = 0, float y2 = 1, float z2 = 0, float w2 = 0, float x3 = 0, float y3 = 0, float z3 = 1, float w3 = 0, float x4 = 0, float y4 = 0, float z4 = 0, float w4 = 1);
	Matrix4D(Vector2D a, Vector2D b, Vector2D c, Vector2D d, Vector2D e, Vector2D f, Vector2D g, Vector2D h);
	Matrix4D(const Matrix2D& a, const Matrix2D& b, const Matrix2D& c, const Matrix2D& d);
	Matrix4D(const Matrix3D& a);
	Matrix4D(Vector4D a, float degrees);
	Matrix4D(Vector4D R, Vector4D U, Vector4D D);
	Matrix4D(Vector4D P);

	~Matrix4D();

	/// Functions
	void Set(float x1 = 0, float y1 = 0, float z1 = 0, float w1 = 0, float x2 = 0, float y2 = 0, float z2 = 0, float w2 = 0, float x3 = 0, float y3 = 0, float z3 = 0, float w3 = 0, float x4 = 0, float y4 = 0, float z4 = 0, float w4 = 0);
	void Set(Vector2D a, Vector2D b, Vector2D c, Vector2D d, Vector2D e, Vector2D f, Vector2D g, Vector2D h);
	void Set(const Matrix2D& a, const Matrix2D& b, const Matrix2D& c, const Matrix2D& d);
	void SetI();

	inline void SetX1(float a) { matrix[0][0] = a; }; inline void SetY1(float a) { matrix[0][1] = a; }; inline void SetZ1(float a) { matrix[0][2] = a; }; inline void SetW1(float a) { matrix[0][3] = a; };
	inline void SetX2(float a) { matrix[1][0] = a; }; inline void SetY2(float a) { matrix[1][1] = a; }; inline void SetZ2(float a) { matrix[1][2] = a; }; inline void SetW2(float a) { matrix[1][3] = a; };
	inline void SetX3(float a) { matrix[2][0] = a; }; inline void SetY3(float a) { matrix[2][1] = a; }; inline void SetZ3(float a) { matrix[2][2] = a; }; inline void SetW3(float a) { matrix[2][3] = a; };
	inline void SetX4(float a) { matrix[3][0] = a; }; inline void SetY4(float a) { matrix[3][1] = a; }; inline void SetZ4(float a) { matrix[3][2] = a; }; inline void SetW4(float a) { matrix[3][3] = a; };

	void TransposeThis();	// Transposes the activ Matrix, does not create a new matrix.
	void InversThis();		// Inverses the activ Matrix, does not create a new matrix.
	void Print();			// Prints a easy readout.
	void GetArr();			// Converts into arr[16].
	float* GetPointer();
	static Matrix4D lookAt(Vector4D cameraPosition, Vector4D cameraDirection, Vector4D cameraUp);
	void MakeToMatrix();	// Used in InvertThis to make arr to matrix
	static Matrix4D GetPositionMatrix(Vector4D vec);

	static Matrix4D RotVec(Vector4D Vec, float degrees);			// Makes a rotation matrix around a vector.
	static Matrix4D RotX(float degrees);					// Makes a rotation matrix in the X-axis
	static Matrix4D RotY(float degrees);					// Makes a rotation matrix in the Y-axis
	static Matrix4D RotZ(float degrees);					// Makes a rotation matrix in the Z-axis

	Matrix4D operator/(float rhs);
	Matrix4D operator*(float rhs);
	Vector4D operator*(Vector4D rhs);
	Matrix4D operator*(Matrix4D rhs);
	void operator+=(Matrix4D rhs);

private:
	float matrix[4][4];
	float arr[16];
};
#pragma endregion