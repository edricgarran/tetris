#ifndef CURSESPP_CURSESPP_HPP
#define CURSESPP_CURSESPP_HPP

#include <stdexcept>

#include <curses.h>


namespace cursespp {
    using Character = chtype;

    class CursesError: std::runtime_error {
        using std::runtime_error::runtime_error;
    };

    namespace detail {
        void check_error(int result, const char* message)
        {
            if (result == ERR) {
                throw CursesError{message};
            }
        }
    }

    class Curses;

    class Window {
    public:
        friend class Curses;

        Window(Window const&) = delete;
        Window& operator=(Window const&) = delete;

        Window(Window&& other):
            window_{other.window_}
        {
            other.window_ = nullptr;
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

        void box(Character vertical_char, Character horizontal_char)
        {
            ::box(window_, vertical_char, horizontal_char);
        }

        void move(int row, int column)
        {
            detail::check_error(
                wmove(window_, row, column), "wmove call failed"
            );
        }

        void add_character(Character ch)
        {
            detail::check_error(
                waddch(window_, ch),
                "waddch call failed"
            );
        }

        void refresh()
        {
            detail::check_error(
                wrefresh(window_),
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

        void set_timeout(int delay)
        {
            timeout(delay);
        }

        ~Window()
        {
            if (window_ and window_ != stdscr) {
                delwin(window_);
            }
        }

    private:
        Window(int rows, int columns, int begin_x, int begin_y):
            window_{newwin(rows, columns, begin_x, begin_y)}
        {}

        Window(WINDOW* window):
            window_{window}
        {

        }

        WINDOW* window_;
    };

    class Curses {
    public:
        Curses()
        {
            initscr();
        }

        ~Curses()
        {
            endwin();
        }

        void cbreak()
        {
            detail::check_error(::cbreak(), "cbreak call failed");
        }

        void noecho()
        {
            detail::check_error(::noecho(), "noecho call failed");
        }

        void curs_set(int visibility)
        {
            detail::check_error(::curs_set(visibility), "curs_set call failed");
        }

        static Window& stdscr()
        {
            static Window stdscr_wrapper = Window{::stdscr};
            return stdscr_wrapper;
        }

        Curses(Curses const&) = delete;
        Curses& operator=(Curses const&) = delete;
        Curses(Curses&&) = delete;
        Curses& operator=(Curses&&) = delete;

        Window newwin(int rows, int columns, int begin_x, int begin_y)
        {
            return Window(rows, columns, begin_x, begin_y);
        }

    private:
        static void check_error(int result, const char* message)
        {
            if (result == ERR) {
                throw CursesError{message};
            }
        }
    };
}

#endif
