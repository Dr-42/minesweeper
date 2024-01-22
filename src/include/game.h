#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>
#include <stdint.h>

typedef enum game_state_t {
	GAME_STATE_PLAYING,
	GAME_STATE_WON,
	GAME_STATE_LOST
} game_state_t;

typedef struct cell_t {
	bool mine;
	bool revealed;
	bool flagged;
	uint8_t adjacent_mines;
} cell_t;

typedef struct board_t {
	cell_t* cells;
	uint8_t flags_remaining;
	game_state_t state;
} board_t;

typedef enum move_type_t {
	MOVE_TYPE_REVEAL,
	MOVE_TYPE_FLAG,
} move_type_t;

board_t* board_init();
void board_free(board_t *board);
game_state_t make_move(board_t *board, move_type_t move_type, uint8_t x, uint8_t y);

#endif // BOARD_H
