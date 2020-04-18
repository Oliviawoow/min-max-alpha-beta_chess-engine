#include "game.h"
#include "init_game.h"
#include "pieces.h"

Game::Game(char wb) {
  make_print_board_matrix(*this, wb);

  // init my color, board and vector of pieces
  m_color = wb;
  m_board = get_initial_board_matrix(m_color);
	m_pieces = get_initial_positions(m_color);

  // init enemy's color, board and vector of pieces
  if (wb == 0) { // if we are white
    e_color = 1; // enemy is black
  } else {       // if we are black
    e_color = 0; // enemy is white
  }
  e_board = get_initial_board_matrix(e_color);
  e_pieces = get_initial_positions(e_color);
}

void Game::remake_print_board(char wb) {
  make_print_board_matrix(*this, wb);
}

bool Game::is_check_m() {
  char x, y;
  for (auto &v : m_pieces) {
    if (v[2] == 7) {  // my king position
      x = v[0];
      y = v[1];
      break;
    }
  }

  // true - check
  // false - king is safe
  int valid = check_check(x, y, m_board);
  if (valid != 0) return true; 
  return false;
}

bool Game::is_check_e() {
  char x, y;
  for (auto &v : e_pieces) {
    if (v[2] == 7) {  // my king position
      x = v[0];
      y = v[1];
      break;
    }
  }

  // true - check
  // false - king is safe
  int valid = check_check(x, y, e_board);
  if (valid != 0) return true;
  return false;
}

// possible ways of what a piece can attack
// return a vector of {x, y, piece type}
std::vector<std::vector<char>> check_attack(char i, char j, std::vector<std::vector<char>> &chess_board) {
  std::vector<std::vector<char>> possible_attacks;
  int cod_piesa = chess_board[i][j];
  switch (cod_piesa) {
    case 1: // pawn
      if (check_validity(i + 1, j + 1, chess_board) >= 2) {
        possible_attacks.push_back(attacks(i + 1, j + 1, chess_board));
      }
      if (check_validity(i + 1, j - 1, chess_board) >= 2) {
        possible_attacks.push_back(attacks(i + 1, j - 1, chess_board));
      }
      break;
    
    case 2: // rook
      for(auto d : all_directions) {
        if (d != up || d != dw || d != rg || d != lf) continue;
        char x = d.first;
        char y = d.second;
        int ok = 0;
        int temp = 1;
        while(temp != 0 && ok == 0) {
          temp = check_check_validity(i + x, j + y, chess_board);
          if (temp == 2) {
            possible_attacks.push_back(attacks(i + x, j + y, chess_board));
            ok = 1;
          } else if (temp == 0) {
            break;
          } else {
            i += x;
            j += y;
          }
        }
      }
      break;

    case 3: // knight
      if (check_validity(i + 1, j + 2, chess_board) >= 2) {
        possible_attacks.push_back(attacks(i + 1, j + 2, chess_board));
      }
      if (check_validity(i + 2, j + 1, chess_board) >= 2) {
       possible_attacks.push_back(attacks(i + 2, j + 1, chess_board));
      }
      if (check_validity(i + 1, j - 2, chess_board) >= 2) {
        possible_attacks.push_back(attacks(i + 1, j - 2, chess_board));
      }
      if (check_validity(i + 2, j - 1, chess_board) >= 2) {
        possible_attacks.push_back(attacks(i + 2, j - 1, chess_board));
      }
      if (check_validity(i - 1, j + 2, chess_board) >= 2) {
        possible_attacks.push_back(attacks(i - 1, j + 2, chess_board));
      }
      if (check_validity(i - 2, j + 1, chess_board) >= 2) {
        possible_attacks.push_back(attacks(i - 2, j + 1, chess_board));
      }
      if (check_validity(i - 1, j - 2, chess_board) >= 2) {
        possible_attacks.push_back(attacks(i - 1, j - 2, chess_board));
      }
      if (check_validity(i - 2, j - 1, chess_board) >= 2) {
        possible_attacks.push_back(attacks(i - 2, j - 1, chess_board));
      }
      break;

    case 4: // black bishop
    case 5: // white bishop
      for(auto d : all_directions) {
        if (d != ur || d != ul || d != dr || d != dl) continue;
        char x = d.first;
        char y = d.second;
        int ok = 0;
        int temp = 1;
        while(temp != 0 && ok == 0) {
          temp = check_check_validity(i + x, j + y, chess_board);
          if (temp == 2) {
            possible_attacks.push_back(attacks(i + x, j + y, chess_board));
            ok = 1;
          } else if (temp == 0) {
            break;
          } else {
            i += x;
            j += y;
          }
        }
      }
      break;

    case 6: // queen
      for(auto d : all_directions) {
        char x = d.first;
        char y = d.second;
        int ok = 0;
        int temp = 1;
        while(temp != 0 && ok == 0) {
          temp = check_check_validity(i + x, j + y, chess_board);
          if (temp == 2) {
            possible_attacks.push_back(attacks(i + x, j + y, chess_board));
            ok = 1;
          } else if (temp == 0) {
            break;
          } else {
            i += x;
            j += y;
          }
        }
      }
      break;

    case 7: // king
      for (auto d: all_directions) {
        char x = d.first;
        char y = d.second;
        if ((check_check_validity(i + x, j + y, chess_board) == 2)) {
            possible_attacks.push_back(attacks(i + x, j + y, chess_board));
        }
      }
      break;
    
    default:
      break;
  }
  return possible_attacks;
}

