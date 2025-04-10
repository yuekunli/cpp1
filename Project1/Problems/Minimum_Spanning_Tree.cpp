#include<vector>
#include<string>
#include<iostream>

using namespace std;

namespace Heap_With_Cannonical_Order_Reference {

	/*
	* ExternalData can be non-contiguous.
	* 
	*                                       actual application data (when used in minimum-spanning-tree, this would be a Node)
	*                                            ^
	*                                            |
	*                                         ped (point to ExternalData)
	*                                            ^
	*                                            |
	*                                            |
	* cannonically positioned array:    node0   node1   (index in heap)                  node x
	*     (array of HeapElementData)   |____|  |____|  |____| ......................... |____|
	*                                     \              / phe (point down)
	*                                      \            /  /
	*                                       \          /  /
	*                                        \        /  V
	*                                         \      /
	*                                          \    /
	*                                           \  /
	*                                            \/
	*                                            /\ ^
	*                                           /  \ \
	*                                          /    \ phed (point up)
	* heap  (array of HeapElement)         |____|  |____|
	*                                       key     key
	*/
	template <typename T>
	class ExternalData
	{
		// why can't the ExternalData just keep all the data instead of having yet another pointer
		// Because application outside this namespace has their own structure for data.
		// I can't ask application to fill out ExternalData
	public:
		T* data;
		int key_in_heap;
	};

	template <typename T>
	class MinHeapCannonicalOrderRef
	{
		struct HeapElementData;

		struct HeapElement
		{
			int key;
			HeapElementData* phed;
		};

		struct HeapElementData
		{
			ExternalData<T>* ped;
			HeapElement* phe;
			size_t index_in_heap;
		};

		vector<HeapElement> h;
		vector<HeapElementData> cannonical_array;

		size_t heap_length;

		void swap_elements(size_t a, size_t b)
		{
			HeapElementData* a_cannonical_positioned_data = h[a].phed;
			HeapElementData* b_cannonical_positioned_data = h[b].phed;

			//a_cannonical_positioned_data->phe = &h[b];
			//b_cannonical_positioned_data->phe = &h[a];
			swap(a_cannonical_positioned_data->phe, b_cannonical_positioned_data->phe);
			swap(a_cannonical_positioned_data->index_in_heap, b_cannonical_positioned_data->index_in_heap);
			
			swap(h[a], h[b]);
		}

	public:

		/*
		* ExternalData can be non-contiguous, but in this constructor, I'm using contiguous ExternalData,
		* initial_head: the index of the element in ExternalData array and canonical_array that is going to be the head of the heap initially.
		* External data vector and HeapElementData vector has a straightforward one-to-one mapping, i.e. the elements at the same index in each
		* arrays respectively map to each other.
		* The mapping between HeapElementData and HeapElement is also easy to see, the initial head point to the HeapElement at index 0.
		* In front of initial head, the HeapElementData and Heap element is skewed by 1, after initial head, it's a straightforward mapping.
		* 
		* 
		*   external data vector           |____|  |____|  |____|   |____|................. |____|
        *                                    ^        ^       ^        ^                      ^
		*                                    |        |       |        |                      |
		*                                    V        V       V        V                      V
		*    array of HeapElementData      |____|  |____|  |____|   |____|................. |____|
		*                                     \       \    initial    |                        |
		*                                      \       \    head      |                        |
		*                                       \       \  /          |                        |
		*                                        \       \/           |                        |
		*                                         \      /\           |                        |
		*                                          \    /  \          |                        |
		*                                           \  /    \         |                        |
		*                                            \/      \        |                        |
		*                                            /\       \       |                        |
		*                                           /  \      |       |                        |
		*                                          /    |     |       |                        |
		*                                         /     |     |       |                        |
		*                                        /      |     |       |                        |
		*                                       /      /      |       |                        |
		*                                      /      |       |       |                        |
		*                                     /       |       |       |                        |
		* heap  (array of HeapElement)     |____|  |____|  |____|   |____|...................|____|
		*/

