#include "utils.h"


using Brick = tuple<int, int, int, int, int, int>;


// Bricks below a particular cube of sand.
set<size_t> find_bricks_below(const vector<Brick>& bricks, int x, int y, int z)
{
    set<size_t> below;

    for (auto b: aoc::range{bricks.size()})
    {
        auto [x1, x2, y1, y2, z1, z2] = bricks[b];
        if ((x >= x1) && (x <= x2) && 
            (y >= y1) && (y <= y2) && 
            (z > z2))
        {
            below.insert(b);
        }
    }

    return below;
}


// Bricks immediately below a particular cube of sand.
set<size_t> find_touches_below(const vector<Brick>& bricks, int x, int y, int z)
{
    set<size_t> below;

    for (auto b: aoc::range{bricks.size()})
    {
        auto [x1, x2, y1, y2, z1, z2] = bricks[b];
        if ((x >= x1) && (x <= x2) && 
            (y >= y1) && (y <= y2) && 
            (z == (z2 + 1)))
        {
            below.insert(b);
        }
    }

    return below;
}


// Bricks below any of the cubes of sand for brick index b.
set<size_t> find_bricks_below(const vector<Brick>& bricks, size_t b)
{
    set<size_t> all_below;

    auto [x1, x2, y1, y2, z1, z2] = bricks[b];

    for (auto x: aoc::range{x1, x2+1})
    {
        for (auto y: aoc::range{y1, y2+1})
        {
            auto below = find_bricks_below(bricks, x, y, z1);
            for (auto f: below) all_below.insert(f);
        }
    }

    return all_below;
}


// Bricks immediately below any of the cubes of sand for brick index b.
set<size_t> find_touches_below(const vector<Brick>& bricks, size_t b)
{
    set<size_t> all_below;

    auto [x1, x2, y1, y2, z1, z2] = bricks[b];

    for (auto x: aoc::range{x1, x2+1})
    {
        for (auto y: aoc::range{y1, y2+1})
        {
            auto below = find_touches_below(bricks, x, y, z1);
            for (auto f: below) all_below.insert(f);
        }
    }

    return all_below;
}


// Return top of highest settled brick below brick index b if all settled.
int all_settled_below_height(const vector<Brick>& bricks, map<size_t, bool>& settled, size_t b)
{
    auto below = find_bricks_below(bricks, b);

    int result = 0;
    for (auto c: below)
    {
        auto [x1, x2, y1, y2, z1, z2] = bricks[c];
        if (!settled[c]) return -1;
        result = max(result, z2);
    }
    return result;
}


// Keep the set of fallers - check if one below not in the set.

int count_the_fallers(const vector<Brick>& bricks, 
    map<size_t, set<size_t>>& above, map<size_t, set<size_t>>& below, size_t b)
{
    int count = 0;
    for (auto b1: above[b])
        if (below[b1].size() < 2)
            count += count_the_fallers(bricks, above, below, b1);
    return count;
}

template <typename T, typename U>
auto part1(T bricks, const U& dim)
{
    aoc::timer timer;

    map<size_t, bool> settled;

    // for (auto b: aoc::range(bricks.size()))
    // {
    //     auto [x1, x2, y1, y2, z1, z2] = bricks[b];
    //     cout << x1 << ":" << x2 << " ";
    //     cout << y1 << ":" << y2 << " ";
    //     cout << z1 << ":" << z2 << "\n";
    // }
    // cout << "\n";

    while (true)
    {
        for (auto b: aoc::range(bricks.size()))
        {
            auto height = all_settled_below_height(bricks, settled, b);
            if (height >= 0)
            {
                auto [x1, x2, y1, y2, z1, z2] = bricks[b];
                auto w1    = height + 1;
                auto w2    = w1 + z2 - z1;
                bricks[b]  = make_tuple(x1, x2, y1, y2, w1, w2);
                settled[b] = true;
            }
        }

        bool all_settled = true;
        for (auto b: aoc::range(bricks.size()))
            all_settled &= settled[b];
        if (all_settled) 
            break;
    }


    map<size_t, set<size_t>> below;
    map<size_t, set<size_t>> above;

    for (auto b: aoc::range(bricks.size()))
    {
        auto rests = find_touches_below(bricks, b);
        for (auto r: rests)
        {
            below[b].insert(r);
            above[r].insert(b);
        }
    }

    for (auto b: aoc::range(bricks.size()))
    {
        cout << b << "  ";
        cout << "below ";
        for (auto c: below[b]) cout << c << " ";
        cout << "above ";
        for (auto c: above[b]) cout << c << " ";
        cout << "\n";
    }

    int count = 0;
    for (auto b: aoc::range(bricks.size()))
    {
        if (above[b].size() == 0) 
        {
            cout << b << " yes\n";            
            ++count;
        }
        else
        {
            bool ok = true;
            for (auto b1: above[b])
                ok &= (below[b1].size() >= 2);
            count += ok;
            cout << b << (ok ? " yes\n" : " no\n");
        }
    }


    // for (auto b: aoc::range(bricks.size()))
    // {
    //     auto [x1, x2, y1, y2, z1, z2] = bricks[b];
    //     cout << x1 << ":" << x2 << " ";
    //     cout << y1 << ":" << y2 << " ";
    //     cout << z1 << ":" << z2 << "\n";
    // }

    return count;
}


template <typename T>
auto part2(T& input)
{
    aoc::timer timer;

    return 0;
}


void run(const char* filename)
{
 
    vector<Brick> bricks;
    int max_x{};
    int max_y{};
    int max_z{};

    auto lines = aoc::read_lines<int,int,int,int,int,int>(filename, R"((\d+),(\d+),(\d+)~(\d+),(\d+),(\d+))");
    for (auto [x1, y1, z1, x2, y2, z2]: lines)
    {
        auto brick = make_tuple(
            min(x1, x2), max(x1, x2), 
            min(y1, y2), max(y1, y2), 
            min(z1, z2), max(z1, z2));
        bricks.push_back(brick);

        max_x = max(max_x, max(x1, x2));
        max_y = max(max_y, max(y1, y2));
        max_z = max(max_z, max(z1, z2));
    }

    auto dim = make_tuple(max_x, max_y, max_z);

    auto p1 = part1(bricks, dim);
    cout << "Part1: " << p1 << '\n';
    //aoc::check_result(p1, 0);

    auto p2 = part2(lines);
    cout << "Part2: " << p2 << '\n';
    //aoc::check_result(p2, 0);
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
