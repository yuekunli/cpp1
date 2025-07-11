#include<iostream>
#include<string>
#include<sstream>
#include<map>
#include<set>
#include<unordered_map>
#include<vector>
#include<algorithm>

namespace _0939_Minimum_Area_Rectangle {

	using namespace std;

	/*
	* All solutions below derive from the same idea.
	* Process all points, make the first map, key is the x-coordinate, values are the y-coordinates.
	* Essentially I'm viewing these points as points on several vertical lines.
	* Each entry in the first map represents a vertical line.
	* 
	*         * (1,5)
	*         * (1,4)                 * (7,4)
	*                    * (3,3)
	*         * (1,2)                 * (7,2)
	*                    * (3,1)
	*
	* m:
	*   key(x coordinate of a vertical line)    value (y coordinates of points on that line)
	*   1                                       2, 4, 5
	*   3                                       1, 3
	*   7                                       2, 4
	*
	* Iterate first map, process each vertical line. On each vertical line, there are some points.
	* Take every two y-coordinates, make them a pair, and then use this pair as key, use the x-coordinate
	* of the vertical line as value, build the second map.
	* Under key [2,4] there would be two values, each value represents a vertical line. Two values means
	* they can form a rectangle.
	* 
	* m2:
	*   key (y coordinates of two points on a same vertial line)   value (x coordinates of the vertial lines)
	*   [2,4]                                                      1, 7
	*   [2,5] (point (1,2) and (1,5) form this pair)               1
	*   [4,5]                                                      1
	*   [1,3]                                                      3
	*/

	// accepted but slow   1548ms beat 7%
	class Solution
	{
		struct PairComp
		{
			bool operator()(pair<int, int>const& a, pair<int, int>const& b) const
			{
				return a.first == b.first && a.second == b.second;
			}

		};
		struct PairHash
		{
			size_t operator()(pair<int, int>const& a) const
			{
				return ((size_t)a.first) * 10'000 + (size_t)a.second;
			}
		};
	public:
		int minAreaRect(vector<vector<int>>& points)
		{
			map<int, vector<int>>m;
			for (auto& p : points)
			{
				int x = p[0];
				m[x].emplace_back(p[1]);
			}

			unordered_map<pair<int, int>, vector<int>, PairHash, PairComp> m2; // a pair of points on the same vertical line
			for (auto& a : m)
			{
				vector<int>& yCoordinates = a.second;
				sort(yCoordinates.begin(), yCoordinates.end());
				size_t sz = yCoordinates.size();
				for (size_t i = 0; i < sz; i++)
				{
					for (size_t j = i + 1; j < sz; j++)
					{
						pair<int, int> p{ yCoordinates[i], yCoordinates[j] };
						m2[p].emplace_back(a.first);
					}
				}
			}

			int minArea = INT32_MAX;
			bool foundRectangle = false;
			for (auto& a : m2)
			{
				vector<int>& xCoordinates = a.second;
				if (xCoordinates.size() <= 1)
					continue;
				foundRectangle = true;
				sort(xCoordinates.begin(), xCoordinates.end());

				size_t sz = xCoordinates.size();
				int height = a.first.second - a.first.first;
				int minWidth = INT32_MAX;
				for (size_t i = 1; i < sz; i++)
				{
					if (xCoordinates[i] - xCoordinates[i - 1] < minWidth)
						minWidth = xCoordinates[i] - xCoordinates[i - 1];
				}
				if (height * minWidth < minArea)
					minArea = height * minWidth;
			}

			return foundRectangle ? minArea : 0;

		}
	};

