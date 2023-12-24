#include "utils.h"


class Spiral
{
public:
    Spiral(int start) 
    : loop{start} 
    , x{start}
    , y{start}
    {    
    }

    pair<int, int> next()
    {
        switch (state)
        {
            case State::R1: right1(); break;
            case State::D:  down(); break;
            case State::L:  left(); break;
            case State::U:  up(); break;
            case State::R:  right(); break;
        }
        return {x, y};
    }

    int loops() const { return loop; }

private:
    void right1()
    {
        ++x; 
        count = loop*2+1; 
        state = State::D;
    }

    void down() 
    {
        --y; 
        --count; 
        if (!count) 
        {
            count = loop*2+2;
            state = State::L;
        }
    }

    void left() 
    {
        --x; 
        --count; 
        if (!count) 
        {
            count = loop*2+2;
            state = State::U;
        }
    }
    
    void up()   
    {
        ++y; 
        --count; 
        if (!count) 
        {
            count = loop*2+2;
            state = State::R;
        }
    }

    void right() 
    {
        ++x; 
        --count; 
        if (!count) 
        {
            state = State::R1;
            ++loop;
        }
    }

private:
    enum class State {R1, D, L, U, R};
    State state{State::R1};
    int count{};
    int loop{};
    int x{};
    int y{};
};


template <typename T>
T det(T x1, T y1, T x2, T y2)
{
    return x1 * y2 - x2 * y1;
}


template <typename T>
auto part1(const T& input)
{
    constexpr int64_t DELTA = 1000;
    constexpr double ONE    = 1;

    aoc::timer timer;

    int crosses = 0;
    for (auto i: aoc::range{input.size()})
    {
        auto [x1, y1, z1, dx1, dy1, dz1] = input[i];
        auto x2 = x1 + DELTA * dx1; 
        auto y2 = y1 + DELTA * dy1; 

        for (auto j: aoc::range{i+1, input.size()})
        {
            auto [x3, y3, z3, dx3, dy3, dz3] = input[j];
            auto x4 = x3 + DELTA * dx3; 
            auto y4 = y3 + DELTA * dy3; 

            // Intersection of two lines in a plane - found by a google search.
            auto den = det<double>(det<double>(x1, ONE, x2, ONE), det<double>(y1, ONE, y2, ONE), 
                           det<double>(x3, ONE, x4, ONE), det<double>(y3, ONE, y4, ONE));
            if (den == 0) continue;
                              
            auto x = det<double>(det<double>(x1, y1, x2, y2), det<double>(x1, ONE, x2, ONE), 
                         det<double>(x3, y3, x4, y4), det<double>(x3, ONE, x4, ONE));
            double px = double(x) / den;

            auto y = det<double>(det<double>(x1, y1, x2, y2), det<double>(y1, ONE, y2, ONE), 
                         det<double>(x3, y3, x4, y4), det<double>(y3, ONE, y4, ONE));
            double py = double(y) / den;

            auto t1 = (px - x1) / dx1;
            auto t3 = (px - x3) / dx3;

            // if (px < 7) continue; 
            // if (px > 27) continue; 
            // if (py < 7) continue; 
            // if (py > 27) continue; 
            if (px < 200000000000000) continue; 
            if (px > 400000000000000) continue; 
            if (py < 200000000000000) continue; 
            if (py > 400000000000000) continue; 
            if (t1 < 0) continue;
            if (t3 < 0) continue;

            ++crosses;
        }
    }

    return crosses;
}


template <typename T>
bool is_integer(T value, T& integer)
{
    return modf(value, &integer) == 0.0;
}


using Integer = int64_t;
using Double  = long double;
using Rock    = tuple<Integer, Integer, Integer, Integer, Integer, Integer>;


