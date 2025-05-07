#include "LibHeaders.h"

namespace _0847_Shortest_Path_Visiting_All_Nodes {

	using namespace std;

	/*
	* ====================================
	* Solution1 has issues
	* Solution2 has issues
	* Solution3 DFS, works, but slow
	* Solution4 optimized on top of Solution3, accepted.
	* Solution5 BFS, slow
	* ====================================
	*/

	class Solution1  // a brute force solution
	{
	public:

		int shortest{ 0 };

		int walk(vector<vector<int>>& g, vector<int>&visited, int visited_count, int steps_taken, int pre, int n)
		{
			int total_nodes = g.size();
			if (visited_count == total_nodes - 1 && visited[n] == 0)
			{
				return steps_taken;
			}

			if (steps_taken > shortest) // early abort
				return steps_taken;

			int previousVisitedState = visited[n];
			if (visited[n] == 0)
			{
				visited[n] = 1;
				visited_count++;
			}

			bool allNextVisited = true;
			int result = numeric_limits<int>::max();
			for (int const& next : g[n])
			{
				if (visited[next] == 0)
				{
					allNextVisited = false;
					int thisRunResult = walk(g, visited, visited_count, steps_taken + 1, n, next);
					if (thisRunResult < result)
						result = thisRunResult;
				}
			}

			if (!allNextVisited)
			{
				visited[n] = previousVisitedState;
				return result;
			}

			if (allNextVisited)
			{
				if (g[n].size() == 1 && g[n][0] == pre && visited_count < total_nodes)
				{
					// have to go back, no other option
					int result = walk(g, visited, visited_count, steps_taken + 1, n, pre);
					visited[n] = previousVisitedState;
					return result;
				}

				int result = numeric_limits<int>::max();
				for (int const& next : g[n])
				{
					if (next != pre)
					{
						int thisRunResult = walk(g, visited, visited_count, steps_taken + 1, n, next);
						if (thisRunResult < result)
							result = thisRunResult;
					}
				}
				visited[n] = previousVisitedState;
				return result;
			}
		}

		int shortestPathLength(vector<vector<int>>& graph)
		{
			shortest = numeric_limits<int>::max();
			int len = graph.size();
			vector<int> visited;
			for (int n = 0; n < len; n++)
			{
				visited.clear();
				visited.assign(len, 0);
				
				int result = walk(graph,visited, 0 /*visited_count*/, 0 /*steps_taken*/, -1 /*previous*/, n);
				if (result < shortest)
					shortest = result;
			}
			return shortest;
		}
	};


	class Solution2
	{
	public:

		/*
		* Ideas to speed things up:
		* Use bitmask to record which nodes are visited and which are not.
		* Use dynamic programming to remember things such as "I'm currently standing on node x,
		* and the state of the visited/not_visited of the whole graph is '011010111'",
		* the number of steps needed to visite the rest of the graph is y.
		*/

		/*
		     0             5
			 |\           /|
			 | 1----3----4 |
			 |/     |     \|
		     2      |      6
			        7
			       / \
		          8---9
		This case doesn't work with this solution, doesn't work with solution1 either.
		When I go from 3 -> 4 -> 5 -> 6 -> 4
		Now I'm on 4, all three neighbors are visited, I come from 6, I don't go back to 6.
		But I still have two options, namely 3 and 5. I go to 3, I can get an answer (if I started from left side).
		But when I come back to 4 from down below, I'll try the 4->5 path. Then 5->6->4, I'm on 4 again.
		Again I try 4->3 first, now when I get to three, record book can help, I return to 4 right away,
		but again, I try 4->5, I'm in a dead loop now.
	    
		*/

		int shortest{ 0 };

