// Activation.h
#ifndef ACTIVATION_H
#define ACTIVATION_H

#include "Matrix.h"

// typedef for activation function type
typedef Matrix (*ActivationFunction) (const Matrix &);

// namespace in which the activation functions are initialized
namespace activation {
    // ReLu
    Matrix relu (const Matrix &m);

    // Softmax
    Matrix softmax (const Matrix &m);
}

#endif //ACTIVATION_H