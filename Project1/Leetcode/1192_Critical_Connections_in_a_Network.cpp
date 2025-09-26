#include<vector>
#include<iostream>
#include<unordered_set>

namespace _1192_Critical_Connections_in_a_Network {

	using namespace std;

	/*
	*    A--+
	*    |  |
	*    B  |
	*    |  |
	*    C--+
	* start with A, go to B, then go to C.
	* When I get to B, immediately, one of B's neighbors is A, I must not treat the B->A edge as a back edge, because obviously it's not.
	* C->A is apparently a back edge, when I retrieve back to A, I try to explore A->C, C is visited, A->C must not be counted as back edge again.
	* C is not on the active path at the moment.
	*/
	// this solution is more of a proof of concept, it records non-critical connections is a sequential way,
	// then go through all connections, do a sequential search in the vector of non-critical connections.
	// this part is obviously very slow.
	class Solution1
	{
		int n;
		void search(int k, vector<vector<int>>& adj, vector<vector<int>>& nonCritical, vector<bool>& visited, vector<int>& activePath, int& activePathIndex, vector<bool>& onActivePath)
		{
			visited[k] = true;
			activePath[activePathIndex] = k;
			onActivePath[k] = true;
			for (auto const& neighbor : adj[k])
			{
				if (!visited[neighbor])
				{
					++activePathIndex;
					search(neighbor, adj, nonCritical, visited, activePath, activePathIndex, onActivePath);
				}
				else
				{
					if (activePathIndex > 0 && neighbor == activePath[activePathIndex - 1])
					{
						continue;
					}
					if (!onActivePath[neighbor])
					{
						continue;
					}
					int i;
					for (i = activePathIndex - 1; i >= 0; --i)
					{
						if (activePath[i] == neighbor)
						{
							break;
						}
					}
					for (int j = i; j < activePathIndex; ++j)
					{
						nonCritical.emplace_back(initializer_list<int>{activePath[j], activePath[j + 1]});
					}
					nonCritical.emplace_back(initializer_list<int>{k, neighbor});
				}
			}
			onActivePath[k] = false;
			activePath[activePathIndex] = -1;
			--activePathIndex;
		}
	public:
		vector<vector<int>> criticalConnections(int n, vector<vector<int>>& connections)
		{
			this->n = n;
			vector<vector<int>> adj(n, vector<int>());
			for (auto const& edge : connections)
			{
				adj[edge[0]].push_back(edge[1]);
				adj[edge[1]].push_back(edge[0]);
			}
			vector<vector<int>> nonCritical;
			vector<bool>visited(n, false);
			vector<int> activePath(n, -1);
			vector<bool>onActivePath(n, false);
			int activePathIndex = 0;
			search(0, adj, nonCritical, visited, activePath, activePathIndex, onActivePath);

			vector<vector<int>>critical;
			bool found;
			for (auto const& edge : connections)
			{
				found = false;
				for (auto const& nc : nonCritical) // this is obviously very slow
				{
					if ((edge[0] == nc[0] && edge[1] == nc[1]) ||
						(edge[1] == nc[0] && edge[0] == nc[1]))
					{
						found = true;
					}
				}
				if (!found)
				{
					critical.emplace_back(edge);
				}
			}
			return critical;
		}
	};

