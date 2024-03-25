#include "Dense.h"

/**
 * Constructor: initializes the weights, bias and activation
 * function with the appropriate values
 * @param weights
 * @param bias
 * @param activationFunction
 */
Dense::Dense (const Matrix& weights, const Matrix& bias, ActivationFunction
activationFunction)
    : weights (weights), bias (bias), function (activationFunction) {
  // TODO: fill func
}

/**
 * Weights getter
 * @return
 */
Matrix Dense::get_weights () const {
  return weights;
}

/**
 * Bias getter
 * @return
 */
Matrix Dense::get_bias () const {
  return bias;
}

/**
 * Activation function getter
 * @return
 */
ActivationFunction Dense::get_activation () const {
  return function;
}

/**
 * Parenthesis operator - applies the activation function
 * with appropriate bias and weights on the input matrix,
 * creating a layer of neural network
 * @param m
 * @return
 */
Matrix Dense::operator() (const Matrix &m) const {
  Matrix res = weights * m + bias;
  return function (res);
}