		MinHeapCannonicalOrderRef(vector<ExternalData<T>> & ved, size_t initial_head)
		{
			size_t len = ved.size();
			heap_length = len;
			h = vector<HeapElement>(len);
			cannonical_array = vector<HeapElementData>(len);

			h[0].key = 0;
			h[0].phed = &cannonical_array[initial_head];

			cannonical_array[initial_head].index_in_heap = 0;
			cannonical_array[initial_head].phe = &h[0];
			cannonical_array[initial_head].ped = &ved[initial_head];
			ved[initial_head].key_in_heap = 0;

			for (size_t i = 0; i < initial_head; i++)
			{
				cannonical_array[i].index_in_heap = i + 1;
				cannonical_array[i].phe = &h[i + 1];
				cannonical_array[i].ped = &ved[i];
				ved[i].key_in_heap = INT_MAX;
				h[i + 1].key = INT_MAX;
				h[i + 1].phed = &cannonical_array[i];
			}
			for (size_t i = initial_head + 1; i < len; i++)
			{
				cannonical_array[i].index_in_heap = i;
				cannonical_array[i].phe = &h[i];
				cannonical_array[i].ped = &ved[i];
				ved[i].key_in_heap = INT_MAX;
				h[i].key = INT_MAX;
				h[i].phed = &cannonical_array[i];
			}
		}

		void change_key_if_smaller(size_t cannonical_index, int new_key)
		{
			int old_key = cannonical_array[cannonical_index].phe->key;

			if (new_key < old_key)
			{
				decrease_key(cannonical_array[cannonical_index].index_in_heap, new_key);
			}
		}
		
		// This heap implementation is tailored for minimum spanning tree, so it doesn't need a generalized increase_key operation
		// but actually an inline increase_key operation is still needed in extract_min
		void decrease_key(size_t i, int new_key)
		{
			h[i].key = new_key;
			h[i].phed->ped->key_in_heap = new_key;
			if (i == 0)
				return;

			size_t p_index = (i - 1) / 2;

			while (p_index > 0 && h[p_index].key > h[i].key)
			{
				swap_elements(p_index, i);
				i = p_index;
				p_index = (i - 1) / 2;

			}
			if (p_index == 0 && h[p_index].key > h[i].key) // I'm using size_t, (0-1)/2 will become big positive number, must not merge this case with the while loop
				swap_elements(p_index, i);
		}

		ExternalData<T>* extract_min()
		{
			ExternalData<T>* ret = h[0].phed->ped;

			if (heap_length == 1)
			{
				heap_length = 0;
				return ret;
			}

			swap_elements(0, heap_length - 1);
			
			heap_length--;

			size_t last_valid = heap_length - 1;

			size_t i = 0, left = 1, right = 2;

			while (left <= last_valid && right <= last_valid && (h[i].key > h[left].key || h[i].key > h[right].key))
			{
				if (h[i].key > h[left].key && h[right].key >= h[left].key) // left is the least
				{
					swap_elements(i, left);
					i = left;

				}
				else if (h[i].key > h[right].key && h[left].key >= h[right].key) // right is the least
				{
					swap_elements(i, right);
					i = right;
				}
				left = 2 * i + 1;
				right = 2 * i + 2;
			}

			if (left <= last_valid && right > last_valid && h[i].key > h[left].key)
				swap_elements(i, left);

			return ret;
		}

		size_t len()
		{
			return heap_length;
		}

		int get_key(size_t cannonical_position_index)
		{
			return cannonical_array[cannonical_position_index].phe->key;
		}

		bool is_still_in_heap(size_t cannonical_position_index)
		{
			return cannonical_array[cannonical_position_index].index_in_heap < heap_length;
		}
	};
}

// this implementation uses the previous heap implementation as a module
namespace Minimum_Spanning_Tree_With_Heap_Module {

	using namespace Heap_With_Cannonical_Order_Reference;

