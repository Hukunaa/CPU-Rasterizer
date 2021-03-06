#pragma once
#include <GPM/Quaternion/Quaternion.h>

// struct GPM::Quaternion;

#pragma region Static Properties

template<typename T>
Matrix4<T> Matrix4<T>::identity = { 1,0,0,0,
                                    0,1,0,0,
                                    0,0,1,0,
                                    0,0,0,1};
template<typename T>
Matrix4<T> Matrix4<T>::zero = { 0,0,0,0,
                                0,0,0,0,
                                0,0,0,0,
                                0,0,0,0 };
#pragma endregion

#pragma region Constuctor

template<typename T>
constexpr Matrix4<T>::Matrix4()
{
    memcpy(m_data, identity.m_data, 16 * sizeof(T));
}

template<typename T>
constexpr Matrix4<T>::Matrix4(const Matrix4<T>& p_matrix)
{
    memcpy(m_data, p_matrix.m_data, 16 * sizeof(T));
}


template<typename T>
constexpr Matrix4<T>::Matrix4(Matrix4<T>&& p_matrix) noexcept
{
    memcpy(m_data, p_matrix.m_data, 16 * sizeof(T));
}

template<typename T>
constexpr Matrix4<T>::Matrix4(const Vector3<T>& p_vector)
{
    m_data[0] = p_vector.x;
    m_data[5] = p_vector.y;
    m_data[10] = p_vector.z;
}

template<typename T>
constexpr Matrix4<T>::Matrix4(const T p_data[16])
{
    if (p_data == nullptr)
        return;

    memcpy(m_data, p_data, 16 * sizeof(T));
}

template<typename T>
constexpr Matrix4<T>::Matrix4(const T p_00, const T p_01, const T p_02, const T p_03,
    const T p_10, const T p_11, const T p_12, const T p_13,
    const T p_20, const T p_21, const T p_22, const T p_23,
    const T p_30, const T p_31, const T p_32, const T p_33)
    : m_data{p_00, p_01, p_02, p_03, p_10, p_11, p_12, p_13, p_20, p_21, p_22, p_23, p_30, p_31, p_32, p_33}
{
}

#pragma endregion

#pragma region Properties

template<typename T>
constexpr T Matrix4<T>::Determinant()
{
    return ((m_data[0] * GetMinor({ m_data[5], m_data[6], m_data[7], m_data[9], m_data[10], m_data[11], m_data[13], m_data[14], m_data[15] }))
          - (m_data[1] * GetMinor({ m_data[4], m_data[6], m_data[7], m_data[8], m_data[10], m_data[11], m_data[12], m_data[14], m_data[15] }))
          + (m_data[2] * GetMinor({ m_data[4], m_data[5], m_data[7], m_data[8], m_data[9], m_data[11], m_data[12], m_data[13], m_data[15] }))
          - (m_data[3] * GetMinor({ m_data[4], m_data[5], m_data[6], m_data[8], m_data[9], m_data[10], m_data[12], m_data[13], m_data[14] })));
}

template<typename T>
constexpr T Matrix4<T>::Determinant(const Matrix4& p_matrix)
{
    return p_matrix.Determinant();
}

template<typename T>
constexpr Matrix4<T>& Matrix4<T>::Transpose()
{
    Matrix4<T> tmpMat(this->m_data);

    for (int n = 0; n < 16; n++)
    {
        int i = n / 4;
        int j = n % 4;

        m_data[n] = tmpMat.m_data[4 * j + i];
    }

    return { *this };
}


template<typename T>
constexpr Matrix4<T> Matrix4<T>::Transpose(const Matrix4<T>& p_matrix)
{
    Matrix4<T> tmpMat = identity;

    for (int n = 0; n < 16; n++)
    {
        int i = n / 4;
        int j = n % 4;

        tmpMat.m_data[n] = p_matrix.m_data[4 * j + i];
    }

    return tmpMat;
}

template<typename T>
constexpr Matrix4<T>& Matrix4<T>::Normalize()
{
    T det = Determinant();

    for(unsigned int i = 0; i < 16; ++i)
    {
        m_data[i] /= det;
    }

    return { *this };
}

template<typename T>
constexpr Matrix4<T> Matrix4<T>::Normalize(const Matrix4<T>& p_matrix)
{
    return p_matrix.Normalize();
}


