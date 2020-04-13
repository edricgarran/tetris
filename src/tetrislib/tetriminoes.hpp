#ifndef TETRIS_TETRIMINOES_HPP
#define TETRIS_TETRIMINOES_HPP

#include <array>

#include "block_type.hpp"
#include "matrix.hpp"

namespace tetris {

// Represent a tetrimino (a tetris piece).
class Tetrimino {
public:
    using Shape = geom::Matrix2D<char, 4, 4>;

    // Build a tetrimino representation with its shape.
    // Args:
    //     shape: A 4x4 boolean matrix representing the shape of the piece.
    constexpr Tetrimino(Shape shape, BlockType type): shape_{shape}, type_{type}
    {}

    const Shape& shape() const
    {
        return shape_;
    }

    BlockType type() const
    {
        return type_;
    }

private:
    Shape shape_;
    BlockType type_;
};

// Tetriminoes "assets".
constexpr auto tetriminoes = std::array<Tetrimino, 7>{
    Tetrimino{
        {{
            // clang-format off
            0, 0, 1, 0,
            0, 0, 1, 0,
            0, 0, 1, 0,
            0, 0, 1, 0,
            // clang-format on
        }},
        BlockType::I,
    },
    {
        {{
            // clang-format off
            0, 0, 1, 0,
            0, 1, 1, 0,
            0, 1, 0, 0,
            0, 0, 0, 0,
            // clang-format on
        }},
        BlockType::Z,
    },
    {
        {{
            // clang-format off
            0, 1, 0, 0,
            0, 1, 1, 0,
            0, 0, 1, 0,
            0, 0, 0, 0,
            // clang-format on
        }},
        BlockType::S,
    },
    {
        {{
            // clang-format off
            0, 0, 0, 0,
            0, 1, 1, 0,
            0, 1, 1, 0,
            0, 0, 0, 0,
            // clang-format on
        }},
        BlockType::O,
    },
    {
        {{
            // clang-format off
            0, 0, 1, 0,
            0, 1, 1, 0,
            0, 0, 1, 0,
            0, 0, 0, 0,
            // clang-format on
        }},
        BlockType::T,
    },
    {
        {{
            // clang-format off
            0, 0, 1, 0,
            0, 0, 1, 0,
            0, 1, 1, 0,
            0, 0, 0, 0,
            // clang-format on
        }},
        BlockType::J,
    },
    {
        {{
            // clang-format off
            0, 1, 0, 0,
            0, 1, 0, 0,
            0, 1, 1, 0,
            0, 0, 0, 0,
            // clang-format on
        }},
        BlockType::L,
    },
};

}

#endif
