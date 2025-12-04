#include <iostream>
#include <vector>
#include "GraphMethod.h"
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <list>
#include <utility>
#include <algorithm>

using namespace std;

bool BFS(Graph* graph, char option, int vertex, ofstream& fout)
{
	// Breadth First Search

	// Exception handling if graph object is missing
	if (graph == nullptr)
	{
		return false;
	}
	// If option is input incorrectly
	if (option != 'O' && option != 'X')
	{
		return false;
	}
	int gSize = graph->getSize();

	// If vertex number is not input or does not exist
	if (vertex < 0 || vertex >= gSize)
	{
		return false;
	}

	// Initialize queue and visited array for BFS
	queue<int> visit;
	bool* visitNode = new bool[gSize + 1];

	for (int i = 0; i < graph->getSize() + 1; i++)
	{
		visitNode[i] = false;
	}

	map<int, int>::iterator iter;

	// Push start vertex to queue and mark as visited
	visit.push(vertex);
	visitNode[vertex] = true;

	fout << "========BFS=======\n";
	if (option == 'O')
	{
		fout << "Directed Graph BFS\n";
	}
	else if (option == 'X')
	{
		fout << "Undirected Graph BFS\n";
	}
	fout << "Start: " << vertex << "\n";

	// Repeat until queue is empty (search loop)
	while (!visit.empty())
	{
		map<int, int> edge;

		// Pop one vertex from the queue
		int v = visit.front();
		visit.pop();
		fout << v;

		// Get directed/undirected adjacent edges according to option
		if (option == 'O')
		{
			graph->getAdjacentEdgesDirect(v, &edge);
		}
		else
		{
			graph->getAdjacentEdges(v, &edge);
		}

		// Iterate through adjacent vertices
		for (iter = edge.begin(); iter != edge.end(); iter++)
		{
			// If vertex is not visited yet, push to queue and mark as visited
			if (!visitNode[iter->first])
			{
				visit.push(iter->first);
				visitNode[iter->first] = true;
			}
		}
		if (!visit.empty())
		{
			fout << " -> ";
		}
	}
	fout << "\n====================\n\n";
	delete[] visitNode;
	return true;
}

bool DFS(Graph* graph, char option, int vertex, ofstream& fout)
{
	// Check if vertex count is valid
	int gSize = graph->getSize();
	if (vertex < 0 || vertex >= gSize)
	{
		return false;
	}


	// Create array for visit check

	bool* visitNode = new bool[gSize + 1];
	for (int i = 0; i < gSize + 1; i++)
	{
		visitNode[i] = false;
	}

	map<int, int>::reverse_iterator iter;
	//stack for DFS
	stack<int> visit;
	visit.push(vertex);

	fout << "========DFS========\n";
	if (option == 'O')
	{
		fout << "Directed Graph DFS\n";
	}
	else if (option == 'X')
	{
		fout << "Undirected Graph DFS\n";
	}
	else
	{
		return false;
	}

	fout << "Start :" << vertex << "\n";
	bool first = true;

	// Repeat until stack is empty (search loop)
	while (!visit.empty())
	{
		map<int, int> edge;
		int top = visit.top();
		visit.pop();

		// Get adjacent edges
		if (option == 'O')
		{
			graph->getAdjacentEdgesDirect(top, &edge);
		}
		else
		{
			graph->getAdjacentEdges(top, &edge);
		}

		// Skip if the node popped from stack is already visited
		if (visitNode[top])
		{
			continue;
		}
		if (!first)
		{
			fout << " -> ";
		}
		fout << top;
		first = false;

		// Mark current node as visited
		visitNode[top] = true;

		// Push adjacent vertices to stack (Assumed logic using reverse_iterator to adjust so smaller numbers come out later (processed first due to stack nature))
		for (iter = edge.rbegin(); iter != edge.rend(); iter++)
		{
			if (!visitNode[iter->first])
			{
				visit.push(iter->first);
			}
		}

	}

	fout << "\n================\n\n";
	delete[] visitNode;
	return true;
}