	/*
	* use a full n-by-n 2D array to record non-critical connections.
	* recording a non-critical connection and checking if a connection is non-critical becomes constant time, 
	* but this apparently consumes too much memory
	*/
	// memory limit exceeded when n == 100,000
	class Solution2
	{
		int n;
		void search(int k, vector<vector<int>>& adj, vector<vector<int>>& nonCritical, vector<int>& vertexValueToIndexOnActivePath, vector<int>& activePath, int& currentActivePathIndex)
		{
			vertexValueToIndexOnActivePath[k] = currentActivePathIndex;
			activePath[currentActivePathIndex] = k;
			
			for (auto const& neighbor : adj[k])
			{
				if (vertexValueToIndexOnActivePath[neighbor] == -2)
				{
					++currentActivePathIndex;
					search(neighbor, adj, nonCritical, vertexValueToIndexOnActivePath, activePath, currentActivePathIndex);
				}
				else if (vertexValueToIndexOnActivePath[neighbor] == -1)
				{
					continue;
				}
				else if (vertexValueToIndexOnActivePath[neighbor] == currentActivePathIndex - 1)
				{
					continue;
				}
				else
				{
					int i = vertexValueToIndexOnActivePath[neighbor];

					for (int j = i; j < currentActivePathIndex; ++j)
					{
						if (activePath[j] < activePath[j + 1])
							nonCritical[activePath[j]][activePath[j + 1]] = 1;
						else
							nonCritical[activePath[j + 1]][activePath[j]] = 1;
					}
					if (k < neighbor)
						nonCritical[k][neighbor] = 1;
					else
						nonCritical[neighbor][k] = 1;
				}
			}
			activePath[currentActivePathIndex] = -1;
			--currentActivePathIndex;
			vertexValueToIndexOnActivePath[k] = -1;
		}
	public:
		vector<vector<int>> criticalConnections(int n, vector<vector<int>>& connections)
		{
			this->n = n;
			vector<vector<int>> adj(n, vector<int>());
			for (auto const& edge : connections)
			{
				adj[edge[0]].push_back(edge[1]);
				adj[edge[1]].push_back(edge[0]);
			}
			vector<vector<int>> nonCritical(n, vector<int>(n, 0));
			vector<int>vertexValueToIndexOnActivePath(n, -2);
			vector<int> activePath(n, -1);
			
			int currentActivePathIndex = 0;
			search(0, adj, nonCritical, vertexValueToIndexOnActivePath, activePath, currentActivePathIndex);

			vector<vector<int>>critical;
			bool found;
			for (auto const& edge : connections)
			{
				found = false;
				if (edge[0] < edge[1])
				{
					if (nonCritical[edge[0]][edge[1]])
						found = true;
				}
				else
				{
					if (nonCritical[edge[1]][edge[0]])
						found = true;
				}
				if (!found)
				{
					critical.emplace_back(edge);
				}
			}
			return critical;
		}
	};

	/*
	* The non-critical connections collection is similar to adjacency-matrix.
	* It's not a full n-by-n 2D array.
	* It has potentially n rows, but each row's length is different.
	*/
	// still memory limit exceeded
	class Solution3
	{
		int n;
		void search(int k, vector<vector<int>>& adj, vector<vector<int>>& nonCritical, vector<int>& vertexValueToIndexOnActivePath, vector<int>& activePath, int& currentActivePathIndex)
		{
			vertexValueToIndexOnActivePath[k] = currentActivePathIndex;
			activePath[currentActivePathIndex] = k;

			for (auto const& neighbor : adj[k])
			{
				if (vertexValueToIndexOnActivePath[neighbor] == -2)
				{
					++currentActivePathIndex;
					search(neighbor, adj, nonCritical, vertexValueToIndexOnActivePath, activePath, currentActivePathIndex);
				}
				else if (vertexValueToIndexOnActivePath[neighbor] == -1)
				{
					continue;
				}
				else if (vertexValueToIndexOnActivePath[neighbor] == currentActivePathIndex - 1)
				{
					continue;
				}
				else
				{
					int i = vertexValueToIndexOnActivePath[neighbor];

					for (int j = i; j < currentActivePathIndex; ++j)
					{
						if (activePath[j] < activePath[j + 1])
							nonCritical[activePath[j]].push_back(activePath[j + 1]);
						else
							nonCritical[activePath[j + 1]].push_back(activePath[j]);
					}
					if (k < neighbor)
						nonCritical[k].push_back(neighbor);
					else
						nonCritical[neighbor].push_back(k);
				}
			}
			activePath[currentActivePathIndex] = -1;
			--currentActivePathIndex;
			vertexValueToIndexOnActivePath[k] = -1;
		}
	public:
		vector<vector<int>> criticalConnections(int n, vector<vector<int>>& connections)
		{
			this->n = n;
			vector<vector<int>> adj(n, vector<int>());
			for (auto const& edge : connections)
			{
				adj[edge[0]].push_back(edge[1]);
				adj[edge[1]].push_back(edge[0]);
			}
			vector<vector<int>> nonCritical(n, vector<int>());
			vector<int> vertexValueToIndexOnActivePath(n, -2);
			vector<int> activePath(n, -1);

			int currentActivePathIndex = 0;
			search(0, adj, nonCritical, vertexValueToIndexOnActivePath, activePath, currentActivePathIndex);

			vector<vector<int>>critical;
			bool found;
			for (auto const& edge : connections)
			{
				found = false;
				if (edge[0] < edge[1])
				{
					for (auto const& v : nonCritical[edge[0]])
					{
						if (edge[1] == v)
						{
							found = true;
							break;
						}
					}
				}
				else
				{
					for (auto const& v : nonCritical[edge[1]])
					{
						if (edge[0] == v)
						{
							found = true;
							break;
						}
					}
						
				}
				if (!found)
				{
					critical.emplace_back(edge);
				}
			}
			return critical;
		}
	};

