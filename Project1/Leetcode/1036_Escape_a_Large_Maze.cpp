#include<vector>
#include<queue>
#include<unordered_set>
#include<unordered_map>
#include<iostream>
#include<iomanip>


namespace _1036_Escape_a_Large_Maze {

    using namespace std;

    class Solution
    {
    public:
        virtual bool isEscapePossible(vector<vector<int>>& blocked, vector<int>& source, vector<int>& target) = 0;
    };

    /*
    * The idea of this solution is that obviously a brute force breadth first search is going to be too slow
    * given the range of input x y coordinates.
    * Instead of searching a path from source to target, I can work with the blocked cells.
    * If the blocked cells encircle target or source or if the blocked cells run from one border to the other,
    * essentially cut the grid into two parts and source and target are in different parts, then there is no reachable path.
    * I can use depth first search or breadth first search to find connected blocked cells, but then the question is
    * how to determine if a number of connected blocked cells form a circle and whether this circle encloses source or target?
    * The connected blocked cells can form some extremely messy shape/pattern, where there are multiple circles among 1 connected blob.
    * The shape can also be extremely twisted so that source is *almost* enclosed by some blocked cells but there is one opening far far away.
    */
#if 0
    class Solution1 {

        void search(vector<vector<int>>& blocked, unordered_set<pair<int, int>>& blocked_set, /*vector<pair<int, int>>& current_connected,*/ pair<int, int>p)
        {
            vector<pair<int, int>> connected;

            connected.emplace_back(p);

            int topLimit = p.second;
            int bottomLimit = p.second;
            int leftLimit = p.first;
            int rightLimit = p.first;

            int limit = 999'999;

            for (int i = 0; i < connected.size(); ++i)
            {
                auto& pa = connected[i];
                blocked_set.erase(pa);
                if (pa.first < leftLimit)
                {
                    leftLimit = pa.first;
                }
                if (pa.first > rightLimit)
                {
                    rightLimit = pa.first;
                }
                if (pa.second < bottomLimit)
                {
                    bottomLimit = pa.second;
                }
                if (pa.second > topLimit)
                {
                    topLimit = pa.second;
                }
                if (pa.first != 0)
                {
                    if (blocked_set.contains(pair<int, int>(pa.first - 1, pa.second)))
                    {
                        connected.emplace_back(pa.first - 1, pa.second);
                    }
                }
                if (pa.first != limit)
                {
                    if (blocked_set.contains(pair<int, int>(pa.first + 1, pa.second)))
                    {
                        connected.emplace_back(pa.first + 1, pa.second);
                    }
                }

                if (pa.second != 0)
                {
                    if (blocked_set.contains(pair<int, int>(pa.first, pa.second - 1)))
                    {
                        connected.emplace_back(pa.first, pa.second - 1);
                    }
                }
                if (pa.second != limit)
                {
                    if (blocked_set.contains(pair<int, int>(pa.first, pa.second + 1)))
                    {
                        connected.emplace_back(pa.first, pa.second + 1);
                    }
                }
            }
        }


    public:
        bool isEscapePossible(vector<vector<int>>& blocked, vector<int>& source, vector<int>& target)
        {
            auto pairComp = [](pair<int, int>const& a, pair<int, int>const& b)
            {
                return a.first == b.first && a.second == b.second;
            };

            auto pairHash = [](pair<int, int>const& a)
            {
                return ((size_t)a.first) * 1'000'000 + (size_t)a.second;
            };

            if (blocked.size() <= 1)
            {
                return true;
            }
            return false; // this solution is abandoned, this line just added to pass compile.
        }
    };
#endif