// Find operation of Union-Find algorithm (Recursively search for parent node)
int getParent(int* mst, int vertex) {
	if (mst[vertex] == vertex)
		return vertex; // If the parent node is itself
	return mst[vertex] = getParent(mst, mst[vertex]);
}

// Union operation of Union-Find algorithm (Merge two sets)
void Union(int* mst, int ver1, int ver2) {
	int root1 = getParent(mst, ver1);
	int root2 = getParent(mst, ver2); // Store roots of the two vertices
	if (root1 < root2)
		mst[root2] = root1;
	else
		mst[root1] = root2; // Connect to the parent with the smaller number
}

// Check if two vertices are in the same set (Cycle detection)
bool isCycle(int* mst, int ver1, int ver2)
{
	return getParent(mst, ver1) == getParent(mst, ver2);
}

// Partition function for Quick Sort
int partition(vector<pair<int, pair<int, int>>>& A, int left, int right)
{
	int pivot = A[(left + right) / 2].first;
	int i = left - 1;
	int j = right + 1;

	while (true)
	{
		do { i++; } while (A[i].first < pivot);
		do { j--; } while (A[j].first > pivot);

		if (i >= j)
			return j;

		swap(A[i], A[j]);
	}
}


void insertion_sort(vector<pair<int, pair<int, int>>>& edges, int left, int right, int pivot) { // Insertion sort
	//int n = right - left + 1, j; // Number of elements to sort
	//for (int i = pivot + 1; i < pivot + n; i++) { // Sort from the pivot position onwards
	//	int key_first = edges[i].first; // Current weight to be inserted
	//	pair<int, int> key_second = edges[i].second; // Associated start and end nodes



	for (int i = left + 1; i <= right; i++)
	{
		auto key = edges[i];
		int j = i - 1;

		while (j >= left && edges[j].first > key.first)
		{
			edges[j + 1] = edges[j];
			j--;
		}
		edges[j + 1] = key;
	}
}


// Function to sort edges based on weight (Using Quick Sort)
void Kruskal_Sort(vector<pair<int, pair<int, int>>>& A, int left, int right)
{
	if (left >= right) return;

	int mid = partition(A, left, right);


	Kruskal_Sort(A, left, mid);
	Kruskal_Sort(A, mid + 1, right);
}
bool Kruskal(Graph* graph, ofstream& fout)
{
	// Select from smallest weight. Ensure no cycle is created
	 // Initialization
	int size = graph->getSize();
	int numEdge = 0, weightSum = 0;

	// Initialize Union-Find array for MST construction
	int* MST = new int[size];
	for (int i = 0; i < size; i++)
	{
		MST[i] = i;
	}

	map<int, int>* edge = new map<int, int>[size];

	vector<pair<int, pair<int, int>>> edges;
	vector<pair<int, pair<int, int>>> mst_edges;

	map<int, int>::iterator iter;


	// Read all edges and store in edges vector in (weight, (i, dest)) format.
	// ex 5->3, weight=10 ----> (10, (5, 3))
	for (int i = 0; i < size; i++)
	{
		graph->getAdjacentEdges(i, &edge[i]);
		for (iter = edge[i].begin(); iter != edge[i].end(); iter++)
		{
			int dest = iter->first;
			int weight = iter->second;
			edges.push_back(make_pair(weight, make_pair(i, dest)));
		}
	}

	// Sort edges according to weight
	sort(edges.begin(), edges.end());
	vector<pair<int, pair<int, int>>>::iterator iter1;

	// Check if fetching one by one from sorted edges forms a cycle,
	// if not, add to MST, add weight, perform Union, and save to mst_edges.
	for (iter1 = edges.begin(); iter1 != edges.end(); iter1++)
	{
		int start = iter1->second.first;
		int end = iter1->second.second;
		int weight = iter1->first;

		// Select only if no cycle is formed
		if (!isCycle(MST, start, end))
		{
			numEdge++;
			weightSum += weight;
			Union(MST, start, end);


			mst_edges.push_back({ start, {weight, end} });
		}
	}

	// Check if MST is complete (Number of edges must be vertex - 1)
	// disconnect
	if (numEdge != size - 1)
	{
		delete[] edge;
		delete[] MST;
		return false;
	}
	else // Completed. Print output
	{
		fout << "=======KRUSKAL===========\n";
		for (int i = 0; i < size; i++)
		{
			map<int, int> link_ver;
			map<int, int>::iterator iter;
			fout << "[" << i << "] ";

			// Iterate through MST edges and store edges connected to current vertex in map for output
			for (iter1 = mst_edges.begin(); iter1 != mst_edges.end(); iter1++)
			{

				int weight = (iter1->second).first;
				int end = (iter1->second).second;
				int start = iter1->first;

				if (i == end)
				{
					link_ver.insert(make_pair(start, weight));
				}

				if (i == start)
				{
					link_ver.insert(make_pair(end, weight));
				}
			}

			// Print connection information of current vertex
			for (iter = link_ver.begin(); iter != link_ver.end(); iter++)
			{
				fout << iter->first << "(" << iter->second << ") ";
			}
			fout << "\n";
		}
		fout << "Cost: " << weightSum << "\n======================\n\n";
		delete[] edge;
		delete[] MST;
		return true;
	}
}

