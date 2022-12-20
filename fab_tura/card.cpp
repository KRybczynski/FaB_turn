#include "card.h"





Card loadCard(char* argv[], int start) {
  Card c;
  c.name = argv[start];
  c.cost = std::stoi(argv[++start]);
  c.pitch_value = std::stoi(argv[++start]);
  c.attack = std::stoi(argv[++start]);
  c.go_again = std::stoi(argv[++start]);
  return c;
}