		int walk(vector<vector<int>>& g, vector<vector<int>>&r, bitset<32>& visited, int visited_count, int steps_taken, int pre, int n)
		{
			int state = static_cast<int>(visited.to_ulong());
			
			if (r[n][state] >= 0)
			{
				return steps_taken + r[n][state];
			}
			
			int total_nodes = g.size();
			if (visited_count == total_nodes - 1 && visited[n] == false)
			{
				r[n][state] = 0;
				return steps_taken;
			}
			
			if (steps_taken > shortest) // early abort
			{
				return steps_taken;
			}

			bool previousVisitedState = visited[n];
			if (visited[n] == false)
			{
				visited[n] = true;
				visited_count++;
			}

			int temporaryState = static_cast<int>(visited.to_ulong());
			if (r[n][temporaryState] == -2)
				r[n][temporaryState] = -1;

			bool allNextVisited = true;
			int result = numeric_limits<int>::max();
			for (int const& next : g[n])
			{
				if (visited[next] == false)
				{
					allNextVisited = false;
					int thisRunResult = walk(g, r, visited, visited_count, steps_taken + 1, n, next);
					if (thisRunResult < result)
						result = thisRunResult;
				}
			}

			if (!allNextVisited)
			{
				visited[n] = previousVisitedState;
				r[n][state] = result - steps_taken;
				return result;
			}
			else
			{
				if (g[n].size() == 1 && g[n][0] == pre && visited_count < total_nodes)
				{
					// have to go back, no other option
					int result = walk(g, r, visited, visited_count, steps_taken + 1, n, pre);
					visited[n] = previousVisitedState;
					r[n][state] = result - steps_taken;
					return result;
				}

				int result = numeric_limits<int>::max();
				for (int const& next : g[n])
				{
					if (next != pre)
					{
						// we know "next" is visited
						if (r[next][temporaryState] >= -1)
							continue;

						int thisRunResult = walk(g, r, visited, visited_count, steps_taken + 1, n, next);
						if (thisRunResult < result)
							result = thisRunResult;
					}
				}
				visited[n] = previousVisitedState;
				r[n][state] = result - steps_taken;
				return result;
			}
		}

		int shortestPathLength(vector<vector<int>>& graph)
		{
			shortest = numeric_limits<int>::max();
			int len = graph.size();
			int stateMax = static_cast<int>(pow(2, len)) - 1;
			vector<vector<int>>r(len, vector<int>(stateMax, -2));
			bitset<32> visited;
			for (int n = 0; n < len; n++)
			{
				visited.reset();
				int result = walk(graph, r, visited, 0 /*visited_count*/, 0 /*steps_taken*/, -1 /*previous*/, n);
				if (result < shortest)
					shortest = result;
			}
			return shortest;
		}
	};


	class Solution3  // Time Limit Exceeded
	{
	public:

		int shortest{ 0 };

		int walk(vector<vector<int>>& g, vector<vector<int>>& r, bitset<32>& visited, int visited_count, int steps_taken, int n)
		{
			bool firstArrive = false;
			int state;
			if (visited[n] == true)
			{
				state = static_cast<int>(visited.to_ulong());
				if (r[n][state] >= 0)
					return steps_taken + r[n][state];
				if (r[n][state] == -1)
					return numeric_limits<int>::max();
			}
			else
			{
				if (visited_count == g.size() - 1)
				{
					state = static_cast<int>(visited.to_ulong());
					r[n][state] = 0;
					return steps_taken;
				}
				firstArrive = true;
				visited[n] = true;
				visited_count++;
				state = static_cast<int>(visited.to_ulong());
			}

			r[n][state] = -1;

			int result = numeric_limits<int>::max();
			for (int const& next : g[n])
			{
				if (visited[next] == false || (visited[next] == true && r[next][state] != -1))
				{
					int thisRunResult = walk(g, r, visited, visited_count, steps_taken + 1, next);
					if (thisRunResult < result)
						result = thisRunResult;
				}
			}

			if (firstArrive)
				visited[n] = false;

			if (result < numeric_limits<int>::max())
				r[n][state] = result - steps_taken;
	
			return result;
		}

		int shortestPathLength(vector<vector<int>>& graph)
		{
			shortest = numeric_limits<int>::max();
			int len = graph.size();
			int stateMax = static_cast<int>(pow(2, len));   
			// if there are 4 nodes, I need 4 bits, maximum will be 15, I need an array of length 16, which is 2^4
			vector<vector<int>>r(len, vector<int>(stateMax, -2));
			// initial value is -2, '-1' means I have been to this node when the graph is in this state,
			// but I don't have a value for that {node,state} tuple yet, but I should not keep exploring that {node, state}
			// tuple, because I'll be in a dead loop.
			bitset<32> visited;
			for (int n = 0; n < len; n++)
			{
				visited.reset();
				int result = walk(graph, r, visited, 0 /*visited_count*/, 0 /*steps_taken*/, n);
				if (result < shortest)
					shortest = result;
			}
			return shortest;
		}
	};


