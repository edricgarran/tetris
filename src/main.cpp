#include <array>
#include "matrix.hpp"


namespace tetris {

class Tetrimino {
public:
    using Shape = geom::Matrix2D<char, 4, 4>;

    constexpr Tetrimino(Shape shape):
        shape_{shape}
    {}

private:
    Shape shape_;
};

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

int main()
{
    constexpr int field_width = 12;
    constexpr int field_height = 18;

    auto field = geom::Matrix2D<char, field_height, field_width>{};

    for (auto x = 0; x < field_width; ++x) {
        for (auto y = 0; x < field_height; ++x) {
            field[{x, y}] =
                (x == 0 or x == field_width - 1 or y == field_height - 1) ?
                9 :
                0;
        }
    }
}