	struct Node
	{
		int id = 0;
		string name;
		Node* tree_edge_source = NULL;
	};

	struct Edge
	{
		struct Node* u;
		struct Node* v;
		int weight;
	};

	struct AdjInfoNode
	{
		struct Node* n;
		int weight;
		struct AdjInfoNode* next;

		AdjInfoNode(Node* _n, int _weight, AdjInfoNode* _next) : n(_n), weight(_weight), next(_next) {}
	};

	class MinimumSpanningTree
	{
	public:

		void trim_down(vector<Node>& nodes, vector<AdjInfoNode*>& adj, size_t start, vector<Edge>& tree)
		{
			vector<ExternalData<Node>> ved(nodes.size());
			for (size_t i = 0; i < nodes.size(); i++)
			{
				ved[i].data = &nodes[i];
			}

			MinHeapCannonicalOrderRef h(ved, start);

			while (true)
			{
				//Node* n = (Node*)(h.extract_min()->data);
				ExternalData<Node>* eData = h.extract_min();
				Node* n = eData->data;

				if (eData->key_in_heap == INT_MAX) // the rest of the vertices are not connected
					break;

				Node* p = n->tree_edge_source;

				if (p != NULL)
					tree.emplace_back(p, n, eData->key_in_heap); 
											// the "key_in_heap" of the min element must be the weight of an edge that
											// connects this node to a node that is already in the minimum-spanning-tree

				if (h.len() == 0)
					break;

				AdjInfoNode* adj_node = adj[n->id];
				while (adj_node != NULL)
				{
					int adj_node_id = adj_node->n->id;
					if (h.is_still_in_heap(adj_node_id))
					{
						int new_distance_to_tree = adj_node->weight;
						int old_distance_to_tree = h.get_key(adj_node_id);

						if (new_distance_to_tree < old_distance_to_tree)
						{
							h.change_key_if_smaller(adj_node_id, new_distance_to_tree);
							nodes[adj_node_id].tree_edge_source = n;
						}
					}
					adj_node = adj_node->next;
				}
			}
		}

		void show(vector<Edge> const& tree)
		{
			for (Edge const& e : tree)
			{
				if (e.u != NULL)
					cout << e.u->id << "   " << e.v->id << "   " << e.weight << endl;
			}
		}
	};



