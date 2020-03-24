#ifndef TETRIS_BOARD_HPP
#define TETRIS_BOARD_HPP

#include "block_type.hpp"
#include "matrix.hpp"
#include "tetriminoes.hpp"

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

        bool in_bounds(Position pos)
        {
            return
                (pos.row >= 0 and pos.row < rows) and
                (pos.column >= 0 and pos.column < columns)
            ;
        }

        bool piece_fits(Tetrimino const& tetrimino,
                        Position top_left,
                        geom::Rotation rotation)
        {
            for (auto row = 0; row < 4; ++row) {
                for (auto column = 0; column < 4; ++column) {
                    auto board_position = top_left + Position{row, column};

                    auto solid = tetrimino.shape()[{{row, column}, rotation}];

                    if (solid) {
                        auto position_empty =
                            blocks_[{board_position}] == BlockType::Empty;

                        if (not in_bounds(board_position) or
                            not position_empty) {
                            return false;
                        }
                    }
                }
            }

            return true;
        }

    private:
        Blocks blocks_;
    };
}

#endif