int length(Graph* graph, int v, int u)
{
	if (v == u)
	{
		return 0;
	}

	int size = graph->getSize();
	map<int, int>* edge = new map<int, int>[size];
	vector<pair<int, pair<int, int>>> edges;

	map<int, int>::iterator iter;
	vector<pair<int, pair<int, int>>>::iterator iter1;

	// Collect all edges
	for (int i = 0; i < size; i++)
	{
		graph->getAdjacentEdges(i, &edge[i]);
		for (iter = edge[i].begin(); iter != edge[i].end(); iter++)
		{
			int destination = iter->first;
			int weight = iter->second;
			edges.push_back(make_pair(i, make_pair(destination, weight)));
		}
	}

	// Find and return weight of specific edge (v -> u)
	for (iter1 = edges.begin(); iter1 != edges.end(); iter1++)
	{
		if (iter1->first == v && (iter1->second).first == u)
		{
			delete[] edge;
			return (iter1->second).second;
		}
	}

	delete[] edge;
	return size;

}

// Select vertex with minimum distance among unvisited vertices in Dijkstra
int select(int* distance, bool* s, int size, int vertex)
{
	int min = INT_MAX;
	int u = 0;

	for (int i = 0; i < size; i++)
	{
		if (!s[i] && i != vertex && min > distance[i])
		{
			min = distance[i];
			u = i;
		}
	}
	return u;
}

bool Dijkstra(Graph* graph, char option, int vertex, ofstream& fout)
{
	if (option != 'O' && option != 'X') return false;

	int size = graph->getSize();
	if (vertex < 0 || vertex >= size) return false;

	// Prepare distance, visited, prev arrays
	vector<int> distance(size, INT_MAX);
	vector<int> prev(size, -1);
	vector<bool> visited(size, false);

	// Start vertex
	distance[vertex] = 0;

	// Dijkstra algorithm main loop
	for (int i = 0; i < size; i++)
	{
		// Select minimum distance among unvisited vertices
		int u = -1;
		int minDist = INT_MAX;
		for (int j = 0; j < size; j++)
		{
			if (!visited[j] && distance[j] < minDist)
			{
				minDist = distance[j];
				u = j;
			}
		}

		if (u == -1) break; // Break if there are no more reachable vertices
		visited[u] = true;

		// Get adjacent edges
		map<int, int> adj;
		if (option == 'O')
			graph->getAdjacentEdgesDirect(u, &adj);
		else
			graph->getAdjacentEdges(u, &adj);

		// Relaxation (Update distance)
		for (auto& a : adj)
		{
			int v = a.first;
			int weight = a.second;

			// If a shorter path is found, update distance and previous vertex info
			if (distance[u] != INT_MAX && distance[v] > distance[u] + weight)
			{
				distance[v] = distance[u] + weight;
				prev[v] = u;
			}
		}
	}

	// ===== Output =====

	fout << "============DIJKSTRA==========\n";
	if (option == 'O')
		fout << "Directed Graph Dijkstra\n";
	else
		fout << "Undirected Graph Dijkstra\n";

	fout << "Start: " << vertex << "\n";

	for (int i = 0; i < size; i++)
	{
		fout << "[" << i << "] ";

		if (i == vertex)
		{
			fout << vertex << " (0)\n";
			continue;
		}

		if (distance[i] == INT_MAX)
		{
			fout << "x\n";
			continue;
		}

		// Print path (Reverse the backtracked path using stack)
		stack<int> path;
		int cur = i;
		while (cur != -1)
		{
			path.push(cur);
			cur = prev[cur];
		}

		while (!path.empty())
		{
			fout << path.top();
			path.pop();
			if (!path.empty()) fout << " -> ";
		}

		fout << " (" << distance[i] << ")\n";
	}

	fout << "=========================\n\n";
	return true;
}

