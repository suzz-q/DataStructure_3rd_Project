#include "MatrixGraph.h"
#include <iostream>
#include <vector>
#include <string>

MatrixGraph::MatrixGraph(bool type, int size) : Graph(type, size)
{
	m_Mat = new int* [size];
	for (int i = 0; i < size; i++)
	{
		m_Mat[i] = new int[size];
		memset(m_Mat[i], 0, sizeof(int) * size);
	}
}

MatrixGraph::~MatrixGraph()
{
	for (int i = 0; i < getSize(); i++)
	{
		delete[] m_Mat[i];
	}
	delete[] m_Mat;
}

void MatrixGraph::getAdjacentEdges(int vertex, map<int, int>* m)
{	
	//for (int i = 0; i < m_Size; i++) {
	//	for (int j = 0; j < m_Size; j++) {
	//		if (i == j) // Skip self-loops
	//			continue;
	//		if (i == vertex && m_Mat[i][j] != 0) // Outgoing edges from the vertex
	//			m->insert(make_pair(j, m_Mat[i][j]));
	//		if (j == vertex && m_Mat[i][j] != 0) // Incoming edges to the vertex
	//			m->insert(make_pair(i, m_Mat[i][j]));
	//	}
	//}

	for (int j = 0; j < m_Size; j++)
	{
		if (vertex != j && m_Mat[vertex][j] != 0)
		{
			(*m)[j] = m_Mat[vertex][j];
		}
	}

	for (int i = 0; i < m_Size; i++)
	{
		if (vertex != i && m_Mat[i][vertex] != 0)
		{
			(*m)[i] = m_Mat[i][vertex];
		}
	}
}

void MatrixGraph::getAdjacentEdgesDirect(int vertex, map<int, int>* m)
{
	for (int j = 0; j < m_Size; j++) {
		if (m_Mat[vertex][j] != 0) { // If there is an edge connected to the vertex
			m->insert(make_pair(j, m_Mat[vertex][j])); // Store the edge's vertex and weight
		}
	}
}

void MatrixGraph::insertEdge(int from, int to, int weight)	
{
	m_Mat[from][to] = weight;
}

bool MatrixGraph::printGraph(ofstream *fout)	
{
	if (m_Size < 0)
	{
		return false;
	}
	*fout << "=========PRINT=========\n";
	*fout << "    ";
	//열 부분 인덱스 출력
	for (int i = 0; i < m_Size; i++)
	{
		*fout << "[" << i << "] ";
	}
	*fout << "\n";

	//각 행 출력
	for (int i = 0; i < m_Size; i++)
	{
		*fout << "[" << i << "] ";
		for (int j = 0; j < m_Size; j++) 
		{
			*fout << m_Mat[i][j] << " ";
		}
		*fout << "\n";
	}

	*fout << "==========================\n\n";
	return true;
}