	class Solution4  // accepted
	{
		/*
		* The key is to visit neighbors that are not visited yet.
		* If all neighbors are visited, visit those whose "state" doesn't lead to dead loop.
		* 
		* If I'm currently on 'x', and 'x' is connected to 'a', 'b', and 'c'.
		* Case 1: 'b' is not visited, 'a' and 'c' are visited.
		* Only try 'b', don't even try 'a' or 'c'.
		* What is the theoretical proof? --  It's an intuition.
		* 
		* Case 2: all three are visited.
		* Visit the neighbor nodes whose {node, graphState} tuple is not visited.
		* The progression of the program is a depth-first-search. The tree of progression has
		* nodes that are identified by that tuple. I can re-visite a node in the graph,
		* but make sure I don't re-visit a node in the progression tree.
		*/
	public:

		int shortest{ 0 };
		int stateMax;
		int len;

		int walk(vector<vector<int>>& g, vector<vector<int>>& r, bitset<32>& visited, int n)
		{
			int state = (int)visited.to_ulong();
			
			if (r[n][state] >= 0)
				return r[n][state];
			if (r[n][state] == -1)
				return numeric_limits<int>::max();
			
			
			if (state == stateMax)
			{
				r[n][state] = 0;
				return 0;
			}
			
			r[n][state] = -1;

			int result = numeric_limits<int>::max();
			bool isNextAllVisited = true;

			for (int const& next : g[n])
			{
				if (!visited[next])
				{
					isNextAllVisited = false;
					break;
				}
			}

			if (!isNextAllVisited)
			{
				for (int const& next : g[n])
				{
					if (!visited[next])
					{
						visited[next] = true;
						
						//int thisRunResult = 1 + walk(g, r, visited, next);
						/*
						* It's wrong to write this line this way. If "walk" call returns a numeric_limits<int>::max()
						* because it leads to a dead loop, adding 1 to the result will make it -2147483646.
						* It becomes the smallest number possible, so of course "result" is going to be overwritten with
						* this number and it will never be changed again.
						*/

						int subsequentResult = walk(g, r, visited, next);
						if (subsequentResult != numeric_limits<int>::max())
						{
							int thisRunResult = subsequentResult + 1;
							if (thisRunResult < result)
								result = thisRunResult;
						}
						visited[next] = false;
					}
				}
			}
			else
			{
				for (int const& next : g[n])
				{
					if (r[next][state] != -1)
					{
						int subsequentResult = walk(g, r, visited, next);
						if (subsequentResult != numeric_limits<int>::max())
						{
							int thisRunResult = subsequentResult + 1;
							if (thisRunResult < result)
								result = thisRunResult;
						}
					}
				}
			}

			r[n][state] = result;

			return result;
		}

		int shortestPathLength(vector<vector<int>>& graph)
		{
			shortest = numeric_limits<int>::max();
			len = graph.size();
			
			stateMax = static_cast<int>(pow(2, len) - 1);
			// if there are 4 nodes, I need 4 bits, maximum will be 15, I need an array of length 16, which is 2^4
			
			vector<vector<int>>r(len, vector<int>(stateMax+1, -2));
			// initial value is -2, '-1' means I have been to this node when the graph is in this state,
			// but I don't have a value for that {node,state} tuple yet, but I should not keep exploring that {node, state}
			// tuple, because I'll be in a dead loop.
			
			bitset<32> visited;
			
			for (int n = 0; n < len; n++)
			{
				visited.reset();
				visited[n] = true;
				int result = walk(graph, r, visited, n);
				if (result < shortest)
					shortest = result;
			}
			return shortest;
		}
	};


