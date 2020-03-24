#ifndef TETRIS_BOARD_HPP
#define TETRIS_BOARD_HPP

#include "block_type.hpp"
#include "matrix.hpp"

namespace tetris {
    class Board {
    public:
        constexpr static auto rows = 20;
        constexpr static auto columns = 10;

        using Blocks = geom::Matrix2D<BlockType, rows, columns>;
        using Position = geom::Position;

        Board():
            blocks_{}
        {
            blocks_.fill(BlockType::Empty);
        }

        BlockType operator[](Position pos) const
        {

            return blocks_[{{pos.row, pos.column}}];
        }

        const Blocks& blocks() const
        {
            return blocks_;
        }

    private:
        Blocks blocks_;
    };
}

#endif