	void Test2()
	{
		vector<Node> n(9);
		vector<AdjInfoNode*> a(9);
		string s[9] = { "a", "b", "c", "d", "e", "f", "g", "h", "i" };
		for (int i = 0; i < 9; i++)
		{
			n[i].id = i;
			n[i].name = s[i];
			n[i].tree_edge_source = NULL;
		}

		AdjInfoNode* c, * p;

		p = new AdjInfoNode(&n[1], 4, NULL);
		a[0] = p;
		c = p;
		p = new AdjInfoNode(&n[7], 8, NULL);
		c->next = p;

		p = new AdjInfoNode(&n[7], 11, NULL);
		a[1] = p;
		c = p;
		p = new AdjInfoNode(&n[0], 4, NULL);
		c->next = p;
		c = p;
		p = new AdjInfoNode(&n[2], 8, NULL);
		c->next = p;

		p = new AdjInfoNode(&n[3], 7, NULL);
		a[2] = p;
		c = p;
		p = new AdjInfoNode(&n[5], 4, NULL);
		c->next = p;
		c = p;
		p = new AdjInfoNode(&n[8], 2, NULL);
		c->next = p;
		c = p;
		p = new AdjInfoNode(&n[1], 8, NULL);
		c->next = p;

		p = new AdjInfoNode(&n[5], 14, NULL);
		a[3] = p;
		c = p;
		p = new AdjInfoNode(&n[2], 7, NULL);
		c->next = p;
		c = p;
		p = new AdjInfoNode(&n[4], 9, NULL);
		c->next = p;

		p = new AdjInfoNode(&n[3], 9, NULL);
		a[4] = p;
		c = p;
		p = new AdjInfoNode(&n[5], 10, NULL);
		c->next = p;

		p = new AdjInfoNode(&n[6], 2, NULL);
		a[5] = p;
		c = p;
		p = new AdjInfoNode(&n[2], 4, NULL);
		c->next = p;
		c = p;
		p = new AdjInfoNode(&n[4], 10, NULL);
		c->next = p;
		c = p;
		p = new AdjInfoNode(&n[3], 14, NULL);
		c->next = p;

		p = new AdjInfoNode(&n[7], 1, NULL);
		a[6] = p;
		c = p;
		p = new AdjInfoNode(&n[8], 6, NULL);
		c->next = p;
		c = p;
		p = new AdjInfoNode(&n[5], 2, NULL);
		c->next = p;

		p = new AdjInfoNode(&n[8], 7, NULL);
		a[7] = p;
		c = p;
		p = new AdjInfoNode(&n[6], 1, NULL);
		c->next = p;
		c = p;
		p = new AdjInfoNode(&n[1], 11, NULL);
		c->next = p;
		c = p;
		p = new AdjInfoNode(&n[0], 8, NULL);
		c->next = p;

		p = new AdjInfoNode(&n[2], 2, NULL);
		a[8] = p;
		c = p;
		p = new AdjInfoNode(&n[6], 6, NULL);
		c->next = p;
		c = p;
		p = new AdjInfoNode(&n[7], 7, NULL);
		c->next = p;


		vector<Edge> mst;

		MinimumSpanningTree MST;
		MST.trim_down(n, a, 4, mst);
		MST.show(mst);

		for (size_t i = 0; i < 9; i++)
		{
			c = a[i];

			while (c != NULL)
			{
				p = c->next;
				delete c;
				c = p;
			}
		}
	}


	void Test_Minimum_Spanning_Tree_With_Heap_Module()
	{
		Test2();
	}
}

namespace Minimum_Spanning_Tree {

	// re-implement the cannonically referenced min-heap

	/*
	* cannonically positioned array:    node0   node1                                   node x
	*     (array of HeapElementData)   |____|  |____|  |____| ......................... |____|
	*                                     \              /
	*                                      \            /
	*                                       \          /
	*                                        \        /
	*                                         \      /
	*                                          \    /
	*                                           \  /
	*                                            \/
	*                                            /\
	*                                           /  \
	*                                          /    \
	* heap  (array of HeapElement)         |____|  |____|
	* 
	* Each HeapElementData has a pointer pointing to the corresponding HeapElement in the heap
	* Each HeapElement has a pointer pointing to the corresponding HeapElementData in the cannonically positioned array
	* 
	* These two arrays collaboratively need to support these operations:
	* (1). Given the ID of a graph node, quickly find its HeapElement in the heap
	* Use ID to locate HeapElementData in the cannonically positioned array, use the pointer to locate the HeapElement
	* 
	* (2). Given a pointer to a HeapElementData, quickly find out which node this is.
	* This is needed when I extract the head of the heap, I need to know which node has just been picked.
	* Such extraction gives me a HeapElement, following its pointer, I can get the HeapElementData object, now I need to know which graph node it represents.
	* HeapElementData has a pointer pointing to the real Node struct
	* 
	* (3). Given a pointer to a HeapElement, quickly find out the index of the element in the heap
	* This is necessary because heap operations need index to calculate parent and child indices.
	* A pointer to HeapElement comes from HeapElementData, there is a field in HeapElementData that tells the index of the HeapElement.
	* 
	* 
	*/
	struct Node
	{
		int id = 0;
		string name;
	};

	struct Edge
	{
		struct Node* u;
		struct Node* v;
		int weight;
	};


	struct HeapElementData;

	struct HeapElement
	{
		struct HeapElementData* cannonicalPos;
		int key;
	};

