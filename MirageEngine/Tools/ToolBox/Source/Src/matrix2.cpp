#include "matrix2.h"
#include <iostream>
#include "vector2.h"
#include <assert.h>
using namespace MirageEngine::Core::Math;

Matrix2::Matrix2(const Matrix2& _copy) noexcept
{
    (*this)(0, 0) = _copy(0, 0);
    (*this)(1, 0) = _copy(1, 0);
    (*this)(0, 1) = _copy(0, 1);
    (*this)(1, 1) = _copy(1, 1);
}

Matrix2::Matrix2(float _m00, float _m01, float _m10, float _m11) noexcept
{
    (*this)(0, 0) = _m00;
    (*this)(0, 1) = _m01;
    (*this)(1, 0) = _m10;
    (*this)(1, 1) = _m11;
}

Matrix2 Matrix2::Identity() { return Matrix2(1, 0, 0, 1); }

Vector2 Matrix2::Diagonal() const { return Vector2((*this)(0, 0), (*this)(1, 1)); }

float Matrix2::Trace() const { return (*this)(0, 0) + (*this)(1, 1); }

Matrix2 Matrix2::Opposed() const { return Matrix2(-(*this)(0, 0), -(*this)(0, 1), -(*this)(1, 0), -(*this)(1, 1)); }

Matrix2 Matrix2::Transpose() const { return Matrix2((*this)(0, 0), (*this)(1, 0), (*this)(0, 1), (*this)(1, 1)); }

Matrix2 Matrix2::AddMatrix2(const Matrix2& _other) const { return Matrix2((*this)(0, 0) + _other(0, 0), (*this)(0, 1) + _other(0, 1), (*this)(1, 0) + _other(1, 0), (*this)(1, 1) + _other(1, 1)); }

Matrix2 Matrix2::ProductScalar(float _n) const { return Matrix2((*this)(0, 0) * _n, (*this)(0, 1) * _n, (*this)(1, 0) * _n, (*this)(1, 1) * _n); }

Matrix2 Matrix2::ProductMatrix2(const Matrix2& _other) const
{
    return Matrix2(
        (*this)(0, 0) * _other(0, 0) + (*this)(1, 0) * _other(0, 1), (*this)(0, 1) * _other(0, 0) + (*this)(1, 1) * _other(0, 1),

        (*this)(0, 0) * _other(1, 0) + (*this)(1, 0) * _other(1, 1), (*this)(0, 1) * _other(1, 0) + (*this)(1, 1) * _other(1, 1));
}

float Matrix2::Determinant() const { return (*this)(0, 0) * (*this)(1, 1) - (*this)(1, 0) * (*this)(0, 1); }

Matrix2 Matrix2::Inverse() const
{
    float det = Determinant();

    if (std::abs(det) < 1e-6f)
    {
        throw std::runtime_error("Matrix is not invertible (det == 0)");
    }

    return Matrix2((*this)(1, 1) / det, -(*this)(0, 1) / det, -(*this)(1, 0) / det, (*this)(0, 0) / det);
}

Matrix2 Matrix2::Rotate(float _angle) { return Matrix2(cos(_angle), sin(_angle), -sin(_angle), cos(_angle)); }

void Matrix2::Print() const { std::cout << (*this)(0, 0) << " " << (*this)(0, 1) << "\n" << (*this)(1, 0) << " " << (*this)(1, 1) << "\n"; }

Matrix2 Matrix2::operator*(const Matrix2& _other) const { return ProductMatrix2(_other); }

Matrix2 Matrix2::operator/(const Matrix2& _other) const { return ProductMatrix2(_other.Inverse()); }

Matrix2 Matrix2::operator+(const Matrix2& _other) const { return AddMatrix2(_other); }

Matrix2 Matrix2::operator-(const Matrix2& _other) const { return AddMatrix2(_other.Opposed()); }

Matrix2& Matrix2::operator*=(const Matrix2& _other)
{
    float m00 = (*this)(0, 0) * _other(0, 0) + (*this)(1, 0) * _other(0, 1);
    float m01 = (*this)(0, 1) * _other(0, 0) + (*this)(1, 1) * _other(0, 1);
    float m10 = (*this)(0, 0) * _other(1, 0) + (*this)(1, 0) * _other(1, 1);
    float m11 = (*this)(0, 1) * _other(1, 0) + (*this)(1, 1) * _other(1, 1);

    (*this)(0, 0) = m00;
    (*this)(0, 1) = m01;
    (*this)(1, 0) = m10;
    (*this)(1, 1) = m11;

    return *this;
}

Matrix2& Matrix2::operator/=(const Matrix2& _other)
{
    *this *= _other.Inverse();
    return *this;
}

Matrix2& Matrix2::operator+=(const Matrix2& _other)
{
    (*this)(0, 0) += _other(0, 0);
    (*this)(0, 1) += _other(0, 1);
    (*this)(1, 0) += _other(1, 0);
    (*this)(1, 1) += _other(1, 1);

    return *this;
}

Matrix2& Matrix2::operator-=(const Matrix2& _other)
{
    (*this)(0, 0) -= _other(0, 0);
    (*this)(0, 1) -= _other(0, 1);
    (*this)(1, 0) -= _other(1, 0);
    (*this)(1, 1) -= _other(1, 1);

    return *this;
}

Matrix2 Matrix2::operator*(float _n) const { return ProductScalar(_n); }

Matrix2& Matrix2::operator*=(float _n)
{
    (*this)(0, 0) = (*this)(0, 0) * _n;
    (*this)(0, 1) = (*this)(0, 1) * _n;
    (*this)(1, 0) = (*this)(1, 0) * _n;
    (*this)(1, 1) = (*this)(1, 1) * _n;
    return *this;
}

bool Matrix2::operator==(const Matrix2& _other) const
{
    return std::abs((*this)(0, 0) - _other(0, 0)) < 1e-6f && std::abs((*this)(0, 1) - _other(0, 1)) < 1e-6f && std::abs((*this)(1, 0) - _other(1, 0)) < 1e-6f
        && std::abs((*this)(1, 1) - _other(1, 1)) < 1e-6f;
}

const float& Matrix2::operator()(size_t _row, size_t _col) const
{
    assert(_row < 2 && _col < 2);
    return M[_row + _col * 2];
}

float& Matrix2::operator()(size_t _row, size_t _col)
{
    assert(_row < 2 && _col < 2);
    return M[_row + _col * 2];
}
