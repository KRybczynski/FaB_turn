#include <iostream>
#include <string>
#include <vector>
#include<chrono>

#include"card.h"
#include"bruteForce.h"
#include"greedy.h"

using namespace std;

void printVectorS(vector<string> a) {
  for (auto el : a) cout << el << " ";
  cout << endl;
}

void cleanHand(vector<Card>& hand) {
  for (auto card : hand) {
    card.used = false;
  }
}

void oneExample(int argc, char* argv[]) {
  std::vector<Card> hand;
  vector<string> g_biggest;
  int output;


  for (int i = 1; i < argc; i += 5) {
    hand.push_back(loadCard(argv, i));
  }

  /*
  hand.push_back(Card("Cl", 2, 0, 4, 0));  // added weapon
  hand.push_back(Card("CnC01", 2, 1, 6, 0));
  hand.push_back(Card("SSP03", 0, 3, 0, 3));
  hand.push_back(Card("SS02", 1, 2, 6, 0));
  hand.push_back(Card("BW02", 3, 2, 6, 0));
  /*
  for (auto a : hand) {
    a.display();
  }

  hand.push_back(Card("cardNumber5", 6, 2, 2, 3));
  hand.push_back(Card("cardNumber1", 3, 1, 9, 0));
  hand.push_back(Card("cardNumber3", 2, 2, 12, 3));
  hand.push_back(Card("cardNumber4", 6, 1, 11, 3));
  hand.push_back(Card("cardNumber6", 8, 3, 3, 1));
  hand.push_back(Card("cardNumber0", 3, 2, 1, 0));
  hand.push_back(Card("cardNumber2", 3, 1, 11, 1));
  */
  g_biggest.reserve(hand.size());
  auto start = std::chrono::high_resolution_clock::now();
  output = bruteForce(hand, 1, 0, hand.size(), g_biggest);
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  std::cout << "brute force: " << output << "; time: " << duration.count() << " microseconds" << std::endl;
  for (int i = 0; i < g_biggest.size(); i++) {
    cout << i << ". " << g_biggest[i] << endl;
  }
  cleanHand(hand);
  g_biggest.clear();
  start = std::chrono::high_resolution_clock::now();
  output = greedy(hand, g_biggest);
  stop = std::chrono::high_resolution_clock::now();
  auto duration2 = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
  cout << endl << "greed: " << output << "; time: " << duration2.count() << " nanoseconds" << std::endl;

  for (int i = 0; i < g_biggest.size(); i++) {
    cout << i << ". " << g_biggest[i] << endl;
  }
}


void benchmark(int argc, char* argv[]) {
  std::vector<Card> hand;
  vector<string> g_biggest;
  int output = 0;



  for (int i = 1; i < argc; i += 5) {
    hand.push_back(loadCard(argv, i));
  }

  g_biggest.reserve(hand.size());
  auto start = std::chrono::high_resolution_clock::now();
  output = bruteForce(hand, 1, 0, hand.size(), g_biggest);
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
  std::cout << duration.count() <<  " ";

  cleanHand(hand);
  g_biggest.clear();
  start = std::chrono::high_resolution_clock::now();
  output = greedy(hand, g_biggest);
  stop = std::chrono::high_resolution_clock::now();
  auto duration2 = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
  cout << duration2.count() << " ";

}


int main(int argc, char* argv[]) {
  //oneExample( argc,  argv);
  benchmark(argc, argv);
}
