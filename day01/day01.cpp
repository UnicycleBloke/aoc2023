#include "utils.h"


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;

    int result = 0;
    for (auto line: input)
    {
        int digits[2];
        int index = 0;

        for (auto c: line)
        {
            if (isdigit(c))
            {
                digits[index] = (c - '0');
                index = 1;
                digits[index] = (c - '0');
            }
        }
        
        int code = digits[0] * 10 + digits[1]; 
        result += code;
    } 

    return result;
}


template <typename T>
auto part2(const T& input)
{
    aoc::timer timer;

    int result = 0;
    for (auto line: input)
    {
        int digits[2];
        int index = 0;
 
        // Replace digit names with digits. We need to scan the string in order rather 
        // than do a global replace of all "one", then all "two". There was some 
        // ambiguity in the question, which did not mention that the names of digits 
        // might overlap. The example data works both with and without overlaps. 
        size_t pos = 0;
        ostringstream os;
        while (pos < line.size())
        {
            // Without overlaps 
            //      if ((line.find("one",   pos)) == pos) { os << "1--";     pos += 3; }
            // else if ((line.find("two",   pos)) == pos) { os << "2--";     pos += 3; }
            // else if ((line.find("three", pos)) == pos) { os << "3----";   pos += 5; }
            // else if ((line.find("four",  pos)) == pos) { os << "4---";    pos += 4; }
            // else if ((line.find("five",  pos)) == pos) { os << "5---";    pos += 4; }
            // else if ((line.find("six",   pos)) == pos) { os << "6--";     pos += 3; }
            // else if ((line.find("seven", pos)) == pos) { os << "7----";   pos += 5; }
            // else if ((line.find("eight", pos)) == pos) { os << "8----";   pos += 5; }
            // else if ((line.find("nine",  pos)) == pos) { os << "9---";    pos += 4; }
            // else                                       { os << line[pos]; pos += 1; }

            // With overlaps
                 if ((line.find("one",   pos)) == pos) { os << "1"; pos += 1; }
            else if ((line.find("two",   pos)) == pos) { os << "2"; pos += 1; }
            else if ((line.find("three", pos)) == pos) { os << "3"; pos += 1; }
            else if ((line.find("four",  pos)) == pos) { os << "4"; pos += 1; }
            else if ((line.find("five",  pos)) == pos) { os << "5"; pos += 1; }
            else if ((line.find("six",   pos)) == pos) { os << "6"; pos += 1; }
            else if ((line.find("seven", pos)) == pos) { os << "7"; pos += 1; }
            else if ((line.find("eight", pos)) == pos) { os << "8"; pos += 1; }
            else if ((line.find("nine",  pos)) == pos) { os << "9"; pos += 1; }
            else                                       { os << line[pos]; pos += 1; }
        }

        auto line2 = os.str();
        for (auto c: line2)
        {
            if (isdigit(c))
            {
                digits[index] = (c - '0');
                index = 1;
                digits[index] = (c - '0');
            }
        }
        
        int code = digits[0] * 10 + digits[1];
        result += code;
    } 

    return result;
}


void run(const char* filename)
{
    // vector<tuple<Args...>>
    //auto lines = aoc::read_lines<int,int,int,int>(filename, R"((\d+)-(\d+),(\d+)-(\d+))");
    
    // vector<string>    
    auto lines = aoc::read_lines(filename, aoc::Blanks::Suppress); 

    auto p1 = part1(lines);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 54159);

    auto p2 = part2(lines);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 53866);
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