std::vector<char> attacks(char i, char j, std::vector<std::vector<char>> &chess_board) {
  int type = chess_board[i][j];
  std::vector<char> move;
  move.push_back(i);
  move.push_back(j);
  move.push_back(type);
  return move;
}

// 0 - safe king
// 1 - king in danger
int check_check(char i, char j, std::vector<std::vector<char>> &chess_board) {
  // check pawn attack
  int cod_piesa = chess_board[i + 1][j + 1];
  if (cod_piesa == 11) return 1;
  cod_piesa = chess_board[i + 1][j - 1];
  if (cod_piesa == 11) return 1;

  // check knight attack
  int temp = check_knight(i, j, chess_board);
  if (temp == 1) return 1;

  for(auto d : all_directions) {
    char x = d.first;
    char y = d.second;
    while (true) {
      temp = check_check_validity(i + x, j + y, chess_board);
      if (temp == 0) return 0; // free way or out of board
      if (temp == 1) return 0; // one of my pieces
      if (temp == 2) {  // enemy piece
        cod_piesa = chess_board[i + x][j + y];
        // check diagonal for queen and bishop attack
        if ((d == ur || d == ul || d == dr || d == dl) && (cod_piesa == 14 || cod_piesa == 15 || cod_piesa == 16)) return 1;
        // check lines for queen and rook attack
        if ((d == up || d == dw || d == rg || d == lf) && (cod_piesa == 12 || cod_piesa == 16)) return 1;
        return 0;
      }
      i += x;
      j += y;
    }
  }
  return 0;
}

// 0 - safe king
// 1 - king in danger
int check_knight(char i, char j, std::vector<std::vector<char>> &chess_board) {
  int cod_piesa = chess_board[i + 1][j + 2];
  if (check_validity(i + 1, j + 2, chess_board) == 2 && cod_piesa == 13) return 1;
  cod_piesa = chess_board[i + 2][j + 1];
  if (check_validity(i + 2, j + 1, chess_board) == 2 && cod_piesa == 13) return 1;
  cod_piesa = chess_board[i + 1][j - 2];
  if (check_validity(i + 1, j - 2, chess_board) == 2 && cod_piesa == 13) return 1;
  cod_piesa = chess_board[i + 2][j - 1];
  if (check_validity(i + 2, j - 1, chess_board) == 2 && cod_piesa == 13) return 1;
  cod_piesa = chess_board[i - 1][j + 2];
  if (check_validity(i - 1, j + 2, chess_board) == 2 && cod_piesa == 13) return 1;
  cod_piesa = chess_board[i - 2][j + 1];
  if (check_validity(i - 2, j + 1, chess_board) == 2 && cod_piesa == 13) return 1;
  cod_piesa = chess_board[i - 1][j - 2];
  if (check_validity(i - 1, j - 2, chess_board) == 2 && cod_piesa == 13) return 1;
  cod_piesa = chess_board[i - 2][j - 1];
  if (check_validity(i - 2, j - 1, chess_board) == 2 && cod_piesa == 13) return 1;
  return 0;
}

// 0 - out of board
// 1 - my pieces
// 2 - enemy pieces
// 3 - free place
int check_check_validity(char i, char j, std::vector<std::vector<char>> &chess_board) {
      if (i < 0 || i >= BOX_LENGTH || j < 0 || j >= BOX_LENGTH) return 0; // out of board
      if (chess_board[i][j] > 10) return 2; // enemy pieces
      if (chess_board[i][j] > 0) return 1; // my pieces
      return 3;
}
