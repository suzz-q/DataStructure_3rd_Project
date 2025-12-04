#include "Manager.h"
#include "GraphMethod.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

Manager::Manager()
{
	graph = nullptr;
	fout.open("log.txt", ios::trunc);
	load = 0;	// nothing loaded yet
}

Manager::~Manager()
{
	if (load)	// if graph was loaded, delete it
		delete graph;
	if (fout.is_open())	// if fout is open
		fout.close();	// close log.txt
}

void Manager::run(const char* command_txt) {
	ifstream fin;	// command file input stream
	fin.open(command_txt, ios_base::in); // open file with read mode

	if (!fin) { // if command file cannot be opened
		fout << "command file open error" << endl;
		return;
	}

	string line;
	char dir = { 0 };	// graph direction option
	int start = -1;	// start vertex number
	int end = -1;

	while (getline(fin, line))
	{
		if (line.empty())
		{
			continue;
		}

		istringstream ss(line);

		string cmd;
		ss >> cmd;

		if (cmd == "LOAD")
		{
			string filename, ex;
			ss >> filename;

			// missing or extra arguments
			if (filename.empty() || ss >> ex)
			{
				printErrorCode(100);
				continue;
			}
			if (LOAD(filename.c_str()))
			{
				fout << "==========LOAD===========\n";
				fout << "Success\n";
				fout << "========================\n\n";
			}
			else
			{
				printErrorCode(100);
			}

		}
		else if (cmd == "PRINT")
		{
			string ex;
			if (ss >> ex)
			{
				printErrorCode(200);
				continue;
			}
			if (!PRINT())
			{
				printErrorCode(200);
			}
		}
		else if (cmd == "BFS")
		{
			if (!(ss >> dir >> start))
			{
				printErrorCode(300);
				continue;
			}
			string ex;
			if (ss >> ex)
			{
				printErrorCode(300);
				continue;
			}
			if (dir != 'O' && dir != 'X')
			{
				printErrorCode(300);
				continue;
			}

			if (start < 0)
			{
				printErrorCode(300);
				continue;
			}

			if (!mBFS(dir, start))
			{
				printErrorCode(300);
			}
		}
		else if (cmd == "DFS")
		{
			if (!(ss >> dir >> start))
			{
				printErrorCode(400);
				continue;
			}
			string ex;
			if (ss >> ex)
			{
				printErrorCode(400);
				continue;
			}

			if (dir != 'O' && dir != 'X')
			{
				printErrorCode(300);
				continue;
			}

			if (start < 0)
			{
				printErrorCode(300);
				continue;
			}

			if (!mDFS(dir, start))
			{
				printErrorCode(400);
			}
		}
		else if (cmd == "KRUSKAL")
		{
			string ex;
			if (ss >> ex)
			{
				printErrorCode(500);
				continue;
			}
			if (!mKRUSKAL())
			{
				printErrorCode(500);
			}
		}
		else if (cmd == "DIJKSTRA")
		{
			if (!(ss >> dir >> start)) {
				printErrorCode(600);
				continue;
			}
			string ex;
			if (ss >> ex) {
				printErrorCode(600);
				continue;
			}

			if (!mDIJKSTRA(dir, start))
			{
				printErrorCode(600);
			}
		}
		else if (cmd == "BELLMANFORD")
		{
			if (!(ss >> dir >> start >> end)) {
				printErrorCode(700);
				continue;
			}
			string ex;
			if (ss >> ex) {
				printErrorCode(700);
				continue;
			}

			if (!mBELLMANFORD(dir, start, end))
			{
				printErrorCode(700);
			}
		}
		else if (cmd == "FLOYD")
		{
			if (!(ss >> dir)) {
				printErrorCode(800);
				continue;
			}
			string ex;
			if (ss >> ex) {
				printErrorCode(800);
				continue;
			}

			if (!mFLOYD(dir))
			{
				printErrorCode(800);
			}
		}
		else if (cmd == "CENTRALITY")
		{
			string ex;
			if (ss >> ex) {
				printErrorCode(900);
				continue;
			}

			if (!mCentrality())
			{
				printErrorCode(900);
			}
		}
		else if (cmd == "EXIT")
		{
			fout << "=============EXIT===========\n"
				<< "Success\n"
				<< "============================\n\n";

			fin.close();
			load = 1;
			return;
		}
	}
	fin.close();
	return;
}