    /*
    * The basic idea of Solution 2, 3, and 4 is that
    * (1). when I'm in a cell, try the best to get closer to the target.
    * If there are some unblocked cells between current cell and the target, I don't need to use breadth first search
    * to "gradually" approach target, instead, I can just keep moving toward target until I hit a blocked cell.
    * (2). When I'm blocked, move "along" the blocked cells. If the blocked cells form a wall (not a circle, i.e. there is
    * a way to get to the other side of the wall), move closely along the wall will eventually get me to the edge of the wall.
    */
    class Solution2 : public Solution
    {
    public:
        bool isEscapePossible(vector<vector<int>>& blocked, vector<int>& source, vector<int>& target) override
        {
            if (blocked.size() <= 1)
            {
                return true;
            }
            auto pairComp = [](pair<int, int>const& a, pair<int, int>const& b)
            {
                return a.first == b.first && a.second == b.second;
            };

            auto pairHash = [](pair<int, int>const& a)
            {
                return ((size_t)a.first) * 1'000'000 + (size_t)a.second;
            };

            int limit = 999'999;
            int cx, cy; // current x and current y
            int tx = target[0], ty = target[1];

            unordered_set<pair<int, int>, decltype(pairHash), decltype(pairComp)> visited(0, pairHash, pairComp);

            unordered_set<pair<int, int>, decltype(pairHash), decltype(pairComp)> blockedSet(0, pairHash, pairComp);
            for (auto const& cell : blocked)
            {
                blockedSet.emplace(cell[0], cell[1]);
            }
            queue<pair<int, int>> q;

            visited.emplace(source[0], source[1]);
            q.emplace(source[0], source[1]);

            while (!q.empty())
            {
                auto cell = q.front();
                cx = cell.first; cy = cell.second;
                q.pop();
                while (true)
                {
                    if (cx > tx && !blockedSet.contains(pair<int, int>(cx - 1, cy)) && !visited.contains(pair<int, int>(cx - 1, cy)))
                    {
                        --cx;
                        visited.emplace(cx, cy);
                        continue;
                    }
                    if (cx < tx && !blockedSet.contains(pair<int, int>(cx + 1, cy)) && !visited.contains(pair<int, int>(cx + 1, cy)))
                    {
                        ++cx;
                        visited.emplace(cx, cy);
                        continue;
                    }

                    if (cy > ty && !blockedSet.contains(pair<int, int>(cx, cy - 1)) && !visited.contains(pair<int, int>(cx, cy - 1)))
                    {
                        --cy;
                        visited.emplace(cx, cy);
                        continue;
                    }
                    if (cy < ty && !blockedSet.contains(pair<int, int>(cx, cy + 1)) && !visited.contains(pair<int, int>(cx, cy + 1)))
                    {
                        ++cy;
                        visited.emplace(cx, cy);
                        continue;
                    }
                    break;
                }
                if (cx == tx && cy == ty)
                {
                    return true;
                }

                if (cx == tx && cy != ty)
                {
                    // blocked on top
                    if (cx > 0 && !blockedSet.contains(pair<int, int>(cx - 1, cy)) && !visited.contains(pair<int, int>(cx - 1, cy)))
                    {
                        q.emplace(cx - 1, cy);
                        visited.emplace(cx - 1, cy);
                    }
                    if (cx < limit && !blockedSet.contains(pair<int, int>(cx + 1, cy)) && !visited.contains(pair<int, int>(cx + 1, cy)))
                    {
                        q.emplace(cx + 1, cy);
                        visited.emplace(cx + 1, cy);
                    }
                    continue;

                }
                else if (cy == ty && cx != tx)
                {
                    // blocked to the left
                    if (cy > 0 && !blockedSet.contains(pair<int, int>(cx, cy - 1)) && !visited.contains(pair<int, int>(cx, cy - 1)))
                    {
                        q.emplace(cx, cy - 1);
                        visited.emplace(cx, cy - 1);
                    }
                    if (cy < limit && !blockedSet.contains(pair<int, int>(cx, cy + 1)) && !visited.contains(pair<int, int>(cx, cy + 1)))
                    {
                        q.emplace(cx, cy + 1);
                        visited.emplace(cx, cy + 1);
                    }
                    continue;
                }
                else // cx != tx && cy != ty
                {
                    // check all 4 directions
                    if (cx > 0 && !blockedSet.contains(pair<int, int>(cx - 1, cy)) && !visited.contains(pair<int, int>(cx - 1, cy)))
                    {
                        q.emplace(cx - 1, cy);
                        visited.emplace(cx - 1, cy);
                    }
                    if (cx < limit && !blockedSet.contains(pair<int, int>(cx + 1, cy)) && !visited.contains(pair<int, int>(cx + 1, cy)))
                    {
                        q.emplace(cx + 1, cy);
                        visited.emplace(cx + 1, cy);
                    }
                    if (cy > 0 && !blockedSet.contains(pair<int, int>(cx, cy - 1)) && !visited.contains(pair<int, int>(cx, cy - 1)))
                    {
                        q.emplace(cx, cy - 1);
                        visited.emplace(cx, cy - 1);
                    }
                    if (cy < limit && !blockedSet.contains(pair<int, int>(cx, cy + 1)) && !visited.contains(pair<int, int>(cx, cy + 1)))
                    {
                        q.emplace(cx, cy + 1);
                        visited.emplace(cx, cy + 1);
                    }
                }

            }
            return false;
        }
    };