bool Bellmanford(Graph* graph, char option, int s_vertex, int e_vertex, ofstream& fout)
{
	// Exception handling and validation
	if (option != 'O' && option != 'X')
	{
		return false;
	}
	int size = graph->getSize();
	if (s_vertex < 0 || s_vertex >= size)
	{
		return false;
	}
	if (e_vertex < 0 || e_vertex >= size)
	{
		return false;
	}

	if (s_vertex == e_vertex)
	{
		fout << "==========BELLMANFORD==========\n";
	}
	int* distance = new int[size]; // Shortest distance from s_vertex to i
	int* before = new int[size];	// Previous vertex in shortest path to i

	map<int, int>* edge = new map<int, int>[size];
	vector<pair<int, pair<int, int>>> edges;

	map<int, int>::iterator iter;
	vector<pair<int, pair<int, int>>>::iterator iter1;

	// Save all edges of the graph to vector
	for (int i = 0; i < size; i++)
	{
		if (option == 'O')
		{
			graph->getAdjacentEdgesDirect(i, &edge[i]);
		}
		else
		{
			graph->getAdjacentEdges(i, &edge[i]);
		} // Get all edges reachable from i
		for (iter = edge[i].begin(); iter != edge[i].end(); iter++)
		{
			int destination = iter->first;
			int weight = iter->second;
			edges.push_back(make_pair(weight, make_pair(i, destination)));

		}
	}

	// Initialize distances to all vertices as infinite
	for (int i = 0; i < size; i++)
	{
		distance[i] = INT_MAX;
		before[i] = -1;
	}
	distance[s_vertex] = 0;

	// Update if there is a shorter path than currently known shortest distance
	// Repeat V-1 times (Shortest path has at most V-1 edges)
	for (int i = 1; i <= size - 1; i++)	// Shortest path consists of at most (total vertices - 1) edges.
	{

		for (iter1 = edges.begin(); iter1 != edges.end(); iter1++)
		{
			int start = (iter1->second).first;
			int end = (iter1->second).second;
			int weight = iter1->first;

			// Update if distance to start is not infinite and a shorter path is found
			if (distance[start] == INT_MAX)
			{
				continue;
			}
			if (distance[end] > distance[start] + weight)
			{
				distance[end] = distance[start] + weight;
				before[end] = start;
			}
		}
	}

	// Check for negative cycles (Negative cycle exists if update occurs when repeated one more time)
	for (iter1 = edges.begin(); iter1 != edges.end(); iter1++)
	{
		int start = (iter1->second).first;
		int end = (iter1->second).second;
		int weight = iter1->first;

		if (distance[start] == INT_MAX)
		{
			continue;
		}
		if (distance[end] > distance[start] + weight)
		{
			delete[] edge;
			delete[] distance;
			delete[] before;
			return false;
		}
	}

	// If e_vertex is unreachable
	if (distance[e_vertex] == INT_MAX)
	{
		fout << "=========BELLMANFORD=======\n";
		if (option == 'O')
		{
			fout << "Directed ";
		}
		else if (option == 'X')
		{
			fout << "Undirected ";
		}

		fout << "Graph Bellman-Ford\n";
		fout << "x\n";
		fout << "======================\n\n";
		delete[] edge;
		delete[] distance;
		delete[] before;
		return true;
	}

	// Generate path by backtracking
	stack<int> sPath; // Stack for printing shortest path
	sPath.push(e_vertex);
	int i = e_vertex;
	while (1)
	{
		if (before[i] == -1)
		{
			break;
		}
		i = before[i];
		sPath.push(i);
	}

	fout << "=========BELLMANFORD=======\n";
	if (option == 'O')
	{
		fout << "Directed ";
	}
	else if (option == 'X')
	{
		fout << "Undirected ";
	}

	fout << "Graph Bellman-Ford\n";

	// Print path
	while (!sPath.empty())
	{
		int vertex = sPath.top();
		sPath.pop();
		fout << vertex;
		if (sPath.size() != 0)
		{
			fout << " -> ";
		}
	}
	fout << "\n";
	fout << "Cost: " << distance[e_vertex]
		<< "\n======================\n\n";
	delete[] edge;
	delete[] distance;
	delete[] before;
	return true;
}

