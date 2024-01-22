#include "game.h"

#include <stdlib.h>
#include <time.h>

const int BOARD_SIZE = 8;
const int NUM_MINES = 10;

board_t* board_init() {
	srand(time(NULL));
	board_t *board = malloc(sizeof(board_t));
	cell_t *cells = malloc(sizeof(cell_t) * BOARD_SIZE * BOARD_SIZE);
	
	for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
		cells[i].mine = false;
		cells[i].flagged = false;
		cells[i].revealed = false;
		cells[i].adjacent_mines = 0;
	}

	for (int i = 0; i < NUM_MINES; i++) {
		int x = rand() % BOARD_SIZE;
		int y = rand() % BOARD_SIZE;
		int index = y * BOARD_SIZE + x;
		if (cells[index].mine) {
			i--;
		} else {
			cells[index].mine = true;
		}
	}

	for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
		int x = i % BOARD_SIZE;
		int y = i / BOARD_SIZE;
		int index = y * BOARD_SIZE + x;
		if (cells[index].mine) {
			continue;
		}
		for (int j = -1; j <= 1; j++) {
			for (int k = -1; k <=1; k++) {
				if (j == 0 && k == 0) {
					continue;
				}
				int x2 = x + j;
				int y2 = y + k;
				if (x2 < 0 || x2 >= BOARD_SIZE || y2 < 0 || y2 >= BOARD_SIZE) {
					continue;
				}
				int index2 = y2 * BOARD_SIZE + x2;
				if (cells[index2].mine) {
					cells[index].adjacent_mines++;
				}
			}
		}
	}

	board->cells = cells;
	board->flags_remaining = NUM_MINES;
	board->state = GAME_STATE_PLAYING;
	return board;
}

void board_free(board_t *board) {
	free(board->cells);
	free(board);
}

game_state_t make_move(board_t *board, move_type_t move_type, uint8_t x, uint8_t y) {
	if (board->state != GAME_STATE_PLAYING) {
		return board->state;
	}
	int idx = y * BOARD_SIZE + x;
	cell_t *cell = &board->cells[idx];
	if (cell->revealed) {
		return GAME_STATE_PLAYING;
	}
	
	if (cell->flagged && move_type == MOVE_TYPE_REVEAL) {
		return GAME_STATE_PLAYING;
	}

	if (move_type == MOVE_TYPE_FLAG) {
		if (cell->flagged) {
			cell->flagged = false;
			board->flags_remaining++;
		} else {
			if (board->flags_remaining == 0) {
				return GAME_STATE_PLAYING;
			}
			cell->flagged = true;
			board->flags_remaining--;
		}
		return GAME_STATE_PLAYING;
	}

	if (cell->mine) {
		for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
			if (board->cells[i].mine) {
				board->cells[i].revealed = true;
			}
		}
		board->state = GAME_STATE_LOST;
		return GAME_STATE_LOST;
	}

	cell->revealed = true;

	if (cell->adjacent_mines == 0) {
		for (int j = -1; j <= 1; j++) {
			for (int k = -1; k <=1; k++) {
				if (j == 0 && k == 0) {
					continue;
				}
				int x2 = x + j;
				int y2 = y + k;
				if (x2 < 0 || x2 >= BOARD_SIZE || y2 < 0 || y2 >= BOARD_SIZE) {
					continue;
				}
				make_move(board, MOVE_TYPE_REVEAL, x2, y2);
			}
		}
	}

	if (board->flags_remaining == 0) {
		board->state = GAME_STATE_WON;
	}

	return board->state;
}

