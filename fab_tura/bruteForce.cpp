#include"bruteForce.h"


void append_vector(vector<string>& global, vector<string>& local) {
  for (auto a : local) global.push_back(a);
  return;
}

int bruteForce(vector<Card>& hand, int action_points, int mana, int n,
  vector<string>& sequence) {
  int damage_total = 0, max_damage = 0;
  vector<string> temp, biggest;
  temp.reserve(n);
  biggest.reserve(n);
  damage_total = max_damage = 0;

  if (action_points == 0 || n == 0) {
    return 0;
  }

  for (auto& to_play : hand) {
    if (to_play.used == true)
      continue;

    to_play.used = true;
    temp.push_back("played: " + to_play.name);

    if (mana >= to_play.cost) {
      damage_total = to_play.attack +
        bruteForce(hand, action_points - 1 + to_play.go_again,
          mana - to_play.cost, n - 1, temp);
    }
    if (max_damage < damage_total) {
      biggest.clear();
      append_vector(biggest, temp);
      max_damage = damage_total;
    }
    temp.clear();
    temp.push_back("pitched: " + to_play.name);

    damage_total = bruteForce(hand, action_points, mana + to_play.pitch_value,
      n - 1, temp);
    if (max_damage < damage_total) {
      biggest.clear();
      append_vector(biggest, temp);
      max_damage = damage_total;
    }
    temp.clear();
    to_play.used = false;
  }
  append_vector(sequence, biggest);
  return max_damage;
}