    class Solution3 : public Solution
    {
    public:
        bool isEscapePossible(vector<vector<int>>& blocked, vector<int>& source, vector<int>& target) override
        {
            if (blocked.size() <= 1)
            {
                return true;
            }
            auto pairComp = [](pair<int, int>const& a, pair<int, int>const& b)
            {
                return a.first == b.first && a.second == b.second;
            };

            auto pairHash = [](pair<int, int>const& a)
            {
                return ((size_t)a.first) * 1'000'000 + (size_t)a.second;
            };

            int limit = 999'999;
            int cx, cy; // current x and current y
            int tx = target[0], ty = target[1];

            unordered_set<pair<int, int>, decltype(pairHash), decltype(pairComp)> visited(0, pairHash, pairComp);

            unordered_set<pair<int, int>, decltype(pairHash), decltype(pairComp)> blockedSet(0, pairHash, pairComp);
            for (auto const& cell : blocked)
            {
                blockedSet.emplace(cell[0], cell[1]);
            }

            auto isThreeSideBlocked = [&blockedSet, &limit](int x, int y)
            {
                int count = 0;
                if (y == limit || blockedSet.contains(pair<int, int>(x, y + 1)))
                {
                    ++count;
                }
                if (x == 0 || blockedSet.contains(pair<int, int>(x - 1, y)))
                {
                    ++count;
                }
                if (x == limit || blockedSet.contains(pair<int, int>(x + 1, y)))
                {
                    ++count;
                }
                if (y == 0 || blockedSet.contains(pair<int, int>(x, y - 1)))
                {
                    ++count;
                }
                return count == 3;
            };

            auto isTooFarAwayFromBlocked = [&blockedSet, &limit](int x, int y)
            {
                int count = 0;
                if (x > 0 && y > 0 && blockedSet.contains(pair<int, int>(x - 1, y - 1)))
                {
                    ++count;
                }
                if (x > 0 && y < limit && blockedSet.contains(pair<int, int>(x - 1, y + 1)))
                {
                    ++count;
                }
                if (x < limit && y > 0 && blockedSet.contains(pair<int, int>(x + 1, y - 1)))
                {
                    ++count;
                }
                if (x < limit && y < limit && blockedSet.contains(pair<int, int>(x + 1, y + 1)))
                {
                    ++count;
                }
                if (x > 0 && blockedSet.contains(pair<int, int>(x - 1, y)))
                {
                    ++count;
                }
                if (x < limit && blockedSet.contains(pair<int, int>(x + 1, y)))
                {
                    ++count;
                }
                if (y > 0 && blockedSet.contains(pair<int, int>(x, y - 1)))
                {
                    ++count;
                }
                if (y < limit && blockedSet.contains(pair<int, int>(x, y + 1)))
                {
                    ++count;
                }
                return count == 0;
            };

            queue<pair<int, int>> q;

            //visited.emplace(source[0], source[1]);
            if (isTooFarAwayFromBlocked(source[0], source[1]))
            {
                visited.emplace(source[0], source[1]);
            }
            q.emplace(source[0], source[1]);

            while (!q.empty())
            {
                auto cell = q.front();
                cx = cell.first; cy = cell.second;
                q.pop();
                while (true)
                {
                    if (cx > tx && !blockedSet.contains(pair<int, int>(cx - 1, cy)) && !visited.contains(pair<int, int>(cx - 1, cy)) && !isThreeSideBlocked(cx - 1, cy))
                    {
                        --cx;
                        //visited.emplace(cx, cy);
                        continue;
                    }
                    if (cx < tx && !blockedSet.contains(pair<int, int>(cx + 1, cy)) && !visited.contains(pair<int, int>(cx + 1, cy)) && !isThreeSideBlocked(cx + 1, cy))
                    {
                        ++cx;
                        //visited.emplace(cx, cy);
                        continue;
                    }

                    if (cy > ty && !blockedSet.contains(pair<int, int>(cx, cy - 1)) && !visited.contains(pair<int, int>(cx, cy - 1)) && !isThreeSideBlocked(cx, cy - 1))
                    {
                        --cy;
                        //visited.emplace(cx, cy);
                        continue;
                    }
                    if (cy < ty && !blockedSet.contains(pair<int, int>(cx, cy + 1)) && !visited.contains(pair<int, int>(cx, cy + 1)) && !isThreeSideBlocked(cx, cy + 1))
                    {
                        ++cy;
                        //visited.emplace(cx, cy);
                        continue;
                    }
                    break;
                }
                if (cx == tx && cy == ty)
                {
                    return true;
                }
                else
                {
                    visited.emplace(cx, cy);
                    // check all 4 directions
                    if (cx > 0 && !blockedSet.contains(pair<int, int>(cx - 1, cy)) && !visited.contains(pair<int, int>(cx - 1, cy)) && !isThreeSideBlocked(cx - 1, cy) && !isTooFarAwayFromBlocked(cx-1, cy))
                    {
                        q.emplace(cx - 1, cy);
                        visited.emplace(cx - 1, cy);
                    }
                    if (cx < limit && !blockedSet.contains(pair<int, int>(cx + 1, cy)) && !visited.contains(pair<int, int>(cx + 1, cy)) && !isThreeSideBlocked(cx + 1, cy) && !isTooFarAwayFromBlocked(cx + 1, cy))
                    {
                        q.emplace(cx + 1, cy);
                        visited.emplace(cx + 1, cy);
                    }
                    if (cy > 0 && !blockedSet.contains(pair<int, int>(cx, cy - 1)) && !visited.contains(pair<int, int>(cx, cy - 1)) && !isThreeSideBlocked(cx, cy - 1) && !isTooFarAwayFromBlocked(cx, cy-1))
                    {
                        q.emplace(cx, cy - 1);
                        visited.emplace(cx, cy - 1);
                    }
                    if (cy < limit && !blockedSet.contains(pair<int, int>(cx, cy + 1)) && !visited.contains(pair<int, int>(cx, cy + 1)) && !isThreeSideBlocked(cx, cy + 1) && !isTooFarAwayFromBlocked(cx, cy+1))
                    {
                        q.emplace(cx, cy + 1);
                        visited.emplace(cx, cy + 1);
                    }
                }
            }
            return false;
        }
    };

