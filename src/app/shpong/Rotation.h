#pragma once

#include <glad/glad.h>

#include <cmath>


namespace ad {

using Matrix = math::Matrix<2, 2, GLfloat>;

typedef GLfloat Angle;

Matrix makeRotation(Angle aAngle)
{
    return {
         std::cos(aAngle),  std::sin(aAngle),
        -std::sin(aAngle),  std::cos(aAngle),
    };
}

} // namespace ad

