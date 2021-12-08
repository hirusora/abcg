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
  Shoot,
  Focus
};
enum class State { Playing, Finished };

struct GameData {
  State m_state{State::Playing};
  std::bitset<10> m_input;

  float m_finishTime;
  abcg::ElapsedTimer m_gameTimer;
  abcg::ElapsedTimer m_restartTimer;
};

#endif