	/*
	* This solution is a more generic solution.
	* When iterating map 'm' and building 'm2'.
	* m2: key: y-coordinates of a pair points that are on the same verticle line
	*     value: an ordered set of the x-coordinates of this pair of points
	*/
	// this solution is not tested.
	class Solution2
	{
		struct PairComp
		{
			bool operator()(pair<int, int>const& a, pair<int, int>const& b) const
			{
				return a.first == b.first && a.second == b.second;
			}

		};
		struct PairHash
		{
			size_t operator()(pair<int, int>const& a) const
			{
				return ((size_t)a.first) * 10'000 + (size_t)a.second;
			}
		};
	public:
		int minAreaRect(vector<vector<int>>& points)
		{
			map<int, vector<int>>m;
			for (auto& p : points)
			{
				int x = p[0];
				m[x].emplace_back(p[1]);
			}

			int minArea = INT32_MAX;
			bool foundRectangle = false;

			unordered_map<pair<int, int>, set<int>, PairHash, PairComp> m2;
			for (auto& a : m)
			{
				vector<int>& yCoordinates = a.second;
				size_t sz = yCoordinates.size();
				for (size_t i = 0; i < sz; i++)
				{
					for (size_t j = i + 1; j < sz; j++)
					{
						pair<int, int> p{};
						if (yCoordinates[i] <= yCoordinates[j])
						{
							p = pair<int, int>(yCoordinates[i], yCoordinates[j]);
						}
						else
						{
							p = pair<int, int>(yCoordinates[j], yCoordinates[i]);
						}
						
						if (m2.contains(p))
						{
							int area = 0;
							auto & xCoordinatesSet = m2[p];

							// using lower_bound to search the insertion position is a generic approach.
							// This is good when the vertical lines are in random order in map "m",
							// but because map "m" is an ordered map, this generic approach is not necessary

							auto it = xCoordinatesSet.lower_bound(a.first);
							if (it == xCoordinatesSet.cend())
							{
								auto it2 = xCoordinatesSet.crbegin();
								area = (p.second - p.first) * (a.first - *it2);
							}
							else if (it == xCoordinatesSet.cbegin())
							{
								area = (p.second - p.first) * (*it - a.first);
							}
							else
							{
								auto it2 = prev(it);
								area = (p.second - p.first) * min((a.first - *it2), (*it - a.first));
							}
							if (area < minArea)
							{
								minArea = area;
								foundRectangle = true;
							}
						}
						else
						{
							m2[p].insert(a.first);
						}
					}
				}
			}
			return foundRectangle ? minArea : 0;

		}
	};

	/*
	* Imagine there are 5 vertical lines, each line has a few points.
	* when building 'm', points on each line are built up in 'm' gradually.
	* I may not see the point on the left most line first, but 'm' is an ordered map.
	* Because map 'm' is ordered, when I iterator 'm', I must get the left - most vertical line first.
	* 
	*     |           |   |
	*     *           *   *
	*     |           |   |
	*     *           *   *
	*     |           |   |
	*     15          18  20
	* 
	* Actually, in map "m2", I don't even need to save all the line positions for each pair.
	* I only need to save the "last" line position.
	* The lines are examined in left to right order (again, because 'm' is ordered map),
	* When I get line at x = 20, the only possible line that make the rectangle area minimum is the
	* line at x = 18, I don't need to know there was a line at x = 15. The last line I examined
	* would be the one nearest to the line I'm examining right now. 
	*/
	// accepted, better than Solution1 but still too slow, 445 ms beat 29%
	class Solution3
	{
		struct PairComp
		{
			bool operator()(pair<int, int>const& a, pair<int, int>const& b) const
			{
				return a.first == b.first && a.second == b.second;
			}

		};
		struct PairHash
		{
			size_t operator()(pair<int, int>const& a) const
			{
				return ((size_t)a.first) * 10'000 + (size_t)a.second;
			}
		};
	public:
		int minAreaRect(vector<vector<int>>& points)
		{
			map<int, vector<int>>m; // this is an ordered map, keys are sorted in ascending order
			for (auto& p : points)
			{
				int x = p[0];
				m[x].emplace_back(p[1]);
			}

			int minArea = INT32_MAX;
			bool foundRectangle = false;

			unordered_map<pair<int, int>, int, PairHash, PairComp> m2;  // map's value is just an integer
			for (auto& a : m)
			{
				vector<int>& yCoordinates = a.second;
				size_t sz = yCoordinates.size();
				if (sz == 1)
				{
					continue;
				}
				for (size_t i = 0; i < sz; i++)
				{
					for (size_t j = i + 1; j < sz; j++)
					{
						pair<int, int> p{};
						if (yCoordinates[i] <= yCoordinates[j])
						{
							p = pair<int, int>(yCoordinates[i], yCoordinates[j]);
						}
						else
						{
							p = pair<int, int>(yCoordinates[j], yCoordinates[i]);
						}

						if (m2.contains(p))
						{
							auto& previousXCoordinate = m2[p];
							int area = (p.second - p.first) * (a.first - previousXCoordinate);
							if (area < minArea)
							{
								minArea = area;
								foundRectangle = true;
							}
							m2[p]=a.first;
						}
						else
						{
							m2[p]=a.first;
						}
					}
				}
			}
			return foundRectangle ? minArea : 0;
		}
	};

