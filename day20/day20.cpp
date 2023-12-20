#include "utils.h"


struct Module
{
    char type;
    string name;
    vector<string> targets;
    bool flipflop_on{false};
    map<string, bool> memory;
    size_t cycles{};
};


template <typename T>
auto broadcast(T& mods, size_t cycles, set<size_t>& periods)
{
    using Pulse = tuple<string, string, bool>;

    size_t high = 0;
    size_t low  = 0;

    vector<Pulse> pulses;
    pulses.push_back({"broadcaster", "broadcaster", false});

    while (pulses.size() > 0)
    {
        vector<Pulse> pulses2;

        for (auto [src, dst, value]: pulses)
        {
            high += value;
            low  += !value;

            if (dst == "output")
            {
            } 
            else
            {
                Module& mod = mods[dst];
                switch (mod.type)
                {
                    case 'b':
                        for (const auto& t: mod.targets)
                            pulses2.push_back({dst, t, value});
                        break;

                    case '%':
                        if (value == false)
                        {
                            mod.flipflop_on = !mod.flipflop_on;
                            for (const auto& t: mod.targets)
                            pulses2.push_back({dst, t, mod.flipflop_on});
                        }
                        break;

                    case '&':
                    {
                        // Added for P2 to detect edge in the combiners.
                        bool was_high = true;
                        for (auto [name, last_pulse]: mod.memory) 
                            was_high &= last_pulse;

                        mod.memory[src] = value;

                        bool all_high = true;
                        for (auto [name, last_pulse]: mod.memory) 
                            all_high &= last_pulse;

                        for (const auto& t: mod.targets)
                            pulses2.push_back({dst, t, !all_high});

                        // If we detect at low to high edge, make a note of the period at 
                        // which this recurs. Added the cycles > 1 to avoid spurious values 
                        // getting into the set from the initialisation step.
                        if (all_high && !was_high && (cycles > 1)) 
                        {
                            periods.insert(cycles - mod.cycles);
                            mod.cycles = cycles;
                        }
                        break;
                    }
                }
            }
        }

        pulses = pulses2;
    }
    //cout << "\n";

    return make_pair(high, low);
}

template <typename T>
auto part1(T mods)
{
    aoc::timer timer;

    size_t cycles = 0;
    set<size_t> periods;

    size_t high = 0;
    size_t low  = 0;
    for (auto i: aoc::range(1000))
    {
        auto [h, l] = broadcast(mods, cycles, periods);
        high += h;
        low  += l;
    }

    return high * low;
}


// This was a vital step. The input creates a group of four 12-bit counters (chains of flipflops), with 
// some cross links to give them interesting periods. The four counters periods are effectively multiplied
// by being used as inputs to the final combiner module.
template <typename T> 
void make_dot_graph(T& mods)
{
    cout << "digraph graphname {\n";

    for (const auto& [name, mod]: mods)
    {
        cout << mod.name << " [label=\"" << mod.type << mod.name << "\"];\n";
        for (const auto& t: mod.targets)
            cout << mod.name << " -> " << t << ";\n"; 
    }

    cout << "}\n";
}


template <typename T>
auto part2(T mods)
{
    //make_dot_graph(mods);

    aoc::timer timer;

    size_t cycles = 0;
    set<size_t> periods;

    // We know that there are four 12-bit counters involved, so just push the button 4096 times 
    // to accumulate the set of periods.
    for (auto i: aoc::range(4096))
    {
        ++cycles;
        broadcast(mods, cycles, periods);
    }

    // It looked as though the four periods were primes so I didn't bother with LCM. That worked.
    size_t result = 1;
    for (auto p: periods) 
    {
        result *= p;
    }
    return result; 
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename);

    map<string, Module> mods;
    for (auto line : lines)
    {
        auto s = aoc::split(line, "->");
        Module mod;
        mod.type = s[0][0];
        mod.targets = aoc::split(s[1], ",");
        mod.name = (mod.type == 'b') ? s[0] : s[0].substr(1);
        mods[mod.name] = mod;

        if (s[0] == "output") mod.type = 'o';
    }

    for (auto& [src, mod]: mods)
    {
        for (auto dst: mod.targets)
        {
            mods[dst].memory[src] = false;
        }
    }

    auto p1 = part1(mods);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 788848550U);

    auto p2 = part2(mods);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 228300182686739U);
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
