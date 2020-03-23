#include <array>
#include <chrono>
#include <iostream>
#include <thread>

#include "assert.hpp"
#include "cursespp.hpp"

#include "matrix.hpp"

namespace tetris {

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


// "Draw" a given char matrix to a curses window.
// Args:
//     window: The window to draw to.
//     matrix: A character matrix to "draw".
//     row_offset: How many rows from the top-left corner to skip before
//                 drawing.
//     column_offset: How many columns from the top-left corner to skip before
//                    drawing.
template <int Rows, int Columns>
void draw_to(cursespp::Window& window,
             geom::Matrix2D<char, Rows, Columns> const& matrix,
             int row_offset = 0,
             int column_offset = 0)
{
    for (auto r = 0; r < Rows; ++r) {
        window.wmove(row_offset + r + 1, column_offset + 1);
        for (auto c = 0; c < Columns; ++c) {
            window.waddch(static_cast<cursespp::Character>(matrix[{r, c}]));
            window.waddch(static_cast<cursespp::Character>(matrix[{r, c}]));
        }
    }
}


int main()
try {
    constexpr int board_width = 10;
    constexpr int board_height = 20;

    auto board = geom::Matrix2D<char, board_height, board_width>{};

    for (auto r = 0; r < board_height; ++r) {
        for (auto c = 0; c < board_width; ++c) {
            board[{r, c}] = ' ';
        }
    }

    auto& curses = cursespp::get_curses();
    auto& main_win = curses.get_stdscr();

    curses.cbreak();
    curses.set_noecho();
    curses.curs_set(0);
    main_win.keypad(true);
    main_win.set_timeout(0);

    main_win.wrefresh();

    auto board_window = curses.newwin(board_height + 2, 2*board_width + 2, 0, 0);

    while (true) {
        using namespace std::chrono;
        using namespace std::chrono_literals;

        auto frame_start = high_resolution_clock::now();

        board_window.add_box(0, 0);

        draw_to(board_window, board);

        board_window.wrefresh();

        if (auto ch = main_win.wgetch(); ch == 'q') {
            break;
        }

        auto done = high_resolution_clock::now();
        auto remaining_time = (frame_start + 16666us) - done;
        std::this_thread::sleep_for(remaining_time);
    }
} catch (assertpp::AssertionError& e){
    if constexpr (assertpp::assertions_enabled) {
        std::clog << e.what() << '\n';
    }
} catch (...) {
    std::clog << "Aborted with unknown error.";
}
