#include "Matrix.h"
#include "Can/Random.h"

#include <ostream>
#include <iostream>
Matrix::Matrix(int r, int c)
	: rows(r)
	, columns(c)
	, size(r* c)
{
	data = new float[r * c];
	for (int i = 0; i < size; i++)
		data[i] = 0.0f;
}
Matrix::Matrix(int r, int c, float* d)
	: rows(r)
	, columns(c)
	, size(r * c)
{
	data = new float[size];
	for (int i = 0; i < size; i++)
		data[i] = d[i];
}

Matrix::Matrix(const Matrix & copy)
	: rows(copy.rows)
	, columns(copy.columns)
	, size(copy.size)
{
	data = new float[size];
	for (int i = 0; i < size; i++)
		data[i] = copy.data[i];
}

Matrix::~Matrix()
{
	delete[] data;
}

Matrix& Matrix::Randomize(float min, float max)
{
	for (int i = 0; i < columns * rows; i++)
		data[i] = Can::Utility::Random::Float(min, max);

	return *this;
}

Matrix* Matrix::MatrixMultiplication(const Matrix & left, const Matrix & right)
{

	Matrix* tmp = new Matrix(left.GetRows(), right.GetColumns());
	int tc = tmp->GetColumns();
	int tr = tmp->GetRows();
	int rr = right.GetRows();
	for (int j = 0; j < tr; j++)
		for (int i = 0; i < tc; i++)
		{
			tmp->data[j * tc + i] = 0;
			for (int k = 0; k < rr; k++)
				tmp->data[j * tc + i] += left.data[j * rr + k] * right.data[k * tc + i];
		}
	return tmp;
}

Matrix* Matrix::MatrixMultiplication(Matrix * left, Matrix * right)
{

	Matrix* tmp = new Matrix(left->GetRows(), right->GetColumns());
	int tc = tmp->GetColumns();
	int tr = tmp->GetRows();
	int rr = right->GetRows();
	for (int j = 0; j < tr; j++)
		for (int i = 0; i < tc; i++)
		{
			tmp->data[j * tc + i] = 0;
			for (int k = 0; k < rr; k++)
				tmp->data[j * tc + i] += left->data[j * rr + k] * right->data[k * tc + i];
		}
	return tmp;
}
Matrix& Matrix::Transpose()
{
	int w = rows * columns;
	float* temp = new float[w];

	for (int j = 0; j < rows; j++)
		for (int i = 0; i < columns; i++)
			temp[i * rows + j] = data[j * columns + i];

	int c = columns;
	SetColumns(rows);
	SetRows(c);
	SetData(temp);

	return *this;
}
Matrix * Matrix::Transpose(Matrix * matrix)
{
	Matrix* result = new Matrix(matrix->GetColumns(), matrix->GetRows());

	for (int j = 0; j < matrix->rows; j++)
		for (int i = 0; i < matrix->columns; i++)
			result->data[i * matrix->rows + j] = matrix->data[j * matrix->columns + i];

	return result;
}

Matrix * operator+(float left, const Matrix & right)
{
	Matrix* result = new Matrix(right.GetRows(), right.GetColumns());

	for (int i = 0; i < result->size; i++)
		result->data[i] = left + right.data[i];

	return result;
}
Matrix* operator+(const Matrix & left, float right)
{
	Matrix* result = new Matrix(left.GetRows(), left.GetColumns());

	for (int i = 0; i < result->size; i++)
		result->data[i] = left.data[i] + right;

	return result;
}
Matrix* operator+(const Matrix & left, const Matrix & right)
{
	Matrix* result = new Matrix(left.GetRows(), left.GetColumns());

	for (int i = 0; i < result->size; i++)
		result->data[i] = left.data[i] + right.data[i];

	return result;
}

Matrix* Matrix::operator+=(float x)
{
	for (int i = 0; i < this->size; i++)
		this->data[i] += x;

	return this;
}
Matrix* Matrix::operator+=(const Matrix & other)
{
	for (int i = 0; i < this->size; i++)
		this->data[i] += other.data[i];

	return this;
}

Matrix* operator-(float left, const Matrix & right)
{
	Matrix* result = new Matrix(right.GetRows(), right.GetColumns());

	for (int i = 0; i < result->size; i++)
		result->data[i] = left - right.data[i];

	return result;
}
Matrix* operator-(const Matrix & left, float right)
{
	Matrix* result = new Matrix(left.GetRows(), left.GetColumns());

	for (int i = 0; i < result->size; i++)
		result->data[i] = left.data[i] - right;

	return result;
}
Matrix* operator-(const Matrix & left, const Matrix & right)
{
	Matrix* result = new Matrix(left.GetRows(), left.GetColumns());

	for (int i = 0; i < result->size; i++)
		result->data[i] = left.data[i] - right.data[i];

	return result;
}