	struct HeapElementData
	{
		struct HeapElement* he;
		size_t index_in_heap;
		Node* data;
		Node* tree_edge_source;
		// for some algorithms, minimum-spanning-tree has an origin node, the tree grows from that node,
		// for a certain node, I need to remember which edge connects the node with the tree.
	};


	/*
	* +----+
	* | A  |-> AdjInfoNode -> AdjInfoNode -> AdjInfoNode -> ...
	* +----+
	* | B  |
	* +----+
	* |    |
	* .    .
	* .    .
	* |    |
	* +----+
	* | X  |
	* +----+
	*/
	struct AdjInfoNode  // struct used in adjacency-list
	{
		struct Node* n;
		int weight;
		struct AdjInfoNode* next;

		AdjInfoNode(Node* _n, int _weight, AdjInfoNode* _next) : n(_n), weight(_weight), next(_next) {}
	};



	// this implementation has no private data member, every method can be static
	// this implementation basically mixes the heap implementation with graph nodes definition.
	// it specializes the heap implementation for this minimum spanning tree purpose.
	// ideally, the heap implementation stays independent and is used as a replacable module.
	class Minimum_Spanning_Tree
	{
		void swap_elements(vector<HeapElement>& h, size_t a, size_t b)
		{
			HeapElement temp(h[a]);

			HeapElementData* a_cannonicalPos = h[a].cannonicalPos;
			HeapElementData* b_cannonicalPos = h[b].cannonicalPos;

			a_cannonicalPos->he = &h[b];
			b_cannonicalPos->he = &h[a];
			swap(a_cannonicalPos->index_in_heap, b_cannonicalPos->index_in_heap);
			h[a] = h[b];
			h[b] = temp;
		}

		/*
		*                 0
		*        1                2
		*    3       4        5       6
		* 
		*  child index : x
		*  parent index: (x-1) / 2
		* 
		*  parent index: x
		*  left child: 2x + 1
		*  right child: 2x + 2
		*/

		void min_heap_decrease_key(vector<HeapElement>& h, size_t i, int newKey)
		{
			h[i].key = newKey;
			if (i == 0)
				return;

			size_t p_index = (i - 1) / 2;

			while (p_index > 0 && h[p_index].key > h[i].key)
			{
				swap_elements(h, p_index, i);
				i = p_index;
				p_index = (i - 1) / 2;

			}
			if (p_index == 0 && h[p_index].key > h[i].key)
				swap_elements(h, p_index, i);
		}

		HeapElement remove_min(vector<HeapElement>& h, size_t heap_length)
		{
			HeapElement ret = h[0];
			
			if (heap_length == 1)
				return ret;

			swap_elements(h, 0, heap_length - 1);
			heap_length--;

			size_t last_valid = heap_length-1;

			size_t i = 0,  left = 1, right = 2;
			
			while (left <= last_valid && right <= last_valid && (h[i].key > h[left].key || h[i].key > h[right].key))
			{
				if (h[i].key > h[left].key && h[right].key >= h[left].key) // left is the least
				{
					swap_elements(h, i, left);
					i = left;
					
				}
				else if (h[i].key > h[right].key && h[left].key >= h[right].key) // right is the least
				{
					swap_elements(h, i, right);
					i = right;
				}
				left = 2 * i + 1;
				right = 2 * i + 2;
			}

			if (left <= last_valid && right > last_valid && h[i].key > h[left].key)
				swap_elements(h, i, left);

			return ret;
		}

	public:

