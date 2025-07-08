#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

enum chess_pieces {KING, QUEEN, ROOK1, ROOK2, BISHOP1, BISHOP2, KNIGHT1, KNIGHT2,
PAWN1, PAWN2, PAWN3, PAWN4, PAWN5, PAWN6, PAWN7, PAWN8};

const char figures[] = {'K', 'Q', 'R', 'R', 'B', 'B', 'k', 'k', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'};

char pos_white[16][3] = {
	"e1", "d1", "a1", "h1", "c1", "f1", "b1", "g1",
	"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2"
};
char pos_black[16][3] = {
	"e8", "d8", "a8", "h8", "c8", "f8", "b8", "g8",
	"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7"
};

void swap(char *c1, char *c2);
void draw_board(void);
int is_taken(char pos[]);
int check_way(char from[], char to[], int piece, int side);
int check_diagonal(char from[], char to[], int piece, int side);
int check_straight(char from[], char to[], int piece, int side);
int check_move(char from[], char to[], int piece, int side);
int is_check(int side);
int gets_to_king(int piece, int side);
int is_mate(int side);
int is_castling(char from[], char to[], int side);
int is_stalemate(int side);
int is_draw(int side);

int main(void) {
	// Завдання з самої умови:
	// enum chess_pieces {KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN};
	// const int piece_value[] = {
	// 	[KING] = 200,
	// 	[QUEEN] = 9,
	// 	[ROOK] = 5,
	// 	[BISHOP] = 3,
	// 	[KNIGHT] = 3,
	// 	[PAWN] = 1
	// };
	while(1) {
		system("clear");
		printf("----------- CHESS GAME -------------\n\n");
		char answer;
		printf("Choose option:\nA - START\tE - EXIT\n");
		scanf("%c", &answer);
		getchar();
		answer = tolower(answer);
		if(answer == 'a') {
			break;
		} else if(answer == 'e') {
			printf("Bye!");
			return 0;
		}
	}

	int running = 1;
	// 0 - white, 1 - black
	int side = 0;
	while(running) {
		int piece;
		char pos_from[3];
		pos_from[2] = '\0';
		system("clear");
		draw_board();
		printf("\nQ - quit\n\n");
		if(side == 0) {
			printf("WHITE\n");
		} else {
			printf("BLACK\n");
		}
		int check = is_check(side);
		if(check) {
			if(is_mate(side)) {
				printf("Check, mate!\n");
				if(side==0) {
					printf("Black wins.\n");
				} else {
					printf("White wins.\n");
				}
				running = 0;
				break;
			} else {
				printf("Check!\n");
			}
		} else if(is_stalemate(side)) {
			printf("Stalemate!\n");
			running = 0;
			break;
		} else if(is_draw(side)) {
			printf("Draw!\n");
			running = 0;
			break;
		}
		int is_empty = 1;
		while(is_empty) {
			printf("Choose position to move from (for example: a1):\n");
			fgets(pos_from, 3, stdin);
			if(tolower(pos_from[0]) == 'q') {
				printf("Bye!");
				exit(0);
			}
			getchar();
			pos_from[strcspn(pos_from, "\n")] = '\0';
			// checking if there are pieces on these coordinates
			if(side == 0) {
				for(int i = 0; i < 16; i++) {
					if(strcmp(pos_from, pos_white[i]) == 0) {
						piece = i;
						is_empty = 0;
					}
				}
			} else {
				for(int i = 0; i < 16; i++) {
					if(strcmp(pos_from, pos_black[i]) == 0) {
						piece = i;
						is_empty = 0;
					}
				}
			}
			if(!is_empty) { // if there is a piece
				char pos_to[3];
				// pos_to[2] = '\0';
				printf("Choose position to move to (for example: a1):\n");
				fgets(pos_to, 3, stdin);
				if(tolower(pos_to[0]) == 'q') {
					printf("Bye!");
					exit(0);
				}
				getchar();
				pos_to[strcspn(pos_to, "\n")] = '\0';
				if(is_castling(pos_from, pos_to, side)) {
					int possible = 1;
					if(check) {
						printf("Your King may be in danger!\n");
						is_empty = 1;
						continue;
					} else {
						if(side == 0) {
							if(strcmp(pos_white[ROOK1], pos_to)==0) {
								for(int piece = 0; piece < 16; piece++) {
									if(!strcmp(pos_white[piece], "b1") || !strcmp(pos_white[piece], "c1") || !strcmp(pos_white[piece], "d1")) {
										possible = 0;
									}
									if(check_move(pos_black[piece], "c1", piece, !side)) {
										possible = 0;
									} else if(check_move(pos_black[piece], "d1", piece, !side)) {
										possible = 0;
									}
								}
								if(possible) {
									strcpy(pos_white[KING], "c1");
									strcpy(pos_white[ROOK1], "d1");
								}
							} else if(strcmp(pos_white[ROOK2], pos_to)==0) {
								for(int piece = 0; piece < 16; piece++) {
									if(!strcmp(pos_white[piece], "f1") || !strcmp(pos_white[piece], "g1")) {
										possible = 0;
									}
									if(check_move(pos_black[piece], "f1", piece, !side)) {
										possible = 0;
									} else if(check_move(pos_black[piece], "g1", piece, !side)) {
										possible = 0;
									}
								}
								if(possible) {
									strcpy(pos_white[KING], "g1");
									strcpy(pos_white[ROOK2], "f1");
								}
							}
						} else {
							if(strcmp(pos_black[ROOK1], pos_to)==0) {
								for(int piece = 0; piece < 16; piece++) {
									if(!strcmp(pos_black[piece], "b8") || !strcmp(pos_black[piece], "c8") || !strcmp(pos_black[piece], "d8")) {
										possible = 0;
									}
									if(check_move(pos_white[piece], "c8", piece, !side)) {
										possible = 0;
									} else if(check_move(pos_white[piece], "d8", piece, !side)) {
										possible = 0;
									}
								}
								if(possible) {
									strcpy(pos_black[KING], "c8");
									strcpy(pos_black[ROOK1], "d8");
								}
							} else if(strcmp(pos_black[ROOK2], pos_to)==0) {
								for(int piece = 0; piece < 16; piece++) {
									if(!strcmp(pos_black[piece], "f8") || !strcmp(pos_black[piece], "g8")) {
										possible = 0;
									}
									if(check_move(pos_white[piece], "f8", piece, !side)) {
										possible = 0;
									} else if(check_move(pos_white[piece], "g8", piece, !side)) {
										possible = 0;
									}
								}
								if(possible) {
									strcpy(pos_black[KING], "g8");
									strcpy(pos_black[ROOK2], "f8");
								}
							}
						}
						if(!possible) {
							printf("Castling is not possible!\n");
							is_empty = 1;
							continue;
						}
					}
				} else if(!check_move(pos_from, pos_to, piece, side)) {
					printf("You can't move from %s to %s!\n", pos_from, pos_to);
					is_empty = 1;
					continue;
				}
				// making a move
				char backup1[3] = {[2] = '\0'};
				char backup2[3] = {[2] = '\0'};
				int piece_backup;
				if(side == 0) {
					strcpy(backup1, pos_white[piece]);
					strcpy(pos_white[piece], pos_to);
					// checking if there is a piece of other side
					for(int j = 0; j < 16; j++) {
						if(strcmp(pos_to, pos_black[j]) == 0) {
							strcpy(backup2, pos_black[j]);
							strcpy(pos_black[j], "");
							piece_backup = j;
						}
					}
					check = is_check(side);
					if(check) {
						printf("2 Your King may be in danger!\n");	
						strcpy(pos_white[piece], backup1);
						if(strcmp(backup2, "")!=0) {
							strcpy(pos_black[piece_backup], backup2);
						}
						is_empty = 1;
					}
				} else {
					strcpy(backup1, pos_black[piece]);
					strcpy(pos_black[piece], pos_to);
					// checking if there is a piece of other side
					for(int j = 0; j < 16; j++) {
						if(strcmp(pos_to, pos_white[j]) == 0) {
							strcpy(backup2, pos_white[j]);
							strcpy(pos_white[j], "");
							piece_backup = j;
						}
					}
					check = is_check(side);
					if(check) {
						printf("Your King may be in danger!\n");
						strcpy(pos_black[piece], backup1);
						if(strcmp(backup2, "")!=0) {
							strcpy(pos_white[piece_backup], backup2);
						}
						is_empty = 1;
					}
				}
			} else {
				printf("There are no pieces of your side on %s\n", pos_from);
			}
		}
		side = !side;
	}

	return 0;
}

void swap(char *c1, char *c2) {
	char temp = *c1;
	*c1 = *c2;
	*c2 = temp;
}
void draw_board(void) {
	printf("+---+---+---+---+---+---+---+---+\n");
	int display_row = 8;
	for(int row = 8; row >= 1; row--) {
		printf("| ");
		for(int col = 'a'; col <= 'h'; col++) {
			char str[3];
			str[0] = col;
			str[1] = (char)(row+48);
			str[2] = '\0';
			int is_empty = 1;
			for(int i = 0; i < 16; i++) {
				if(strcmp(pos_white[i], str) == 0) {
					printf("%c | ", figures[i]);
					is_empty = 0;
				}
			}
			for(int i = 0; i < 16; i++) {
				if(strcmp(pos_black[i], str) == 0) {
					printf("%c | ", figures[i]);
					is_empty = 0;
				}
			}
			if(is_empty) printf("  | ");
		}
		printf("%d\n", display_row--);
		printf("+---+---+---+---+---+---+---+---+\n");
	}
	printf("  a   b   c   d   e   f   g   h\n");
}
int is_taken(char pos[]) {
	for(int i = 0; i < 16; i++) {
		if(strcmp(pos, pos_white[i])==0)
			return 1;
		else if(strcmp(pos, pos_black[i])==0)
			return 2;
	}
	return 0;
}
// check whether there is another piece on the way
int check_way(char from[], char to[], int piece, int side) {
	switch(piece) {
		case KING:
			break;
		case QUEEN:
			if(from[0]==to[0] || from[1]==to[1]) {
				if(!check_straight(from, to, piece, side)) {
					return 0;
				}
			} else if(!check_diagonal(from, to, piece, side)) {
				return 0;
			}
			break;
		case ROOK1:
		case ROOK2:
			if(!check_straight(from, to, piece, side))
				return 0;
			break;
		case BISHOP1:
		case BISHOP2:
			if(!check_diagonal(from, to, piece, side))
				return 0;
			break;
		case KNIGHT1:
		case KNIGHT2:
			break;
		case PAWN1 ... PAWN8:
			break;
		default:
			return 0;
	}
	return 1;
}
int check_diagonal(char from[], char to[], int piece, int side) {
	if(abs(from[0]-to[0])>1) {
		char pos[3];
		pos[2] = '\0';
		if(from[0]>to[0] && from[1]<to[1]) {
			for(int i = from[0], j = from[1]; i > to[0] && j < to[1]; i--, j++) {
				pos[0] = i;
				pos[1] = j;
				if(side==0) {
					if(is_taken(pos)==1 && strcmp(pos, from))
						return 0;
					else if(is_taken(pos)==2 && strcmp(pos, to))
						return 0;
				} else if(side==1) {
					if(is_taken(pos)==2 && strcmp(pos, from))
						return 0;
					else if(is_taken(pos)==1 && strcmp(pos, to))
						return 0;
				}
			}
		} else if(from[0]<to[0] && from[1]<to[1]) {
			for(int i = from[0], j = from[1]; i < to[0] && j < to[1]; i++, j++) {
				pos[0] = i;
				pos[1] = j;
				if(side==0) {
					if(is_taken(pos)==1 && strcmp(pos, from))
						return 0;
					else if(is_taken(pos)==2 && strcmp(pos, to))
						return 0;
				} else if(side==1) {
					if(is_taken(pos)==2 && strcmp(pos, from))
						return 0;
					else if(is_taken(pos)==1 && strcmp(pos, to))
						return 0;
				}
			}
		} else if(from[0]<to[0] && from[1]>to[1]) {
			for(int i = from[0], j = from[1]; i < to[0] && j > to[1]; i++, j--) {
				pos[0] = i;
				pos[1] = j;
				if(side==0) {
					if(is_taken(pos)==1 && strcmp(pos, from))
						return 0;
					else if(is_taken(pos)==2 && strcmp(pos, to))
						return 0;
				} else if(side==1) {
					if(is_taken(pos)==2 && strcmp(pos, from))
						return 0;
					else if(is_taken(pos)==1 && strcmp(pos, to))
						return 0;
				}
			}
		} else if(from[0]>to[0] && from[1]>to[1]) {
			for(int i = from[0], j = from[1]; i > to[0] && j > to[1]; i--, j--) {
				pos[0] = i;
				pos[1] = j;
				if(side==0) {
					if(is_taken(pos)==1 && strcmp(pos, from))
						return 0;
					else if(is_taken(pos)==2 && strcmp(pos, to))
						return 0;
				} else if(side==1) {
					if(is_taken(pos)==2 && strcmp(pos, from))
						return 0;
					else if(is_taken(pos)==1 && strcmp(pos, to))
						return 0;
				}
			}
		}
	}
	return 1;
}
int check_straight(char from[], char to[], int piece, int side) {
	if(abs(from[0]-to[0])>1 || abs(from[1]-to[1])>1) {
		char pos[3];
		pos[2] = '\0';
		if(from[0] == to[0] && from[1]!=to[1]) {
			if(from[1]<to[1]) {
				for(int i = from[1]; i < to[1]; i++) {
					pos[0] = from[0];
					pos[1] = i;
					if(side==0) {
						if(is_taken(pos)==1 && strcmp(pos, from))
							return 0;
						else if(is_taken(pos)==2 && strcmp(pos, to))
							return 0;
					} else if(side==1) {
						if(is_taken(pos)==2 && strcmp(pos, from))
							return 0;
						else if(is_taken(pos)==1 && strcmp(pos, to))
							return 0;
					}
				}
			} else if(from[1]>to[1]) {
				for(int i = from[1]; i > to[1]; i--) {
					pos[0] = from[0];
					pos[1] = i;
					if(side==0) {
						if(is_taken(pos)==1 && strcmp(pos, from))
							return 0;
						else if(is_taken(pos)==2 && strcmp(pos, to))
							return 0;
					} else if(side==1) {
						if(is_taken(pos)==2 && strcmp(pos, from))
							return 0;
						else if(is_taken(pos)==1 && strcmp(pos, to))
							return 0;
					}
				}
			}
		} else if(from[0] != to[0] && from[1]==to[1]) {
			if(from[0]<to[0]) {
				for(int i = from[0]; i < to[0]; i++) {
					pos[0] = i;
					pos[1] = from[1];
					if(side==0) {
						if(is_taken(pos)==1 && strcmp(pos, from))
							return 0;
						else if(is_taken(pos)==2 && strcmp(pos, to))
							return 0;
					} else if(side==1) {
						if(is_taken(pos)==2 && strcmp(pos, from))
							return 0;
						else if(is_taken(pos)==1 && strcmp(pos, to))
							return 0;
					}
				}
			} else if(from[0]>to[0]) {
				for(int i = from[0]; i > to[0]; i--) {
					pos[0] = i;
					pos[1] = from[1];
					if(side==0) {
						if(is_taken(pos)==1 && strcmp(pos, from))
							return 0;
						else if(is_taken(pos)==2 && strcmp(pos, to))
							return 0;
					} else if(side==1) {
						if(is_taken(pos)==2 && strcmp(pos, from))
							return 0;
						else if(is_taken(pos)==1 && strcmp(pos, to))
							return 0;
					}
				}
			}
		}
	}
	return 1;
}
int check_move(char from[], char to[], int piece, int side) {
	if(!check_way(from, to, piece, side)) {
		return 0;
	}
	if(to[0]<'a' || to[0]>'h' || to[1]-48<1 || to[1]-48>8)
		return 0;
	if(side == 0) {
		for(int i = 0; i < 16; i++) {
			if(strcmp(pos_white[i], to) == 0)
				return 0;
		}
	} else {
		for(int i = 0; i < 16; i++) {
			if(strcmp(pos_black[i], to) == 0)
				return 0;
		}
	}
	if(from[0]==to[0] && from[1]==to[1])
		return 0;
	switch(piece) {
		case KING:
			if(abs(from[1]-to[1]) > 1)
				return 0;
			break;
		case QUEEN:
			if(abs(from[0]-to[0])>1 || abs(from[1]-to[1])>1) {
					if(from[0]!=to[0] && from[1]!=to[1]) {
						if(abs(from[0]-to[0])!=abs(from[1]-to[1]))
							return 0;
					}
			}
			break;
		case ROOK1:
		case ROOK2:
			if(from[0]!=to[0]&&from[1]!=to[1])
				return 0;
			break;
		case BISHOP1:
		case BISHOP2:
			if(from[0]==to[0] || from[1]==to[1]) {
				return 0;
			} else if(abs(from[0]-to[0])!=abs(from[1]-to[1])) {
				return 0;
			}
			break;
		case KNIGHT1:
		case KNIGHT2:
			if( (abs(to[0]-96-from[0])!=1 && abs(to[1]-from[1])!=2) && (abs(to[0]-96-from[0])!=2 && abs(to[1]-from[1])!=1) )
				return 0;
			else if(from[0] == to[0] || from[1] == to[1])
				return 0;
			break;
		case PAWN1 ... PAWN8:
			if(side == 0) {
				int taken = 0;
				for(int i = 0; i < 16; i++) {
					if(strcmp(pos_black[i], to) == 0) {
						taken = 1;
					}
				}
				if(!taken && (to[1] <= from[1] || to[0] != from[0])) {
					return 0;
				} else if(taken && to[0] == from[0]) {
					return 0;
				} else if(taken && (abs(from[0]-to[0])>1 || abs(from[1]-to[1])>1) ) {
					return 0;
				} else if((to[1]-from[1] > 2)||(from[1]!='2' && to[1]-from[1] > 1)) {
					return 0;
				}
			} else {
				int taken = 0;
				for(int i = 0; i < 16; i++) {
					if(strcmp(pos_white[i], to) == 0)
						taken = 1;
				}
				if(!taken && (to[1] >= from[1] || to[0] != from[0])) {
					return 0;
				} else if(taken && to[0] == from[0]) {
					return 0;
				} else if(taken && (abs(from[0]-to[0])>1 || abs(from[1]-to[1])>1) ) {
					return 0;
				} else if((from[1]-to[1] > 2)||(from[1]!='7' && from[1]-to[1] > 1)) {
					return 0;
				}
			}
			break;
		default:
			return 0;
	}
	return 1;
}
// check pieces around King (whether there is a check)
int is_check(int side) {
	for(int piece = 0; piece < 16; piece++) {
		if(gets_to_king(piece, !side)) {
			return 1;
		}
	}
	return 0;
}
// side is reversed!!!
int gets_to_king(int piece, int side) {
	char pos[3] = {[2]='\0'};
	char check_pos[3] = {[2]='\0'};
	if(side == 0) {
		strcpy(pos, pos_white[piece]);
		switch(piece) {
			case KING:
				break;
			case QUEEN:
				for(int i = pos[0], j = pos[1]; i >= 'a' && j <= '8'; i--, j++) {
					check_pos[0] = i; check_pos[1] = j;
					if(is_taken(check_pos)==1 && strcmp(check_pos, pos)!=0) {
						break;
					} else if(is_taken(check_pos)==2) {
						if(strcmp(check_pos, pos_black[KING])==0)
							return 1;
						else
							break;
					}
				}
				for(int i = pos[0], j = pos[1]; i <= 'h' && j <= '8'; i++, j++) {
					check_pos[0] = i; check_pos[1] = j;
					if(is_taken(check_pos)==1 && strcmp(check_pos, pos)!=0) {
						break;
					} else if(is_taken(check_pos)==2) {
						if(strcmp(check_pos, pos_black[KING])==0)
							return 1;
						else
							break;
					}
				}
				for(int i = pos[0], j = pos[1]; i <= 'h' && j >= '1'; i++, j--) {
					check_pos[0] = i; check_pos[1] = j;
					if(is_taken(check_pos)==1 && strcmp(check_pos, pos)!=0) {
						break;
					} else if(is_taken(check_pos)==2) {
						if(strcmp(check_pos, pos_black[KING])==0)
							return 1;
						else
							break;
					}
				}
				for(int i = pos[0], j = pos[1]; i >= 'a' && j >= '1'; i--, j--) {
					check_pos[0] = i; check_pos[1] = j;
					if(is_taken(check_pos)==1 && strcmp(check_pos, pos)!=0) {
						break;
					} else if(is_taken(check_pos)==2) {
						if(strcmp(check_pos, pos_black[KING])==0)
							return 1;
						else
							break;
					}
				}
				for(int i = pos[0]; i <= 'h'; i++) {
					check_pos[0] = i; check_pos[1] = pos[1];
					if(is_taken(check_pos)==1 && strcmp(check_pos, pos)!=0) {
						break;
					} else if(is_taken(check_pos)==2) {
						if(strcmp(check_pos, pos_black[KING])==0)
							return 1;
						else
							break;
					}
				}
				for(int i = pos[0]; i >= 'a'; i--) {
					check_pos[0] = i; check_pos[1] = pos[1];
					if(is_taken(check_pos)==1 && strcmp(check_pos, pos)!=0) {
						break;
					} else if(is_taken(check_pos)==2) {
						if(strcmp(check_pos, pos_black[KING])==0)
							return 1;
						else
							break;
					}
				}
				for(int i = pos[1]; i <= '8'; i++) {
					check_pos[0] = pos[0]; check_pos[1] = i;
					if(is_taken(check_pos)==1 && strcmp(check_pos, pos)!=0) {
						break;
					} else if(is_taken(check_pos)==2) {
						if(strcmp(check_pos, pos_black[KING])==0)
							return 1;
						else
							break;
					}
				}
				for(int i = pos[1]; i >= '1'; i--) {
					check_pos[0] = pos[0]; check_pos[1] = i;
					if(is_taken(check_pos)==1 && strcmp(check_pos, pos)!=0) {
						break;
					} else if(is_taken(check_pos)==2) {
						if(strcmp(check_pos, pos_black[KING])==0)
							return 1;
						else
							break;
					}
				}
				break;
			case ROOK1:
			case ROOK2:
				for(int i = pos[0]; i <= 'h'; i++) {
					check_pos[0] = i; check_pos[1] = pos[1];
					if(is_taken(check_pos)==1 && strcmp(check_pos, pos)!=0) {
						break;
					} else if(is_taken(check_pos)==2) {
						if(strcmp(check_pos, pos_black[KING])==0)
							return 1;
						else
							break;
					}
				}
				for(int i = pos[0]; i >= 'a'; i--) {
					check_pos[0] = i; check_pos[1] = pos[1];
					if(is_taken(check_pos)==1 && strcmp(check_pos, pos)!=0) {
						break;
					} else if(is_taken(check_pos)==2) {
						if(strcmp(check_pos, pos_black[KING])==0)
							return 1;
						else
							break;
					}
				}
				for(int i = pos[1]; i <= '8'; i++) {
					check_pos[0] = pos[0]; check_pos[1] = i;
					if(is_taken(check_pos)==1 && strcmp(check_pos, pos)!=0) {
						break;
					} else if(is_taken(check_pos)==2) {
						if(strcmp(check_pos, pos_black[KING])==0)
							return 1;
						else
							break;
					}
				}
				for(int i = pos[1]; i >= '1'; i--) {
					check_pos[0] = pos[0]; check_pos[1] = i;
					if(is_taken(check_pos)==1 && strcmp(check_pos, pos)!=0) {
						break;
					} else if(is_taken(check_pos)==2) {
						if(strcmp(check_pos, pos_black[KING])==0)
							return 1;
						else
							break;
					}
				}
				break;
			case BISHOP1:
			case BISHOP2:
				for(int i = pos[0], j = pos[1]; i >= 'a' && j <= '8'; i--, j++) {
					check_pos[0] = i; check_pos[1] = j;
					if(is_taken(check_pos)==1 && strcmp(check_pos, pos)!=0) {
						break;
					} else if(is_taken(check_pos)==2) {
						if(strcmp(check_pos, pos_black[KING])==0)
							return 1;
						else
							break;
					}
				}
				for(int i = pos[0], j = pos[1]; i <= 'h' && j <= '8'; i++, j++) {
					check_pos[0] = i; check_pos[1] = j;
					if(is_taken(check_pos)==1 && strcmp(check_pos, pos)!=0) {
						break;
					} else if(is_taken(check_pos)==2) {
						if(strcmp(check_pos, pos_black[KING])==0)
							return 1;
						else
							break;
					}
				}
				for(int i = pos[0], j = pos[1]; i <= 'h' && j >= '1'; i++, j--) {
					check_pos[0] = i; check_pos[1] = j;
					if(is_taken(check_pos)==1 && strcmp(check_pos, pos)!=0) {
						break;
					} else if(is_taken(check_pos)==2) {
						if(strcmp(check_pos, pos_black[KING])==0)
							return 1;
						else
							break;
					}
				}
				for(int i = pos[0], j = pos[1]; i >= 'a' && j >= '1'; i--, j--) {
					check_pos[0] = i; check_pos[1] = j;
					if(is_taken(check_pos)==1 && strcmp(check_pos, pos)!=0) {
						break;
					} else if(is_taken(check_pos)==2) {
						if(strcmp(check_pos, pos_black[KING])==0)
							return 1;
						else
							break;
					}
				}
				break;
			case KNIGHT1:
			case KNIGHT2:
				strcpy(pos, pos_white[piece]);
				pos[0]-=2; pos[1]+=1;
				if(strcmp(pos, pos_black[KING])==0)
					return 1;

				strcpy(pos, pos_white[piece]);
				pos[0]-=1; pos[1]+=2;
				if(strcmp(pos, pos_black[KING])==0)
					return 1;

				strcpy(pos, pos_white[piece]);
				pos[0]+=1; pos[1]+=2;
				if(strcmp(pos, pos_black[KING])==0)
					return 1;

				strcpy(pos, pos_white[piece]);
				pos[0]+=2; pos[1]+=1;
				if(strcmp(pos, pos_black[KING])==0)
					return 1;

				strcpy(pos, pos_white[piece]);
				pos[0]+=2; pos[1]-=1;
				if(strcmp(pos, pos_black[KING])==0)
					return 1;

				strcpy(pos, pos_white[piece]);
				pos[0]+=1; pos[1]-=2;
				if(strcmp(pos, pos_black[KING])==0)
					return 1;

				strcpy(pos, pos_white[piece]);
				pos[0]-=1; pos[1]-=2;
				if(strcmp(pos, pos_black[KING])==0)
					return 1;

				strcpy(pos, pos_white[piece]);
				pos[0]-=2; pos[1]-=1;
				if(strcmp(pos, pos_black[KING])==0)
					return 1;
				break;
			case PAWN1 ... PAWN8:
				strcpy(pos, pos_white[piece]);
				pos[0]+=1; pos[1]+=1;
				if(strcmp(pos, pos_black[KING])==0)
					return 1;

				strcpy(pos, pos_white[piece]);
				pos[0]+=1; pos[1]-=1;
				if(strcmp(pos, pos_black[KING])==0)
					return 1;

				strcpy(pos, pos_white[piece]);
				pos[0]-=1; pos[1]-=1;
				if(strcmp(pos, pos_black[KING])==0)
					return 1;

				strcpy(pos, pos_white[piece]);
				pos[0]-=1; pos[1]+=1;
				if(strcmp(pos, pos_black[KING])==0)
					return 1;
				break;
			default:
				return 0;
		}
	} else {
		strcpy(pos, pos_black[piece]);
		switch(piece) {
			case KING:
				break;
			case QUEEN:
				for(int i = pos[0], j = pos[1]; i >= 'a' && j <= '8'; i--, j++) {
					check_pos[0] = i; check_pos[1] = j;
					if(is_taken(check_pos)==2 && strcmp(check_pos, pos)!=0) {
						break;
					} else if(is_taken(check_pos)==1) {
						if(strcmp(check_pos, pos_white[KING])==0)
							return 1;
						else
							break;
					}
				}
				for(int i = pos[0], j = pos[1]; i <= 'h' && j <= '8'; i++, j++) {
					check_pos[0] = i; check_pos[1] = j;
					if(is_taken(check_pos)==2 && strcmp(check_pos, pos)!=0) {
						break;
					} else if(is_taken(check_pos)==1) {
						if(strcmp(check_pos, pos_white[KING])==0)
							return 1;
						else
							break;
					}
				}
				for(int i = pos[0], j = pos[1]; i <= 'h' && j >= '1'; i++, j--) {
					check_pos[0] = i; check_pos[1] = j;
					if(is_taken(check_pos)==2 && strcmp(check_pos, pos)!=0) {
						break;
					} else if(is_taken(check_pos)==1) {
						if(strcmp(check_pos, pos_white[KING])==0)
							return 1;
						else
							break;
					}
				}
				for(int i = pos[0], j = pos[1]; i >= 'a' && j >= '1'; i--, j--) {
					check_pos[0] = i; check_pos[1] = j;
					if(is_taken(check_pos)==2 && strcmp(check_pos, pos)!=0) {
						break;
					} else if(is_taken(check_pos)==1) {
						if(strcmp(check_pos, pos_white[KING])==0)
							return 1;
						else
							break;
					}
				}
				for(int i = pos[0]; i <= 'h'; i++) {
					check_pos[0] = i; check_pos[1] = pos[1];
					if(is_taken(check_pos)==2 && strcmp(check_pos, pos)!=0) {
						break;
					} else if(is_taken(check_pos)==1) {
						if(strcmp(check_pos, pos_white[KING])==0)
							return 1;
						else
							break;
					}
				}
				for(int i = pos[0]; i >= 'a'; i--) {
					check_pos[0] = i; check_pos[1] = pos[1];
					if(is_taken(check_pos)==2 && strcmp(check_pos, pos)!=0) {
						break;
					} else if(is_taken(check_pos)==1) {
						if(strcmp(check_pos, pos_white[KING])==0)
							return 1;
						else
							break;
					}
				}
				for(int i = pos[1]; i <= '8'; i++) {
					check_pos[0] = pos[0]; check_pos[1] = i;
					if(is_taken(check_pos)==2 && strcmp(check_pos, pos)!=0) {
						break;
					} else if(is_taken(check_pos)==1) {
						if(strcmp(check_pos, pos_white[KING])==0)
							return 1;
						else
							break;
					}
				}
				for(int i = pos[1]; i >= '1'; i--) {
					check_pos[0] = pos[0]; check_pos[1] = i;
					if(is_taken(check_pos)==2 && strcmp(check_pos, pos)!=0) {
						break;
					} else if(is_taken(check_pos)==1) {
						if(strcmp(check_pos, pos_white[KING])==0)
							return 1;
						else
							break;
					}
				}
				break;
			case ROOK1:
			case ROOK2:
				for(int i = pos[0]; i <= 'h'; i++) {
					check_pos[0] = i; check_pos[1] = pos[1];
					if(is_taken(check_pos)==2 && strcmp(check_pos, pos)!=0) {
						break;
					} else if(is_taken(check_pos)==1) {
						if(strcmp(check_pos, pos_white[KING])==0)
							return 1;
						else
							break;
					}
				}
				for(int i = pos[0]; i >= 'a'; i--) {
					check_pos[0] = i; check_pos[1] = pos[1];
					if(is_taken(check_pos)==2 && strcmp(check_pos, pos)!=0) {
						break;
					} else if(is_taken(check_pos)==1) {
						if(strcmp(check_pos, pos_white[KING])==0)
							return 1;
						else
							break;
					}
				}
				for(int i = pos[1]; i <= '8'; i++) {
					check_pos[0] = pos[0]; check_pos[1] = i;
					if(is_taken(check_pos)==2 && strcmp(check_pos, pos)!=0) {
						break;
					} else if(is_taken(check_pos)==1) {
						if(strcmp(check_pos, pos_white[KING])==0)
							return 1;
						else
							break;
					}
				}
				for(int i = pos[1]; i >= '1'; i--) {
					check_pos[0] = pos[0]; check_pos[1] = i;
					if(is_taken(check_pos)==2 && strcmp(check_pos, pos)!=0) {
						break;
					} else if(is_taken(check_pos)==1) {
						if(strcmp(check_pos, pos_white[KING])==0)
							return 1;
						else
							break;
					}
				}
				break;
			case BISHOP1:
			case BISHOP2:
				for(int i = pos[0], j = pos[1]; i >= 'a' && j <= '8'; i--, j++) {
					check_pos[0] = i; check_pos[1] = j;
					if(is_taken(check_pos)==2 && strcmp(check_pos, pos)!=0) {
						break;
					} else if(is_taken(check_pos)==1) {
						if(strcmp(check_pos, pos_white[KING])==0)
							return 1;
						else
							break;
					}
				}
				for(int i = pos[0], j = pos[1]; i <= 'h' && j <= '8'; i++, j++) {
					check_pos[0] = i; check_pos[1] = j;
					if(is_taken(check_pos)==2 && strcmp(check_pos, pos)!=0) {
						break;
					} else if(is_taken(check_pos)==1) {
						if(strcmp(check_pos, pos_white[KING])==0)
							return 1;
						else
							break;
					}
				}
				for(int i = pos[0], j = pos[1]; i <= 'h' && j >= '1'; i++, j--) {
					check_pos[0] = i; check_pos[1] = j;
					if(is_taken(check_pos)==2 && strcmp(check_pos, pos)!=0) {
						break;
					} else if(is_taken(check_pos)==1) {
						if(strcmp(check_pos, pos_white[KING])==0)
							return 1;
						else
							break;
					}
				}
				for(int i = pos[0], j = pos[1]; i >= 'a' && j >= '1'; i--, j--) {
					check_pos[0] = i; check_pos[1] = j;
					if(is_taken(check_pos)==2 && strcmp(check_pos, pos)!=0) {
						break;
					} else if(is_taken(check_pos)==1) {
						if(strcmp(check_pos, pos_white[KING])==0)
							return 1;
						else
							break;
					}
				}
				break;
			case KNIGHT1:
			case KNIGHT2:
				strcpy(pos, pos_black[piece]);
				pos[0]-=2; pos[1]+=1;
				if(strcmp(pos, pos_white[KING])==0)
					return 1;

				strcpy(pos, pos_black[piece]);
				pos[0]-=1; pos[1]+=2;
				if(strcmp(pos, pos_white[KING])==0)
					return 1;

				strcpy(pos, pos_black[piece]);
				pos[0]+=1; pos[1]+=2;
				if(strcmp(pos, pos_white[KING])==0)
					return 1;

				strcpy(pos, pos_black[piece]);
				pos[0]+=2; pos[1]+=1;
				if(strcmp(pos, pos_white[KING])==0)
					return 1;

				strcpy(pos, pos_black[piece]);
				pos[0]+=2; pos[1]-=1;
				if(strcmp(pos, pos_white[KING])==0)
					return 1;

				strcpy(pos, pos_black[piece]);
				pos[0]+=1; pos[1]-=2;
				if(strcmp(pos, pos_white[KING])==0)
					return 1;

				strcpy(pos, pos_black[piece]);
				pos[0]-=1; pos[1]-=2;
				if(strcmp(pos, pos_white[KING])==0)
					return 1;

				strcpy(pos, pos_black[piece]);
				pos[0]-=2; pos[1]-=1;
				if(strcmp(pos, pos_white[KING])==0)
					return 1;
				break;
			case PAWN1 ... PAWN8:
				strcpy(pos, pos_black[piece]);
				pos[0]+=1; pos[1]+=1;
				if(strcmp(pos, pos_white[KING])==0)
					return 1;

				strcpy(pos, pos_black[piece]);
				pos[0]+=1; pos[1]-=1;
				if(strcmp(pos, pos_white[KING])==0)
					return 1;

				strcpy(pos, pos_black[piece]);
				pos[0]-=1; pos[1]-=1;
				if(strcmp(pos, pos_white[KING])==0)
					return 1;

				strcpy(pos, pos_black[piece]);
				pos[0]-=1; pos[1]+=1;
				if(strcmp(pos, pos_white[KING])==0)
					return 1;
				break;
			default:
				return 0;
		}
	}
	return 0;
}
// Перебирать ходы каждой фигуры и, если ходов нет, возвращать 1 (мат)	
int is_mate(int side) {
	char backup[16][3];
	for(int i = 0; i < 16; i++) {
		if(side==0)
			strcpy(backup[i], pos_white[i]);
		else
			strcpy(backup[i], pos_black[i]);
	}
	char from[3] = {[2]='\0'};
	char to[3] = {[2]='\0'};
	for(int piece = 0; piece < 16; piece++) {
		for(int i = 0; i < 16; i++) {
			if(side==0)
				strcpy(pos_white[i], backup[i]);
			else
				strcpy(pos_black[i], backup[i]);
		}
		if(side == 0) {
			if(!strcmp(pos_white[piece], ""))
				continue;
			else
				strcpy(from, pos_white[piece]);
		} else {
			if(!strcmp(pos_black[piece], ""))
				continue;
			else
				strcpy(from, pos_black[piece]);
		}
		for(int row = '1'; row<='8';row++) {
			to[1]=row;
			for(int col = 'a'; col<='h'; col++) {
				to[0]=col;
				if(!strcmp(to, from))
					continue;
				// making a move to check if there 
				if(check_move(from, to, piece, side)) {
					if(side==0) {
						strcpy(pos_white[piece], to);
					} else {
						strcpy(pos_black[piece], to);
					}
					for(int i = 0; i < 16; i++) {
						if(side==0) {
							if(!strcmp(pos_black[i], to))
								strcpy(pos_black[i], "");
						} else {
							if(!strcmp(pos_white[i], to))
								strcpy(pos_white[i], "");
						}
					}
					if(!is_check(side)) {
						for(int i = 0; i < 16; i++) {
							if(side==0)
								strcpy(pos_white[i], backup[i]);
							else
								strcpy(pos_black[i], backup[i]);
						}
						return 0;
					}
				}
			}
		}
	}
	for(int i = 0; i < 16; i++) {
		if(side==0) {
			printf("%s ", backup[i]);
			strcpy(pos_white[i], backup[i]);
		}
		else {
			strcpy(pos_black[i], backup[i]);
		}
	}
	printf("\n");
	return 1;
}
int is_castling(char from[], char to[], int side) {
	if(side == 0) {
		if(strcmp(from, pos_white[KING])==0 && strcmp(to, pos_white[ROOK1])==0) {
			if(strcmp(pos_white[KING], "e1")==0 && strcmp(pos_white[ROOK1], "a1")==0) {
				return 1;
			}
		} else if(strcmp(from, pos_white[KING])==0 && strcmp(to, pos_white[ROOK2])==0) {
			if(strcmp(pos_white[KING], "e1")==0 && strcmp(pos_white[ROOK2], "h1")==0) {
				return 1;
			}
		}
	} else {
		if(strcmp(from, pos_black[KING])==0 && strcmp(to, pos_black[ROOK1])==0) {
			if(strcmp(pos_black[KING], "e8")==0 && strcmp(pos_black[ROOK1], "a8")==0) {
				return 1;
			}
		} else if(strcmp(from, pos_black[KING])==0 && strcmp(to, pos_black[ROOK2])==0) {
			if(strcmp(pos_black[KING], "e8")==0 && strcmp(pos_black[ROOK2], "h8")==0) {
				return 1;
			}
		}
	}
	return 0;
}
// пат
int is_stalemate(int side) {
	char pieces[16][3];
	char from[3] = {[2]='\0'};
	char to[3] = {[2]='\0'};
	for(int i = 0; i < 16; i++) {
		if(side == 0)
			strcpy(pieces[i], pos_white[i]);
		else
			strcpy(pieces[i], pos_black[i]);
	}
	for(int piece = 0; piece < 16; piece++) {
		if(side == 0) {
			if(strcmp(pos_white[piece], ""))
				strcpy(from, pos_white[piece]);
			else
				continue;
		}
		else {
			if(strcmp(pos_black[piece], ""))
				strcpy(from, pos_black[piece]);
			else
				continue;
		}
		for(int row = '1'; row <= '8'; row++) {
			to[1] = row;
			for(int col = 'a'; col <= 'h'; col++) {
				to[0] = col;
				if(check_move(from, to, piece, side)) {
					if(!is_mate(side))
						return 0;
				}
			}
		}
	}
	return 1;
}
int is_draw(int side) {
	for(int piece = 1; piece < 16; piece++) {
		if(side == 0) {
			if(strcmp(pos_white[piece], ""))
				return 0;
		} else {
			if(strcmp(pos_black[piece], ""))
				return 0;
		}
	}
	return 1;
}