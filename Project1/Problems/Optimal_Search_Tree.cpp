#include<vector>
#include<iostream>
#include<iomanip>
#include<queue>
#include<deque>

using namespace std;

namespace Optimal_Search_Tree {


	/*
	*       k4
	*      /  \
	*     k3   d4
	*    /  \
	*   d3  d4
	* 
	* cost[3][4] = cost[3][3] + prob_sum[3][4]  --- is this equation right?
	* 
	* This equation is incorrect.
	* 
	* The correct equation is:
	* 
	* cost[3][4] = cost of left subtree + cost of right subtree + probability sum[3][4]
	*                                     \___________________/
	*                                      this is the probability of the d4 dummy node
	*            = cost of left subtree + Pd4 + (Pd3 + Pk3 + Pd4 + Pk4 + Pd4)
	* 
	*  Note that Pd4 should appear twice in the equation
	* 
	* the left subtree is sanken down by 1 level, every node's cost grows up by 1 multiplying its probability.
	* the right subtree is also sanken down by 1 level, every node's cost grows up by 1 multiplying its probability.
	* root node's probability multiplying by 1 is the expected cost of the root node.
	* 
	* sum of every node's probability in left subtree + sum of every node's probability in right subtree   <--- this is increased cost comparing to two subtrees.
	* 
	*/


	class Solution1
	{
	public:

		void solve(vector<double> const& p,	vector<double> const& q)
		{
			size_t n = p.size();

			vector<vector<double>>cost(n, vector<double>(n, 0.0));
			vector<vector<double>>prob_sum(n, vector<double>(n, 0.0));
			vector<vector<int>>choice(n, vector<int>(n, 0));

			for (size_t i = 1; i < n; ++i)
			{
				cost[i][i] = p[i] + 2 * q[i - 1] + 2 * q[i];
				prob_sum[i][i] = p[i] + q[i - 1] + q[i];
				choice[i][i] = i;
			}

			for (size_t l = 2; l < n; ++l)
			{
				for (size_t i = 1; i <= n - l; ++i)
				{
					size_t end = i + l - 1;
					prob_sum[i][end] = prob_sum[i][end - 1] + p[end] + q[end];
					
					double local_min = INT_MAX;
					double cost_of_this_choice;
					
					for (size_t choose = i; choose <= end; ++choose)
					{
						if (choose == i)
						{
							cost_of_this_choice = q[i-1] + cost[i+1][end] + prob_sum[i][end];
						}
						else if (choose == end)
						{
							cost_of_this_choice = cost[i][end - 1] + q[end] + prob_sum[i][end];
						}
						else
						{
							cost_of_this_choice = cost[i][choose - 1] + cost[choose + 1][end] + prob_sum[i][end];
						}
						if (cost_of_this_choice < local_min)
						{
							local_min = cost_of_this_choice;
							choice[i][end] = choose;
						}
					}
					cost[i][end] = local_min;
				}
			}

			cout <<"optimal cost: " << cost[1][n - 1] << endl << endl;

			/*
			for (int i = 0; i < n; ++i)
			{
				for (int j = 0; j < n; ++j)
				{
					cout << "  " << cost[i][j];
				}
				cout << endl;
			}
			*/

			print_tree(choice, 1, n - 1, find_total_levels(choice, 1, n - 1));
		}

		int find_total_levels(vector<vector<int>> const & choice, int start, int end)
		{
			if (start == end)
			{
				return 2;
			}
			int left_subtree_levels, right_subtree_levels;
			int node_id = choice[start][end];
			if (node_id != start)
			{
				left_subtree_levels = find_total_levels(choice, start, node_id - 1);
			}
			else
			{
				left_subtree_levels = 1;
			}
			if (node_id != end)
			{
				right_subtree_levels = find_total_levels(choice, node_id + 1, end);
			}
			else
			{
				right_subtree_levels = 1;
			}
			return max(left_subtree_levels, right_subtree_levels) + 1;
		}

		/*
		*                     x                  <-- level 5
		*             x               x
		*         x       x       x       x
		*       x   x   x   x   x   x   x   x
		*      x x x x x x x x x x x x x x x x    <-- level 1
		*      |                             |
		*     order_on_this_level = 1       order_on_this_level = 16
		* 
		*  real nodes IDs range from 1 to n
		*  dummy nodes IDs range from 0 to n
		*          r1    r2    r3    r4
		*      d0     d1    d2    d3    d4
		*/

