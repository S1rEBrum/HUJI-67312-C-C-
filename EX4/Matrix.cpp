#include "Matrix.h"
#include <cmath>

#define INDEX 0.1
#define RREF_IDX 1e-8

// constructor
Matrix::Matrix (int rows, int cols) : rows (rows), cols (cols),
                                      data (nullptr) {
  if (rows <= 0 || cols <= 0) {
    throw std::exception ();
  }
  data = new float[rows * cols];
  for (int i = 0; i < rows * cols; ++i) {
    data[i] = 0.0;
  }
}

// default constructor
Matrix::Matrix () : Matrix (1, 1) {}

// copy constructor
Matrix::Matrix (const Matrix &m) : rows (m.rows),
                                   cols (m.cols),
                                   data (new float[m.rows * m.cols]) {
  for (int i = 0; i < rows * cols; ++i) {
    this->data[i] = m.data[i];
  }
}

// destructor
Matrix::~Matrix () {
  delete[] data;
}

// rows getter
int Matrix::get_rows () const {
  return rows;
}

// cols getter
int Matrix::get_cols () const {
  return cols;
}

/**
 * Transforms a matrix into its transpose matrix
 * Supports function call chaining/concatenation
 * transpose method for Matrix class
 * @return
 */
Matrix &Matrix::transpose () {
  int temp = rows;
  rows = cols;
  cols = temp;
  auto *transposed_data = new float[rows * cols];
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      transposed_data[i * cols + j] = data[j * rows + i];
    }
  }
  delete[] data;
  data = transposed_data;
  return *this;
}

/**
 * Transforms a matrix into a column vector
 * Supports function call chaining/concatenation
 * @return
 */
Matrix &Matrix::vectorize () {
  rows = rows * cols;
  cols = 1;
  return *this;
}

/**
 * Simple print function - prints values with space between
 * Prints newline after each row
 */
void Matrix::plain_print () const {
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      std::cout << data[i * cols + j] << ' ';
    }
    std::cout << std::endl;
  }
}

/**
 * Dot product - element-wise multiplication of matrices
 * @param other - the RHS matrix
 * @return
 */
Matrix Matrix::dot (const Matrix &other) const {
  if (rows != other.rows || cols != other.cols) {
    throw std::exception ();
  }

  Matrix res (rows, cols);
  for (int i = 0; i < rows * cols; ++i) {
    res.data[i] = data[i] * other.data[i];
  }

  return res;
}

/**
 * Frobenius norm of the martix
 * @return
 */
float Matrix::norm () const {
  float sum = 0;
  for (int i = 0; i < rows * cols; ++i) {
    sum += data[i] * data[i];
  }
  return std::sqrt (sum);
}

/**
 * Reduced row echelon form of the original matrix
 * The original matrix is unchanged
 * @return
 */
Matrix Matrix::rref () const {
  Matrix rref_matrix (*this);
  int lead = 0;
  for (int r = 0; r < rref_matrix.rows; ++r) {
    if (lead >= rref_matrix.cols) {
      break;
    }
    int i = r;
    while (std::abs (rref_matrix.data[i * cols + lead]) < RREF_IDX) {
      ++i;
      if (i == rref_matrix.rows) {
        i = r;
        ++lead;
        if (lead == rref_matrix.cols) {
          break;
        }
      }
    }
    for (int j = 0; j < rref_matrix.cols; ++j) {
      std::swap (rref_matrix.data[r * cols + j], rref_matrix.data[i * cols
                                                                  + j]);
    }
    float div = rref_matrix.data[r * cols + lead];
    if (std::abs (div) < RREF_IDX) {
      continue;
    }
    for (int j = 0; j < rref_matrix.cols; ++j) {
      rref_matrix.data[r * cols + j] /= div;
    }
    for (int i = 0; i < rref_matrix.rows; ++i) {
      if (i != r) {
        float mult = rref_matrix.data[i * cols + lead];
        for (int j = 0; j < rref_matrix.cols; ++j) {
          rref_matrix.data[i * cols + j] -=
              mult * rref_matrix.data[r * cols + j];
        }
      }
    }
    ++lead;
  }
  return rref_matrix;
}

/**
 * Returns an index of the largest number in matrix
 * @return
 */
float Matrix::argmax () const {
  int max_value = 0;
  for (int i = 1; i < rows * cols; ++i) {
    if (data[i] > data[max_value]) {
      max_value = i;
    }
  }
  return (float) max_value;
}

/**
 * Returns the sum of all the elements in the matrix
 * @return
 */