bool Manager::LOAD(const char* filename)
{
	// delete existing graph if it exists
	if (graph != nullptr)
	{
		delete graph;
	}

	ifstream fdata;
	fdata.open(filename);

	// file does not exist
	if (!fdata.is_open())
	{
		printErrorCode(100);
		return false;
	}

	string temp;

	if (strcmp(filename, "graph_L.txt") == 0)
	{
		int vertexs = -1; // number of vertices

		int start_vertex = -1;
		int end_vertex = -1;
		int weight = -1;

		// type check (L)
		getline(fdata, temp);
		if (temp != "L")
		{
			return false;
		}

		// read number of vertices
		getline(fdata, temp);
		if (!temp.empty())
		{
			vertexs = stoi(temp);
		}

		// create graph
		graph = new ListGraph(false, vertexs);
		int index = -1; // vertex index

		while (getline(fdata, temp))
		{
			if (index >= vertexs)
			{
				break;
			}

			// no space = only vertex number = starting vertex
			if (temp.find(" ") == string::npos)
			{
				if (!temp.empty())
				{
					start_vertex = stoi(temp);

					// vertex order check
					if (start_vertex != index + 1 && index != -1)
					{
						return false;
					}
				}
				index = start_vertex;
			}
			else
			{
				istringstream ss(temp);
				ss >> end_vertex >> weight;
				graph->insertEdge(index, end_vertex, weight);
			}
		}
	}
	else if (strcmp(filename, "graph_M.txt") == 0)
	{
		int end_vertex = -1;
		int vertexs = 0;
		int weight = -1;

		getline(fdata, temp);
		if (temp != "M")
		{
			return false;
		}

		// read number of vertices
		getline(fdata, temp);
		if (!temp.empty())
		{
			vertexs = stoi(temp);
		}
		graph = new MatrixGraph(true, vertexs);
		for (int i = 0; i < vertexs; i++)
		{
			for (int j = 0; j < vertexs; j++)
			{
				fdata >> weight;
				graph->insertEdge(i, j, weight);
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}

bool Manager::PRINT()
{
	// no stored graph
	if (!graph)
	{
		return false;
	}

	if (!graph->printGraph(&fout))
	{
		return false;
	}
	return true;
}

bool Manager::mBFS(char option, int vertex)
{
	// empty graph
	if (graph == nullptr)
	{
		return false;
	}

	return BFS(graph, option, vertex, fout);
}

bool Manager::mDFS(char option, int vertex)
{
	if (graph == nullptr)
	{
		return false;
	}

	return DFS(graph, option, vertex, fout);
}

bool Manager::mDIJKSTRA(char option, int vertex)
{
	if (graph == nullptr)
	{
		return false;
	}

	return Dijkstra(graph, option, vertex, fout);
}

bool Manager::mKRUSKAL()
{
	if (graph == nullptr)
	{
		return false;
	}

	return Kruskal(graph, fout);
}

bool Manager::mBELLMANFORD(char option, int s_vertex, int e_vertex)
{
	if (graph == nullptr)
	{
		return false;
	}

	return Bellmanford(graph, option, s_vertex, e_vertex, fout);
}

bool Manager::mFLOYD(char option)
{
	if (graph == nullptr)
	{
		return false;
	}

	return FLOYD(graph, option, fout);
}

bool Manager::mCentrality()
{
	if (graph == nullptr)
	{
		return false;
	}

	return Centrality(graph, fout);
}

void Manager::printErrorCode(int n)
{
	fout << "========ERROR=======" << endl;
	fout << n << endl;
	fout << "====================" << endl << endl;
}