template<typename T>
template<typename U>
constexpr Matrix4<T>& Matrix4<T>::Scale(const Vector3<U>& p_scale)
{
    this *= CreateScale(p_scale);

    return { *this };
}

template<typename T>
template<typename U>
constexpr Matrix4<T> Matrix4<T>::CreateScale(const Vector3<U>& p_scale)
{
    Matrix4<T> tmpScale = identity;
    tmpScale.m_data[0] = p_scale.x;
    tmpScale.m_data[5] = p_scale.y;
    tmpScale.m_data[10] = p_scale.z;

    return tmpScale;
}

template<typename T>
constexpr Matrix4<T>& Matrix4<T>::Rotate(const Quaternion& p_rotation)
{
    this *= p_rotation.ToMatrix4();

    return { *this };
}

template<typename T>
constexpr Matrix4<T> Matrix4<T>::CreateRotation(const Quaternion& p_rotation)
{
    return p_rotation.ToMatrix4();
}

template<typename T>
template<typename U>
constexpr Matrix4<T>& Matrix4<T>::Translate(const Vector3<U>& p_translate)
{
    this *= CreateTranslate(p_translate);

    return { *this };
}

template<typename T>
template<typename U>
constexpr Matrix4<T> Matrix4<T>::CreateTranslation(const Vector3<U>& p_translate)
{
    Matrix4<T> tmpTrans = identity;

    tmpTrans.m_data[3] = p_translate.x;
    tmpTrans.m_data[7] = p_translate.y;
    tmpTrans.m_data[11] = p_translate.z;

    return tmpTrans;
}

template<typename T>
template<typename U>
constexpr Matrix4<T>& Matrix4<T>::Transform(const Vector3<U>& p_translate, const Quaternion& p_rotation, const Vector3<U>& p_scale)
{
    this *= CreateTransform(p_translate, p_rotation, p_scale);

    return { *this };
}

template<typename T>
template<typename U>
constexpr Matrix4<T> Matrix4<T>::CreateTransformation(const Vector3<U>& p_translate, const Quaternion& p_rotation, const Vector3<U>& p_scale)
{
    Matrix4<T> tmpTrans = CreateTranslation(p_translate);
    Matrix4<T> tmpRot = CreateRotation(p_rotation);
    Matrix4<T> tmpScale = CreateScale(p_scale);

    Matrix4<T> tmpMat = tmpTrans * tmpRot * tmpScale;

    // tmpTrans *= tmpRot;
    //tmpMat *= tmpScale;

    return tmpMat;
}


#pragma endregion 


template<typename T>
constexpr void Matrix4<T>::SetColumn(int p_column, const Vector4<T>& p_vector)
{
    int columnStart = p_column;
    m_data[columnStart] = p_vector.x;
    m_data[columnStart + 4] = p_vector.y;
    m_data[columnStart + 8] = p_vector.z;
    m_data[columnStart + 12] = p_vector.w;
}

template<typename T>
constexpr void Matrix4<T>::SetRow(int p_row, const Vector4<T>& p_vector)
{
    int rowStart = (4 * p_row);
    m_data[rowStart] = p_vector.x;
    m_data[rowStart + 1] = p_vector.y;
    m_data[rowStart + 2] = p_vector.z;
    m_data[rowStart + 3] = p_vector.w;
}

template<typename T>
T Matrix4<T>::GetMinor(Matrix3<T> p_minor)
{
    return ((p_minor.m_data[0] * ((p_minor.m_data[4] * p_minor.m_data[8]) - (p_minor.m_data[5] * p_minor.m_data[7])))
          - (p_minor.m_data[1] * ((p_minor.m_data[3] * p_minor.m_data[8]) - (p_minor.m_data[5] * p_minor.m_data[6])))
          + (p_minor.m_data[2] * ((p_minor.m_data[3] * p_minor.m_data[7]) - (p_minor.m_data[4] * p_minor.m_data[6]))));
}

template<typename T>
Matrix4<T> Matrix4<T>::Inverse()
{
    //TODO need determinant to complete
}

#pragma region Conversions

