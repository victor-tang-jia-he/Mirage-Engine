#include "pch.h"
#include <matrix4.h>
#include <quaternion.h>
using namespace MirageEngine::Core::Math;

// Mat4
inline static bool operator==(const Matrix4& _m1, const glm::mat4& _m2)
{
    return std::abs(_m1(0, 0) - _m2[0][0]) < EPSILON && std::abs(_m1(1, 0) - _m2[0][1]) < EPSILON && std::abs(_m1(2, 0) - _m2[0][2]) < EPSILON && std::abs(_m1(3, 0) - _m2[0][3]) < EPSILON
        && std::abs(_m1(0, 1) - _m2[1][0]) < EPSILON && std::abs(_m1(1, 1) - _m2[1][1]) < EPSILON && std::abs(_m1(2, 1) - _m2[1][2]) < EPSILON && std::abs(_m1(3, 1) - _m2[1][3]) < EPSILON
        && std::abs(_m1(0, 2) - _m2[2][0]) < EPSILON && std::abs(_m1(1, 2) - _m2[2][1]) < EPSILON && std::abs(_m1(2, 2) - _m2[2][2]) < EPSILON && std::abs(_m1(3, 2) - _m2[2][3]) < EPSILON
        && std::abs(_m1(0, 3) - _m2[3][0]) < EPSILON && std::abs(_m1(1, 3) - _m2[3][1]) < EPSILON && std::abs(_m1(2, 3) - _m2[3][2]) < EPSILON && std::abs(_m1(3, 3) - _m2[3][3]) < EPSILON;
}

inline static bool operator==(const glm::mat4& _m1, const Matrix4& _m2) { return _m2 == _m1; }

// Mat3
inline static bool operator==(const Matrix3& _m1, const glm::mat3& _m2)
{
    return std::abs(_m1(0, 0) - _m2[0][0]) < EPSILON && std::abs(_m1(0, 1) - _m2[0][1]) < EPSILON && std::abs(_m1(0, 2) - _m2[0][2]) < EPSILON && std::abs(_m1(1, 0) - _m2[1][0]) < EPSILON
        && std::abs(_m1(1, 1) - _m2[1][1]) < EPSILON && std::abs(_m1(1, 2) - _m2[1][2]) < EPSILON && std::abs(_m1(2, 0) - _m2[2][0]) < EPSILON && std::abs(_m1(2, 1) - _m2[2][1]) < EPSILON
        && std::abs(_m1(2, 2) - _m2[2][2]) < EPSILON;
}

inline static bool operator==(const glm::mat3& _m1, const Matrix3& _m2) { return _m2 == _m1; }

void PrintGLMMat4(const glm::mat4& mat, const std::string& name = "Matrix")
{
    const float* data = glm::value_ptr(mat);

    std::cout << name << ":" << std::endl;
    std::cout << std::fixed << std::setprecision(4);

    for (int row = 0; row < 4; ++row)
    {
        std::cout << "[ ";
        for (int col = 0; col < 4; ++col)
        {
            // GLM est column-major, donc l'index est col * 4 + row
            std::cout << std::setw(10) << data[col * 4 + row];
            if (col < 3) std::cout << ", ";
        }
        std::cout << " ]" << std::endl;
    }
    std::cout << std::endl;
}

// -------------------------------------------------------------------------------- //
//									TEST MATRIX4									//
// -------------------------------------------------------------------------------- //

/// <summary>
///  Test Matrix4
/// </summary>

Matrix4   Mat4(1, 0, 2, 4, 0, 1, 3, 2, 2, -1, 0, 3, 4, -1, 3, 9);
glm::mat4 glmMat4(
    1, 0, 2, 4,   // colonne 0
    0, 1, -1, -1, // colonne 1
    2, 3, 0, 3,   // colonne 2
    4, 2, 3, 9);  // colonne 3

TEST(Matrix4Test, Identity)
{
    Matrix4   newMat    = Matrix4::Identity();
    glm::mat3 newGlmMat = glm::identity<glm::mat3>();
    EXPECT_TRUE(newMat == newGlmMat);
}

TEST(Matrix4Test, Equal)
{
    Matrix4 newMat(Mat4);
    EXPECT_TRUE(Mat4 == glmMat4);
    EXPECT_TRUE(Mat4 == newMat);
}

TEST(Matrix4Test, Diagonal)
{
    Vector3 newVec(Mat4.Diagonal());
    EXPECT_TRUE((newVec.X - glmMat4[0][0]) < EPSILON);
    EXPECT_TRUE((newVec.Y - glmMat4[1][1]) < EPSILON);
    EXPECT_TRUE((newVec.Z - glmMat4[2][2]) < EPSILON);
}

TEST(Matrix4Test, Oppose)
{
    Matrix4   newMat    = Mat4.Opposed();
    glm::mat4 newGlmMat = -glmMat4;
    EXPECT_TRUE(newMat == newGlmMat);
}

TEST(Matrix4Test, Transpose)
{
    Matrix4   newMat    = Mat4.Transpose();
    glm::mat4 newGlmMat = glm::transpose(glmMat4);
    EXPECT_TRUE(newMat == newGlmMat);
}