    /*
    * On top of the basic ideas, a few questions need to be addressed in order to form a complete algorithm:
    * 
    * (1). Should the starting cell be added to "visited" set immediately at the beginning?
    * 
    * No. I should first move from the starting cell toward the target until I'm blocked. At that point, the
    * cell I'm in should become the "true" starting point. In another word, only cells that matter on this grid
    * are blocked cells and the unblocked cells that are closely along the blocked ones.
    * In one of the test cases, the starting cell must not be added as visited at the beginning, because the path
    * that escape an "almost" enclosed circle of blocked cells goes through that starting cell.
    * 
    * (2). What cells should be enqueued for next step when hitting a blocked cell?
    * 
    * Like afore mentioned, cells that are far from blocked cells don't matter in the challenge.
    * When I enqueue a cell, make sure it's not too far away from any blocked cells. In another word,
    * check the surrounding 8 cells of the candidate, if none of the 8 is blocked, this cell is not eligible for queue.
    * This isn't just a performance enhancement. Imagine that target is enclosed by a few blocked cells that form a
    * small circle. If I keep enqueuing unblocked cells, I will end up exhausting almost all cells on the entire grid,
    * then only to find that the target is unreachable.
    * 
    * (3). When aggresively moving toward target, one type of cell is off limit.
    * 
    * For example, I'm current in cell A, and I'm moving to cell B. If B is blocked in 3 of the 4 cardinal directions,
    * (one direction is not blocked, because I can move from A to B), then don't move to B.
    * It's certain that after moving to B, I'll have to back out again. If A is added to visited, beause A is also blocked
    * on some directions and aggresively moving toward target leads me to A and stop at A, then moving to B is a suicide move,
    * because I can't move back to A since A is visited.
    * 
    * (4). When I get a cell from the breadth-first-search queue, do I first enqueue surrounding cells or make aggresive move?
    * 
    * When I get a cell from the bfs queue, I need to enqueue its eligible surrounding cells. And then move aggresively
    * toward target, when blocked, enqueue the surrounding eligible cells as well.
    * Aggressively moving toward target may not be a valid option to escape some surrounding blocked cells.
    * One of the test cases proves that do the enqueuing at two locations is even necessary.
    *   X
    * X   X       T
    *   A   X
    * X   X
    *   X
    * For the test case that resembles this shape of blocked cells, when I'm at the opening (cell A), if I just move toward
    * target, and add 'A' to visited, I'll be moving away from the opening and because I add 'A' to visited, I can't step on
    * A again, I essentially blocked the opening. So when I'm on A, add its surrounding eligible cells to bfs queue, then
    * move toward target, when stopped by blocked cell(s), enqueue at that location again.
    */
    // accepted 975ms beat 15%, memory 13MB beat 99%
    // memory usage is significantly lower than the majority of the accepted answers.
    // I wonder if they don't use unordered_set, instead just use a 2-D array of 999999 by 999999
    class Solution4 : public Solution
    {
    public:
        bool isEscapePossible(vector<vector<int>>& blocked, vector<int>& source, vector<int>& target) override
        {
            if (blocked.size() <= 1)
            {
                return true;
            }
            auto pairComp = [](pair<int, int>const& a, pair<int, int>const& b)
            {
                return a.first == b.first && a.second == b.second;
            };

            auto pairHash = [](pair<int, int>const& a)
            {
                return ((size_t)a.first) * 1'000'000 + (size_t)a.second;
            };

            int limit = 999'999;
            int cx, cy; // current x and current y
            int tx = target[0], ty = target[1];
            queue<pair<int, int>> q;

            unordered_set<pair<int, int>, decltype(pairHash), decltype(pairComp)> visited(0, pairHash, pairComp);

            unordered_set<pair<int, int>, decltype(pairHash), decltype(pairComp)> blockedSet(0, pairHash, pairComp);
            for (auto const& cell : blocked)
            {
                blockedSet.emplace(cell[0], cell[1]);
            }

            auto isThreeSideBlocked = [&blockedSet, &limit](int x, int y)
            {
                int count = 0;
                if (y == limit || blockedSet.contains(pair<int, int>(x, y + 1)))
                {
                    ++count;
                }
                if (x == 0 || blockedSet.contains(pair<int, int>(x - 1, y)))
                {
                    ++count;
                }
                if (x == limit || blockedSet.contains(pair<int, int>(x + 1, y)))
                {
                    ++count;
                }
                if (y == 0 || blockedSet.contains(pair<int, int>(x, y - 1)))
                {
                    ++count;
                }
                return count == 3;
            };

            auto isTooFarAwayFromBlocked = [&blockedSet, &limit](int x, int y)
            {
                int count = 0;
                if (x > 0 && y > 0 && blockedSet.contains(pair<int, int>(x - 1, y - 1)))
                {
                    ++count;
                }
                if (x > 0 && y < limit && blockedSet.contains(pair<int, int>(x - 1, y + 1)))
                {
                    ++count;
                }
                if (x < limit && y > 0 && blockedSet.contains(pair<int, int>(x + 1, y - 1)))
                {
                    ++count;
                }
                if (x < limit && y < limit && blockedSet.contains(pair<int, int>(x + 1, y + 1)))
                {
                    ++count;
                }
                if (x > 0 && blockedSet.contains(pair<int, int>(x - 1, y)))
                {
                    ++count;
                }
                if (x < limit && blockedSet.contains(pair<int, int>(x + 1, y)))
                {
                    ++count;
                }
                if (y > 0 && blockedSet.contains(pair<int, int>(x, y - 1)))
                {
                    ++count;
                }
                if (y < limit && blockedSet.contains(pair<int, int>(x, y + 1)))
                {
                    ++count;
                }
                return count == 0;
            };

            auto moveTowardTarget = [&blockedSet, &visited, &limit, &isThreeSideBlocked, &tx, &ty](int& x, int& y)
            {
                while (true)
                {
                    if (x > tx && 
                        !blockedSet.contains(pair<int, int>(x - 1, y)) && 
                        !visited.contains(pair<int, int>(x - 1, y)) && 
                        (!isThreeSideBlocked(x - 1, y) || x-1 == tx))
                    {
                        --x;
                        continue;
                    }
                    if (x < tx && 
                        !blockedSet.contains(pair<int, int>(x + 1, y)) && 
                        !visited.contains(pair<int, int>(x + 1, y)) && 
                        (!isThreeSideBlocked(x + 1, y) || x+1 == tx))
                    {
                        ++x;
                        continue;
                    }

                    if (y > ty && 
                        !blockedSet.contains(pair<int, int>(x, y - 1)) && 
                        !visited.contains(pair<int, int>(x, y - 1)) && 
                        (!isThreeSideBlocked(x, y - 1) || y-1 == ty))
                    {
                        --y;
                        continue;
                    }
                    if (y < ty && 
                        !blockedSet.contains(pair<int, int>(x, y + 1)) && 
                        !visited.contains(pair<int, int>(x, y + 1)) && 
                        (!isThreeSideBlocked(x, y + 1) || y+1 == ty))
                    {
                        ++y;
                        continue;
                    }
                    break;
                }
            };

            auto enqueueEligibleCells = [&blockedSet, &visited, &q, &limit, &isThreeSideBlocked, &isTooFarAwayFromBlocked](int x, int y)
            {
                if (x > 0 && !blockedSet.contains(pair<int, int>(x - 1, y)) && !visited.contains(pair<int, int>(x - 1, y)) && !isThreeSideBlocked(x - 1, y) && !isTooFarAwayFromBlocked(x - 1, y))
                {
                    q.emplace(x - 1, y);
                    visited.emplace(x - 1, y);
                }
                if (x < limit && !blockedSet.contains(pair<int, int>(x + 1, y)) && !visited.contains(pair<int, int>(x + 1, y)) && !isThreeSideBlocked(x + 1, y) && !isTooFarAwayFromBlocked(x + 1, y))
                {
                    q.emplace(x + 1, y);
                    visited.emplace(x + 1, y);
                }
                if (y > 0 && !blockedSet.contains(pair<int, int>(x, y - 1)) && !visited.contains(pair<int, int>(x, y - 1)) && !isThreeSideBlocked(x, y - 1) && !isTooFarAwayFromBlocked(x, y - 1))
                {
                    q.emplace(x, y - 1);
                    visited.emplace(x, y - 1);
                }
                if (y < limit && !blockedSet.contains(pair<int, int>(x, y + 1)) && !visited.contains(pair<int, int>(x, y + 1)) && !isThreeSideBlocked(x, y + 1) && !isTooFarAwayFromBlocked(x, y + 1))
                {
                    q.emplace(x, y + 1);
                    visited.emplace(x, y + 1);
                }
            };

            cx = source[0];
            cy = source[1];

            moveTowardTarget(cx, cy);

            if (cx == tx && cy == ty)
            {
                return true;
            }

            q.emplace(cx, cy);
            visited.emplace(cx, cy);

            while (!q.empty())
            {
                auto cell = q.front();
                cx = cell.first; cy = cell.second;
                q.pop();
                int cxCopy = cx;
                int cyCopy = cy;
                moveTowardTarget(cxCopy, cyCopy);

                if (cxCopy == tx && cyCopy == ty)
                {
                    return true;
                }
                else
                {
                    enqueueEligibleCells(cx, cy);
                    enqueueEligibleCells(cxCopy, cyCopy);
                }
            }
            return false;
        }
    };


