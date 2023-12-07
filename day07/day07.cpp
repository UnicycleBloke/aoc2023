#include "utils.h"


struct Card
{
    int value;
    int count;
};


bool operator<(const Card& c1, const Card& c2)
{
    if (c1.count != c2.count) return c1.count > c2.count;
    return c1.value > c2.value;
}


enum Type
{
    FiveOfAKind,
    FourOfAKind,
    FullHouse,
    ThreeOfAKind,
    TwoPair,
    OnePair,
    HighCard
};


struct Hand
{
    string hand;
    vector<Card> cards;
    int bid;
    Type type;
    int first;
    int second;
};


Type type_of_hand(const Hand& hand)
{
    if (hand.cards[0].count == 5) return FiveOfAKind;
    if (hand.cards[0].count == 4) return FourOfAKind;
    if ((hand.cards[0].count == 3) && (hand.cards[1].count == 2)) return FullHouse;
    if (hand.cards[0].count == 3) return ThreeOfAKind;
    if ((hand.cards[0].count == 2) && (hand.cards[1].count == 2)) return TwoPair;
    if (hand.cards[0].count == 2) return OnePair;
    return HighCard;
}


void score_hand(Hand& hand)
{
    hand.type   = type_of_hand(hand);
    hand.first  = hand.cards[0].value;
    hand.second = hand.cards[0].value;
    if (hand.type != FiveOfAKind)
        hand.second = hand.cards[1].value;
}


template <int from_char(char)>
bool compare_hands(const Hand& h1, const Hand& h2)
{
    if (h1.type != h2.type) return static_cast<int>(h1.type) > static_cast<int>(h2.type);
    if (h1.hand[0] != h2.hand[0]) return from_char(h1.hand[0]) < from_char(h2.hand[0]);
    if (h1.hand[1] != h2.hand[1]) return from_char(h1.hand[1]) < from_char(h2.hand[1]);
    if (h1.hand[2] != h2.hand[2]) return from_char(h1.hand[2]) < from_char(h2.hand[2]);
    if (h1.hand[3] != h2.hand[3]) return from_char(h1.hand[3]) < from_char(h2.hand[3]);
    return from_char(h1.hand[4]) < from_char(h2.hand[4]);
};


int from_char1(char c)
{
    static const string kinds = "23456789TJQKA";
    return kinds.find(c);
};


template <typename T>
auto parse1(T& lines)
{
    vector<Hand> hands;
    for (auto line: lines)
    {
        auto s = aoc::split(line, " ");

        map<int, int> cards;
        for (auto c: aoc::range(5))
        {
            auto value = from_char1(s[0][c]);
            if (cards.find(value) == cards.end())
                cards[value] = 1;
            else
                cards[value] = cards[value] + 1;
        }

        Hand hand;
        for (auto [k, v]: cards)
            hand.cards.push_back(Card{k, v});
        sort(hand.cards.begin(), hand.cards.end());

        hand.hand = s[0];
        hand.bid = stoi(s[1]); 
        score_hand(hand);
        hands.push_back(hand);
    }
    return hands;
}


template <typename T>
auto part1(T& lines)
{
    aoc::timer timer;
    auto hands = parse1(lines);
    
    size_t winnings{};
    sort(hands.begin(), hands.end(), compare_hands<from_char1>);

    for (auto h: aoc::range(hands.size()))
        winnings += ((h + 1) * hands[h].bid);
    return winnings;
}


int from_char2(char c)
{
    static const string kinds = "J23456789TQKA";
    return kinds.find(c);
};


template <typename T>
auto parse2(T& lines)
{
    vector<Hand> hands;
    for (auto line: lines)
    {
        auto s = aoc::split(line, " ");

        map<int, int> cards;
        for (auto c: aoc::range(5))
        {
            auto value = from_char2(s[0][c]);
            if (cards.find(value) == cards.end())
                cards[value] = 1;
            else
                cards[value] = cards[value] + 1;
        }

        Hand hand;
        int jokers = 0;
        for (auto [k, v]: cards)
        {
            if (k == 0) 
               jokers = v;
            else
               hand.cards.push_back(Card{k, v});
        }
        sort(hand.cards.begin(), hand.cards.end());

        if (hand.cards.size() == 0) // All jokers
            hand.cards.push_back(Card{from_char2('A'), jokers});
        else
            hand.cards[0].count += jokers;

        hand.hand = s[0];
        hand.bid  = stoi(s[1]); 
        hand.type = type_of_hand(hand);
        hands.push_back(hand);

    }
    return hands;
}


template <typename T>
auto part2(T& lines)
{
    aoc::timer timer;
    auto hands = parse2(lines);
    
    size_t winnings{};
    sort(hands.begin(), hands.end(), compare_hands<from_char2>);

    for (auto h: aoc::range(hands.size()))
        winnings += ((h + 1) * hands[h].bid);
    return winnings;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename);

    auto p1 = part1(lines);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 251806792U);

    auto p2 = part2(lines);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 252113488U);
}


int main(int argc, char** argv)
{
    aoc::timer timer;
    try
    {
        if (argc < 2)
        {
            cout << "Provide input file name\n";
            return -1;
        }

        fs::path path{argv[1]};
        if (!fs::is_regular_file(path))
        {
            cout << "Path '" << argv[1] << "' does not exist or is not a file\n";
            return -1;
        }

        run(argv[1]);
    }
    catch (std::exception& e)
    {
        cout << e.what();
    }
}