	/*
	* Let's not use pair<> as key in map "m2".
	* Instead, use a map of maps.
	* unordered_map<int, unordered_map<int, int>> m2
	*               |                   |    |
	*               |                   |    +---- x-coordinate of a vertical line
	*               |                   |
	*         the smaller of the      the bigger of the two y-coordinates
	*         two y-coordinates
	*/
	class Solution4
	{
	public:
		int minAreaRect(vector<vector<int>>& points)
		{
			map<int, vector<int>>m;
			for (auto& p : points)
			{
				int x = p[0];
				m[x].emplace_back(p[1]);
			}

			int minArea = INT32_MAX;
			bool foundRectangle = false;

			unordered_map<int, unordered_map<int, int>> m2;
			for (auto& a : m)
			{
				vector<int>& yCoordinates = a.second;
				size_t sz = yCoordinates.size();
				if (sz == 1)
				{
					continue;
				}
				for (size_t i = 0; i < sz; i++)
				{
					for (size_t j = i + 1; j < sz; j++)
					{
						int height = abs(yCoordinates[i] - yCoordinates[j]);
						int lowerY = min(yCoordinates[i], yCoordinates[j]);
						int higherY = max(yCoordinates[i], yCoordinates[j]);
						if (m2.contains(lowerY) && m2[lowerY].contains(higherY))
						{							
							auto area = height * (a.first - m2[lowerY][higherY]);
							if (area < minArea)
							{
								minArea = area;
								foundRectangle = true;
							}
						}
						m2[lowerY][higherY] = a.first; 
						// if m2 doesn't have this pair of y-coordinates, 
						// set the value here for the first time, 
						// if m2 already has this pair, update it here
					}
				}
			}
			return foundRectangle ? minArea : 0;
		}
	};

	/*
	* When I get a vertical line, if there are 100 points, I examine every 2 pair,
	* this is a O(n^2) operation, this is the bottleneck.
	*   |
	*   * p1
	*   |
	*   .
	*   .
	*   |
	* One way to improve, let's say I get this vertical line, I'm trying to check
	* every pair [p1, some_point], basically point p1 with every other point other than p1 itself.
	* If on the horizontal line where p1 is on, there is no more than 1 point,
	* than it's useless to check every pair of [p1, some_point], they are not going to form rectangle.
	* So I can quickly give up p1 and move onto other points on this line.
	*/
	// accepted 138ms beat 87%, memory 47MB beats 21%
	class Solution5
	{
	public:
		int minAreaRect(vector<vector<int>>& points)
		{
			map<int, vector<int>>m;
			unordered_map<int, char>pointsCountOnHorizontalLines;
			for (auto& p : points)
			{
				int x = p[0];
				int y = p[1];
				m[x].emplace_back(p[1]);
				if (pointsCountOnHorizontalLines.contains(y))
				{
					pointsCountOnHorizontalLines[y]=2; // don't need to save exact number of points on this horizontal line
					                                   // all I need to know is that there are more than 1.
				}
				else
				{
					pointsCountOnHorizontalLines[y] = 1;
				}
			}

			int minArea = INT32_MAX;
			bool foundRectangle = false;

			unordered_map<int, unordered_map<int, int>> m2;
			for (auto& a : m)
			{
				vector<int>& yCoordinates = a.second;
				size_t sz = yCoordinates.size();
				if (sz == 1)
				{
					continue;
				}
				for (size_t i = 0; i < sz; i++)
				{
					if (pointsCountOnHorizontalLines[yCoordinates[i]] < 2) // quickly move on
					{
						continue;
					}
					for (size_t j = i + 1; j < sz; j++)
					{
						if (pointsCountOnHorizontalLines[yCoordinates[j]] < 2)
						{
							continue;
						}
						int height = abs(yCoordinates[i] - yCoordinates[j]); 
						// if two y-coordinates on this vertical line are already bigger
						// than the min area found so far, quickly move on, 
						// all the coordinates are integers, the area can only become even bigger
						if (height > minArea)
						{
							continue;
						}
						int lowerY = min(yCoordinates[i], yCoordinates[j]);
						int higherY = max(yCoordinates[i], yCoordinates[j]);
						if (m2.contains(lowerY) && m2[lowerY].contains(higherY))
						{
							auto area = height * (a.first - m2[lowerY][higherY]);
							if (area < minArea)
							{
								minArea = area;
								foundRectangle = true;
							}
						}
						m2[lowerY][higherY] = a.first;
					}
				}
			}
			return foundRectangle ? minArea : 0;
		}
	};


