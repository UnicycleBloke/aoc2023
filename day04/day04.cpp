#include "utils.h"


template <typename T>
auto part1(const T& cards)
{
    aoc::timer timer;

    int total_score = 0;
    for (auto& card: cards)
    {
        int score = 0;
        for (auto n: card.nums)
        {
            if (card.wins.find(n) != card.wins.end())
                score = (score == 0) ? 1 : (score * 2);
        }
        total_score += score;
    }

    return total_score;
}


template <typename T>
auto part2(T& cards)
{
    aoc::timer timer;

    // We process the cards in order and only ever create copies 
    // for the cards not yet processed. The instructions say we 
    // won't run off the end. So we can just increment a counter 
    // for the following cards as we go.
    for (auto i: aoc::range{cards.size()})
    {
        const auto& card = cards[i];

        int score = 0;
        for (auto n: card.nums)
        {
            if (card.wins.find(n) != card.wins.end())
                ++score;
        }

        for (auto j: aoc::range{score})
        {
            cards[i+j+1].copies += card.copies;
        }
    }

    int copies = 0;
    for (const auto& card: cards)
        copies += card.copies;
    return copies;
}


struct Card
{
    int      index{1};
    int      copies{1};
    set<int> wins;
    set<int> nums;
};


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename);

    // Parsing was the hardest part of the problem (again).
    int index = 1;
    vector<Card> cards;
    for (const auto& line: lines)
    {
        auto parts  = aoc::split(line, ":");
        auto parts2 = aoc::split(parts[1], "|");
        auto wins   = aoc::split(parts2[0], " ");
        auto nums   = aoc::split(parts2[1], " ");

        Card card;
        card.index = index++;
        for (auto w: wins)
            card.wins.insert(stoi(w));
        for (auto n: nums)
            card.nums.insert(stoi(n));

        cards.push_back(card);
    }

    auto p1 = part1(cards);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 23750);

    auto p2 = part2(cards);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 13261850);
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