template <typename T>
bool intersects(T& input, Integer vxr, Integer vyr, size_t i, size_t j, Rock& rock)
{
    // Check if two of the input lines will intersect and, if so, calculate the 
    // origin and velocity of the rock from their value. We provide guesses for 
    // the x and y velocity of the rock, and work out everything else from there.
    auto [px1, py1, pz1, vx1, vy1, vz1] = input[i];
    auto [px2, py2, pz2, vx2, vy2, vz2] = input[j];

    // The first line is imagined to hit the rock at t1, the second at t2.
    // Create the coefficients to solve simultaneous equations for t1 and t2. I was 
    // chuffed to work out the maths for this myself.
    //     a1*t1 + b1*t2 = c1
    //     a2*t1 + b2*t2 = c2
    //    
    // t1 = | a1 b1 | / | c1 b1 |
    //      | a2 b2 |   | c2 b2 |    
    Integer a1 = vxr - vx1;
    Integer b1 = vx2 - vxr;
    Integer c1 = px1 - px2;

    Integer a2 = vyr - vy1;
    Integer b2 = vy2 - vyr;
    Integer c2 = py1 - py2;

    // Cramer's rule for solving a system of equations.
    Integer den  = det(a1, b1, a2, b2);
    Integer num1 = det(c1, b1, c2, b2);
    Integer num2 = det(a1, c1, a2, c2);

    // When the determinant of the coefficient matrix D is zero, the formulas of Cramer’s rule are 
    // undefined. In this case, the system is either dependent or inconsistent depending on the values 
    // of Dx and Dy. When D=0 and both Dx=0 and Dy=0 the system is dependent. When D=0 and either Dx or Dy
    // is nonzero then the system is inconsistent.
    if (den == 0)
    {
        // Dependent system - infinitely many solutions. Return true as if we had found a solution. 
        // The calling code passed in the valid solution so it will all work out.
        return (num1 == 0) && (num2 == 0);
    }

    Double t1 = num1;
    t1 /= den;
    // We only care about solutions in the future.
    if (t1 < 0) return false;

    Double t2 = num2;
    t2 /= den;
    // We only care about solutions in the future.
    if (t2 < 0) return false;

    // Oops! This might be overconstrained as we could hit two hailstones at the same time.
    // Actually this should not be possible. We are told none of the hailstones hit each other.
    if (t1 == t2) return false;

    // The rock's z velocity must be integral.
    Double vzr = pz1 - pz2 + vz1 * t1 - vz2 * t2;
    vzr /= (t1 - t2); 
    Double temp;
    if (!is_integer(vzr, temp)) return false;

    // Calulate the starting position for the rock. This too must be integral.
    Double pxr = px1 + (vx1 - vxr) * t1;
    if (!is_integer(pxr, temp)) return false;
    Double pyr = py1 + (vy1 - vyr) * t1;
    if (!is_integer(pyr, temp)) return false;
    Double pzr = pz1 + (vz1 - vzr) * t1;
    if (!is_integer(pzr, temp)) return false;

    // cout << fixed << setprecision(0) << px2 << " " << py2 << " " << pz2 << " @ ";
    // cout << vx2 << " " << vy2 << " " << vz2 << " ";
    // cout << "time: " << t2 << "\n";
    // cout << fixed << setprecision(0) << pxr << " " << pyr << " " << pzr << " @ ";
    // cout << vxr << " " << vyr << " " << vzr << " sum=" << (pxr+pyr+pzr) << "\n\n";

    // Account for rounding of these possibly non-integer values. Already got this above...
    rock = make_tuple(pxr, pyr, pzr, vxr, vyr, vzr);
    return true;
}


template <typename T>
auto part2(T& input)
{
    aoc::timer timer;

    Spiral spiral{0};
    int loops = 0; 

    while (true)    
    {
        auto [vxr, vyr] = spiral.next();

        if (loops < spiral.loops())
        {
            // cout << "loops=" << spiral.loops() << "\n";
            loops = spiral.loops();
        }

        constexpr size_t LINE0 = 0;
        constexpr size_t LINE1 = 2;
        static_assert(LINE0 != LINE1);

        Rock rock1;        
        if (intersects(input, vxr, vyr, LINE0, LINE1, rock1))
        {
            bool all_intersect = true;
            for (auto j: aoc::range(1U, input.size()))
            {
                if ((j == LINE0) || (j == LINE1)) continue;

                Rock rock2 = rock1;        
                if (!intersects(input, vxr, vyr, LINE0, j, rock2) || (rock1 != rock2))
                {
                    all_intersect = false;
                    break;
                }
            }

            if (all_intersect)
            {
                auto [pxr, pyr, pzr, vxr, vyr, vzr] = rock1;
                // cout << vxr << " " << vyr << " " << vzr << "\n";
                // cout << pxr << " " << pyr << " " << pzr << "\n\n";
                return pxr + pyr + pzr;
            }
        }
    }

    return Integer{};
}


void run(const char* filename)
{
    auto lines = aoc::read_lines<Integer,Integer,Integer,Integer,Integer,Integer>(filename, R"((\d+),\s+(\d+),\s+(\d+)\s+@\s+(-*\d+),\s+(-*\d+),\s+(-*\d+))");

    auto p1 = part1(lines);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 14046);

    auto p2 = part2(lines);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 808107741406756);
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