	class Solution5  // breadth-first-search   Time Limit Exceeded
	{
		/*
		* The "state" of the execution is described by this tuple: ("which node I'm standing on", "the state of the graph")
		* The "state" of the graph/sequence/.... can become part of some composite information that describe the "state" of
		* something bigger (e.g. the progression/execution of the program)
		*/
		/*
		* Because the nodes and edges can be re-visited, I can't determine the progression only by checking which node
		* I've visited. Essentially I need to determine the "next" step by the "state" of the progression, the state of
		* the progression usually *is* the state of the graph/sequence/.... But in this problem they are different.
		* The state of the graph is just part of the state of the progression.
		*/
		struct A
		{
			int id; // node id
			int graphState; // describe which node has been visited.
			A(int id_, int graphState_) : id(id_), graphState(graphState_) {}
		};
	public:
		int shortestPathLength(vector<vector<int>>& graph)
		{
			vector<set<int>> visited(graph.size());   // visited[5] is a set. This set stores the graph states when I stood on node#5, the combination of node#5 and graph states are already visited.
			vector<A> queue[2];
			int qSwitch = 0;
			vector<A>* q = &queue[qSwitch];
			int steps = 0;
			int allVisitedGraphState = (1 << graph.size()) - 1;  // if there are 4 nodes, their IDs are 0,1,2,3. I need 4 bits. 1<<4 gives me 10000. 10000 - 1 gives me 1111 exactly 4 bits.
			for (int i = 0; i < graph.size(); i++)
			{
				q->emplace_back(i, 1 << i);
				visited[i].insert(1 << i);
			}
			while (q->size() > 0)
			{
				qSwitch++;
				qSwitch %= 2;
				vector<A>* nextQ = &queue[qSwitch];
				for (A& a : *q)
				{
					if (a.graphState == allVisitedGraphState)
					{
						return steps;
					}
					vector<int>& connected = graph[a.id];
					for (int& nextNodeId : connected)
					{
						int potentialNextGraphState = a.graphState | (1 << nextNodeId);
						if (visited[nextNodeId].find(potentialNextGraphState) != visited[nextNodeId].end())
						{
							// have been on that node with that particular graph state
						}
						else
							nextQ->emplace_back(nextNodeId, potentialNextGraphState);
					}
				}
				q->clear();
				q = nextQ;
				steps++;
			}
			return -1;
		}
	};


	/*
	* Heuristic:
	* record the step counter when arriving at a node.
	* for example if there are 4 nodes: a,b,c,d
	* if the step counter for each node is: 3 1 4 2
	* This means the tour path is b->d->a->c
	* When standing on a node, try the non-visited node.
	* If all neighbors are visited, go to the one with the lowest step counter.
	* If all neighbors are visited, I'm kind of in a loop, going to the node with the lowest step counter
	* can lead me to a shorter path to escape the loop.
	*/
	class Solution6
	{
	public:
		int shortestPathLength(vector<vector<int>>& graph)
		{
			return 0;
		}
	};



	void Test_0847_Shortest_Path_Visiting_All_Nodes()
	{
		int test;
		vector<vector<int>> g1{ {1,2,3},{0},{0},{0} };
		vector<vector<int>> g2{ {1},{0,2,4},{1,3,4},{2},{1,2} };


		/*
		* 
		*    0             5
		*    |\           /|
		*    | 1----3----4 |
		*    |/     |     \|
		*    2      |      6
		*           7
		*          / \
		*         8---9
		* 
		* shortest path 
		* 
		* 8 -> 9 -> 7 -> 3 -> 1 -> 0 -> 2 -> 1 -> 3 -> 4 -> 5 -> 6
		* 
		* 11 steps
		*/

		vector<vector<int>> g3{ {1,2},{0,2,3},{0,1},{1,4,7},{3,5,6},{4,6},{4,5},{3,8,9},{7,9},{7,8} };
		//                      0     1       2     3       4       5     6     7       8     9


		/*
		* 
		*              3
		*             /|
		*    0----1--2 |
		*         |   \|
		*         4    5
		* 
		* 5 -> 3 -> 2 -> 1 -> 4 -> 1 -> 0
		* 
		* 6 steps
		* 
		*/

		vector<vector<int>> g4{ {1},{0,2,4},{1,3,5},{2,5},{1},{2,3} };
		//                      0   1       2       3     4   5


		Solution4 s;
		while (true)
		{
			cout << "run which test? ";
			cin >> test;
			
			switch (test)
			{
			case 1:
				cout << s.shortestPathLength(g1) << '\n';
				break;
			case 2:
				cout << s.shortestPathLength(g2) << '\n';
				break;
			case 3:
				cout << s.shortestPathLength(g3) << '\n';
				break;
			case 4:
				cout << s.shortestPathLength(g4) << '\n';
				break;
			default:
				return;
			}
		}
	}
}