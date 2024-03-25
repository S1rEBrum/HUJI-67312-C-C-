// MlpNetwork.h
#ifndef MLPNETWORK_H
#define MLPNETWORK_H

#include "Dense.h"
#include "Activation.h"

#define MLP_SIZE 4

/**
 * @struct digit
 * @brief Identified (by Mlp network) digit with
 *        the associated probability.
 * @var value - Identified digit value
 * @var probability - identification probability
 */
typedef struct digit {
    unsigned int value;
    float probability;
} digit;

const matrix_dims img_dims = {28, 28};
const matrix_dims weights_dims[] = {{128, 784},
                                    {64,  128},
                                    {20,  64},
                                    {10,  20}};
const matrix_dims bias_dims[] = {{128, 1},
                                 {64,  1},
                                 {20,  1},
                                 {10,  1}};

/**
 * MLP Network class initializes the layers and runs the
 * neural network
 */
class MlpNetwork {
 private:
  Dense layers[MLP_SIZE];
 public:
  // Constructor
  MlpNetwork (const Matrix weights[MLP_SIZE],
              const Matrix biases[MLP_SIZE]);

  // parenthesis operator
  digit operator() (const Matrix &m) const;
};

#endif // MLPNETWORK_H