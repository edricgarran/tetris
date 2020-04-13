#include "tetris.hpp"

#include <vector>

#include "containers.hpp"


namespace {

geom::Rotation next(geom::Rotation rot)
{
    return static_cast<geom::Rotation>(
        (static_cast<int>(rot) + 1) % 4
    );
}

}

namespace tetris {

void Tetris::apply_input(Input input)
{
    auto maybe_new_rotation = [&]() -> std::optional<geom::Rotation> {
        switch(input) {
            case Input::Rotate: {
                return next(state.falling.rotation);
            }
            default: {
                return std::nullopt;
            }
        }
    }();

    auto maybe_movement = [&]() -> std::optional<geom::Position> {
        switch(input) {
            case Input::Down: {
                return geom::Position{1, 0};
            }
            case Input::Left: {
                return geom::Position{0, -1};
            }
            case Input::Right: {
                return geom::Position{0, 1};
            }
            default: {
                return std::nullopt;
            }
        }
    }();

    if (maybe_movement or maybe_new_rotation) {
        auto new_position = state.falling.position + maybe_movement.value_or(geom::Position{0, 0});
        auto new_rotation = maybe_new_rotation.value_or(state.falling.rotation);

        if (board_.piece_fits(state.falling.tetrimino,
                             new_position,
                             new_rotation)) {
            state.falling.position = new_position;
            state.falling.rotation = new_rotation;
        }
    }
}

void Tetris::check_for_game_over()
{
    state.game_over = not board_.piece_fits(state.falling.tetrimino,
                                            state.falling.position,
                                            state.falling.rotation);
}

bool Tetris::try_drop()
{
    auto down = state.falling.position + geom::Position{1, 0};

    if (not board_.piece_fits(state.falling.tetrimino, down, state.falling.rotation)) {
        return false;
    }

    state.falling.position = down;
    return true;
}

void Tetris::pick_new_tetrimino()
{
    state.falling = FallingTetrimino{random_tetrimino(rng_)};
}

void Tetris::lock_tetrimino()
{
    auto& tetrimino = state.falling.tetrimino.get();

    for (auto r = 0; r < 4; ++r) {
        for (auto c = 0; c < 4; ++c) {
            if (tetrimino.shape()[{{r, c}, state.falling.rotation}]) {
                board_[state.falling.position + geom::Position{r, c}] = tetrimino.type();
            }
        }
    }
}

void Tetris::mark_cleared_lines()
{
    for (auto r = 0; r < 4; ++r) {
        auto row = r + state.falling.position.row;

        auto full = [&]()
        {
            for (auto c = 0; c < board_.columns; ++c) {
                if (board_[{row, c}] == BlockType::Empty) {
                    return false;
                }
            }

            return true;

        }();

        if (full) {
            state.cleared_lines.push_back(row);
        }
    }

    if (not state.cleared_lines.empty()) {
        for (auto row: state.cleared_lines) {
            for (auto c = 0; c < board_.columns; ++c) {
                board_[{row, c}] = BlockType::Line;
            }
        }

        state.clearing_ticks = state.ticks_to_fall;
    }
}


void Tetris::clear_lines()
{
    auto writing_row = board_.rows - 1;

    for (auto row = writing_row; row >= 0; --row) {
        if (util::contains(state.cleared_lines, row)) {
            continue;
        }

        for (auto c = 0; c < board_.columns; ++c) {
            board_[{writing_row, c}] = board_[{row, c}];
        }

        --writing_row;
    }

    state.cleared_lines.clear();
}

State Tetris::game_tick(Input input)
{
    if (state.clearing_ticks > 0) {
         --state.clearing_ticks;
        return State::Clearing;
    }

    if (not state.cleared_lines.empty()) {
        clear_lines();
    }

    apply_input(input);

    if (state.ticks < state.ticks_to_fall) {
        return State::Default;
    }

    if (try_drop()) {
        return State::Dropped;
    }

    lock_tetrimino();
    mark_cleared_lines();
    pick_new_tetrimino();
    check_for_game_over();

    return State::Dropped;
}

}
