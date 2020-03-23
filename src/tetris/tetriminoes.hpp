#ifndef TETRIS_TETRIMINOES_HPP
#define TETRIS_TETRIMINOES_HPP

#include <array>

#include "matrix.hpp"

namespace tetris{

// Represent a tetrimino (a tetris piece).
class Tetrimino {
public:
    using Shape = geom::Matrix2D<char, 4, 4>;

    // Build a tetrimino representation with its shape.
    // Args:
    //     shape: A 4x4 boolean matrix representing the shape of the piece.
    constexpr Tetrimino(Shape shape):
        shape_{shape}
    {}

    const Shape& shape() const {
        return shape_;
    }

private:
    Shape shape_;
};

// Tetriminoes "assets".
constexpr auto tetriminoes = std::array<Tetrimino, 7>{
    // I
    Tetrimino{
        {{
            0, 0, 1, 0,
            0, 0, 1, 0,
            0, 0, 1, 0,
            0, 0, 1, 0,
        }}
    },
    // Z
    {
        {{
            0, 0, 1, 0,
            0, 1, 1, 0,
            0, 1, 0, 0,
            0, 0, 0, 0,
        }}
    },
    // S
    {
        {{
            0, 1, 0, 0,
            0, 1, 1, 0,
            0, 0, 1, 0,
            0, 0, 0, 0,
        }}
    },
    // O
    {
        {{
            0, 0, 0, 0,
            0, 1, 1, 0,
            0, 1, 1, 0,
            0, 0, 0, 0,
        }}
    },
    // T
    {
        {{
            0, 0, 1, 0,
            0, 1, 1, 0,
            0, 0, 1, 0,
            0, 0, 0, 0,
        }}
    },
    // J
    {
        {{
            0, 0, 1, 0,
            0, 0, 1, 0,
            0, 1, 1, 0,
            0, 0, 0, 0,
        }}
    },
    // L
    {
        {{
            0, 1, 0, 0,
            0, 1, 0, 0,
            0, 1, 1, 0,
            0, 0, 0, 0,
        }}
    },

};

}

#endif
