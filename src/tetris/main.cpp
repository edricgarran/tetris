#include <chrono>
#include <iostream>
#include <thread>

#include "assert.hpp"
#include "cursespp.hpp"
#include "block_type.hpp"
#include "board.hpp"


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
                int row_offset = 0,
                int column_offset = 0)
{
    for (auto r = 0; r < tetris::Board::rows; ++r) {
        window.wmove(row_offset + r + 1, column_offset + 1);

        for (auto c = 0; c < tetris::Board::columns; ++c) {
            window.waddch(board_character(board[{r, c}]));
            window.waddch(board_character(board[{r, c}]));
        }
    }
}


int main()
try {
    constexpr int board_width = 10;
    constexpr int board_height = 20;

    auto board = tetris::Board{};

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

        draw_board(board_window, board);

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
