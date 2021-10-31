#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

#include "abcg.hpp"

enum class Input { Right, Left, Down, Up, Fire, Focus };
enum class State { Playing, Finished };

struct GameData {
  State m_state{State::Playing};
  std::bitset<6> m_input;  // [focus, fire, up, down, left, right]

  abcg::ElapsedTimer m_gameTimer;
  float m_finishTime;
};

#endif