TEST(Matrix4Test, Addition)
{
    Matrix4   newMat    = Mat4.AddMatrix4(Mat4);
    glm::mat4 newGlmMat = glmMat4 + glmMat4;
    EXPECT_TRUE(newMat == newGlmMat);
}

TEST(Matrix4Test, ProductScalar)
{
    Matrix4   newMat    = Mat4.ProductScalar(10.f);
    glm::mat4 newGlmMat = glmMat4 * 10.f;
    EXPECT_TRUE(newMat == newGlmMat);
}

TEST(Matrix4Test, ProductMatrix4)
{
    Matrix4   newMat    = Mat4.ProductMatrix4(Mat4);
    glm::mat4 newGlmMat = glmMat4 * glmMat4;
    EXPECT_TRUE(newMat == newGlmMat);
}

TEST(Matrix4Test, ToArray)
{
    std::array<float, 16> array  = { 1, 0, 2, 4, 0, 1, 3, 2, 2, -1, 0, 3, 4, -1, 3, 9 };
    std::array<float, 16> newMat = Mat4.ToArray();
    EXPECT_TRUE(newMat == array);
}

TEST(Matrix4Test, Determinant)
{
    float newMat    = Mat4.Determinant();
    float newGlmMat = glm::determinant(glmMat4);
    EXPECT_TRUE((newMat - newGlmMat) < EPSILON);
}

TEST(Matrix4Test, Inverse)
{
    Matrix4   newMat    = Mat4.Inverse();
    glm::mat4 newGlmMat = glm::inverse(glmMat4);
    EXPECT_TRUE(newMat == newGlmMat);
}

TEST(Matrix4Test, AddOperator)
{
    Matrix4   amat = Mat4 + Mat4;
    glm::mat4 aglm = glmMat4 + glmMat4;

    EXPECT_TRUE(amat == aglm);
}

TEST(Matrix4Test, SubOperator)
{
    Matrix4   amat = Mat4 - Mat4;
    glm::mat4 aglm = glmMat4 - glmMat4;

    EXPECT_TRUE(amat == aglm);
}

TEST(Matrix4Test, ProductOperator)
{
    Matrix4   amat = Mat4 * Mat4;
    glm::mat4 aglm = glmMat4 * glmMat4;

    EXPECT_TRUE(amat == aglm);
}

TEST(Matrix4Test, DivOperator)
{
    Matrix4   amat = Mat4 / Mat4;
    glm::mat4 aglm = glmMat4 / glmMat4;

    EXPECT_TRUE(amat == aglm);
}

TEST(Matrix4Test, AddEqualOperator)
{
    Matrix4 amat = Mat4;
    amat += amat;
    glm::mat4 aglm = glmMat4 + glmMat4;

    EXPECT_TRUE(amat == aglm);
}

TEST(Matrix4Test, SubEqualOperator)
{
    Matrix4 amat = Mat4;
    amat -= amat;
    glm::mat4 aglm = glmMat4 - glmMat4;

    EXPECT_TRUE(amat == aglm);
}

TEST(Matrix4Test, DivEqualOperator)
{
    Matrix4 amat = Mat4;
    amat /= Mat4;
    glm::mat4 aglm = glmMat4 / glmMat4;
    amat.Print();
    EXPECT_TRUE(amat == aglm);
}

TEST(Matrix4Test, ProductEqualOperator)
{
    Matrix4 amat = Mat4;
    amat *= Mat4;
    glm::mat4 aglm = glmMat4 * glmMat4;
    PrintGLMMat4(aglm);
    amat.Print();
    EXPECT_TRUE(amat == aglm);
}

TEST(Matrix4Test, GetSubMatrix)
{
    Matrix3   sub = Mat4.GetSubMatrix(0, 0);
    glm::mat3 glmSub;
    for (int r = 1; r < 4; ++r)
        for (int c = 1; c < 4; ++c) glmSub[r - 1][c - 1] = glmMat4[r][c];

    EXPECT_TRUE(sub == glmSub);

    sub      = Mat4.GetSubMatrix(2, 1);
    int idxR = 0;
    for (int r = 0; r < 4; ++r)
    {
        if (r == 2) continue;
        int idxC = 0;
        for (int c = 0; c < 4; ++c)
        {
            if (c == 1) continue;
            glmSub[idxR][idxC] = glmMat4[r][c];
            idxC++;
        }
        idxR++;
    }
    EXPECT_TRUE(sub == glmSub);
}

TEST(Matrix4Test, GetSubMatrix3x3Determinant)
{
    float     det = Mat4.GetSubMatrix3x3Determinant(0, 0);
    glm::mat3 glmSub;
    for (int r = 1; r < 4; ++r)
        for (int c = 1; c < 4; ++c) glmSub[r - 1][c - 1] = glmMat4[r][c];
    float glmDet = glm::determinant(glmSub);
    EXPECT_TRUE(det == glmDet);

    det = Mat4.GetSubMatrix3x3Determinant(3, 3);
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c) glmSub[r][c] = glmMat4[r][c];
    glmDet = glm::determinant(glmSub);
    EXPECT_TRUE(det == glmDet);
}

