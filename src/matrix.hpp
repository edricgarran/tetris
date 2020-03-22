#ifndef GEOM_MATRIX_H
#define GEOM_MATRIX_H

#include <array>
#include <cassert>
#include <tuple>

#include "unreachable.hpp"

namespace geom {

enum class MatrixRotation {
    R0,
    R90,
    R180,
    R270,
};

template <typename T, int Rows, int Columns>
class Matrix2D {
private:
    constexpr static auto unsigned_size =
        static_cast<std::size_t>(Rows * Columns);

    using ContentArray = std::array<T, unsigned_size>;

    struct RotationConstants {
        int top_left, row_multiplier, column_multiplier;
    };

public:
    struct Position {
        int row, column;
        MatrixRotation rotation = MatrixRotation::R0;
    };

    constexpr Matrix2D() = default;

    constexpr Matrix2D(const ContentArray& contents):
        contents_{contents}
    {}

    constexpr T& operator[](Position pos)
    {
        auto constants = rotation_constants(pos.rotation);

        auto array_position = static_cast<std::size_t>(
            constants.top_left +
            constants.row_multiplier * pos.column +
            constants.column_multiplier * pos.column
        );

        return contents_[
            array_position
        ];
    }

private:
    constexpr static RotationConstants rotation_constants(MatrixRotation rotation) {
        switch(rotation) {
            case MatrixRotation::R0: {
                return {
                    0,
                    Rows,
                    1,
                };
            }
            case MatrixRotation::R90: {
                return {
                    (Rows - 1) * Columns,
                    1,
                    -Columns,
                };
            }
            case MatrixRotation::R180: {
                return {
                    (Rows * Columns) - 1,
                    -Columns,
                    -1,
                };
            }
            case MatrixRotation::R270: {
                return {
                    Columns - 1,
                    -1,
                    Columns,
                };
            }
        }

        UTIL_MARK_UNREACHABLE;
    }

    ContentArray contents_;
};
}

#endif
