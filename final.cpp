#include <iostream>
#include <initializer_list>
#include <cassert>


template <unsigned rowCount, unsigned columnCount, typename T = float>
class Matrix
{
public:
    static const int rowCount_ = rowCount;
    static const int columnCount_ = columnCount;

    T e[columnCount * rowCount];

    Matrix(std::initializer_list<T> l)
    {
        assert(l.size() == rowCount * columnCount);
        for (auto i = l.begin(); i!= l.end(); ++i)
        {
            e[i - l.begin()] = *i;
        }
    }

    Matrix(std::initializer_list<std::initializer_list<T>> l)
    {
        assert(l.size() == rowCount);
        for (auto r = l.begin(); r != l.end(); ++r)
        {
            assert(r->size() == columnCount);
            for (auto c = r->begin(); c != r->end(); ++c)
            {
                e[(r - l.begin()) * columnCount + (c - r->begin())] = *c;
            }
        }
    }

    Matrix()
    {

    }

    void print()
    {
        for (int i = 0; i < rowCount; i++)
        {
            for (int j = 0; j < columnCount; j++)
            {
                std::cout << e[i * columnCount + j] << " ";
            }
            std::cout << std::endl;
        }
    }

    // Calculates the trace of the matrix
    T trace()
    {
        static_assert(rowCount == columnCount, "Matrix must be square to calculate determinant");
        T sum = 0;
        for (int i = 0; i < rowCount; i++)
        {
            sum += e[i * columnCount + i];
        }
        return sum;
    }

    // Calculates the determinant of the matrix
    T determinant()
    {
        static_assert(rowCount == columnCount, "Matrix must be square to calculate determinant");

        T det = 0;
        if constexpr(rowCount == 2) {
            det = e[0] * e[3] - e[1] * e[2];
        } else {
            for (int c = 0; c < columnCount; c++) {
                T cofactor = (c % 2 == 0 ? 1 : -1) * e[c] * submatrix(0, c).determinant();
                det += cofactor;
            }
        }
        return det;
    }

    // Returns a submatrix obtained by deleting the specified row and column
    Matrix<rowCount-1, columnCount-1, T> submatrix(int row, int col)
    {
        Matrix<rowCount-1, columnCount-1, T> sub;
        int k = 0;
        for (int i = 0; i < rowCount; i++) {
            if (i == row) continue;
            int l = 0;
            for (int j = 0; j < columnCount; j++) {
                if (j == col) continue;
                sub.e[k * (columnCount-1) + l] = e[i * columnCount + j];
                l++;
            }
            k++;
        }
        return sub;
    }
};

template <unsigned RC1, unsigned CRC12, unsigned CC2, typename T>
Matrix<RC1, CC2, T> operator*(const Matrix<RC1, CRC12, T>& lhs, const Matrix<CRC12, CC2, T>& rhs)
{
    Matrix<RC1, CC2, T> r;

    for (int i = 0; i < CC2; i++)
    {
        for (int j = 0; j < RC1; j++)
        {
            T s = T();
            for (int k = 0; k < CRC12; k++)
            {
                s += lhs.e[CRC12 * j + k] * rhs.e[CC2 * k + i];
            }
            r.e[j * CC2 + i] = s;
        }
    }

    return r;
}

int main()
{

Matrix<3, 3> a = {{1.0, 2.0, 0.0,},
                  {4.0, 5.0, 6.0,},
                  {7.0, 8.0, 9.0}};



std::cout<<a.trace();

std::cout<<a.determinant();
// a.submatrix(1,2).print();
// a.print();


}
