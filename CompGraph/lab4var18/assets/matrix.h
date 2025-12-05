#pragma once

#include <initializer_list>

class Matrix 
{
  private:
    int _sizeX;
    int _sizeY;
    float** _mat;    
  public:
    Matrix(int sizeX, int sizeY, std::initializer_list<std::initializer_list<float>> mat);
    Matrix(int sizeX, int sizeY);
    Matrix(const Matrix& other);
    Matrix& operator=(const Matrix& other);
  
    int getX() const { return _sizeX; };
    int getY() const { return _sizeY; };
  
    void print();
  
    float* operator[](int x) const { return this->_mat[x]; };
  
    void operator+=(const Matrix& b);
    void operator*=(const Matrix& b);
    Matrix operator*(const Matrix& b);
    Matrix operator+(const Matrix& b);
  
    ~Matrix();
};