    void test1(Solution& solu)
    {
        vector<vector<int>>blocked{ {0,1}, {1,0} };
        vector<int> source{ 0, 0 };
        vector<int> target{ 0,2 };
        cout << "test1: " << boolalpha << solu.isEscapePossible(blocked, source, target) << "\n";
    }


    void test2(Solution& solu)
    {
        /*
        *
        * 7                 T
        * 6         X X X X
        * 5         X       X
        * 4         X   S     X
        * 3     X X           X
        * 2   X             X
        * 1       X X X X X
        * 0
        *   0 1 2 3 4 5 6 7 8 9
        */
        vector<vector<int>> blocked{ {1,2},{2,3},{3,1},{3,3},{4,1},{4,4},{4,5},{4,6},{5,1},{5,6},{6,1},{6,6},{7,1},{7,6},{8,2},{8,5},{9,3},{9,4} };
        vector<int> source{ 6,4 };
        vector<int> target{ 888'888,888'888 };
        cout << "test2: " << boolalpha << solu.isEscapePossible(blocked, source, target) << "\n";
    }


    void test3(Solution& solu)
    {
        /*
        * 4     T
        * 3   X X X
        * 2   X   X
        * 1   X S X
        * 0
        *   0 1 2 3 4
        */
        vector<vector<int>>blocked{ {1,1},{1,2},{1,3},{2,3},{3,1},{3,2},{3,3} };
        vector<int>source{ 2,1 };
        vector<int>target{ 2,4 };
        cout << "test3: " << boolalpha << solu.isEscapePossible(blocked, source, target) << "\n";
    }

