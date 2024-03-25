#include "MlpNetwork.h"

/**
 * Constructor: initializes the 4 layers of the network with
 * the appropriate weights, biases and activation functions
 * @param weights
 * @param biases
 * @param activationFunctions
 */
MlpNetwork::MlpNetwork (const Matrix *weights,
                        const Matrix *biases)
    : layers{
    Dense (weights[0], biases[0], activation::relu
    ),
    Dense (weights[1], biases[1], activation::relu
    ),
    Dense (weights[2], biases[2], activation::relu
    ),
    Dense (weights[3], biases[3], activation::softmax
    )} {}

/**
 * Parenthesis operator: when applied to input matrix,
 * runs all 4 layers of the neural network on it and
 * outputs the estimated value 
 * @param input
 * @return
 */
digit MlpNetwork::operator() (const Matrix &m) const {
  Matrix output = Matrix (m);
  for (const auto &layer: layers) {
    output = layer (output);
  }
  digit result;
  result.value = output.argmax ();
  result.probability = output[result.value];
  return result;
}

