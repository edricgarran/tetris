#include "tetris.hpp"

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

bool Tetris::game_tick(Input input)
{
    apply_input(input);

    if (state.ticks < state.ticks_to_fall) {
        return false;
    }

    if (try_drop()) {
        return true;
    }

    lock_tetrimino();
    // TODO: Clear lines.
    pick_new_tetrimino();
    check_for_game_over();

    return true;
}

}
