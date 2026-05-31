from typing import Literal

#---------------------------------------------------------------
# HELPER FUNCTIONS
#---------------------------------------------------------------

def map_value_char(c):
    val_char = {
        "T": 10,
        "J": 11,
        "Q": 12,
        "K": 13,
        "A": 14
    }
    assert c in "23456789TJQKA"
    if c in "23456789":
        return int(c)
    return val_char[c]

def check_consecutive(L: list):
    # Assume descending order; require at least two values
    if len(L) < 2:
        return False
    return all(L[i] - L[i + 1] == 1 for i in range(len(L) - 1))

def check_equality(L: list):
    # Require at least a pair to be considered equal
    if len(L) < 2:
        return False
    return all(L[i] == L[i + 1] for i in range(len(L) - 1))

def compare_lists(L1: list, L2: list):
    # Assume descending order of both lists
    assert len(L1) == len(L2)
    for i in range(len(L1)):
        if L1[i] != L2[i]:
            return 1 if L1[i] > L2[i] else 0
    assert False, "Lists are equal"

#---------------------------------------------------------------
# CHECKING HANDS
#---------------------------------------------------------------

# Highest value card
def high_card(cards_extended):
    cards, cards_by_value, cards_by_suit = cards_extended
    values = [c[0] for c in cards_by_value]
    suits = [c[1] for c in cards_by_suit]
    return values[0]

# Two cards of the same value
def one_pair(cards_extended):
    cards, cards_by_value, cards_by_suit = cards_extended
    values = [c[0] for c in cards_by_value]
    suits = [c[1] for c in cards_by_suit]
    for i in [0, 1, 2, 3]:
        j = i + 2
        if check_equality(values[i:j]):
            return values[i]
    return None

# Two different pairs
def two_pairs(cards_extended):
    cards, cards_by_value, cards_by_suit = cards_extended
    values = [c[0] for c in cards_by_value]
    suits = [c[1] for c in cards_by_suit]
    for i in [0, 1]:
        j = i + 2
        if check_equality(values[i:j]):
            for gap in [0, 1 - i]:
                k = j + gap
                l = k + 2
                if check_equality(values[k:l]):
                    return values[i], values[k]
    return None

# Three cards of the same value
def three_of_a_kind(cards_extended):
    cards, cards_by_value, cards_by_suit = cards_extended
    values = [c[0] for c in cards_by_value]
    suits = [c[1] for c in cards_by_suit]
    for i in [0, 1, 2]:
        j = i + 3
        if check_equality(values[i:j]):
            return values[i]

# All cards are consecutive values
def straight(cards_extended):
    cards, cards_by_value, cards_by_suit = cards_extended
    values = [c[0] for c in cards_by_value]
    suits = [c[1] for c in cards_by_suit]
    if check_consecutive(values):
        return values[0]
    return None

# All cards of the same suit
def flush(cards_extended):
    cards, cards_by_value, cards_by_suit = cards_extended
    values = [c[0] for c in cards_by_value]
    suits = [c[1] for c in cards_by_suit]
    if check_equality(suits):
        return values[0]
    return None

# Three of a kind and a pair
def full_house(cards_extended):
    cards, cards_by_value, cards_by_suit = cards_extended
    values = [c[0] for c in cards_by_value]
    suits = [c[1] for c in cards_by_suit]

    for len in [2, 3]:
        if check_equality(values[:len]) and check_equality(values[len:]):
            return (values[0], values[len]) if len == 3 else (values[len], values[0])
                 
    return None

# Four cards of the same value
def four_of_a_kind(cards_extended):
    cards, cards_by_value, cards_by_suit = cards_extended
    values = [c[0] for c in cards_by_value]
    suits = [c[1] for c in cards_by_suit]
    for i in [0, 1]:
        j = i + 4
        if check_equality(values[i:j]):
            return values[i]
    return None

# All cards are consecutive values of same suit
def straight_flush(cards_extended):
    cards, cards_by_value, cards_by_suit = cards_extended
    values = [c[0] for c in cards_by_value]
    suits = [c[1] for c in cards_by_suit]
    if check_consecutive(values) and check_equality(suits):
        return values[0]
    return None 

# Ten, Jack, Queen, King, Ace, in same suit
def royal_flush(cards_extended):
    cards, cards_by_value, cards_by_suit = cards_extended
    values = [c[0] for c in cards_by_value]
    suits = [c[1] for c in cards_by_suit]
    if cards_by_value[0][0] == 14 and check_consecutive(values) and check_equality(suits):
        return 14
    return None

# Reverse order, from the strongest hand to the weakest one
result_order = [
    royal_flush,
    straight_flush,
    four_of_a_kind,
    full_house,
    flush,
    straight,
    three_of_a_kind,
    two_pairs,
    one_pair,
    high_card
]

def check_result_order(cards):
    for index, check in enumerate(result_order):
        if check(cards) is not None:
            return index
    assert False

def winner(P1, P2) -> Literal[0, 1]:
    # First, check the result order for both players
    if check_result_order(P1) != check_result_order(P2):
        return 1 if check_result_order(P1) < check_result_order(P2) else 0
    rank = check_result_order(P1)

    # Check the highest value card for both players
    highest_card_P1 = result_order[rank](P1)
    highest_card_P2 = result_order[rank](P2)
    if highest_card_P1 != highest_card_P2:
        return 1 if highest_card_P1 > highest_card_P2 else 0
    
    # At this point, compare the values of the cards in descending order
    values_P1 = [c[0] for c in P1[1]]
    values_P2 = [c[0] for c in P2[1]]
    return compare_lists(values_P1, values_P2)

def extend_cards(cards):
    # Create two another versions of the cards, one sorted by value and another sorted by suit
    cards_by_value = sorted(cards, key=lambda c: c[0], reverse=True)
    cards_by_suit = sorted(cards, key=lambda c: c[1])
    return cards, cards_by_value, cards_by_suit

if __name__ == "__main__":
    with open("0054_poker.txt") as f:
        lines = f.read().strip().split("\n")
        P1 = []
        P2 = []
        games = []
        for line in lines:
            cards = line.split()
            cards_rv = [(map_value_char(c[0]), c[1]) for c in cards]
            games.append((cards_rv[:5], cards_rv[5:]))
            P1.append(extend_cards(cards_rv[:5]))
            P2.append(extend_cards(cards_rv[5:]))
            
        print(sum(winner(P1[i], P2[i]) for i in range(len(games))))