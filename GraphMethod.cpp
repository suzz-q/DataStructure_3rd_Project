#include <iostream>
#include <vector>
#include "GraphMethod.h"
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <list>
#include <utility>

using namespace std;

bool BFS(Graph* graph, char option, int vertex, ofstream& fout)
{
	//너비 우선 탐색
	
	if (graph == nullptr)
	{
		return false;
	}
	//옵션을 잘못 입력한 경우
	if (option != 'O' && option != 'X')
	{
		return false;
	}
	int gSize = graph->getSize();

	//정점 번호가 입력되지 않았거나 존재하지 않는 경우
	if (vertex<0 || vertex>=gSize)
	{
		return false;
	}

	queue<int> visit;
	bool* visitNode = new bool[gSize + 1];

	for (int i = 0; i < graph->getSize() + 1; i++)
	{
		visitNode[i] = false;
	}

	map<int, int>::iterator iter;

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
	fout << "Start: " << vertex<<"\n";

	while (!visit.empty())
	{
		map<int, int> edge;

		int v = visit.front();
		visit.pop();
		fout << v;
		if (option == 'O')
		{
			graph->getAdjacentEdgesDirect(v, &edge);
		}
		else
		{
			graph->getAdjacentEdges(v, &edge);
		}
		//graph->getAdjacentEdges(v, &edge);
		for (iter = edge.begin(); iter != edge.end(); iter++)
		{
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
	//정점 개수가 유효한지 확인
	int gSize = graph->getSize();
	if (vertex<0 || vertex>=gSize)
	{
		return false;
    }

	
	//방문 체크를 위한 배열 생성

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

	fout << "Start :" << vertex << "\n";
	
	while (!visit.empty())
	{
		map<int, int> edge;
		int top = visit.top();
		visit.pop();

		if (option == 'O')
		{
			graph->getAdjacentEdgesDirect(top, &edge);
		}
		else
		{
			graph->getAdjacentEdges(top, &edge);
		}
		if (visitNode[top])
		{
			continue;
		}

		fout << top;
		
		visitNode[top] = true;
		for (iter = edge.rbegin(); iter != edge.rend(); iter++)
		{
			if (!visitNode[iter->first])
			{
				visit.push(iter->first);
			}
		}
		if (!visit.empty() && !visitNode[visit.top()])
		{
			fout << " -> ";
		}
	}

	fout << "\n================\n\n";
	delete[] visitNode;
	return true;
}

int getParent(int* mst, int vertex) {
	if (mst[vertex] == vertex)
		return vertex; // If the parent node is itself
	return getParent(mst, mst[vertex]);
}

void Union(int* mst, int ver1, int ver2) {
	int root1 = getParent(mst, ver1);
	int root2 = getParent(mst, ver2); // Store roots of the two vertices
	if (root1 < root2)
		mst[root2] = root1;
	else
		mst[root1] = root2; // Connect to the parent with the smaller number
}
bool isCycle(int* mst, int ver1, int ver2)
{
	return getParent(mst, ver1) == getParent(mst, ver2);
}
//int partition(vector<pair<int, pair<int, int>>>& edges, int left, int right) { // Divide and conquer
//	int low = left + 1, high = right;
//	int pivot = edges[left].first; // Use the weight of the left edge as the pivot
//
//	while (low < high) {
//		while (edges[low].first < pivot) low++; // Increment `low` until a value >= pivot is found
//		while (edges[high].first > pivot) high--; // Decrement `high` until a value <= pivot is found
//
//		if (low < high) { // If `low` and `high` have not crossed
//			swap(edges[low], edges[high]); // Swap their values
//		}
//	}
//	swap(edges[left], edges[high]); // Swap the pivot with the value at `high`
//
//	return high; // Return the new position of the pivot
//}

int partition(vector<pair<int, pair<int, int>>>& A, int left, int right)
{
	int pivot = A[left].first;
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

	//	for (j = i - 1; j >= 0 && edges[j].first > key_first; j--) { // Traverse in reverse order and shift larger values
	//		edges[j + 1] = edges[j];
	//	}
	//	edges[j + 1].first = key_first;
	//	edges[j + 1].second = key_second; // Insert the original information
	//}

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

void Kruskal_Sort(vector<pair<int, pair<int, int>>>& edges, int left, int right)
{
	//if (left < right)
	//{
	//	if (right - left + 1 <= 6)
	//	{
	//		insertion_sort(edges, left, right, left);
	//	}
	//	else { // Otherwise, proceed with partitioning
	//		int pivot = partition(edges, left, right);
	//		Kruskal_Sort(edges, left, pivot - 1);
	//		Kruskal_Sort(edges, pivot + 1, right);
	//	}
	//}

	if (left < right)
	{
		int p = partition(edges, left, right);
		Kruskal_Sort(edges, left, p);
		Kruskal_Sort(edges, p + 1, right);
	}

}


bool Kruskal(Graph* graph, ofstream& fout)
{
   //weight가 작은 것부터 선택. cycle은 생기지 않도록
	//초기화 
	int size = graph->getSize();
	int numEdge = 0, weightSum = 0;
	int* MST = new int[size];
	for (int i = 0; i < size; i++)
	{
		MST[i] = i;
	}

	map<int, int>* edge = new map<int, int>[size];

	vector<pair<int, pair<int, int>>> edges;
	vector<pair<int, pair<int, int>>> mst_edges;

	map<int, int>::iterator iter;


	//모든 간선을 읽어 (weight,(i, dest))형태로 edges 벡터에 저장함.
	//ex 5->3, weight=10 ----> (10, (5, 3))
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

	//가중치에 따라 edge 정렬 
	Kruskal_Sort(edges, 0, edges.size() - 1);
	vector<pair<int, pair<int, int>>>::iterator iter1;

	//정렬된 edge에서 하나씩 가져와 싸이클을 형성하는지 확인하고, 
	// 형성하지 않는다면 MST에 추가하고, 가중치를 더하고, Union을 수행하고 mst_edge에 저장한다.
	for (iter1 = edges.begin(); iter1 != edges.end(); iter1++)
	{
		int start = iter1->second.first;
		int end = iter1->second.second;
		int weight = iter1->first;

		if (!isCycle(MST, start, end))
		{
			numEdge++;
			weightSum += weight;
			Union(MST, start, end);
			mst_edges.push_back(make_pair(start, make_pair(weight, end)));
		}
	}

	//MST가 완성되었는지 확인
	//disconnect
	if (numEdge != size - 1)
	{
		delete[] edge;
		delete[] MST;
		return false;
	}
	else //완성됨. 출력하기
	{
		fout << "=======KRUSKAL===========\n";
		for (int i = 0; i < size; i++)
		{
			map<int, int> link_ver;
			map<int, int>::iterator iter;
			fout << "[" << i << "] ";
			for (iter1 = mst_edges.begin(); iter1 != mst_edges.end(); iter1++)
			{
				//메모리 낭비 심하지 않나
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

			for (iter = link_ver.begin(); iter != link_ver.end(); iter++)
			{
				fout << iter->first << "(" << iter->second << ")";
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

int select(int* distance, bool* s, int size, int vertex)
{
	int min=INT_MAX;
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
	if (option != 'O' && option != 'X')
	{
		return false;
	}

	int size = graph->getSize();
	if (vertex < 0 || vertex >= size)
	{
		return false;
	}

	//현재 포함된 노트 집합, 영역
	bool* s = new bool[size];
	int* distance = new int[size];
	int* prev = new int[size];

	for (int i = 0; i < size; i++)
	{
		s[i] = false;
		prev[i] = -1;
		distance[i] = length(graph, vertex, i);
		if (distance[i] != size)	//연결 되었다면
		{
			prev[i] = vertex;	//직전 노드가 vertex
		}
	}
	s[vertex] = true;
	prev[vertex] = -1;

	int u = vertex;
	for (int i = 0; i < size - 1; i++)
	{
		u = select(distance, s, size, u);
		s[u] = true;
		for (int w = 0; w < size; w++)
		{
			if (!s[w])
			{
				int existing_dis = distance[w];
				distance[w] = min((distance[u] + length(graph, u, w)), distance[w]);
				if (existing_dis != distance[w])
				{
					prev[w] = u;
				}

			}
		}
	}
	fout << "============DIJKSTRA==========\n";
	if (option == 'O')
	{
		fout << "Directed Graph Dijkstra\n";
	}
	else
	{
		fout << "Undirected Graph Dijkstra\n";
	}

	fout << "Start: "<<vertex<<"\n";

	for (int i = 0; i < size; i++)
	{
		
		stack<int> print_vertex; //최단경로를 뒤집어서 출력하기 위한 stack
		if (i == vertex)//자기 자신을 건너 뜀
		{
			continue;
		}
		bool flag = false;
		int node = i;
		fout << "[" << i << "] ";
		while (1)
		{
			if (prev[node] == -1)
			{
				break;
			}
			flag = true;
			print_vertex.push(node);
			node = prev[node];
		}

		if (!flag)
		{
			fout << "x\n";
		}
		else
		{
			print_vertex.push(vertex);
			while (!print_vertex.empty())
			{
				int node = print_vertex.top();
				print_vertex.pop();
				fout << node;
				if (print_vertex.size() != 0)
				{
					fout << " -> ";
				}
			}
			fout << " (" << distance[i] << ")\n";
		}
	}
	fout << "=========================\n\n";
	delete[] s;
	delete[] distance;
	delete[] prev;
	return true;

	
}

bool Bellmanford(Graph* graph, char option, int s_vertex, int e_vertex, ofstream& fout)
{
	if (option != 'O' && option != 'X')
	{
		return false;
	}
	int size = graph->getSize();
	if (s_vertex<0 || s_vertex>=size)
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
	int* distance = new int[size]; //s_vertex에서 i까지의 최단 거리
	int* before = new int[size];	//i까지의 최단 경로에서 직전 정점
	
	map<int, int>* edge = new map<int, int>[size];
	vector<pair<int, pair<int, int>>> edges;

	map<int, int>::iterator iter;
	vector<pair<int, pair<int, int>>>::iterator iter1;

	for (int i = 0; i < size; i++)
	{
		if (option == 'O')
		{
			graph->getAdjacentEdgesDirect(i, &edge[i]);
		}
		else
		{
			graph->getAdjacentEdges(i, &edge[i]);
		} //i에서 갈 수 있는 모든 edge를 가져옴
		for (iter = edge[i].begin(); iter != edge[i].end(); iter++)
		{
			int destination = iter->first;
			int weight = iter->second;
			edges.push_back(make_pair(weight, make_pair(i, destination)));

		}
	}

	//모든 정점까지의 거리를 무한대로 초기화
	for (int i = 0; i < size; i++)
	{
		distance[i] = INT_MAX;
		before[i] = -1;
	}
	distance[s_vertex] = 0;

	//현재 알고 있는 최단 거리보다 더 짧은 경로가 있으면 갱신함
	for (int i = 1; i <= size - 1; i++)	//최단 경로는 최대 모든 vertex의 개수 -1 개의 edge로 구성된다.
	{
		
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
				distance[end] = distance[start] + weight;
				before[end] = start;
			}
		}
	}

	//음수 사이클이 존재하는지 검사
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

	//e_vertex에 도달할 수 없는 경우
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
		fout<< "======================\n\n";
		delete[] edge;
		delete[] distance;
		delete[] before;
		return true;
	}

	stack<int> sPath; //최단 경로 출력을 위한 stack
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

	//모든 edge 가져옴
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

	//arr[i][j]=i에서 j로 가는 최단거리
	for (int i = 0; i < size; i++)
	{
		arr[i] = new int[size];
	}

	//arr 초기화 i->j 간선이 있으면 그 가중치, 없으면 INT_MAX(무한대)
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

	//k=경유지. i->k->j가 더 짧으면 갱신
	for(int k=0; k<size; k++)
	{
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				//k를 경유하는 경우가 더 짧은 경우
				if (arr[i][k] + arr[k][j] < arr[i][j])
				{
					//둘다 도달가능한 할 때
					if (arr[i][k] != INT_MAX && arr[k][j] != INT_MAX)
					{
						arr[i][j] = arr[i][k] + arr[k][j]; //업데이트
					}
				}
			}
		}
	}

	//음수 사이클 검사
	for (int i = 0; i < size; i++)
	{
		if (arr[i][i] != 0)
		{
			delete[] edge;
			for (int j = 0; j < size; j++)
			{
				delete[] arr[i];
			}
			delete[] arr;
			return false;
		}
	}

	//출력
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

	for (int i = 0; i < size; i++)
	{
		fout << "[" << i << "]\t";
	}
	fout << "\n";

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

bool Centrality(Graph* graph, ofstream &fout) 
{
	int size = graph->getSize();

	if (size <= 0)
	{
		return false;
	}

	//배열 구성
	int** distance = new int* [size];
	for (int i = 0; i < size; i++)
	{
		distance[i] = new int[size];
	}

	map<int, int>* edge = new map<int, int>[size];
	vector<pair<int, pair<int, int>>> edges;
	map<int, int>::iterator iter;
	vector<pair<int, pair<int, int>>>::iterator iter1;

	//모든 edge 가져옴
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

	//distance 초기화
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			bool flag = false;
			if (i == j)
			{
				distance[i][j] = 0;
				continue;
			}

			for (iter1 = edges.begin(); iter1 != edges.end(); iter1++)
			{
				if (i == (iter1->second).first && j == (iter1->second).second)
				{
					distance[i][j] = iter1->first;
					flag = true;
					break;
				}
			}

			if (!flag)
			{
				distance[i][j] = INT_MAX;
			}
		}
	}

	//FLOYD 수행
	for (int k = 0; k < size; k++)
	{
		for (int i = 0; i < size; i++)
		{
			if (distance[i][k] == INT_MAX)
			{
				continue;
			}

			for (int j = 0; j < size; j++)
			{
				if (distance[k][j] > distance[i][k] + distance[k][j])
				{
					distance[i][j] = distance[i][k] + distance[k][j];
				}
			}
		}
	}

	//음수 사이클 검사
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


	// ===== 중심성 계산 =====
	double* centrality = new double[size];
	bool* available = new bool[size];

	for (int i = 0; i < size; i++)
	{
		long long sum = 0;
		bool ok = true;

		for (int j = 0; j < size; j++)
		{
			if (i == j) continue;

			if (distance[i][j] == INT_MAX)
			{
				ok = false;    // 도달 불가 → x 출력
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

	// 최대 중심성 찾기
	double maxC = -1.0;
	for (int i = 0; i < size; i++)
	{
		if (available[i] && centrality[i] > maxC)
		{
			maxC = centrality[i];
		}
	}

	// ===== 출력 =====
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
			// sum을 다시 구함 → 분수 형태 출력
			long long sum = 0;
			for (int j = 0; j < size; j++)
			{
				if (i != j) sum += distance[i][j];
			}

			fout << (size - 1) << "/" << sum;

			if (centrality[i] == maxC)
			{
				fout << " <- Most Central";
			}
			fout << "\n";
		}
	}
	fout << "=======================\n\n";

	// 메모리 해제
	delete[] edge;
	delete[] available;
	delete[] centrality;

	for (int i = 0; i < size; i++)
		delete[] distance[i];
	delete[] distance;

	return true;
}