	/*
	* Use a ordered set in "m1". This way when I examine y-coordinates on a vertical line,
	* they are already in sorted order.
	*/
	// this is the best so far, 126 ms beat 89%, but there is still room for improvement
	class Solution6
	{
	public:
		int minAreaRect(vector<vector<int>>& points)
		{
			map<int, set<int>>m;
			unordered_map<int, char>pointsCountOnHorizontalLines;
			for (auto& p : points)
			{
				int x = p[0];
				int y = p[1];
				m[x].insert(y);
				if (pointsCountOnHorizontalLines.contains(y))
				{
					pointsCountOnHorizontalLines[y] = 2;
				}
				else
				{
					pointsCountOnHorizontalLines[y] = 1;
				}
			}

			int minArea = INT32_MAX;
			bool foundRectangle = false;
			int xCoordinateOfPreviousVerticalLine = m.cbegin()->first;
			unordered_map<int, unordered_map<int, int>> m2;
			for (auto& a : m)
			{

				auto& yCoordinates = a.second;
				size_t sz = yCoordinates.size();
				if (sz == 1)
				{
					continue;
				}
				for (auto i = yCoordinates.cbegin(); i != yCoordinates.cend(); i = next(i))
				{
					if (pointsCountOnHorizontalLines[*i] < 2)
					{
						continue;
					}
					for (auto j = next(i); j != yCoordinates.cend(); j = next(j))
					{
						if (pointsCountOnHorizontalLines[*j] < 2)
						{
							continue;
						}
						int height = *j - *i;
						if (height > minArea)
						{
							break; // note that this is a "break" instead of "continue"
							// this the difference over last Solution.
							// the y-coordinates are in sorted order, so if this 'j' is already too big
							// for the current 'i', the next a few 'j's are going to be bigger, so quickly move on.
						}
						if (a.first - xCoordinateOfPreviousVerticalLine > minArea)
						{
							// if the distance between the two vertical lines are bigger than the min area found so far
							// also quickly move on. But don't forget to save the position of this vertical line,
							// the next vertical line may be very close to this one, which may lead to smaller area.
							m2[*i][*j] = a.first;  
							continue;
						}
						if (m2.contains(*i) && m2[*i].contains(*j))
						{
							auto area = height * (a.first - m2[*i][*j]);
							if (area < minArea)
							{
								minArea = area;
								foundRectangle = true;
							}
						}
						m2[*i][*j] = a.first;
					}
				}
				xCoordinateOfPreviousVerticalLine = a.first;
			}
			return foundRectangle ? minArea : 0;
		}
	};

	void Test_0939_Minimum_Area_Rectangle()
	{
		Solution3 solu;
		string s;
		vector<int>tmp;
		vector<vector<int>>points;
		while (true)
		{
			s.clear();
			cout << "points: ";
			getline(cin, s);
			if (s.size() == 0)break;
			stringstream ss(s);
			tmp.clear();
			points.clear();
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(tmp));
			size_t sz = tmp.size();
			for (size_t i = 0; i < sz; i += 2)
			{
				points.emplace_back(initializer_list({ tmp[i], tmp[i + 1] }));
			}
			cout << solu.minAreaRect(points) << "\n\n";
		}
	}
}