Matrix* Matrix::operator-=(float x)
{
	for (int i = 0; i < this->size; i++)
		this->data[i] -= x;

	return this;
}
Matrix* Matrix::operator-=(const Matrix & other)
{
	for (int i = 0; i < this->size; i++)
		this->data[i] -= other.data[i];

	return this;
}

Matrix* operator*(float left, const Matrix & right)
{
	Matrix* result = new Matrix(right.GetRows(), right.GetColumns());

	for (int i = 0; i < result->size; i++)
		result->data[i] = left * right.data[i];

	return result;
}
Matrix* operator*(const Matrix & left, float right)
{
	Matrix* result = new Matrix(left.GetRows(), left.GetColumns());

	for (int i = 0; i < result->size; i++)
		result->data[i] = left.data[i] * right;

	return result;
}
Matrix* operator*(const Matrix & left, const Matrix & right)
{
	if (left.GetRows() != right.GetRows() || left.GetColumns() != right.GetColumns())
	{
		std::cout << "A*B matrix doesn't match" << std::endl;
		return nullptr;
	}
	Matrix* result = new Matrix(left.GetRows(), left.GetColumns());

	for (int i = 0; i < result->size; i++)
		result->data[i] = left.data[i] * right.data[i];

	return result;
}

Matrix* Matrix::operator*=(float x)
{
	for (int i = 0; i < this->size; i++)
		this->data[i] *= x;

	return this;
}
Matrix* Matrix::operator*=(const Matrix & other)
{
	if (this->GetRows() != other.GetRows() || this->GetColumns() != other.GetColumns())
	{
		std::cout << "*= matrix doesn't match" << std::endl;
		return nullptr;
	}
	for (int i = 0; i < this->size; i++)
		this->data[i] *= other.data[i];

	return this;
}

Matrix* operator/(float left, const Matrix & right)
{
	Matrix* result = new Matrix(right.GetRows(), right.GetColumns());

	for (int i = 0; i < result->size; i++)
		result->data[i] = left / right.data[i];

	return result;
}
Matrix* operator/(const Matrix & left, float right)
{
	Matrix* result = new Matrix(left.GetRows(), left.GetColumns());

	for (int i = 0; i < result->size; i++)
		result->data[i] = left.data[i] / right;

	return result;
}
Matrix* operator/(const Matrix & left, const Matrix & right)
{
	Matrix* result = new Matrix(left.GetRows(), left.GetColumns());

	for (int i = 0; i < result->size; i++)
		result->data[i] = left.data[i] / right.data[i];

	return result;
}

Matrix* Matrix::operator/=(float x)
{
	for (int i = 0; i < this->size; i++)
		this->data[i] /= x;

	return this;
}
Matrix* Matrix::operator/=(const Matrix & other)
{
	for (int i = 0; i < this->size; i++)
		this->data[i] /= other.data[i];

	return this;
}

Matrix* Matrix::Activation(func f)
{

	for (int i = 0; i < size; i++)
		data[i] = f(data[i]);

	return this;
}

Matrix* Matrix::Activation(func2 f)
{
	float sum = 0.0f;
	for (int i = 0; i < size; i++)
		sum += data[i];
	for (int i = 0; i < size; i++)
		data[i] = f(data[i], sum);

	return this;
}

Matrix * Matrix::Map(Matrix * matrix, func f)
{
	Matrix* result = new Matrix(matrix->GetRows(), matrix->GetColumns());


	for (int i = 0; i < matrix->size; i++)
		result->data[i] = f(matrix->data[i]);
	return result;
}

void Matrix::Print() const
{
	std::cout << "Matrix: " << this->GetRows() << " - " << this->GetColumns() << "\n";

	for (int j = 0; j < this->GetRows(); j++)
	{
		std::cout << data[j * this->GetColumns()];
		for (int i = 1; i < this->GetColumns(); i++)
		{
			std::cout << ", " << data[j * this->GetColumns() + i];
		}
		std::cout << std::endl;
	}
}



std::ostream& operator<<(std::ostream & stream, Matrix & matrix)
{
	int rows = matrix.GetRows();
	int cols = matrix.GetColumns();
	float* data = matrix.data;

	stream << "Matrix: " << rows << " - " << cols << "\n[ ";

	for (int j = 0; j < rows - 1; j++)
	{
		stream << data[j * cols];
		for (int i = 1; i < cols; i++)
		{
			stream << ", " << data[j * cols + i];
		}
		stream << std::endl;
	}
	stream << ", " << data[rows * cols - 1] << "]\n";


	return stream;
}