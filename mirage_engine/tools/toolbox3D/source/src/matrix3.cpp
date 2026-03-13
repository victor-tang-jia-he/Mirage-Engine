#include "matrix3.h"

using namespace Bwt::Core::Maths;
Matrix3 Matrix3::Inverse() const
{
    const float det = Determinant();
    if (std::abs(det) < 1e-6f)
    {
        throw std::runtime_error("Matrix is not invertible (det == 0)");
    }

    Matrix3 inverse;
    const float invDet = 1.0f / det;

    for (int col = 0; col < 3; ++col)
    {
        for (int row = 0; row < 3; ++row)
        {
            const float cofactor =
                ((row + col) % 2 == 0 ? 1.f : -1.f) *
                GetSubMatrix(row, col).Determinant();

            inverse.m[row][col] = cofactor * invDet;
        }
    }

    return inverse;
}
Matrix2 Matrix3::GetSubMatrix(int excludeRow, int excludeCol) const
{
    Matrix2 sub;

    int subCol = 0;
    for (int col = 0; col < 3; ++col)
    {
        if (col == excludeCol) continue;

        int subRow = 0;
        for (int row = 0; row < 3; ++row)
        {
            if (row == excludeRow) continue;

            sub.m[subRow][subCol] = m[row][col];
            ++subRow;
        }
        ++subCol;
    }

    return sub;
}
Matrix3 Matrix3::luDecomposition() const
{
    Matrix3 lower = Matrix3::Identity();
    Matrix3 upper = *this;

    // colonne 0
    lower.m[1][0] = upper.m[1][0] / upper.m[0][0];
    lower.m[2][0] = upper.m[2][0] / upper.m[0][0];

    for (int j = 0; j < 3; ++j)
    {
        upper.m[1][j] -= lower.m[1][0] * upper.m[0][j];
        upper.m[2][j] -= lower.m[2][0] * upper.m[0][j];
    }

    // colonne 1
    lower.m[2][1] = upper.m[2][1] / upper.m[1][1];

    for (int j = 1; j < 3; ++j)
    {
        upper.m[2][j] -= lower.m[2][1] * upper.m[1][j];
    }

    return lower * upper;
}
Matrix3 Matrix3::RotateMatrixVector(const Vector3& vec, float rad)
{
	float length = vec.GetNorm();
	float x = vec.x / length;
	float y = vec.y / length;
	float z = vec.z / length;

	float c = std::cos(rad);
	float s = std::sin(rad);
	float oneMinusC = 1 - c;

	// Rodrigues formula
	return Matrix3(
		c + x * x * oneMinusC, y * x * oneMinusC + z * s, z * x * oneMinusC - y * s,
        x * y * oneMinusC - z * s, c + y * y * oneMinusC, z * y * oneMinusC + x * s,
        x * z * oneMinusC + y * s, y * z * oneMinusC - x * s, c + z * z * oneMinusC);
}