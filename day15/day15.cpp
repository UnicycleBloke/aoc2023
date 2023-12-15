#include "utils.h"


auto calc_hash(const string& input)
{
    int hash = 0;
    for (auto c: input)
    {
        hash += static_cast<int>(c);
        hash *= 17;
        hash %= 256;
    }
    return hash;
}


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;
     
    int result = 0; 
    for (auto& line: input)
        result += calc_hash(line);

    return result;
}


template <typename T>
auto part2(T& input)
{
    aoc::timer timer;

    struct Box
    {
        map<string, int> labels;
        vector<string>   lenses;
    };

    array<Box, 256> boxes;

    for (auto& line: input)    
    {
        {
            bool match = false;
            auto [label, op] = aoc::parse_line<string, char>(R"((\w+)(.))", line, match);
            if (match)
            {
                auto b = calc_hash(label);
                //cout << "Box " << b << " " << label << op << "\n";

                auto& lenses = boxes[b].lenses;
                lenses.erase(remove(lenses.begin(), lenses.end(), label), lenses.end());
            }
        }

        {
            bool match = false;
            auto [label, op, len] = aoc::parse_line<string, char, int>(R"((\w+)(.)(\d))", line, match);
            if (match)
            {
                auto b = calc_hash(label);
                //cout << "Box " << b << " " << label << op << len << "\n";

                auto& box = boxes[b];
                if (find(box.lenses.begin(), box.lenses.end(), label) == box.lenses.end())
                    box.lenses.push_back(label);
                box.labels[label] = len;
            }
        }
    }

    int result = 0;
    for (auto b: aoc::range(256))
    {
        auto& box = boxes[b];

        // if (box.lenses.size() > 0) cout << "Box: " << b << " ";         
        // for (auto& lens: box.lenses)
        // {
        //     cout << "[" << lens << " " << box.labels[lens] << "]";
        // }
        // if (box.lenses.size() > 0) cout << "\n";

        for (auto s: aoc::range(box.lenses.size()))
        {
            auto power = (b + 1) * (s + 1) * box.labels[box.lenses[s]];
            result += power;
        }
    }

    return result;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename);
    auto input = aoc::split(lines[0], ",");

    auto p1 = part1(input);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 511257);

    auto p2 = part2(input);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 239484);
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
