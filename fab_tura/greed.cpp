#include"greedy.h"

void playCard(Card& to_play, int& mana, int& action_points, int& damage, vector<string>& seq) {
  damage += to_play.attack;
  mana -= to_play.cost;
  action_points = action_points - 1 + to_play.go_again;
  to_play.used = true;
  seq.push_back("played: " + to_play.name);
}

void pitchCard(Card& to_pitch, int& mana, vector<string>& seq) {
  to_pitch.used = true;
  mana += to_pitch.pitch_value;
  seq.push_back("pitched: " + to_pitch.name);
}

void greedGathering(vector<Card>& hand, int& n, Card to_play, vector<string>& seqence, int& mana) {
  vector<int> pitched;
  while (mana < to_play.cost && n > 0) {
    int m_index = findBestCard(hand, pitch);
    n--;
    pitchCard(hand[m_index], mana, seqence);
    pitched.push_back(m_index);
  }
  if (mana < to_play.cost) {
    for (auto index : pitched) {
      hand[index].used = false;
      n++;
      mana -= hand[index].pitch_value;
      seqence.pop_back();
    }
  }
}

bool aBetterThanB(const Card a, const Card b, playMode mode) {
  switch (mode)
  {
  case pitch:
    if (a.pitch_value > b.pitch_value)
      return true;

    if (a.pitch_value == b.pitch_value)
      if (a.attack < b.attack)
        return true;

    if (a.pitch_value == b.pitch_value && a.attack == b.attack)
      if (a.cost < b.cost)
        return true;

    return false;
    break;
  case play:
    if (a.attack > b.attack)
      return true;

    if (a.attack == b.attack)
      if (a.cost < b.cost)
        return true;

    if (a.attack == b.attack && a.cost == b.cost)
      if (a.pitch_value < b.pitch_value)
        return true;

    return false;
    break;
  default:
    break;
  }
  return false;
}


int findBestCard(vector<Card>& hand, playMode mode) {
  int
    max_index = -1;
  Card max_value;
  
  for (int i = 0; i < hand.size(); i++) {
    if (hand[i].used == true)
      continue;
    if(aBetterThanB(hand[i], max_value, mode)) {
      max_value = hand[i];
      max_index = i;
    }
  }
  return max_index;
}



int greedy(std::vector<Card>& hand, vector<string>& seqence) {
  int
    damage = 0,
    n = hand.size(),
    mana = 0,
    action_points = 1;

  while (n > 0 && action_points > 0) {
    int max_index = findBestCard(hand, play);
    n--;
    hand[max_index].used = true;
    if (mana >= hand[max_index].cost) {
      playCard(hand[max_index], mana, action_points, damage, seqence);
    }
    else {
      greedGathering(hand, n, hand[max_index], seqence, mana);
      if (mana >= hand[max_index].cost) {
        playCard(hand[max_index], mana, action_points, damage, seqence);
      }
      else {
        pitchCard(hand[max_index], mana, seqence);
      }
    }
  }
  return damage;
}