		// the elements in cannonical array have straightforward 1-to-1 mapping with elements in "nodes" array
		// i.e. cannonical element at index x maps to "nodes" element at index x.
		// 
		// I start with node 0, what if node 0 is disconnected with every thing else?
		// obivously this implementation has the problem that it only grows a minimum spanning tree
		// from node 0, and if node 0 happens to be in a rather isolated part of the graph,
		// this implementation won't examine the majority of the graph.
		void trim_down(vector<Node> & nodes, vector<AdjInfoNode*>& adj, vector<Edge>& tree)
		{
			size_t len = adj.size();
			
			vector<HeapElement> h(len);

			vector<HeapElementData> a(len);  // cannonically ordered nodes

			for (size_t i = 0; i < len; i++)
			{
				h[i].key = i == 0 ? 0 : INT_MAX; // I'll grow the tree from node at index 0
				h[i].cannonicalPos = &a[i];
				a[i].data = &nodes[i];
				a[i].tree_edge_source = NULL;
				a[i].he = &h[i];
				a[i].index_in_heap = i;
			}

			while (true)
			{
				HeapElement min_element = remove_min(h, len);
				
				if (min_element.key == INT_MAX) // this element is disconnected, all the connected elements should've been extracted.
					break;

				Node* n = (Node*)min_element.cannonicalPos->data;
				Node* p = (Node*)min_element.cannonicalPos->tree_edge_source;
				
				tree.emplace_back(p, n, min_element.key);

				len--; // len was passed by value in remove_min, so I need to do decrement len here.

				if (len == 0)
					break;

				AdjInfoNode* adj_node = adj[n->id];
				while (adj_node != NULL)
				{
					int adj_node_id = adj_node->n->id;
					if (a[adj_node_id].index_in_heap < len)
					{
						int new_distance_to_tree = adj_node->weight;
						int old_distance_to_tree = a[adj_node_id].he->key;
						if (new_distance_to_tree < old_distance_to_tree)
						{
							min_heap_decrease_key(h, a[adj_node_id].index_in_heap, new_distance_to_tree);
							a[adj_node_id].tree_edge_source = n;
						}
					}
					adj_node = adj_node->next;
				}
			}
			//return tree;
		}

		void show(vector<Edge> const & tree)
		{
			for (Edge const& e : tree)
			{
				if (e.u != NULL)
					cout << e.u->id << "   " << e.v->id << "   " << e.weight << endl;
			}
		}
	};



	/*
	*                   0
	*                 / | \
	*                /  |  \
	*           13  /   |   \
	*              /    |    \  16
	*             /  15 |     \
    *            /      |      \
	*           1-------2-------3
	*           |   11      19  |
	*            \_____________/
	*                  27
	* 
	*   expected minimum spanning tree:
	* 
	*                    0
	*                   / \
	*                  /   \
	*                 /     \
	*                1--2    3
	* 
	*/
	void Test1()
	{
		vector<Node> nodes(4);
		for (int i = 0; i < 4; i++)
		{
			nodes[i].id = i;
		}

		AdjInfoNode *current, * p;

		vector<AdjInfoNode*> adj(4);

		p = new AdjInfoNode(&nodes[1], 13, NULL);
		adj[0] = p;
		current = p;
		p = new AdjInfoNode(&nodes[2], 15, NULL);
		current->next = p;
		current = p;
		p = new AdjInfoNode(&nodes[3], 16, NULL);
		current->next = p;

		p = new AdjInfoNode(&nodes[0], 13, NULL);
		adj[1] = p;
		current = p;
		p = new AdjInfoNode(&nodes[2], 11, NULL);
		current->next = p;
		current = p;
		p = new AdjInfoNode(&nodes[3], 27, NULL);
		current->next = p;

		p = new AdjInfoNode(&nodes[0], 15, NULL);
		adj[2] = p;
		current = p;
		p = new AdjInfoNode(&nodes[1], 11, NULL);
		current->next = p;
		current = p;
		p = new AdjInfoNode(&nodes[3], 19, NULL);
		current->next = p;
		
		p = new AdjInfoNode(&nodes[0], 16, NULL);
		adj[3] = p;
		current = p;
		p = new AdjInfoNode(&nodes[1], 27, NULL);
		current->next = p;
		current = p;
		p = new AdjInfoNode(&nodes[2], 19, NULL);
		current->next = p;

		Minimum_Spanning_Tree mst;
		vector<Edge> minimum_spanning_tree;
		mst.trim_down(nodes, adj, minimum_spanning_tree);
		mst.show(minimum_spanning_tree);


		current = adj[0];
		p = current->next;
		delete current;
		current = p;
		p = current->next;
		delete current;
		delete p;

		delete adj[1]->next->next->next;
		delete adj[1]->next->next;
		delete adj[1];

		delete adj[2]->next->next->next;
		delete adj[2]->next->next;
		delete adj[2];

		delete adj[3]->next->next->next;
		delete adj[3]->next->next;
		delete adj[3];
	}

