#include <array>
#include <chrono>
#include <thread>

#include "cursespp.hpp"

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


template <int Rows, int Columns>
void draw_to(cursespp::Window& window,
             geom::Matrix2D<char, Rows, Columns> const& matrix,
             int row_offset = 0,
             int column_offset = 0)
{
    for (auto r = 0; r < Rows; ++r) {
        window.move(row_offset + r + 1, column_offset + 1);
        for (auto c = 0; c < Columns; ++c) {
            window.add_character(static_cast<cursespp::Character>(matrix[{r, c}]));
            window.add_character(static_cast<cursespp::Character>(matrix[{r, c}]));
        }
    }
}


int main()
{
    constexpr int board_width = 10;
    constexpr int board_height = 20;

    auto board = geom::Matrix2D<char, board_height, board_width>{};

    for (auto r = 0; r < board_height; ++r) {
        for (auto c = 0; c < board_width; ++c) {
            board[{r, c}] = ' ';
        }
    }

    auto curses = cursespp::Curses{};

    curses.cbreak();
    curses.noecho();
    curses.curs_set(0);
    curses.stdscr().keypad(true);
    curses.stdscr().set_timeout(0);

    curses.stdscr().refresh();

    auto board_window = curses.newwin(board_height + 2, 2*board_width + 2, 0, 0);

    while (true) {
        using namespace std::chrono;
        using namespace std::chrono_literals;

        auto frame_start = high_resolution_clock::now();

        board_window.box(0, 0);

        draw_to(board_window, board);

        board_window.refresh();

        if (auto ch = getch(); ch == 'q') {
            break;
        }

        auto done = high_resolution_clock::now();
        auto remaining_time = (frame_start + 16666us) - done;
        std::this_thread::sleep_for(remaining_time);
    }
}
