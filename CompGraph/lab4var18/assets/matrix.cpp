#include "matrix.h"

#include <initializer_list>
#include <iostream>
#include <stdexcept>

Matrix::Matrix(int sizeX, int sizeY, std::initializer_list<std::initializer_list<float>> mat):
_sizeX(sizeX), _sizeY(sizeY) 
{
  this->_mat = new float*[sizeY];
  int i = 0, j;
  for (const auto row : mat) {
    j = 0;
    this->_mat[i] = new float[sizeX];
    for(const auto elem : row) {
      this->_mat[i][j++] = elem;
    }
    i++;
  }
}

Matrix::Matrix(int sizeX, int sizeY):
_sizeX(sizeX), _sizeY(sizeY) 
{
  this->_mat = new float*[sizeY];
  for(int i = 0; i < sizeY; i++) {
    _mat[i] = new float[sizeX];
    for (int j = 0; j < sizeX; j++) {
      this->_mat[i][j] = 0;
    }
  }
}

Matrix::Matrix(const Matrix& other) : 
_sizeX(other._sizeX), _sizeY(other._sizeY) 
{ 
  _mat = new float*[_sizeY];
  for (int i = 0; i < _sizeY; i++) {
    _mat[i] = new float[_sizeX];
    for (int j = 0; j < _sizeX; j++) {
      _mat[i][j] = other._mat[i][j];
    }
  }
}

Matrix& Matrix::operator=(const Matrix& other) 
{
  if (this == &other) return *this;
    
  for (int i = 0; i < _sizeY; i++) {
    delete[] _mat[i];
  }
  delete[] _mat;
    
  _sizeX = other._sizeX;
  _sizeY = other._sizeY;
    
  _mat = new float*[_sizeY];
  for (int i = 0; i < _sizeY; i++) {
    _mat[i] = new float[_sizeX];
    for (int j = 0; j < _sizeX; j++) {
      _mat[i][j] = other._mat[i][j];
    }
  }
    
  return *this;
}

void Matrix::print() 
{
  for (int i = 0; i < _sizeY; i++) { 
    for (int j = 0; j < _sizeX; j++) { 
      std::cout << _mat[i][j] << ' ';
    }
    std::cout << std::endl;
  }
}

void Matrix::operator+=(const Matrix& b) 
{
  if(b.getX() != _sizeX || b.getY() != _sizeY)
    throw std::runtime_error("Error with getting matrix sum! Incorrect size");
  for (int i = 0; i < _sizeY; i++) {
    for(int j = 0; j < _sizeX; j++) {
      this->_mat[i][j] += b[i][j];
    }
  }
}

void Matrix::operator*=(const Matrix& b) 
{
  if(b.getX() != _sizeY)
    throw std::runtime_error("Error with getting matrix multimplie! Incorrect size");
    
  float** newMat = new float*[_sizeY];
  for (int i = 0; i < _sizeY; i++) {
    newMat[i] = new float[b.getX()];
    for (int j = 0; j < b.getX(); j++) 
      newMat[i][j] = 0;
  }

  for (int i = 0; i < _sizeY; i++) {
    for (int j = 0; j < b.getX(); j++) {
      for (int k = 0; k < _sizeX; k++) {
        newMat[i][j] += _mat[i][k] * b[k][j];
      }
    }
  }

  for (int i = 0; i < _sizeY; i++)
    delete[] _mat[i];
  delete[] _mat;

  this->_mat = newMat;
  this->_sizeX = b.getX();
}

Matrix Matrix::operator*(const Matrix& b) 
{
  if(_sizeX != b.getY())
    throw std::runtime_error("Error with getting matrix multiply! Incorrect size");
    
  Matrix out(b.getX(), _sizeY);
  for (int i = 0; i < _sizeY; i++) {
    for (int j = 0; j < b.getX(); j++) {
      out[i][j] = 0;
      for (int k = 0; k < _sizeX; k++) 
        out[i][j] += _mat[i][k] * b[k][j];
    }
  }
  return out;
}

Matrix Matrix::operator+(const Matrix& b) 
{
  if(b.getX() != _sizeX || b.getY() != _sizeY)
    throw std::runtime_error("Error with getting matrix sum! Incorrect size");

  Matrix out(_sizeX, _sizeY);
  for (int i = 0; i < _sizeY; i++) {
    for (int j = 0; j < _sizeX; j++) {
      out[i][j] = _mat[i][j];
      out[i][j] += b[i][j];
    }
  }
  return out;
}

Matrix::~Matrix() 
{
  for (int i = 0; i < _sizeY; i++) {
    delete _mat[i];
  }
}