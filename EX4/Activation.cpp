#include "Activation.h"
#include <cmath>

/**
 * ReLu activation function
 * Returns Matrix[i] if it is greater or equal 0
 * Returns 0 otherwise
 * @param m
 * @return
 */
Matrix activation::relu (const Matrix &m) {
  Matrix res = Matrix (m);
  for (int i = 0; i < m.get_cols () * m.get_rows (); ++i) {
    res[i] = (m[i] >= 0) ? m[i] : 0;
  }
  return res;
}

/**
 * Softmax activation function
 * Returns normalized exp of the Matrix[i]
 * 1/sum(exp(x_k)) * exp(x_k)
 * @param m
 * @return
 */
Matrix activation::softmax (const Matrix &m) {
  Matrix res = Matrix (m);
  float sum = 0;
  for (int i = 0; i < m.get_cols () * m.get_rows (); ++i) {
    sum += std::exp (m[i]);
  }
  for (int i = 0; i < m.get_cols () * m.get_rows (); ++i) {
    res[i] = (1 / sum) * std::exp (m[i]);
  }
  return res;
}