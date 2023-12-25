#include "utils.h"


template <typename T>
set<string> find_out_links(T& input, const set<string>& cluster)
{
    set<string> out;
    for (const auto& node: cluster)
    {
        for (const auto& to: input[node].to)
            if (cluster.find(to) == cluster.end())
                out.insert(to);
    }
    return out;
}


template <typename T>
auto grow_cluster(T& input, set<string> cluster, set<string> out, set<string>& consumed, size_t& result)
{
    if (out.size() == 0) return;
    if (out.size() == 3) 
    {
        // We have found a cluster.
        for (auto node: cluster) 
            consumed.insert(node);

        // cout << "cluster of " << cluster.size() << "\n";
        // for (auto link: out) cout << link << " ";
        // cout << "\n";
        // for (auto node: cluster) cout << node << " ";
        // cout << "\n\n";
        
        result *= cluster.size();
        return;
    }

    for (const auto& node: out)
    {
        if (consumed.find(node) != consumed.end()) continue;

        auto cluster2 = cluster;
        cluster2.insert(node);

        // Add any other nodes which have more than one link to the
        // cluster. Repeat until the cluster stops growing.
        size_t size = 0;
        while (size < cluster2.size())
        {
            size = cluster2.size();

            for (auto& [node, comp]: input)
            {
                // Already in the cluster.
                if (cluster2.find(node) != cluster2.end()) continue;

                // If there is only one link to the existing cluster, this 
                // node is likely part of another cluster.
                size_t links = 0;
                for (auto node2: input[node].to)
                    if (cluster2.find(node2) != cluster2.end()) ++links;
                if (links > 1) 
                    cluster2.insert(node);
            }
        }
        
        // Recurse to deal with the issue of sub-clusters which would terminate 
        // the cluster growth early.
        auto out2 = find_out_links(input, cluster2);
        grow_cluster(input, cluster2, out2, consumed, result);
    }

    return;
}


template <typename T>
auto make_cluster(T& input, string name, set<string>& consumed, size_t& result)
{
    // Make a seed cluster
    set<string> cluster;
    cluster.insert(name);
    for (auto node: input[name].to) 
        cluster.insert(node);

    auto out = find_out_links(input, cluster);
    grow_cluster(input, cluster, out, consumed, result);
}


template <typename T>
auto part1(T& input, const set<string>& nodes)
{
    aoc::timer timer;

    size_t result = 1;

    set<string> consumed;
    for (const auto& name: nodes)
    {
        // cout << name << ": ";
        // for (auto c: consumed) cout << c << " ";
        // cout << "\n";

        if (consumed.find(name) == consumed.end())
            make_cluster(input, name, consumed, result);    
    }

    return result;
}


template <typename T>
auto part2(T& input)
{
    aoc::timer timer;

    return 0;
}


struct Component
{
    string      name;
    set<string> to;
};


void run(const char* filename)
{
    map<string, Component> input;
    set<string> names;

    auto lines = aoc::read_lines(filename);
    for (auto line: lines)
    {
        auto src = aoc::split(line, ":");
        auto dst = aoc::split(src[1], " ");
        auto s = src[0];
        names.insert(s);

        for (auto d: dst)        
        {
            names.insert(d);
            input[s].name = s;
            input[s].to.insert(d);
            input[d].to.insert(s);
        }
    }
    // cout << "num nodes " << names.size() << "\n";

    auto p1 = part1(input, names);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 547080U);
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