	/*
	* Convert an edge to an unsigned long long integer and put that integer in an unordered_set,
	* theoratically writting and checking that unordered_set is constant time, but still doesn't work
	*/
	// time limit exceeded
	class Solution4
	{
		int n;
		void search(int k, vector<vector<int>>& adj, unordered_set<unsigned long long>& nonCritical, vector<int>& vertexValueToIndexOnActivePath, vector<int>& activePath, int& currentActivePathIndex)
		{
			vertexValueToIndexOnActivePath[k] = currentActivePathIndex;
			activePath[currentActivePathIndex] = k;

			for (auto const& neighbor : adj[k])
			{
				if (vertexValueToIndexOnActivePath[neighbor] == -2)
				{
					++currentActivePathIndex;
					search(neighbor, adj, nonCritical, vertexValueToIndexOnActivePath, activePath, currentActivePathIndex);
				}
				else if (vertexValueToIndexOnActivePath[neighbor] == -1)
				{
					continue;
				}
				else if (vertexValueToIndexOnActivePath[neighbor] == currentActivePathIndex - 1)
				{
					continue;
				}
				else
				{
					int i = vertexValueToIndexOnActivePath[neighbor];

					for (int j = i; j < currentActivePathIndex; ++j)
					{
						if (activePath[j] < activePath[j + 1])
							nonCritical.insert((unsigned long long)activePath[j] * 100000ull + activePath[j + 1]);
						else
							nonCritical.insert((unsigned long long)activePath[j + 1] * 100000ull + activePath[j]);
					}
					if (k < neighbor)
						nonCritical.insert((unsigned long long)k * 100000ull + neighbor);
					else
						nonCritical.insert((unsigned long long)neighbor * 100000ull + k);
				}
			}
			activePath[currentActivePathIndex] = -1;
			--currentActivePathIndex;
			vertexValueToIndexOnActivePath[k] = -1;
		}
	public:
		vector<vector<int>> criticalConnections(int n, vector<vector<int>>& connections)
		{
			this->n = n;
			vector<vector<int>> adj(n, vector<int>());
			for (auto const& edge : connections)
			{
				adj[edge[0]].push_back(edge[1]);
				adj[edge[1]].push_back(edge[0]);
			}
			unordered_set<unsigned long long> nonCritical;
			vector<int> vertexValueToIndexOnActivePath(n, -2);
			vector<int> activePath(n, -1);

			int currentActivePathIndex = 0;
			search(0, adj, nonCritical, vertexValueToIndexOnActivePath, activePath, currentActivePathIndex);

			vector<vector<int>>critical;
			
			for (auto const& edge : connections)
			{
				
				if (edge[0] < edge[1])
				{
					if (nonCritical.contains((unsigned long long)edge[0] * 100000ull + edge[1]))
					{
						continue;
					}
				}
				else
				{
					if (nonCritical.contains((unsigned long long)edge[1] * 100000ull + edge[0]))
					{
						continue;
					}

				}
				
				critical.emplace_back(edge);
				
			}
			return critical;
		}
	};

	/*
	*   A
	*   |
	*   B
	*   |
	*   C
	*   | \
	*   D  \
	*   |   \
	*   E    |
	*   |   /
	*   F  /
	*   | /
	*   G 
	* 
	* Run DFS, start from A, go down the straight line, reach G, then there is a back edge,
	* C to G form a circle, every edge in this circlr is not a critical edge.
	* In the first a few solutions, when this back edge appears, I try to put the edges of the circle in a set,
	* then later examine all edges, compare each one with edges in that set, if an edge appear in that set,
	* don't count it as a critical one.
	* 
	* In this solution, when this back edge appears, I only put some markers in an array.
	* As DFS runs on, the active path constantly expand and contract, when the active path contract,
	* for example, when retreating from G along the straight line back up, check if the edge (F,G) is covered by a pair
	* of markers. In this case, it is covered by the marker pair [C, G]. This means (F, G) is part of a circle.
	*/
	// accepted 199ms beat 54%
	class Solution5
	{
		int n;
		const int VISITED_BUT_NOT_ON_ACTIVE_PATH = -1;
		const int NOT_VISITED = -2;

