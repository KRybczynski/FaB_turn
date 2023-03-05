import yaml
import random
import subprocess
import math
import sys 
import logging


logging.basicConfig(level=logging.DEBUG, format='%(asctime)s -  %(levelname)s-  %(message)s')
logging.disable(logging.CRITICAL)

#loads data from config yaml file
def loadData(path):    
    with open(path, "r") as stream:
        try:
            return yaml.safe_load(stream)
        except yaml.YAMLError as exc:
            return False


#create card with given name and random attributes 
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

#create list with given number of cards
def createRandomDeck(n):
    deck = []    
    for i in range(n):
        deck.append(createRandomCard(i))    
    random.shuffle(deck)   
    return deck

def deckToArg(deck):
    out = []
    for card in deck:
        for key in card.keys():
            out.append(str(card[key]))
    return out

def prettyInput(deck):
    for card in deck:
        print(f'name: {card["id"]}, cost: {card["cost"]}, pitch_value: {card["pitch_value"]}, attack_value: {card["attack"]}, go_again: {card["go_again"]}));')
    print()

def findForRandom(c_path, n):
    deck = createRandomDeck(n)
    logging.debug(deck)
    prettyInput(deck)
    c_input = [c_path] + deckToArg(deck)
    logging.debug(c_input)
    p = subprocess.run(c_input, capture_output=True)
    print(p.stdout.decode("utf-8"))
    return

#for testing speed of c++ program
def benchmark():
    d_to_draw =[]    
    for i in range(1, 10): #testing time from 1 card to 10 cards
        bf = []
        gr = []
        for n in range(10):
            deck = createRandomDeck(i)
            c_input = [".\\Release\\fab_tura"] + deckToArg(deck)
            p = subprocess.run(c_input, capture_output=True)
            logging.debug(p.stdout.decode("utf-8"))
            out = p.stdout.decode("utf-8").split()
            out = list(map(float, out))
            bf.append(out[0])
            gr.append(out[1])
        logging.debug(f"{i};{math.fsum(bf)/len(bf)};{math.fsum(gr)/len(gr)}")
        d_to_draw.append([math.fsum(bf)/len(bf), math.fsum(gr)/len(gr)])
    print(d_to_draw)
    return

#python RNG_cards.py [path_to_c_program] [number of cards in hand]
if __name__ == "__main__":
    n = 0
    try:
        n = int(sys.argv[2])
    except:
       print("wrong input")
       quit()
    findForRandom(sys.argv[1], n)
    #benchmark()
