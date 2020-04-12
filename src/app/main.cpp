#include <chrono>
#include <iostream>
#include <thread>
#include <optional>
#include <random>
#include <variant>

#include "assert.hpp"
#include "block_type.hpp"
#include "board.hpp"
#include "cursespp.hpp"
#include "matrix.hpp"
#include "tetriminoes.hpp"
#include "tetris.hpp"


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
                    tetris::FallingTetrimino const& falling)
{
    auto& tetrimino = falling.tetrimino.get();
    auto type = tetrimino.type();

    for (auto r = 0; r < 4; ++r) {
        auto window_row = falling.position.row + r + 1;
        auto first_column = 2*(falling.position.column) + 1;

        for (auto c = 0; c < 4; ++c) {
            auto window_column = first_column + 2*c;
            auto solid = tetrimino.shape()[{{r, c}, falling.rotation}];

            if (solid) {
                window.wmove(window_row, window_column);

                window.waddch(board_character(type));
                window.waddch(board_character(type));
            }
        }
    }
}


int main()
try {
    auto& curses = cursespp::get_curses();
    auto& main_win = curses.get_stdscr();

    curses.cbreak();
    curses.set_noecho();
    curses.curs_set(0);
    main_win.keypad(true);
    main_win.set_timeout(0);

    main_win.wrefresh();

    auto rd = std::random_device{};
    auto engine = std::default_random_engine{rd()};
    auto game = tetris::Tetris{std::move(engine)};

    auto board_window = curses.newwin(game.board().rows + 2, 2*game.board().columns + 2, 0, 0);
    board_window.add_box(0, 0);

    while (not game.is_over()) {
        using namespace std::chrono;
        using namespace std::chrono_literals;

        auto frame_start = high_resolution_clock::now();

        // Input
        auto ch = main_win.wgetch();

        switch(ch) {
            case 'q': {
                return 0;
            }
        }

        auto input = [&]()
        {
            switch(ch) {
                case KEY_UP: {
                    return tetris::Input::Rotate;
                }
                case KEY_DOWN: {
                    return tetris::Input::Down;
                }
                case KEY_LEFT: {
                    return tetris::Input::Left;
                }
                case KEY_RIGHT: {
                    return tetris::Input::Right;
                }
                default: {
                    return tetris::Input::Nothing;
                }
            }
        }();

        // Tick
        game.advance(input);

        // Draw
        draw_board(board_window, game.board());
        draw_tetrimino(board_window, game.falling_tetrimino());

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
