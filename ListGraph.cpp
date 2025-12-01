#include "ListGraph.h"
#include <iostream>
#include <utility>

ListGraph::ListGraph(bool type, int size) : Graph(type, size)
{
	m_Type = type;
	m_List = new map<int, int>[size];
	m_Size = size;
}

ListGraph::~ListGraph()	
{
	delete[] m_List;
}

void ListGraph::getAdjacentEdges(int vertex, map<int, int>* m)	 //Definition of getAdjacentEdges(No Direction == Undirected)
{
	map<int, int>::iterator iter;
	for (int i = 0; i < m_Size; i++)
	{
		for (iter = m_List[i].begin(); iter != m_List[i].end(); iter++)
		{
			if (i == vertex)
			{
				m->insert(make_pair((*iter).first, (*iter).second));
			}
			else if (iter->first == vertex)
			{
				m->insert(make_pair(i, (*iter).second));
			}
		}
	}
}

void ListGraph::getAdjacentEdgesDirect(int vertex, map<int, int>* m)	//Definition of getAdjacentEdges(Directed graph)
{
	map<int, int>::iterator iter;
	for (iter = m_List[vertex].begin(); iter != m_List[vertex].end(); iter++)
		m->insert(make_pair((*iter).first, (*iter).second));
}

void ListGraph::insertEdge(int from, int to, int weight) //Definition of insertEdge
{
	//m_List[from].insert(make_pair(to, weight));
	m_List[from][to] = weight;
}

bool ListGraph::printGraph(ofstream *fout)	//Definition of print Graph
{
	*fout << "============PRINT==========\n";
	for (int i = 0; i < m_Size; i++)
	{
		*fout << "[" << i << "]";
		for (auto j = m_List[i].begin(); j != m_List[i].end(); j++)
		{
			*fout << " -> (" << j->first << "," << j->second << ")";
		}
		*fout << "\n";
	}
	*fout << "=============================\n\n";

	return true;
}