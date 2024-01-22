#include "game.h"
#include "tui_io.h"
#include <locale.h>

int main() {
	setlocale(LC_CTYPE, "");
	board_t* board = board_init();
	game_state_t state = GAME_STATE_PLAYING;
	while(state == GAME_STATE_PLAYING){
		print_board(board);
		get_move(board);
		state = board->state;
	}
}
