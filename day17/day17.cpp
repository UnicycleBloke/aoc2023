#include "utils.h"


enum Dir { Left, Right, Up, Down, None };


// row, col, entry direction, direction count
using  Vertex   = tuple<size_t, size_t, Dir, int>; 
using  Vertices = set<Vertex>;
using  Edges    = map<Vertex, vector<pair<Vertex, int>>>;


template <typename T>
auto part1(const T& blocks, const Vertices& vertices, Edges& edges)
{
    constexpr size_t AOC_INF = 0xFFFF'FFFF;

    aoc::timer timer;

    Vertex source{0, 0, Right, 1};

    // Initialise a map of distances from the source. 
    // Everything is infinity for now except the source itself, which is zero.
    map<Vertex, size_t> dist;
    for (auto v: vertices)
    {
        dist[v] = AOC_INF;
    }
    dist[source] = 0;

    // Create a map of predecessor nodes. We don't really need this. 
    map<Vertex, Vertex> pred;

    // The set of vertices which have not yet been visited.
    set<Vertex> queue = vertices;
    // for (auto v: vertices)
    // {
    //     queue.insert(v);
    // }

    while (queue.size() > 0)
    {
        cout << queue.size() << " ";

        // Find the vertex in the queue with the lowest distance from the source.
        // Default to the first item in the map in case all are infinity - is this possible?
        Vertex min_v = *queue.begin();
        size_t min_dist = AOC_INF;
        for (auto v: queue)
        {
            if (dist[v] < min_dist)
            {
                min_dist = dist[v];
                min_v    = v;
            }
        }

        // Go through the outbound edges from the minimum distance vertex.
        for (auto [v2, cost]: edges[min_v])
        {
            dist[v2] = min(dist[v2], dist[min_v] + cost);
            pred[v2] = min_v;
        }

        // Mark the vertex we just dealt with as finished/visited.
        queue.erase(min_v);
    }

    vector<size_t> row(blocks[0].size(), AOC_INF);
    vector<vector<size_t>> grid(blocks.size(), row);

    for (auto [v, d]: dist)
    {
        auto [row, col, dir, cnt] = v;
        grid[row][col] = min(grid[row][col], d);
    }

    for (auto r: aoc::range(blocks.size()))
    {
        for (auto c: aoc::range(blocks[0].size()))
        {
            cout << grid[r][c] << " ";            
        }
        cout << "\n";
    }

    return 0;
}


template <typename T>
auto part2(T& blocks)
{
    aoc::timer timer;

    return 0;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename, aoc::Blanks::Suppress); 

    Vertices vertices;
    // Expand each node into a set of 12 nodes using the direction and count to differentiate them. 
    // The idea is that, for example, you cannot go left from a node whose left count is 3.
    for (auto r: aoc::range(lines.size()))
    {
        for (auto c: aoc::range(lines[0].size()))
        {
            // For the count of steps 1, 2, 3 - 1-based.
            for (auto i: aoc::range(1, 4))
            {
                vertices.insert(Vertex{r, c, Left,  i});
                vertices.insert(Vertex{r, c, Right, i});
                vertices.insert(Vertex{r, c, Up,    i});
                vertices.insert(Vertex{r, c, Down,  i});
            }
        }
    }

    // Create a set of edges to connect the new nodes. These capture the constraint about 3 moves.
    // The idea is that, for example going left from {r, c, h, Left, 1} will go to {r, c-1, h, Left, 2}.
    // But going left from {r, c, h, Left, 3} is not possible because there is no edge. 
    Edges edges;
    for (auto v: vertices)
    {
        auto [row, col, dir, cnt] = v;

        // Left
        if (col > 0) 
        {
            auto cost = lines[row][col-1] - '0';
            switch (dir)
            {
                case Left:
                    if (cnt < 3) edges[v].push_back({Vertex{row, col-1, Left, cnt+1}, cost});
                    break; 
                //case Right:
                case Up:
                case Down:
                    edges[v].push_back({Vertex{row, col-1, Left, 1}, cost});
            }
        }

        // Right
        if ((col+1) < lines[0].size())
        {
            auto cost = lines[row][col+1] - '0';
            switch (dir)
            {
                case Right:
                    if (cnt < 3) edges[v].push_back({Vertex{row, col+1, Right, cnt+1}, cost});
                    break; 
                //case Left:
                case Up:
                case Down:
                    edges[v].push_back({Vertex{row, col+1, Right, 1}, cost});
            }
        }

        // Up
        if (row > 0) 
        {
            auto cost = lines[row-1][col] - '0';
            switch (dir)
            {
                case Up:
                    if (cnt < 3) edges[v].push_back({Vertex{row-1, col, Up, cnt+1}, cost});
                    break; 
                //case Down:
                case Left:
                case Right:
                    edges[v].push_back({Vertex{row-1, col, Up, 1}, cost});
            }
        }

        // Down
        if ((row+1) < lines.size()) 
        {
            auto cost = lines[row+1][col] - '0';
            switch (dir)
            {
                case Down:
                    if (cnt < 3) edges[v].push_back({Vertex{row+1, col, Down, cnt+1}, cost});
                    break; 
                //case Up:
                case Left:
                case Right:
                   edges[v].push_back({Vertex{row+1, col, Down, 1}, cost});
            }
        }
    }

    auto p1 = part1(lines, vertices, edges); // 1225 too high
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
