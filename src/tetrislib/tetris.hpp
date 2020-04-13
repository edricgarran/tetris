#ifndef TETRIS_TETRIS_HPP
#define TETRIS_TETRIS_HPP

#include <functional>
#include <random>
#include <vector>

#include "board.hpp"
#include "unreachable.hpp"

namespace tetris {

class Rng {
public:
    Rng(std::default_random_engine engine): engine_{std::move(engine)} {}

    int get_int()
    {
        return distribution(engine_);
    }

private:
    std::default_random_engine engine_;
    std::uniform_int_distribution<int> distribution{0, 6};
};

struct FallingTetrimino {
    FallingTetrimino(Tetrimino const& t): tetrimino{t} {}

    std::reference_wrapper<Tetrimino const> tetrimino;
    geom::Position position{0, 0};
    geom::Rotation rotation{geom::Rotation::R0};
};

struct GameState {
    GameState(FallingTetrimino t): falling(std::move(t)) {}

    FallingTetrimino falling;
    int clearing_ticks = 0;
    int ticks_to_fall = 20;
    int ticks = 1;
    bool game_over = false;
    std::vector<int> cleared_lines;
};

enum class Input {
    Left,
    Right,
    Down,
    Rotate,
    Nothing,
};

inline Tetrimino const& random_tetrimino(Rng& rng)
{
    return tetriminoes[static_cast<std::size_t>(rng.get_int())];
}

enum class State {
    Default,
    Dropped,
    Clearing,
};

class Tetris {
public:
    Tetris(Rng rng): rng_(std::move(rng)), state{{random_tetrimino(rng_)}} {}

    bool is_over() const
    {
        return state.game_over;
    }

    Board const& board() const
    {
        return board_;
    }

    FallingTetrimino const& falling_tetrimino() const
    {
        return state.falling;
    }

    void advance(Input input)
    {
        if (not state.game_over) {
            auto reset = game_tick(input);
            state.ticks = [&]()
            {
                switch (reset) {
                    case State::Default: {
                        return state.ticks + 1;
                    }
                    case State::Clearing: {
                        return state.ticks;
                    }
                    case State::Dropped: {
                        return 0;
                    }
                }

                UTIL_MARK_UNREACHABLE;
            }();
        }
    }

private:
    void apply_input(Input input);
    void check_for_game_over();
    bool try_drop();
    void lock_tetrimino();
    void pick_new_tetrimino();
    void mark_cleared_lines();
    void clear_lines();
    State game_tick(Input input);

    Rng rng_;
    Board board_;
    GameState state;
};

}

#endif