	void Test2()
	{
		vector<Node> n(9);
		vector<AdjInfoNode*> a(9);
		string s[9] = { "a", "b", "c", "d", "e", "f", "g", "h", "i" };
		for (int i = 0; i < 9; i++)
		{
			n[i].id = i;
			n[i].name = s[i];
		}

		AdjInfoNode* c, * p;

		p = new AdjInfoNode(&n[1], 4, NULL);
		a[0] = p;
		c = p;
		p = new AdjInfoNode(&n[7], 8, NULL);
		c->next = p;

		p = new AdjInfoNode(&n[7], 11, NULL);
		a[1] = p;
		c = p;
		p = new AdjInfoNode(&n[0], 4, NULL);
		c->next = p;
		c = p;
		p = new AdjInfoNode(&n[2], 8, NULL);
		c->next = p;

		p = new AdjInfoNode(&n[3], 7, NULL);
		a[2] = p;
		c = p;
		p = new AdjInfoNode(&n[5], 4, NULL);
		c->next = p;
		c = p;
		p = new AdjInfoNode(&n[8], 2, NULL);
		c->next = p;
		c = p;
		p = new AdjInfoNode(&n[1], 8, NULL);
		c->next = p;

		p = new AdjInfoNode(&n[5], 14, NULL);
		a[3] = p;
		c = p;
		p = new AdjInfoNode(&n[2], 7, NULL);
		c->next = p;
		c = p;
		p = new AdjInfoNode(&n[4], 9, NULL);
		c->next = p;

		p = new AdjInfoNode(&n[3], 9, NULL);
		a[4] = p;
		c = p;
		p = new AdjInfoNode(&n[5], 10, NULL);
		c->next = p;
		
		p = new AdjInfoNode(&n[6], 2, NULL);
		a[5] = p;
		c = p;
		p = new AdjInfoNode(&n[2], 4, NULL);
		c->next = p;
		c = p;
		p = new AdjInfoNode(&n[4], 10, NULL);
		c->next = p;
		c = p;
		p = new AdjInfoNode(&n[3], 14, NULL);
		c->next = p;

		p = new AdjInfoNode(&n[7], 1, NULL);
		a[6] = p;
		c = p;
		p = new AdjInfoNode(&n[8], 6, NULL);
		c->next = p;
		c = p;
		p = new AdjInfoNode(&n[5], 2, NULL);
		c->next = p;

		p = new AdjInfoNode(&n[8], 7, NULL);
		a[7] = p;
		c = p;
		p = new AdjInfoNode(&n[6], 1, NULL);
		c->next = p;
		c = p;
		p = new AdjInfoNode(&n[1], 11, NULL);
		c->next = p;
		c = p;
		p = new AdjInfoNode(&n[0], 8, NULL);
		c->next = p;

		p = new AdjInfoNode(&n[2], 2, NULL);
		a[8] = p;
		c = p;
		p = new AdjInfoNode(&n[6], 6, NULL);
		c->next = p;
		c = p;
		p = new AdjInfoNode(&n[7], 7, NULL);
		c->next = p;


		vector<Edge> mst;

		Minimum_Spanning_Tree MST;
		MST.trim_down(n, a, mst);
		MST.show(mst);

		for (size_t i = 0; i < 9; i++)
		{
			c = a[i];
			
			while (c != NULL)
			{
				p = c->next;
				delete c;
				c = p;
			}
		}
	}

	void Test_Minimum_Spanning_Tree()
	{
		//Test1();
		Test2();
	}

}