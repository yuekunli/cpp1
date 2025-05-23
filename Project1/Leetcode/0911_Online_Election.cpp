#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<unordered_map>
#include<algorithm>
#include<numeric>

namespace _0911_Online_Election {

	using namespace std;

	// accepted but slow, 310ms beats 6%
	class TopVotedCandidate
	{
	public:

		struct TimeLeader
		{
			int epoch;
			int leader;
			TimeLeader(int epoch_, int leader_) :epoch(epoch_), leader(leader_) {}
		};

		vector<TimeLeader> epochLeaderChronicle;

		unordered_map<int, int> m; // "candidate id" : "votes received"
		                           // another approach is to scan the "persons" array once, and find the max id, and use an array to keep tally
									// revision 2025-05-21: constraint: 0 <= persons[i] <= persons.length. If there are 20 votes, persons' IDs are 0 ~ 19

		TopVotedCandidate(vector<int>& persons, vector<int>& times)
		{
			int currentLeader = -1;
			auto currentObject = this;
			// using "inner_product" is kind of an over-kill. All I want is just to process each (vote, time) pair.
			(void)inner_product(persons.begin(), persons.end(), times.begin(), 0,
				[](int const& a, int const& b)
				{
					return 0;
				},
				[&currentLeader, currentObject](int const& id, int const& t)
				{
					if (currentObject->m.find(id) == currentObject->m.end())
					{
						currentObject->m[id] = 1;
					}
					else
					{
						currentObject->m[id]++;
					}
					if (currentLeader == -1 || (currentLeader != id && currentObject->m[currentLeader] <= currentObject->m[id]))
					{
						// leader change
						currentObject->epochLeaderChronicle.emplace_back(t, id);
						currentLeader = id;
					}
					return 0;
				});
		}

		int q(int t)
		{
			auto compLambda = [](TimeLeader const& a, int const& b)
			{
				return a.epoch < b;
			};

			auto lb_it = lower_bound(epochLeaderChronicle.begin(), epochLeaderChronicle.end(), t, compLambda);

			if (lb_it != epochLeaderChronicle.end())
			{
				if (lb_it->epoch == t)
					return lb_it->leader;
				else
				{
					if (lb_it == epochLeaderChronicle.begin()) // no vote have been cast at time t
						return -1;
					else
						return prev(lb_it)->leader;
				}
			}
			else
			{
				return (epochLeaderChronicle.rbegin())->leader;
			}
		}
	};

	// accepted 28ms beats 93%
	// this solution is exactly the same as the first solution in terms of algorithm.
	// The first solution tries to be fancy and exquisite, this one simplifies things.
	class TopVotedCandidate2
	{
		vector<pair<int, int>> epochLeaderChronicle;
		unordered_map<int, int> m;

	public:
		TopVotedCandidate2(vector<int>& persons, vector<int>& times)
		{
			size_t len = persons.size();
			int currentLeader = -1;
			int currentLeaderVotes = -1;
			for (int i = 0; i < len; ++i)
			{
				if (m.find(persons[i]) == m.end())
				{
					m[persons[i]] = 1;
				}
				else
				{
					m[persons[i]] += 1;
					if (persons[i] == currentLeader)
					{
						++currentLeaderVotes;
					}
				}
				if (m[persons[i]] >= currentLeaderVotes && persons[i] != currentLeader)
				{
					currentLeader = persons[i];
					currentLeaderVotes = m[persons[i]];
					epochLeaderChronicle.emplace_back( times[i], currentLeader);
				}
			}
		}

		~TopVotedCandidate2() = default;

		int q(int t)
		{
			static auto compLambda = [](pair<int,int> const& a, int const& b)
			{
				return a.first < b;
			};

			auto lb_it = lower_bound(epochLeaderChronicle.begin(), epochLeaderChronicle.end(), t, compLambda);

			if (lb_it != epochLeaderChronicle.end())
			{
				if (lb_it->first == t)
					return lb_it->second;
				else
				{
					if (lb_it == epochLeaderChronicle.begin()) // no vote have been cast at time t
						return -1;
					else
						return prev(lb_it)->second;
				}
			}
			else
			{
				return (epochLeaderChronicle.rbegin())->second;
			}
		}
	};

	void Test_0911_Online_Election()
	{
		vector<int>persons{0,0,0,0,1};
		vector<int>times{0,6,39,52,75};

		vector<int>query_times{3,12,25,15,24,8};
		vector<int>query_results;
		
		TopVotedCandidate2* obj = new TopVotedCandidate2(persons, times);

		transform(query_times.begin(), query_times.end(), back_inserter(query_results),
			[&](int&t) {
				int result = obj->q(t);
				return (result);
			});

		for (int& i : query_results)
			cout << i << ' ';
		cout << "\n";

		delete obj;
	}
}