template<typename T>
constexpr std::string Matrix4<T>::ToString() noexcept
{
    std::stringstream StringStream;
    StringStream << '[' << m_data[0] << "  " << m_data[1] << "  " << m_data[2] << "  " << m_data[3] << "]\n"
        << '[' << m_data[4] << "  " << m_data[5] << "  " << m_data[6] << "  " << m_data[7] << "]\n"
        << '[' << m_data[8] << "  " << m_data[9] << "  " << m_data[10] << "  " << m_data[11] << "]\n"
        << '[' << m_data[12] << "  " << m_data[13] << "  " << m_data[14] << "  " << m_data[15] << "]\n";
    return { StringStream.str() };
}

template<typename T>
constexpr std::string Matrix4<T>::ToString(const Matrix4<T>& p_matrix)
{
    std::stringstream StringStream;
    StringStream << '[' << p_matrix.m_data[0] << "  " << p_matrix.m_data[1] << "  " << p_matrix.m_data[2] << "  " << p_matrix.m_data[3] << "]\n"
        << '[' << p_matrix.m_data[4] << "  " << p_matrix.m_data[5] << "  " << p_matrix.m_data[6] << "  " << p_matrix.m_data[7] << "]\n"
        << '[' << p_matrix.m_data[8] << "  " << p_matrix.m_data[9] << "  " << p_matrix.m_data[10] << "  " << p_matrix.m_data[11] << "]\n"
        << '[' << p_matrix.m_data[12] << "  " << p_matrix.m_data[13] << "  " << p_matrix.m_data[14] << "  " << p_matrix.m_data[15] << "]\n";
    return { StringStream.str() };
}

#pragma endregion

#pragma region Arithmetic Operations

#pragma region Add

template<typename T>
template<typename U>
Matrix4<T>& Matrix4<T>::Add(const Matrix4<U>& p_other)
{
    for (int i = 0; i < 16; i++)
    {
        m_data[i] += p_other.m_data[i];
    }

    return { *this };
}

template<typename T>
template<typename U>
constexpr  Matrix4<T> Matrix4<T>::Add(const Matrix4<T>& p_left, const Matrix4<U>& p_right)
{
    return Matrix4<T>(p_left).Add(p_right);
}

template<typename T>
template<typename U>
constexpr Matrix4<T> Matrix4<T>::operator+(const Matrix4<U>& p_other) const
{
    return Add(this, p_other);
}

template<typename T>
template<typename U>
Matrix4<T> Matrix4<T>::operator+=(const Matrix4<U>& p_other)
{
    return Add(p_other);
}

#pragma endregion

#pragma region Subtract

template<typename T>
template<typename U>
Matrix4<T> Matrix4<T>::Subtract(const Matrix4<U>& p_other)
{
    for (int i = 0; i < 16; i++)
    {
        m_data[i] -= p_other.m_data[i];
    }

    return { *this };
}

template<typename T>
template<typename U>
constexpr Matrix4<T> Matrix4<T>::Subtract(const Matrix4<T>& p_left, const Matrix4<U>& p_right)
{
    return Matrix4<T>(p_left).Subtract(p_right);
}

template<typename T>
template<typename U>
constexpr Matrix4<T> Matrix4<T>::operator-(const Matrix4<U>& p_other) const
{
    return Subtract(this, p_other);
}

template<typename T>
template<typename U>
Matrix4<T>& Matrix4<T>::operator-=(const Matrix4<U>& p_other)
{
    return Subtract(p_other);
}


#pragma endregion

#pragma region Multiply

template<typename T>
template<typename U>
Matrix4<T>& Matrix4<T>::Multiply(U p_scalar)
{
    for (unsigned int i = 0; i < 16; ++i)
    {
        m_data[i] *= p_scalar;
    }

    return { *this };
}

template<typename T>
template<typename U>
constexpr Matrix4<T> Matrix4<T>::Multiply(const Matrix4<T>& p_left, U p_right)
{
    return Matrix4<T>(p_left).Multiply(p_right);
}


template<typename T>
template<typename U>
Matrix4<T>& Matrix4<T>::Multiply(const Matrix4<U>& p_other)
{
    Matrix4<T> tmpMat(this->m_data);

    for (int i = 0; i < 16; i += 4)
    {
        for (int j = 0; j < 4; j++)
        {
            m_data[i + j] = (tmpMat.m_data[i] * p_other.m_data[j])
                          + (tmpMat.m_data[i + 1] * p_other.m_data[j + 4])
                          + (tmpMat.m_data[i + 2] * p_other.m_data[j + 8])
                          + (tmpMat.m_data[i + 3] * p_other.m_data[j + 12]);
        }
    }

    return { *this };
}