float Matrix::sum () const {
  float total_sum = 0;
  for (int i = 0; i < rows * cols; ++i) {
    total_sum += data[i];
  }
  return total_sum;
}

/**
 * A += B
 * @param other
 * @return
 */
Matrix &Matrix::operator+= (const Matrix &other) {
  if (rows != other.rows || cols != other.cols) {
    throw std::exception ();
  }
  for (int i = 0; i < rows * cols; ++i) {
    data[i] += other.data[i];
  }
  return *this;
}

/**
 * A + B
 * @param other
 * @return
 */
Matrix Matrix::operator+ (const Matrix &other) const {
  if (rows != other.rows || cols != other.cols) {
    throw std::exception ();
  }
  Matrix res (rows, cols);
  for (int i = 0; i < rows * cols; ++i) {
    res.data[i] = data[i] + other.data[i];
  }
  return res;
}

/**
 * A = B (assignment operation)
 * In case of different dimensions - changes the matrix
 * @param other
 * @return
 */
Matrix &Matrix::operator= (const Matrix &other) {
  if (this == &other) {
    return *this;
  }
  delete[] data;
  rows = other.rows;
  cols = other.cols;
  data = new float[rows * cols];
  for (int i = 0; i < rows * cols; ++i) {
    data[i] = other.data[i];
  }
  return *this;
}

/**
 * A * B (matrix multiplication)
 * @param other
 * @return
 */
Matrix Matrix::operator* (const Matrix &other) const {
  if (cols != other.rows) {
    throw std::exception ();
  }
  Matrix res (rows, other.cols);
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < other.cols; ++j) {
      for (int k = 0; k < cols; ++k) {
        res.data[i * res.cols + j] +=
            data[i * cols + k] * other.data[k * other.cols + j];
      }
    }
  }
  return res;
}

/**
 * M * c (scalar multiplication on the right)
 * @param c
 * @return
 */
Matrix Matrix::operator* (float c) const {
  Matrix res (rows, cols);
  for (int i = 0; i < rows * cols; ++i) {
    res.data[i] = data[i] * c;
  }
  return res;
}

/**
 * c * M (scalar multiplication on the left)
 * @param c
 * @param mat
 * @return
 */
Matrix operator* (float c, const Matrix &mat) {
  Matrix res (mat.rows, mat.cols);
  for (int i = 0; i < mat.rows * mat.cols; ++i) {
    res.data[i] = c * mat.data[i];
  }
  return res;
}

/**
 * Parenthesis indexing - M(i,j)
 * @param row
 * @param col
 * @return
 */
float &Matrix::operator() (int row, int col) {
  if (row < 0 || row >= rows || col < 0 || col >= cols) {
    throw std::exception ();
  }
  return data[row * cols + col];
}

/**
 * Parenthesis indexing - M(i,j) for const matrix
 * @param row
 * @param col
 * @return
 */
float Matrix::operator() (int row, int col) const {
  if (row < 0 || row >= rows || col < 0 || col >= cols) {
    throw std::exception ();
  }
  return data[row * cols + col];
}

/**
 * Brackets indexing - m[k], k is in (i*j)
 * @param k
 * @return
 */
float &Matrix::operator[] (int k) {
  if (k < 0 || k >= rows * cols) {
    throw std::exception ();
  }
  return data[k];
}

/**
 * Brackets indexing - m[k], k is in (i*j) for const matrix
 * @param k
 * @return
 */
float Matrix::operator[] (int k) const {
  if (k < 0 || k >= rows * cols) {
    throw std::exception ();
  }
  return data[k];
}

/**
 * output stream operator - pretty print of the matrix
 * @param os
 * @param matrix
 * @return
 */
std::ostream &operator<< (std::ostream &os, const Matrix &m) {
  for (int i = 0; i < m.rows; ++i) {
    for (int j = 0; j < m.cols; ++j) {
      if (m.data[i * m.cols + j] > INDEX) {
        os << "**";
      }
      else {
        os << "  ";
      }
    }
    os << std::endl;
  }
  return os;
}

/**
 * input stream operator - fills the matrix elements
 * @param is
 * @param m
 * @return
 */
std::istream &operator>> (std::istream &is, const Matrix &m) {
  if (!is) {
    throw std::exception ();
  }
  is.seekg (0, std::ios::beg);
  for (int i = 0; i < m.rows * m.cols; ++i) {
    float value;
    is.read ((char *) &value, sizeof (float));
    if (!is) {
      throw std::exception ();
    }
    m.data[i] = value;
  }
  return is;
}







