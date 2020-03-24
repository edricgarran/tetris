#include <chrono>
#include <iostream>
#include <thread>
#include <optional>

#include "assert.hpp"
#include "block_type.hpp"
#include "board.hpp"
#include "cursespp.hpp"
#include "matrix.hpp"
#include "tetriminoes.hpp"


// Map block types to curses characters.
//
// Args:
//     type: The type of the block to be rendered.
// Returns:
//     A character representing such block.
cursespp::Character board_character(tetris::BlockType type)
{
    return static_cast<cursespp::Character>(
        " IOSZLJT="[static_cast<std::size_t>(type)]
    );
}


// "Draw" a tetris board.
// Args:
//     window: The window to draw to.
//     board: The tetris board.
//     row_offset: How many rows from the top-left corner to skip before
//                 drawing.
//     column_offset: How many columns from the top-left corner to skip before
//                    drawing.
void draw_board(cursespp::Window& window,
                tetris::Board const& board,
                geom::Position offset = {0, 0})
{
    for (auto r = 0; r < tetris::Board::rows; ++r) {
        window.wmove(offset.row + r + 1, offset.column + 1);

        for (auto c = 0; c < tetris::Board::columns; ++c) {
            window.waddch(board_character(board[{r, c}]));
            window.waddch(board_character(board[{r, c}]));
        }
    }
}


void draw_tetrimino(cursespp::Window& window,
                    tetris::Tetrimino const& tetrimino,
                    geom::MatrixPosition pos)
{
    auto type = tetrimino.type();

    for (auto r = 0; r < 4; ++r) {
        auto window_row = pos.position.row + r + 1;
        auto first_column = 2*(pos.position.column) + 1;

        for (auto c = 0; c < 4; ++c) {
            auto window_column = first_column + 2*c;
            auto solid = tetrimino.shape()[{{r, c}, pos.rotation}];

            if (solid) {
                window.wmove(window_row, window_column);

                window.waddch(board_character(type));
                window.waddch(board_character(type));
            }
        }
    }
}


geom::Rotation next(geom::Rotation rot)
{
    return static_cast<geom::Rotation>(
        (static_cast<int>(rot) + 1) % 4
    );
}

int main()
try {
    auto board = tetris::Board{};

    auto& curses = cursespp::get_curses();
    auto& main_win = curses.get_stdscr();

    curses.cbreak();
    curses.set_noecho();
    curses.curs_set(0);
    main_win.keypad(true);
    main_win.set_timeout(0);

    main_win.wrefresh();

    auto board_window = curses.newwin(board.rows + 2, 2*board.columns + 2, 0, 0);
    board_window.add_box(0, 0);

    auto current_tetrimino_index = std::size_t{1u};
    auto current_position = geom::Position{0, 0};
    auto current_rotation = geom::Rotation::R0;

    while (true) {
        using namespace std::chrono;
        using namespace std::chrono_literals;

        auto frame_start = high_resolution_clock::now();

        auto const& current_tetrimino = tetris::tetriminoes[current_tetrimino_index];

        // Input
        auto ch = main_win.wgetch();

        switch(ch) {
            case 'q': {
                return 0;
            }
        }

        auto maybe_new_rotation = [&]() -> std::optional<geom::Rotation> {
            switch(ch) {
                case KEY_UP: {
                    return next(current_rotation);
                }
                default: {
                    return std::nullopt;
                }
            }
        }();

        auto movement = [&]() -> std::optional<geom::Position> {
            switch(ch) {
                case KEY_DOWN: {
                    return geom::Position{1, 0};
                }
                case KEY_LEFT: {
                    return geom::Position{0, -1};
                }
                case KEY_RIGHT: {
                    return geom::Position{0, 1};
                }
                default: {
                    return std::nullopt;
                }
            }
        }();

        if (movement or maybe_new_rotation) {
            auto new_position = current_position + movement.value_or(geom::Position{0, 0});
            auto new_rotation = maybe_new_rotation.value_or(current_rotation);

            if (board.piece_fits(current_tetrimino,
                                 new_position,
                                 new_rotation)) {
                current_position = new_position;
                current_rotation = new_rotation;
            }
        }

        // Draw
        draw_board(board_window, board);
        draw_tetrimino(board_window, current_tetrimino, {current_position, current_rotation});

        board_window.wrefresh();

        // Sleep for the remainder of the frame.
        auto done = high_resolution_clock::now();
        auto remaining_time = (frame_start + 16666us) - done;
        std::this_thread::sleep_for(remaining_time);
    }
} catch (assertpp::AssertionError const& e){
    if constexpr (assertpp::assertions_enabled) {
        std::clog << e.what() << '\n';
    }
    return 1;
} catch (cursespp::CursesError const& e) {
    std::clog << e.what() << '\n';
    return 1;
} catch (...) {
    std::clog << "Aborted with unknown error.";
    return 1;
}
