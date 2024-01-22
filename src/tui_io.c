#include "tui_io.h"
#include "game.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

extern const int BOARD_SIZE;
extern const int NUM_MINES;

const wchar_t MINE = L'💣';
const wchar_t FLAG = L'🚩';
const wchar_t UNREVEALED = L'▒';

uint8_t wtoi(wchar_t c){
	switch(c){
		case L'0': return 0;
		case L'1': return 1;
		case L'2': return 2;
		case L'3': return 3;
		case L'4': return 4;
		case L'5': return 5;
		case L'6': return 6;
		case L'7': return 7;
		case L'8': return 8;
		case L'9': return 9;
		default: {
			wprintf(L"Invalid character: %lc\n", c);
			return 255;
		}
	}
}


void print_board(board_t* board){
	wprintf(L"Flags: %d\n", board->flags_remaining);
	wprintf(L"  0 1 2 3 4 5 6 7\n");
	wprintf(L"  ---------------\n");
	for(int i = 0; i < BOARD_SIZE; i++){
		wprintf(L"%d|", i);
		for(int j = 0; j < BOARD_SIZE; j++){
			size_t idx = i * BOARD_SIZE + j;
			if(board->cells[idx].revealed){
				if(board->cells[idx].mine){
					wprintf(L"%lc", MINE);
				} else {
					if (board->cells[idx].adjacent_mines == 0){
						wprintf(L"  ");
					} else {
						wprintf(L"%d ", board->cells[idx].adjacent_mines);
					}
				}
			} else if(board->cells[idx].flagged){
				wprintf(L"%lc", FLAG);
			} else {
				wprintf(L"%lc%lc", UNREVEALED, UNREVEALED);
			}
		}
		wprintf(L"\n");
	}
}

void get_move(board_t* board){
	wprintf(L"Enter move: ");
	wchar_t move = '\0';
	wchar_t x_in = '\0';
	wchar_t y_in = '\0';
	// flush stdin
	wscanf(L"%lc %lc %lc", &move, &x_in, &y_in);
	while(getwchar() != L'\n');
	uint8_t x = wtoi(x_in);
	uint8_t y = wtoi(y_in);
	if(x >= BOARD_SIZE || y >= BOARD_SIZE){
		wprintf(L"Invalid move\n");
		wprintf(L"x and y must be less than %d\n", BOARD_SIZE);
		wprintf(L"x: %d, y: %d\n", x, y);
		return;
	}
	switch(move){
		case 'r': {
			wprintf(L"Revealing cell at %d, %d\n", x, y);
			game_state_t state = make_move(board, MOVE_TYPE_REVEAL, x, y);
			if(state == GAME_STATE_LOST){
				wprintf(L"You lose!\n");
				print_board(board);
			} else if(state == GAME_STATE_WON){
				wprintf(L"You win!\n");
				print_board(board);
			}
		} break;
		case 'f': {
			wprintf(L"Flagging cell at %d, %d\n", x, y);
			game_state_t state = make_move(board, MOVE_TYPE_FLAG, x, y);
			if(state == GAME_STATE_LOST){
				wprintf(L"You lose!\n");
				print_board(board);
			} else if(state == GAME_STATE_WON){
				wprintf(L"You win!\n");
				print_board(board);
			}
		} break;
		default: {
			wprintf(L"Invalid move\n");
			wprintf(L"Valid moves are:\n");
			wprintf(L"r x y - reveal cell at x, y\n");
			wprintf(L"f x y - flag cell at x, y\n");
			wprintf(L"Got: %c %d %d\n", move, x, y);
		} break;
	}
}

