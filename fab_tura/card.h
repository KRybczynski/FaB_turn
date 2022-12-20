#pragma once
#include<iostream>
#include<string>


struct Card {
  std::string name;
  int cost, pitch_value, attack, go_again;
  bool used;
  void display() {
    std::cout << name << " cost " << cost << " pitch value " << pitch_value
      << " attack " << attack << " go_again " << go_again << std::endl;
  }
  Card()
    : name(" "),
    cost(9999),
    pitch_value(0),
    attack(0),
    go_again(0),
    used(false) {}
  Card(std::string name, int cost, int pith_value, int attack, int go_again)
    : 
    name(name),
    cost(cost),
    pitch_value(pith_value),
    attack(attack),
    go_again(go_again),
    used(false) {}
  ~Card() {}
};

Card loadCard(char* argv[], int start);