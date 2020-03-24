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


// Fixed, compile-time-sized matrix implementation.
template <typename T, int Rows, int Columns>
class Matrix2D {
private:
    constexpr static auto unsigned_size =
        static_cast<std::size_t>(Rows * Columns);

public:
    // Internal buffer type. Also used for initialization.
    using ContentArray = std::array<T, unsigned_size>;

    // Access information for an element in the matrix based on its 2D
    // position.
    struct Position {
        int row, column;
        MatrixRotation rotation = MatrixRotation::R0;
    };

    // Default constructor.
    //
    // Primitive type matrices won't have their elements initialized.
    constexpr Matrix2D() = default;

    // Construct a matrix with a 1D array with its contents.
    constexpr Matrix2D(ContentArray const& contents):
        contents_{contents}
    {}

    // Access a matrix element.
    //
    // Rotation of the matrix can be specified using `pos.rotation`.
    //
    // Args:
    //     pos: The position to access.
    //
    // Returns:
    //     The element in position `pos`.
    constexpr T& operator[](Position pos)
    {
        return contents_[index_of(pos)];
    }

    // Const version of `operator[]`.
    constexpr T const& operator[](Position pos) const
    {
        return contents_[index_of(pos)];
    }

    // Read-only reference to the contents array.
    //
    // Returns:
    //     Const reference to the internal buffer.
    constexpr ContentArray const& contents() const
    {
        return contents_;
    }

    void fill(const T& value)
    {
        contents_.fill(value);
    }

private:
    struct RotationConstants {
        int top_left, row_multiplier, column_multiplier;
    };

    // Get the constants to use when calculating the position on the inner 1D
    // array, given a 2D position and a rotation.
    //
    // For a 2D RxC matrix, the elements are indexed in a 1D array as follows:
    //
    //      0  1   2  ... C-1
    //      C ... ... ... 2C-1
    //     2C ... ... ... 3C-1
    //    ... ... ... ... ...
    // (R-1)C ... ... ... RC-1
    //
    // Therefore by rotating the matrix the top-left corner (first 1D index)
    // changes, as well as how the index changes when the 2D indices move.
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

    // Calculate the index of a 2D position with a given rotation in the
    // 1D internal array.
    constexpr auto index_of(Position pos) const
    {
        auto constants = rotation_constants(pos.rotation);

        return static_cast<std::size_t>(
            constants.top_left +
            constants.row_multiplier * pos.column +
            constants.column_multiplier * pos.column
        );
    }

    ContentArray contents_;
};
}

#endif