bool FLOYD(Graph* graph, char option, ofstream& fout)
{
	int size = graph->getSize();
	int** arr = new int* [size];

	map<int, int>* edge = new map<int, int>[size];
	vector<pair<int, pair<int, int>>> edges;
	map<int, int>::iterator iter;
	vector<pair<int, pair<int, int>>>::iterator iter1;

	// Get all edges
	for (int i = 0; i < size; i++)
	{
		if (option == 'O')
		{
			graph->getAdjacentEdgesDirect(i, &edge[i]);
		}
		else
		{
			graph->getAdjacentEdges(i, &edge[i]);
		}
		for (iter = edge[i].begin(); iter != edge[i].end(); iter++)
		{
			int des = iter->first;
			int weight = iter->second;
			edges.push_back(make_pair(weight, make_pair(i, des)));
		}
	}

	// arr[i][j] = shortest distance from i to j
	for (int i = 0; i < size; i++)
	{
		arr[i] = new int[size];
	}

	// Initialize arr: weight if edge i->j exists, otherwise INT_MAX (infinite)
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			bool flag = false;
			if (i == j)
			{
				arr[i][j] = 0;
				continue;
			}
			for (iter1 = edges.begin(); iter1 != edges.end(); iter1++)
			{
				if (flag)
				{
					break;
				}

				if (i == (iter1->second).first && j == (iter1->second).second)
				{
					arr[i][j] = iter1->first;
					flag = true;
				}
			}
			if (!flag)
			{
				arr[i][j] = INT_MAX;
			}
		}
	}

	// k = intermediate node. Update if i->k->j is shorter (Floyd-Warshall core logic)
	for (int k = 0; k < size; k++)
	{
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				// Must check feasibility first (Prevent INT_MAX overflow)
				if (arr[i][k] != INT_MAX && arr[k][j] != INT_MAX)
				{
					if (arr[i][k] + arr[k][j] < arr[i][j])
					{
						arr[i][j] = arr[i][k] + arr[k][j];
					}
				}
			}
		}
	}



	// Check for negative cycles (Negative cycle if distance back to self is less than 0)
	for (int i = 0; i < size; i++)
	{
		if (arr[i][i] < 0) // Negative cycle only if less than 0, not just non-zero
		{
			for (int r = 0; r < size; r++) delete[] arr[r];
			delete[] arr;
			delete[] edge;
			return false;
		}
	}

	// Output
	fout << "============FLOYD=========\n";
	if (option == 'O')
	{
		fout << "Directed ";
	}
	else if (option == 'X')
	{
		fout << "Undirected ";
	}

	fout << "Graph Floyd\n\t";

	// Print table header
	for (int i = 0; i < size; i++)
	{
		fout << "[" << i << "]\t";
	}
	fout << "\n";

	// Print result matrix
	for (int i = 0; i < size; i++)
	{
		fout << "[" << i << "]";
		for (int j = 0; j < size && fout << '\t'; j++)
		{
			if (arr[i][j] == INT_MAX)
			{
				fout << "x";
			}
			else
			{
				fout << arr[i][j];
			}
		}
		fout << "\n";
	}
	fout << "===========================\n\n";
	delete[] edge;
	for (int i = 0; i < size; i++)
	{
		delete[] arr[i];
	}
	delete[] arr;
	return true;
}

