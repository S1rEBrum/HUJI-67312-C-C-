// Matrix.h
#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>

// You don't have to use the struct. Can help you with MlpNetwork.h
struct matrix_dims {
    int rows, cols;
};

class Matrix {
 private:
  int rows;
  int cols;
  float *data;
 public:
  // constructor
  Matrix (int rows, int cols);
  // default constructor
  Matrix ();
  // copy constructor
  Matrix (const Matrix &m);

  // destructor
  ~Matrix ();

  // rows getter
  int get_rows () const;
  // cols getter
  int get_cols () const;

  // transpose function
  Matrix &transpose ();

  // vectorize function
  Matrix &vectorize ();

  // plain print
  void plain_print () const;

  // dot product
  Matrix dot (const Matrix &other) const;

  // Frobenius norm
  float norm () const;

  // rref
  Matrix rref () const;

  // argmax
  float argmax () const;

  // sum
  float sum () const;

  //------------------------------------
  //------------OPERATORS---------------

  // +=
  Matrix &operator+= (const Matrix &other);

  // +
  Matrix operator+ (const Matrix &other) const;

  // =
  Matrix &operator= (const Matrix &other);

  // * (matrix multiplication)
  Matrix operator* (const Matrix &other) const;

  // * (scalar from right)
  Matrix operator* (float c) const;

  // * (scalar from the left)
  friend Matrix operator* (float c, const Matrix &mat);

  // ()
  float &operator() (int row, int col);

  // () const
  float operator() (int row, int col) const;

  // []
  float &operator[] (int k);

  // [] const
  float operator[] (int k) const;

  // <<
  friend std::ostream &operator<< (std::ostream &os, const Matrix &m);

  // >>
  friend std::istream &operator>> (std::istream &is, const Matrix &m);
};

#endif //MATRIX_H