		void search(
			int k, 
			vector<vector<int>>& adj, 
			vector<vector<int>>& cc, 
			vector<int>& vertexToApIndex, // Ap: Active path
			vector<int>& activePath, 
			int& apIndex,
			vector<vector<int>>& loopMarkers,
			int& releventLmkIndex)
		{
			vertexToApIndex[k] = apIndex;
			activePath[apIndex] = k;

			for (auto const& neighbor : adj[k])
			{
				if (vertexToApIndex[neighbor] == NOT_VISITED)
				{
					++apIndex;
					search(neighbor, adj, cc, vertexToApIndex, activePath, apIndex, loopMarkers, releventLmkIndex);
				}
				else if (vertexToApIndex[neighbor] == VISITED_BUT_NOT_ON_ACTIVE_PATH)
				{
					continue;
				}
				else if (vertexToApIndex[neighbor] == apIndex - 1)
				{
					continue;
				}
				else
				{
					int neighborIndexOnAp = vertexToApIndex[neighbor];
					++releventLmkIndex;
					if (loopMarkers.size() < releventLmkIndex + 1)
					{
						loopMarkers.emplace_back(initializer_list<int>{neighborIndexOnAp, apIndex});
					}
					else
					{
						loopMarkers[releventLmkIndex][0] = neighborIndexOnAp;
						loopMarkers[releventLmkIndex][1] = apIndex;
					}
				}
			}
			if (apIndex > 0)
			{
				bool isInLoop = false;
				if (releventLmkIndex >= 0)
				{
					auto & loop = loopMarkers[releventLmkIndex];
					if (loop[0] <= apIndex - 1 && apIndex <= loop[1])
					{
						isInLoop = true;
						loop[1] = apIndex-1;
					}
					else if (apIndex == loop[0])
					{
						if (releventLmkIndex == 0)
						{
							releventLmkIndex = -1;
						}
						else
						{
							// find next relevent loop
							int i;
							for (i = releventLmkIndex - 1; i >= 0; --i)
							{
								if (loopMarkers[releventLmkIndex][0] <= loopMarkers[i][0])
								{
									continue;
								}
								else
								{
									if (loopMarkers[releventLmkIndex][1] < loopMarkers[i][1])
									{
										loopMarkers[i][1] = loopMarkers[releventLmkIndex][1];
									}
									break;
								}
							}
							releventLmkIndex = i;

							if (releventLmkIndex >= 0)
							{
								// test coverage again
								auto & loop2 = loopMarkers[releventLmkIndex];
								if (loop2[0] <= apIndex - 1 && apIndex <= loop2[1])
								{
									isInLoop = true;
									loop2[1] = apIndex-1;
								}
							}
						}
					}	
				}
				if (!isInLoop)
				{
					cc.emplace_back(initializer_list<int>{activePath[apIndex - 1], activePath[apIndex]});
				}
			}
			activePath[apIndex] = -1;
			vertexToApIndex[k] = VISITED_BUT_NOT_ON_ACTIVE_PATH;
			--apIndex;
		}
	public:
		vector<vector<int>> criticalConnections(int n, vector<vector<int>>& connections)
		{
			this->n = n;
			vector<vector<int>> adj(n, vector<int>());
			for (auto const& edge : connections)
			{
				adj[edge[0]].emplace_back(edge[1]);
				adj[edge[1]].emplace_back(edge[0]);
			}
			vector<vector<int>> cc; // critical connections
			vector<int> vertexToApIndex(n, NOT_VISITED);
			vector<int> activePath(n, -1);
			vector<vector<int>> loopMarkers;
			int apIndex = 0;
			int releventLmkIndex = -1;
			search(0, adj, cc, vertexToApIndex, activePath, apIndex, loopMarkers, releventLmkIndex);

			return cc;
		}
	};


	void test1()
	{
		Solution5 solu;
		auto connections = vector<vector<int>>{ {0, 1}, {1, 2}, {2, 0}, {1,3} };
		auto ret = solu.criticalConnections(4, connections);

		for (auto const& e : ret)
		{
			cout << e[0] << "  " << e[1] << endl;
		}
	}

	/*
	*        0--1--2--3--4--5--6--7--8--9--10--11
	*              |  |     |     |     |      |
	*              |  +-----+     |     |      |
	*              +--------------+     +------+
	*      
	*/
	void test2()
	{
		Solution5 solu;
		auto connections = vector<vector<int>>{ {0,1}, {1,2}, {2,3}, {3,4}, {4,5}, {5,6}, {6,7}, {7,8}, {8,9}, {9,10}, {10,11}, {2,7}, {3,5}, {9,11} };
		auto ret = solu.criticalConnections(12, connections);
		for (auto const& e : ret)
		{
			cout << e[0] << "  " << e[1] << endl;
		}
	}

	/*
	*   0--1--2--3--4
	*         |     |
	*         +-----+
	*/
	void test3()
	{
		Solution5 solu;
		auto connections = vector<vector<int>>{ {0,1}, {1,2}, {2,3}, {3,4}, {2,4} };
		auto ret = solu.criticalConnections(5, connections);
		for (auto const& e : ret)
		{
			cout << e[0] << "  " << e[1] << endl;
		}
	}

	void Test_1192_Critical_Connections_in_a_Network()
	{
		//test1();
		test2();
		//test3();
	}
}