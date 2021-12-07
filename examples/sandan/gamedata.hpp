#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

#include "abcg.hpp"

enum class Input {
  Right,
  Left,
  Down,
  Up,
  Forward,
  Backward,
  ZoomIn,
  ZoomOut,
  Shot,
  Focus
};
enum class State { Menu, Playing, Finished };

struct GameData {
  State m_state{State::Menu};
  std::bitset<10> m_input;
};

#endif