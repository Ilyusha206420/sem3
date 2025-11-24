#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SIZE 8

typedef struct {
    int row;
    int col;
} Position;

bool is_valid_position(int row, int col) {
    return row >= 0 && row < SIZE && col >= 0 && col < SIZE;
}

bool is_king_in_check(Position king, Position rook, Position opponent_king) {
    if (king.row == rook.row || king.col == rook.col) {
        if (king.row == rook.row) {
            int step = (king.col < rook.col) ? 1 : -1;
            for (int col = king.col + step; col != rook.col; col += step) {
                if (col == opponent_king.col && opponent_king.row == king.row) {
                    return false;
                }
            }
        } 
        else {
            int step = (king.row < rook.row) ? 1 : -1;
            for (int row = king.row + step; row != rook.row; row += step) {
                if (row == opponent_king.row && opponent_king.col == king.col) {
                    return false;
                }
            }
        }
        return true;
    }
    
    int row_diff = abs(king.row - opponent_king.row);
    int col_diff = abs(king.col - opponent_king.col);
    return (row_diff <= 1 && col_diff <= 1);
}

bool is_legal_move(Position new_pos, Position white_king, Position rook) {
    if ((new_pos.row == white_king.row && new_pos.col == white_king.col) ||
        (new_pos.row == rook.row && new_pos.col == rook.col)) {
        return false;
    }
    
    int row_diff = abs(new_pos.row - white_king.row);
    int col_diff = abs(new_pos.col - white_king.col);
    return !(row_diff <= 1 && col_diff <= 1);
}

bool has_legal_moves(Position black_king, Position white_king, Position rook) {
    for (int row = -1; row <= 1; row++) {
        for (int col = -1; col <= 1; col++) {
            if (row == 0 && col == 0) continue;
            
            Position new_pos = {black_king.row + row, black_king.col + col};
            if (is_valid_position(new_pos.row, new_pos.col) &&
                is_legal_move(new_pos, white_king, rook) &&
                !is_king_in_check(new_pos, rook, white_king)) {
                return true;
            }
        }
    }
    return false;
}

bool is_checkmate(Position black_king, Position white_king, Position rook) {
    return is_king_in_check(black_king, rook, white_king) && 
           !has_legal_moves(black_king, white_king, rook);
}

void print_board(Position black_king, Position white_king, Position rook) {
    printf("  a b c d e f g h\n");
    for (int row = 0; row < SIZE; row++) {
        printf("%d ", row + 1);
        for (int col = 0; col < SIZE; col++) {
            if (row == black_king.row && col == black_king.col) {
                printf("K ");
            } else if (row == white_king.row && col == white_king.col) {
                printf("k ");
            } else if (row == rook.row && col == rook.col) {
                printf("R ");
            } else {
                printf(". ");
            }
        }
        printf("\n");
    }
}

bool parse_position(const char* input, Position* pos) {
    if (strlen(input) != 2) return false;
    
    pos->col = tolower(input[0]) - 'a';
    pos->row = input[1] - '1';
    
    return is_valid_position(pos->row, pos->col);
}

bool find_mating_move(Position* white_king, Position* rook, Position black_king) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if ((row != white_king->row || col != white_king->col) &&
                (row != black_king.row || col != black_king.col)) {
                Position new_rook = {row, col};
                if (is_checkmate(black_king, *white_king, new_rook)) {
                    *rook = new_rook;
                    return true;
                }
            }
        }
    }

    for (int row = -1; row <= 1; row++) {
        for (int col = -1; col <= 1; col++) {
            if (row == 0 && col == 0) continue;
            
            Position new_king = {white_king->row + row, white_king->col + col};
            if (is_valid_position(new_king.row, new_king.col) &&
                !(new_king.row == rook->row && new_king.col == rook->col) &&
                !(new_king.row == black_king.row && new_king.col == black_king.col) &&
                !is_king_in_check(new_king, *rook, black_king)) {
                
                if (is_checkmate(black_king, new_king, *rook)) {
                    *white_king = new_king;
                    return true;
                }
            }
        }
    }
    
    return false;
}