TEST(Matrix4Test, CreateTransformMatrixEuler)
{
    Vector3 pos(10.0f, 20.0f, 30.0f);
    Vector3 rot(ToRad(45.0f), ToRad(30.0f), ToRad(15.0f));
    Vector3 scale(2.0f, 3.0f, 4.0f);

    Matrix4 mat = Matrix4::CreateTransformMatrix(pos, rot, scale);

    glm::mat4 glmMat = glm::translate(glm::mat4(1.0f), glm::vec3(10, 20, 30)) * glm::rotate(glm::mat4(1.0f), rot.X, glm::vec3(1, 0, 0)) * glm::rotate(glm::mat4(1.0f), rot.Y, glm::vec3(0, 1, 0))
                     * glm::rotate(glm::mat4(1.0f), rot.Z, glm::vec3(0, 0, 1)) * glm::scale(glm::mat4(1.0f), glm::vec3(2, 3, 4));

    PrintGLMMat4(glmMat);
    mat.Print();
    EXPECT_TRUE(mat == glmMat);
}

TEST(Matrix4Test, CreateTransformMatrixQuaternion)
{
    Vector3    pos(5.0f, -2.0f, 8.0f);
    Quaternion rot(ToRad(60.0f), ToRad(25.0f), ToRad(10.0f)); // Euler -> Quat
    Vector3    scale(1.5f, 1.5f, 1.5f);

    Matrix4 mat = Matrix4::CreateTransformMatrix(pos, rot, scale);

    glm::quat glmRot(glm::radians(glm::vec3(60.0f, 25.0f, 10.0f)));
    glm::mat4 glmMat = glm::translate(glm::mat4(1.0f), glm::vec3(5, -2, 8)) * glm::mat4_cast(glmRot) * glm::scale(glm::mat4(1.0f), glm::vec3(1.5f));

    EXPECT_TRUE(mat == glmMat);
}

TEST(Matrix4Test, CreateTranslationMatrix)
{
    Vector3   translate(10.0f, -5.0f, 3.5f);
    Matrix4   mat    = Matrix4::CreateTranslationMatrix(translate);
    glm::mat4 glmMat = glm::translate(glm::mat4(1.0f), glm::vec3(10, -5, 3.5f));
    EXPECT_TRUE(mat == glmMat);
}

TEST(Matrix4Test, CreateScaleMatrix)
{
    Vector3   scale(2.0f, 0.5f, 3.0f);
    Matrix4   mat    = Matrix4::CreateScaleMatrix(scale);
    glm::mat4 glmMat = glm::scale(glm::mat4(1.0f), glm::vec3(2, 0.5f, 3));
    EXPECT_TRUE(mat == glmMat);
}

TEST(Matrix4Test, CreateXRotationMatrix)
{
    float     angle  = glm::radians(45.0f);
    Matrix4   mat    = Matrix4::CreateXRotationMatrix(angle);
    glm::mat4 glmMat = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1, 0, 0));
    EXPECT_TRUE(mat == glmMat);
}

TEST(Matrix4Test, CreateYRotationMatrix)
{
    float     angle  = glm::radians(60.0f);
    Matrix4   mat    = Matrix4::CreateYRotationMatrix(angle);
    glm::mat4 glmMat = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0));
    EXPECT_TRUE(mat == glmMat);
}

TEST(Matrix4Test, CreateZRotationMatrix)
{
    float     angle  = glm::radians(30.0f);
    Matrix4   mat    = Matrix4::CreateZRotationMatrix(angle);
    glm::mat4 glmMat = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 0, 1));
    EXPECT_TRUE(mat == glmMat);
}

TEST(Matrix4Test, GetPerspectiveMatrix)
{
    unsigned int width = 1920, height = 1080;
    float        near = 0.1f, far = 1000.0f, fov = glm::radians(60.0f);

    Matrix4   mat    = Matrix4::GetPerspectiveMatrix(width, height, near, far, fov);
    glm::mat4 glmMat = glm::perspective(fov, (float)width / height, near, far);

    EXPECT_TRUE(mat == glmMat);
}

TEST(Matrix4Test, LookAt)
{
    Vector3 eye(5.0f, 3.0f, 10.0f);
    Vector3 center(0.0f, 0.0f, 0.0f);
    Vector3 up(0.0f, 1.0f, 0.0f);

    Matrix4   mat    = Matrix4::LookAt(eye, center, up);
    glm::mat4 glmMat = glm::lookAt(glm::vec3(5, 3, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    EXPECT_TRUE(mat == glmMat);
}

TEST(Matrix4Test, OperatorParenthesesConst)
{
    EXPECT_FLOAT_EQ(Mat4(0, 2), 2.0f);
    EXPECT_FLOAT_EQ(Mat4(2, 1), -1.0f);
    EXPECT_FLOAT_EQ(Mat4(3, 3), 9.0f);
}

TEST(Matrix4Test, OperatorParenthesesNonConst)
{
    Matrix4 mat = Mat4;
    mat(0, 2)   = 99.0f;
    EXPECT_FLOAT_EQ(mat(0, 2), 99.0f);
}