bool Centrality(Graph* graph, ofstream& fout)
{
	int size = graph->getSize();

	if (size <= 0)
	{
		return false;
	}

	// Configure arrays
	int** distance = new int* [size];
	for (int i = 0; i < size; i++)
	{
		distance[i] = new int[size];
	}

	map<int, int>* edge = new map<int, int>[size];
	vector<pair<int, pair<int, int>>> edges;
	map<int, int>::iterator iter;
	vector<pair<int, pair<int, int>>>::iterator iter1;

	// Get all edges
	for (int i = 0; i < size; i++)
	{
		graph->getAdjacentEdges(i, &edge[i]);
		for (iter = edge[i].begin(); iter != edge[i].end(); iter++)
		{
			int des = iter->first;
			int weight = iter->second;
			edges.push_back(make_pair(weight, make_pair(i, des)));
		}
	}




	// Initialize all values to INT_MAX
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (i == j) distance[i][j] = 0;
			else distance[i][j] = INT_MAX;
		}
	}

	// Iterate through edge list once and update bidirectionally
	for (iter1 = edges.begin(); iter1 != edges.end(); iter1++)
	{
		int u = (iter1->second).first;
		int v = (iter1->second).second;
		int w = iter1->first;

		// Update both directions (Keep smaller value)
		if (distance[u][v] > w) distance[u][v] = w;
		if (distance[v][u] > w) distance[v][u] = w;
	}


	// Calculate all-pairs shortest paths (Using Floyd-Warshall algorithm)
	for (int k = 0; k < size; k++)
	{
		for (int i = 0; i < size; i++)
		{
			if (distance[i][k] == INT_MAX) continue;

			for (int j = 0; j < size; j++)
			{
				if (distance[k][j] == INT_MAX) continue;

				if (distance[i][j] > distance[i][k] + distance[k][j])
				{
					distance[i][j] = distance[i][k] + distance[k][j];
				}
			}
		}
	}


	// Check for negative cycles
	for (int i = 0; i < size; i++)
	{
		if (distance[i][i] != 0)
		{
			delete[] edge;
			for (int j = 0; j < size; j++)
			{
				delete[] distance[i];
			}
			delete[] distance;
			return false;
		}
	}


	// ===== Calculate Centrality =====
	double* centrality = new double[size];
	bool* available = new bool[size];

	// Calculate Closeness Centrality for each vertex
	for (int i = 0; i < size; i++)
	{
		long long sum = 0;
		bool ok = true;

		for (int j = 0; j < size; j++)
		{
			if (i == j) continue;

			// Centrality can be calculated only if reachable to all other vertices
			if (distance[i][j] == INT_MAX)
			{
				ok = false; // Unreachable -> print x
				break;
			}
			sum += distance[i][j];
		}

		if (!ok)
		{
			available[i] = false;
			centrality[i] = -1;
		}
		else
		{
			available[i] = true;
			centrality[i] = (double)(size - 1) / (double)sum;
		}
	}

	// Find max centrality
	double maxC = -1.0;
	for (int i = 0; i < size; i++)
	{
		if (available[i] && centrality[i] > maxC)
		{
			maxC = centrality[i];
		}
	}

	// ===== Output =====
	fout << "========CENTRALITY========\n";

	for (int i = 0; i < size; i++)
	{
		fout << "[" << i << "] ";

		if (!available[i])
		{
			fout << "x\n";
		}
		else
		{
			// Recalculate sum -> Print in fraction format
			long long sum = 0;
			for (int j = 0; j < size; j++)
			{
				if (i != j) sum += distance[i][j];
			}

			fout << (size - 1) << "/" << sum;

			// Mark vertex with highest centrality
			if (centrality[i] == maxC)
			{
				fout << " <- Most Central";
			}
			fout << "\n";
		}
	}
	fout << "=======================\n\n";

	// Free memory
	delete[] edge;
	delete[] available;
	delete[] centrality;

	for (int i = 0; i < size; i++)
		delete[] distance[i];
	delete[] distance;

	return true;
}