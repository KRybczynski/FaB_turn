#pragma once
#include<vector>
#include<iostream>

#include"card.h"

using namespace std;

int bruteForce(vector<Card>& hand, int action_points, int mana, int n,
  vector<string>& sequence);

void append_vector(vector<string>& global, vector<string>& local);