    void test4(Solution& solu)
    {
        /*
        *
        *           X
        *         X   X
        * 12 -- X       X
        * 11 --     S     X
        * 10 -- X       X
        *       | X   X
        *       |   X
        *       |
        *       10
        */
        vector<vector<int>> blocked{ {10,12}, {11,13}, {12,14}, {13,13}, {14,12}, {15,11}, {14,10}, {13,9}, {12,8}, {11,9}, {10,10} };
        vector<int>source{ 12,11 };
        vector<int>target{ 999994,999990 };
        cout << "test4: " << boolalpha << solu.isEscapePossible(blocked, source, target) << "\n";
    }


    void test5(Solution& solu)
    {
        /*
        * 12
        * 11      X
        * 10    X T X
        * 9       X
        *     8 9 101112
        * 
        */
        vector<vector<int>> blocked{ {10, 9},{9, 10}, {10, 11}, { 11, 10 } };
        vector<int>source{ 0,0 };
        vector<int>target{ 10, 10 };
        cout << "test5: " << boolalpha << solu.isEscapePossible(blocked, source, target) << "\n";
    }
    
    void test6(Solution& solu)
    {
        vector<vector<int>> blocked{ 
            {100005, 100073}, { 100006, 100074 }, { 100007, 100075 }, { 100008, 100076 }, 
            { 100009, 100077 }, { 100010, 100078 }, { 100011, 100079 }, { 100012, 100080 }, 
            { 100013, 100081 }, { 100014, 100082 }, { 100015, 100083 }, { 100016, 100084 }, 
            { 100017, 100085 }, { 100018, 100086 }, { 100019, 100087 }, { 100020, 100088 }, 
            { 100021, 100089 }, { 100022, 100090 }, { 100023, 100091 }, { 100024, 100092 }, 
            { 100025, 100091 }, { 100026, 100090 }, { 100027, 100089 }, { 100028, 100088 }, 
            { 100029, 100087 }, { 100030, 100086 }, { 100031, 100085 }, { 100032, 100084 }, 
            { 100033, 100083 }, { 100034, 100082 }, { 100035, 100081 }, { 100036, 100080 }, 
            { 100037, 100079 }, { 100038, 100078 }, { 100039, 100077 }, { 100040, 100076 }, 
            { 100041, 100075 }, { 100042, 100074 }, { 100043, 100073 }, { 100044, 100072 }, 
            { 100043, 100071 }, { 100042, 100070 }, { 100041, 100069 }, { 100040, 100068 }, 
            { 100039, 100067 }, { 100038, 100066 }, { 100037, 100065 }, { 100036, 100064 }, 
            { 100035, 100063 }, { 100034, 100062 }, { 100033, 100061 }, { 100032, 100060 }, 
            { 100031, 100059 }, { 100030, 100058 }, { 100029, 100057 }, { 100028, 100056 }, 
            { 100027, 100055 }, { 100026, 100054 }, { 100025, 100053 }, { 100024, 100052 }, 
            { 100023, 100053 }, { 100022, 100054 }, { 100021, 100055 }, { 100020, 100056 }, 
            { 100019, 100057 }, { 100018, 100058 }, { 100017, 100059 }, { 100016, 100060 }, 
            { 100015, 100061 }, { 100014, 100062 }, { 100013, 100063 }, { 100012, 100064 }, 
            { 100011, 100065 }, { 100010, 100066 }, { 100009, 100067 }, { 100008, 100068 }, 
            { 100007, 100069 }, { 100006, 100070 }, { 100005, 100071 }};
        vector<int> source{ 100024, 100072 };
        vector<int> target{ 999994, 999990 };
        cout << "test6: " << boolalpha << solu.isEscapePossible(blocked, source, target) << "\n";
    }

    void test7(Solution& solu)
    {
    
        vector<vector<int>> blocked{ {0,3}, {1,0}, {1,1}, {1,2}, {1,3} };
        vector<int>source{ 0,0 };
        vector<int>target{ 0,2 };
        cout << "test7: " << boolalpha << solu.isEscapePossible(blocked, source, target) << "\n";
    }


    void Test_1036_Escape_a_Large_Maze()
    {
        Solution4 solu;
        //test1(solu);
        //test2(solu);
        //test3(solu);
        //test4(solu);
        //test5(solu);
        //test6(solu);
        test7(solu);
    }
}
