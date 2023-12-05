#include "utils.h"


struct Range
{
    size_t src;
    size_t dst;
    size_t len;
};


enum Index
{
    SeedToSoil,
    SoilToFertilizer,
    FertilizerToWater,
    WaterToLight,
    LightToTemperature,
    TemperatureToHumidity,
    HumidityToLocation,
    
    NumMaps
};


struct Input
{
    vector<size_t> seeds{};
    array<vector<Range>, NumMaps> maps{};
};


template <typename T>
size_t map_src_to_dst(T& input, Index index, size_t src)
{
    for (const auto& entry: input.maps[index])
    {
        if ((src >= entry.src) && (src < (entry.src + entry.len)))
            return entry.dst + src - entry.src;
    }

    return src;
}

void print(const Range& r)
{
    std::cout << "src: " << r.src << '-' << (r.src + r.len -1) << ' ';
    std::cout << "dst: " << r.dst << '-' << (r.dst + r.len -1) << ' ';
    std::cout << "len: " << r.len << '\n';
}


void print(const char* step, const std::vector<Range>& src)
{
    // cout << step << '\n';
    // cout << src.size() << '\n';
    // for (const auto& r: src)
    //     print(r);
    // std::cout << '\n';
}

bool has_overlaps(const std::vector<Range>& src)
{
    return false;
}

// This was a bit fiddly to implement. The goal is to combine two of the maps by 
// splitting the ranges where necessary.
//
// If we have Seeds => Soil and Soil => Fertiliser, the result is Seeds => Fertiliser.
// Each input vector is a collection of ranges. The src ranges must be accounted for 
// in the result but checking for overlaps and non-overlaps with the ranges in dst.
// We assume that none of the ranges in a given vector themselves overlap.  
std::vector<Range> merge_ranges(std::vector<Range> src, std::vector<Range>& dst)
{
    std::vector<Range> result;

    // The algo assumed the dst ranges are sorted. 
    sort(dst.begin(), dst.end(), [](auto a, auto b) { return a.src < b.src; });

    for (const auto& r1: src)
    {
        auto beg1 = r1.dst;
        auto end1 = r1.dst + r1.len;
        auto len1 = r1.len;

        for (const auto& r2: dst)
        {
            auto beg2 = r2.src;
            auto end2 = r2.src + r2.len;
            auto len2 = r2.len;

            // The src and dst ranges don't overlap at all. Ignore for now. 
            if (end2 < beg1) continue;
            if (beg2 > end1) continue;

            // Prefix sub-range
            // The start of the src range is not in the dst map, so that 
            // sub-range is passed straight through.
            if (beg1 < beg2)
            {
                Range r;
                r.src = beg1;
                r.dst = beg1;
                r.len = beg2 - beg1;
                result.push_back(r);

                // Adjust the src values for the sub-range we have dealt with. 
                beg1 += r.len;
                len1 -= r.len;
            }

            // Common sub-range
            // Now the sub-range which is common to the src and dst.
            auto beg = std::max(beg1, beg2);
            auto end = std::min(end1, end2);
            auto len = end - beg;

            if (len > 0)
            {
                Range r;
                r.src = beg;
                r.len = len;
                r.dst = r.src + r2.dst - r2.src;
                result.push_back(r);

                // Adjust the src values for the sub-range we have dealt with. 
                beg1 += r.len;
                len1 -= r.len;
            }
        }

        // Suffix sub-range
        // The last part of the src range if it does not overlap a dst range.
        if (len1 > 0)
        {
            Range r;
            r.src = beg1;
            r.dst = beg1;
            r.len = len1;
            result.push_back(r);
        }
    }

    return result;
}


template <typename T>
auto part1(T& input)
{
    aoc::timer timer;

    size_t min_loc = -1;

    for (auto seed: input.seeds)
    {
        auto value = seed;
        value = map_src_to_dst(input, SeedToSoil, value);
        value = map_src_to_dst(input, SoilToFertilizer, value);
        value = map_src_to_dst(input, FertilizerToWater, value);
        value = map_src_to_dst(input, WaterToLight, value);
        value = map_src_to_dst(input, LightToTemperature, value);
        value = map_src_to_dst(input, TemperatureToHumidity, value);
        value = map_src_to_dst(input, HumidityToLocation, value);

        min_loc = min(min_loc, value);
    }

    return min_loc;
}


template <typename T>
auto part2(T& input)
{
    aoc::timer timer;

    // The pairs of numbers are translated into ranges.
    vector<Range> seeds;
    for (auto i: aoc::range(0UL, input.seeds.size(), 2UL))
    {
        auto seed = input.seeds[i];
        auto size = input.seeds[i + 1];
        seeds.push_back(Range{seed, seed, size});
    }

    print("Seeds", seeds);
    seeds = merge_ranges(seeds, input.maps[SeedToSoil]);
    print("SeedToSoil", seeds);
    seeds = merge_ranges(seeds, input.maps[SoilToFertilizer]);
    print("SoilToFertilizer", seeds);
    seeds = merge_ranges(seeds, input.maps[FertilizerToWater]);
    print("FertilizerToWater", seeds);
    seeds = merge_ranges(seeds, input.maps[WaterToLight]);
    print("WaterToLight", seeds);
    seeds = merge_ranges(seeds, input.maps[LightToTemperature]);
    print("LightToTemperature", seeds);
    seeds = merge_ranges(seeds, input.maps[TemperatureToHumidity]);
    print("TemperatureToHumidity", seeds);
    seeds = merge_ranges(seeds, input.maps[HumidityToLocation]);
    print("HumidityToLocation", seeds);

    size_t min_loc = -1;
    for (const auto& r: seeds)
        min_loc = min(min_loc, r.dst);
    return min_loc;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename, aoc::Blanks::Suppress, aoc::Trim::No);

    Input input{};
    Index index = SeedToSoil;

    for (auto line: lines)
    {
        auto items = aoc::split(line, " ");

        if (items[0] == "seeds:")
        {
            for (auto i: aoc::range(1UL, items.size()))
                input.seeds.push_back(stoll(items[i]));
        }
        else if (items[0] == "seed-to-soil") index = SeedToSoil;
        else if (items[0] == "soil-to-fertilizer") index = SoilToFertilizer;
        else if (items[0] == "fertilizer-to-water") index = FertilizerToWater;
        else if (items[0] == "water-to-light") index = WaterToLight;
        else if (items[0] == "light-to-temperature") index = LightToTemperature;
        else if (items[0] == "temperature-to-humidity") index = TemperatureToHumidity;
        else if (items[0] == "humidity-to-location") index = HumidityToLocation;
        else
        {
            Range entry;
            entry.dst = stoll(items[0]);
            entry.src = stoll(items[1]);
            entry.len = stoll(items[2]);
            input.maps[index].push_back(entry);
        }
    }

    auto p1 = part1(input);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 157211394UL);

    auto p2 = part2(input);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 50855035UL);
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
