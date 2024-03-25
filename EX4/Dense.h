// Dense.h
#ifndef DENSE_H
#define DENSE_H

#include "Activation.h"

/**
 * Class Dense: has matrix weights, bias and function
 * activates the layer with the appropriate activation function
 */
class Dense {
 private:
  Matrix weights;
  Matrix bias;
  ActivationFunction function;
 public:
  // Constructor
  Dense (const Matrix &weights, const Matrix &bias, ActivationFunction
  activationFunction);

  // getters
  Matrix get_weights () const;
  Matrix get_bias () const;
  ActivationFunction get_activation () const;

  // parenthesis operator
  Matrix operator() (const Matrix &input) const;

};

#endif //DENSE_H
