#pragma once
#include <cmath>

typedef float (*func)(float x);
typedef float (*func2)(float x, float y);

class Matrix
{
public:
	Matrix(int r, int c);
	Matrix(int r, int c, float *d);
	Matrix(const Matrix& copy);
	~Matrix();

	Matrix& Randomize(float min, float max);
	static Matrix* MatrixMultiplication(const Matrix& left, const Matrix& right);
	static Matrix* MatrixMultiplication(Matrix* left, Matrix* right);
	Matrix& Transpose();
	static Matrix* Transpose(Matrix* matrix);
	Matrix* Activation(func f);
	Matrix* Activation(func2 f);
	static Matrix* Map(Matrix* matrix, func f);

	friend Matrix* operator+(float left, const Matrix& right);
	friend Matrix* operator+(const Matrix& left, float right);
	friend Matrix* operator+(const Matrix& left, const Matrix& right);
	Matrix* operator+=(float x);
	Matrix* operator+=(const Matrix& other);

	friend Matrix* operator-(float left, const Matrix& right);
	friend Matrix* operator-(const Matrix& left, float right);
	friend Matrix* operator-(const Matrix& left, const Matrix& right);
	Matrix* operator-=(float x);
	Matrix* operator-=(const Matrix& other);

	friend Matrix* operator*(float left, const Matrix& right);
	friend Matrix* operator*(const Matrix& left, float right);
	friend Matrix* operator*(const Matrix& left, const Matrix& right);
	Matrix* operator*=(float x);
	Matrix* operator*=(const Matrix& other);

	friend Matrix* operator/(float left, const Matrix& right);
	friend Matrix* operator/(const Matrix& left, float right);
	friend Matrix* operator/(const Matrix& left, const Matrix& right);
	Matrix* operator/=(float x);
	Matrix* operator/=(const Matrix& other);


	void Print() const;

public:
	inline int GetRows() const { return rows; }
	inline int GetColumns() const { return columns; }

	inline void SetRows(int r) { rows = r; }
	inline void SetColumns(int c) { columns = c; }
	inline void SetData(float* d) { data = d; }

private:
	int size;
	int rows;
	int columns;
public:
	float* data;
};
