#ifndef CURSESPP_CURSESPP_HPP
#define CURSESPP_CURSESPP_HPP

#include <stdexcept>

#include <curses.h>

#include "assert.hpp"

// Thin wrapper over curses.
// Since this is a thin wrapper, we cannot avoid re-exporting curses' API.
// Unfortunately, this includes macros and pollutes the global namespace.

// Methods that forward curses functions abide by the following rules:
// Forwarded curses functions.
// - Functions that may be macros according to the curses manpages are renamed.
// - Functions that aren't macros keep their curses name.
// - Functions that may return error are wrapped with
//   `detail::check_error`.

namespace cursespp {

// Alias for curses' custom character type.
using Character = chtype;

// Error on an ncurses function call.
struct CursesError: std::runtime_error {
    using std::runtime_error::runtime_error;
};

namespace detail {

// Throw an exception in case a curses function signals error.
//
// Args:
//     result: The return of an curses function that returns ERR in
//             case of error.
//     message: What message to pass to the exception.
void check_error(int result, const char* message)
{
    if (result == ERR) {
        throw CursesError{message};
    }
}

}

// A curses window wrapper providing RAII.
//
// Can only be created by a Curses object.
class Window {
public:
    friend class Curses;

    // Constructor, destructor, assignment and swap for proper RAII.

    Window(Window const&) = delete;
    Window& operator=(Window const&) = delete;

    Window(Window&& other):
        window_{other.release_window()}
    {
    }

    Window& operator=(Window&& other)
    {
        auto aux = std::move(other);
        swap(aux);
        return *this;
    }

    void swap(Window& other)
    {
        std::swap(window_, other.window_);
    }

    // Curses forwards

    // Forward `box` (may be a macro)
    void add_box(Character vertical_char, Character horizontal_char)
    {
        ::box(window_, vertical_char, horizontal_char);
    }

    void wmove(int row, int column)
    {
        detail::check_error(
            ::wmove(window_, row, column), "wmove call failed"
        );
    }

    void waddch(Character ch)
    {
        detail::check_error(
            ::waddch(window_, ch),
            "waddch call failed"
        );
    }

    int wgetch()
    {
        return ::wgetch(window_);
    }

    void wrefresh()
    {
        detail::check_error(
            ::wrefresh(window_),
            "wrefresh call failed"
        );
    }

    void keypad(bool enable)
    {
        detail::check_error(
            ::keypad(window_, enable),
            "keypad call failed"
        );
    }

    // Forward `wtimeout` (may be a macro).
    void set_timeout(int delay)
    {
        ::wtimeout(window_, delay);
    }

    ~Window()
    {
        if (window_ and window_ != stdscr) {
            delwin(window_);
        }
    }

private:
    Window(WINDOW* window):
        window_{window}
    {}

    WINDOW* release_window()
    {
        assertpp::assert_predicate(
            [&] { return window_ != stdscr; },
            "stdscr's Window shouldn't be released/moved."
        );

        auto released = window_;
        window_ = nullptr;
        return released;
    }

    WINDOW* window_;
};

class Curses {
public:
    // We should only initialize the library once.
    friend Curses& get_curses();

    ~Curses()
    {
        endwin();
    }

    // Unmovable and uncopiable. Should always be passed by reference.
    Curses(Curses const&) = delete;
    Curses& operator=(Curses const&) = delete;
    Curses(Curses&&) = delete;
    Curses& operator=(Curses&&) = delete;

    // Curses forwards

    void cbreak()
    {
        detail::check_error(::cbreak(), "cbreak call failed");
    }

    // Forward `noecho` (may be a macro).
    void set_noecho()
    {
        detail::check_error(::noecho(), "noecho call failed");
    }

    void curs_set(int visibility)
    {
        detail::check_error(::curs_set(visibility), "curs_set call failed");
    }

    // Get `stdscr` as a Window object.
    //
    // Notice that if you end up moving this you will break everything.
    // Please don't.
    static Window& get_stdscr()
    {
        static Window stdscr_wrapper = Window{::stdscr};
        return stdscr_wrapper;
    }

    Window newwin(int rows, int columns, int begin_x, int begin_y)
    {
        return Window{::newwin(rows, columns, begin_x, begin_y)};
    }

private:
    Curses()
    {
        initscr();
    }
};

inline Curses& get_curses()
{
    static Curses curses{};
    return curses;
}
}

#endif
