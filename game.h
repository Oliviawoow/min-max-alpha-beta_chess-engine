#ifndef GAME_H
#define GAME_H

#include <bits/stdc++.h>

class Game {

 public:
  std::vector<std::vector<char*>> _print_board;
  // std::vector<std::vector<char>> positions;

  char m_color = -1;
  std::vector<std::vector<char>> m_board;
	std::vector<std::vector<char>> m_pieces;

  char e_color = -1;
  std::vector<std::vector<char>> e_board;
	std::vector<std::vector<char>> e_pieces;

  Game(char wb);

  void remake_print_board(char wb);

  bool game_over();
  bool is_check_m();
  bool is_check_e();
};

int check_check(char i, char j, std::pair<char, char> &d, std::vector<std::vector<char>> &chess_board);
int check_check_validity(char i, char j, std::vector<std::vector<char>> &chess_board);
#endif // GAME_H