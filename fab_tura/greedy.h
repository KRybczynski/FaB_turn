#pragma once
#include"card.h"
#include<vector>

using namespace std;

enum playMode { pitch, play };

void playCard(Card& to_play, int& mana, int& action_points, int& damage, vector<string>& seq);

void pitchCard(Card& to_pitch, int& mana, vector<string>& seq);


void greedGathering(vector<Card>& hand, int& n, Card to_play, vector<string>& seqence, int& mana);


int findBestCard(vector<Card>& hand, playMode mode);




int greedy(std::vector<Card>& hand, vector<string>& seqence);
bool aBetterThanB(const Card a, const Card b, playMode mode);