template<typename T>
template<typename U>
constexpr Matrix4<T> Matrix4<T>::Multiply(const Matrix4<T>& p_left, const Matrix4<U>& p_right)
{
    return Matrix4<T>(p_left).Multiply(p_right);
}

template<typename T>
template<typename U>
Vector4<U>& Matrix4<T>::Multiply(const Vector4<U>& p_other)
{
    Vector4<T> tmpVec = Vector4F::zero;

    tmpVec.x = (m_data[0] * p_other.x)
        + (m_data[1] * p_other.y)
        + (m_data[2] * p_other.z)
        + (m_data[3] * p_other.w);

    tmpVec.y = (m_data[4] * p_other.x)
        + (m_data[5] * p_other.y)
        + (m_data[6] * p_other.z)
        + (m_data[7] * p_other.w);

    tmpVec.z = (m_data[8] * p_other.x)
        + (m_data[9] * p_other.y)
        + (m_data[10] * p_other.z)
        + (m_data[11] * p_other.w);

    tmpVec.w = (m_data[12] * p_other.x)
        + (m_data[13] * p_other.y)
        + (m_data[14] * p_other.z)
        + (m_data[15] * p_other.w);

    return tmpVec;
}


template<typename T>
template<typename U>
constexpr Matrix4<T> Matrix4<T>::operator*(const Matrix4<U>& p_other) const
{
    return Matrix4<T>(*this).Multiply(p_other) ;
}

template<typename T>
template<typename U>
Matrix4<T>& Matrix4<T>::operator*=(const Matrix4<U>& p_other)
{
    return Multiply(p_other);
}


#pragma endregion



template<typename T>
template<typename U>
Vector4<T> Matrix4<T>::Multiply(const Matrix4<U>& p_matrix, const Vector4<T>& p_vector)
{
    Vector4<T> tmpVec = Vector4F::zero;

    tmpVec.x = (p_matrix[0] * p_vector.x)
        + (p_matrix[1] * p_vector.y)
        + (p_matrix[2] * p_vector.z)
        + (p_matrix[3] * p_vector.w);

    tmpVec.y = (p_matrix[4] * p_vector.x)
        + (p_matrix[5] * p_vector.y)
        + (p_matrix[6] * p_vector.z)
        + (p_matrix[7] * p_vector.w);

    tmpVec.z = (p_matrix[8] * p_vector.x)
        + (p_matrix[9] * p_vector.y)
        + (p_matrix[10] * p_vector.z)
        + (p_matrix[11] * p_vector.w);

    tmpVec.w = (p_matrix[12] * p_vector.x)
        + (p_matrix[13] * p_vector.y)
        + (p_matrix[14] * p_vector.z)
        + (p_matrix[15] * p_vector.w);

    return tmpVec;
}

template<typename T>
bool Matrix4<T>::Equals(const Matrix4<T>& p_matrix, const Matrix4<T>& p_other)
{
    for (int i = 0; i < 16; i++)
    {
        if (p_matrix.m_data[i] != p_other.m_data[i])
            return false;
    }
    return true;
}

template<typename T>
void Matrix4<T>::Set(Matrix4<T>& p_matrix, const Matrix4<T>& p_other)
{
    memcpy(p_matrix.m_data, p_other.m_data, sizeof(T) * 16);
}

#pragma endregion 

#pragma region Operators
//operators


template<typename T>
Vector4<T> Matrix4<T>::operator*(const Vector4<T>& p_vector)
{
    return Matrix4<T>(*this).Multiply(p_vector);
}

template<typename T>
bool Matrix4<T>::operator==(const Matrix4<T>& p_matrix)
{
    return Equals(*this, p_matrix);
}

template<typename T>
bool Matrix4<T>::operator!=(const Matrix4<T>& p_matrix)
{
    return !Equals(*this, p_matrix);
}

template<typename T>
void Matrix4<T>::operator=(const Matrix4<T>& p_matrix)
{
    Set(*this, p_matrix);
}

template<typename T>
T Matrix4<T>::operator[](int p_position)
{
    return m_data[p_position];
}

#pragma endregion