#include "game.h"
#include "tui_io.h"
#include <locale.h>
#include <wchar.h>

int main() {
	setlocale(LC_CTYPE, "");
	board_t* board = board_init();
	game_state_t state = GAME_STATE_PLAYING;
	while(state == GAME_STATE_PLAYING){
		print_board(board);
		get_move(board);
		state = evaluate_state(board);
		if(state == GAME_STATE_LOST){
			print_board(board);
			wprintf(L"You lost!\n");
			break;
		}
		else if(state == GAME_STATE_WON){
			print_board(board);
			wprintf(L"You won!\n");
			break;
		}
	}
}
