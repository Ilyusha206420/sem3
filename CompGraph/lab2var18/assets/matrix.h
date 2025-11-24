#pragma once

#include <initializer_list>

class Matrix {
private:
    int _sizeX;
    int _sizeY;
    float** _mat;    
public:
    Matrix(int sizeX, int sizeY, std::initializer_list<std::initializer_list<float>> mat);
    Matrix(int sizeX, int sizeY);
    Matrix(const Matrix& other);
    Matrix& operator=(const Matrix& other);

    int getX() const;
    int getY() const;

    void print();

    float* operator[](int x) const;

    void operator+=(const Matrix& b);
    void operator*=(const Matrix& b);
    Matrix operator*(const Matrix& b);

    ~Matrix();
};