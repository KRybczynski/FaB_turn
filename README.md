# Szukanie najlepszego możliwego zagrania w grze karcianej

## Opis szczegółowy
Szukanie zagrania dającego największą liczą punktów obrażeń w grze karcianej "Flesh and Blood"

## Zasady gry
> W opisie zasad gry zachowam niektóre angielskie słowa kluczowe, ponieważ nie czuję się na tyle kompetentny żeby tłumaczyć gry planszowe

Karta posiada następujące parametry:
  - **Attack Value** - liczba obrażeń zadawanych przez kartę
  - **Cost** - liczba punktów zasobów potrzebnych do wydania aby zagrać kartę
  - **Pitch Value** - liczba punków zasobów otrzymywanych przy poświęcaniu karty
  - **Go again** - liczba punktów akcji które otrzymujemy po zagraniu

W turze posiadamy następujące zasoby:
  - **Punkty akcji** - zagranie każdej karty kosztuje jeden punkt akcji
  - **Punkty zasobów** - punkty którymi opłacamy koszt karty, którą chcemy zagrać
  - **Karty** - karty posiadane przez gracza w ręce

Karty można użyć na dwa sposoby:
  * **Play** - gracz wydaje jeden punkt akcji i tyle punktów zasobów ile wynosi **cost** zagrywanej karty. Następnie otrzymujemy tyle punktów akcji ile wynosi wartość **go again** danej karty
  * **Pitch** - usuwamy kartę z reki i do **punktów zasobów** dodajemy **pitch value** poświęcanej karty

## Opis problemu
Dla N kart na ręce znaleźć sekwencję zagrywania kart która daje największą sumę **Attack Value**

### realizacja programowa 
Do generowania kart wykorzystałem pythona i jego moduł random. Program napisany w pythonie generuje podaną ilość kart, a następnie wywołuje program napisany w języku c++, wprowadzając do niego wygenerowane dane.

Tworzenie karty z losowymi parametrami
```python
def createRandomCard(id):
    card = {
        "id" : "",
        "cost" : "",
        "pitch_value": "",
        "attack": "",
        "go_again": "" 
    }
    configs = loadData("config.yaml")
    
    card["id"] = f"nr{id}"
    card["cost"] = random.randint(configs["cost"]["min"], configs["cost"]["max"])
    card["pitch_value"] = random.randint(configs["pitch_value"]["min"], configs["pitch_value"]["max"])
    card["attack"] = random.randint(configs["attack"]["min"], configs["attack"]["max"])
    card["go_again"] = random.randint(configs["go_again"]["min"], configs["go_again"]["max"])
    return card
```

### struktura reprezentująca kartę
```cpp
struct Card {
    string name;
    int cost, pitch_value, attack, go_again;
    bool used;
}
```
Oprócz własności kart struktura posiada dwie zmienne pomocnicze
* name - służącą od indentyfikacji
* used - zmienna pomocnicza w realizacji programowej


### algorytm brute force

argumenty
  * hand - wektor przechowujący dane o wszystkich kartach jakie posiadamy w danej turze
  * action points - ilość posiadanych aktualnie punktów akcji
  * mana - ilość aktualnie posiadanej many
  * n - ilość kart dostępnych do zagrania
``` cpp
int bruteForce(std::vector<Card>& hand, int action_points, int mana, int n,
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
```
Przeszukiwanie rozwiązań w ten sposób wymaga sprawdzenia każdej możliwej wariacji, z czego każda karta może być użyta na dwa sposoby. Powoduje to, że złożoność obliczeniowa problemu wynosi ** O(2^n * n!) **
### funkcje pomocnicze
``` cpp
void append_vector(vector<string>& global, vector<string>& local) {
  for (auto a : local) global.push_back(a);
  return;
}
```
### algorytm zachłanny
Dla danej ręki szukamy kartę o największej wartości **attack value**. Jeżeli nie mamy zasobów aby ją zagrać to próbujemy je zdobyć w sposób zachłanny. Jeżeli karta jest niemożliwa do zagrania to poświęcamy ją na zasoby
```cpp
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
```
### funkcje pomocnicze
```cpp
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



void pitchCard(Card& to_pitch, int& mana, vector<string>& seq) {
  to_pitch.used = true;
  mana += to_pitch.pitch_value;
  seq.push_back("pitched: " + to_pitch.name);
}

void playCard(Card& to_play, int& mana, int& action_points, int& damage, vector<string>& seq) {
  damage += to_play.attack;
  mana -= to_play.cost;
  action_points = action_points - 1 + to_play.go_again;
  to_play.used = true;
  seq.push_back("played: " + to_play.name);
}
```
## Testy 
Testy wykonywane są dla następujących parametrów generatora kart
``` yaml
cost:
  min: 0
  max: 8
pitch_value:
  min: 1
  max: 3
attack:
  min: 0
  max: 14
go_again:
  min: 0
  max: 3
```

## Przykład działania programu 
![obraz](https://user-images.githubusercontent.com/73962045/222984014-983c231d-9e64-4d0b-90c7-d5b6f90fcfe2.png)

### średni czas wykonywania algorytmów ==
| liczba kart | brute force[ns] | greed[ns] |
| ----------- | --------------- | --------- |
|  1  |  1220.0  |  270.0  | 
| 2 | 870.0 | 510.0 | 
| 3 | 3210.0 | 720.0 | 
| 4 | 11110.0 | 830.0 | 
| 5 | 72890.0 | 1140.0 | 
| 6 | 713680.0 | 1200.0 | 
| 7 | 4857820.0 | 1380.0 | 
| 8 | 90537230.0 | 1860.0 | 
| 9 | 1237812040.0 | 2090.0 | 

![obraz](https://user-images.githubusercontent.com/73962045/222984232-4e03be21-af97-4aab-a888-fafa623683cf.png)