		struct Node_Printing_Info
		{
			int node_id;
			int order_on_this_level;
			int level;
			bool should_print_endl;
			bool is_dummy;
			int start;
			int end;
		};

		void print_tree(vector<vector<int>> const& choice, int start, int end, int total_levels)
		{
			std::deque<Node_Printing_Info> que;
			int node_id = choice[start][end];
			que.emplace_back(node_id, 1, total_levels, true, false, start, end);
			int cursor_pos = 0;
			int cursor_level = total_levels;
			int offset_of_this_node_in_this_subtree;
			int offset_of_this_on_this_level;
			int one_subtree_width;
			while (!que.empty())
			{
				Node_Printing_Info npi = que.front();
				que.pop_front();

				one_subtree_width = pow(2, npi.level);
				int offset_of_this_subtree_on_this_level = one_subtree_width * (npi.order_on_this_level - 1);
				int offset_of_this_node_in_this_subtree = one_subtree_width / 2;
				int total_offset = offset_of_this_subtree_on_this_level + offset_of_this_node_in_this_subtree;
				
				if (npi.level < cursor_level)
				{
					cout << endl;
					cursor_pos = 0;
					--cursor_level;
				}
				int print_space = total_offset - cursor_pos;

				//cout << setw(print_space) << std::right << (npi.is_dummy? 'D' : 'X');
				if (npi.is_dummy)
				{
					cout << setw(print_space) << std::right << 'D';
				}
				else
				{
					cout << setw(print_space) << std::right << npi.node_id;
				}
				cursor_pos += print_space;
				
				if (npi.is_dummy)
				{
					continue;
				}
				if (npi.start == npi.end)
				{
					que.emplace_back(npi.start - 1, npi.order_on_this_level * 2 - 1, npi.level - 1, false, true, 0, 0);
					que.emplace_back(npi.start, npi.order_on_this_level * 2, npi.level - 1, npi.should_print_endl, true, 0, 0);
					continue;
				}

				if (npi.start != npi.node_id)
				{
					int left_child_node_id = choice[npi.start][npi.node_id - 1];
					que.emplace_back(left_child_node_id, npi.order_on_this_level * 2 - 1, npi.level - 1, false, false, npi.start, npi.node_id - 1);
				}
				else
				{
					que.emplace_back(npi.start - 1, npi.order_on_this_level * 2 - 1, npi.level - 1, false, true, 0, 0);
				}
				if (npi.node_id != npi.end)
				{
					int right_child_node_id = choice[npi.node_id + 1][npi.end];
					que.emplace_back(right_child_node_id, npi.order_on_this_level * 2, npi.level - 1, npi.should_print_endl, false, npi.node_id + 1, npi.end);
				}
				else
				{
					que.emplace_back(npi.end, npi.order_on_this_level * 2, npi.level - 1, npi.should_print_endl, true, 0, 0);
				}
			}
		}
	};

	void test_print_tree_neatly()
	{
		vector<vector<int>> choice(5, vector<int>(5, 0));
		choice[1][1] = 1;
		choice[2][2] = 2;
		choice[3][3] = 3;
		choice[4][4] = 4;
		choice[1][4] = 2;
		choice[3][4] = 4;

		Solution1 so{};

		int total_levels = so.find_total_levels(choice, 1, 4);

		so.print_tree(choice, 1, 4, total_levels);
	}

	void test_optimal_search_tree1()
	{
		vector<double> p{0.00,     0.10,     0.15,     0.10,     0.25};
		vector<double> q{     0.05,     0.10,     0.05,     0.15,     0.05};
		Solution1 so{};
		so.solve(p, q);
	}

	void test_optimal_search_tree2()
	{
		vector<double> p{ 0.00, 0.15, 0.10, 0.05, 0.10, 0.20 };
		vector<double> q{ 0.05, 0.10, 0.05, 0.05, 0.05, 0.10 };
		Solution1 so{};
		so.solve(p, q);
	}

	void test_optimal_search_tree()
	{
		//test_optimal_search_tree1();
		test_optimal_search_tree2();
	}
}