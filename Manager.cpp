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
	load = 0;	//Anything is not loaded
}

Manager::~Manager()
{
	if(load)	//if graph is loaded, delete graph
		delete graph;	
	if(fout.is_open())	//if fout is opened, close file
		fout.close();	//close log.txt File
}

void Manager::run(const char* command_txt){
	ifstream fin;	//Command File Input File Stream
	fin.open(command_txt, ios_base::in);//File open with read mode
		
	if(!fin) { //If command File cannot be read, Print error
		fout<<"command file open error"<<endl;
		return;	//Return
	}
	
	string line;
	char dir = { 0 };	//방향성이 있는지 없는지 
	int start = -1;	//시작 정점 번호
	int end= - 1;

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
			string filename;
			ss >> filename;

			if (filename.empty())
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
			if (!PRINT())
			{
				printErrorCode(200);
			}
		}
		else if (cmd == "BFS")
		{
			ss >> dir >> start;
			if (dir != 'O'&& dir !='X')
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
			ss >> dir >> start;
			if (!mDFS(dir, start))
			{
				printErrorCode(400);
			}
		}
		else if (cmd == "KRUSKAL")
		{
			if (!mKRUSKAL())
			{
				printErrorCode(500);
			}
		}
		else if (cmd == "DIJKSTRA")
		{
			ss >> dir >> start;

			if (!mDIJKSTRA(dir, start))
			{
				printErrorCode(600);
			}
		}
		else if (cmd == "BELLMANFORD")
		{
			ss >> dir >> start >> end;
			if (!mBELLMANFORD(dir, start, end))
			{
				printErrorCode(700);
			}
		}
		else if (cmd == "FLOYD")
		{
			ss >> dir;
			if (!mFLOYD(dir))
			{
				printErrorCode(800);
			}
		}
		else if (cmd == "CENTRALITY")
		{
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
	// If graph information already exists
	if (graph != nullptr) 
	{ 
		delete graph;
	}
	
	ifstream fdata;
	fdata.open(filename);
	//텍스트 파일이 존재하지 않을 경우 에러코드 출력
	if (!fdata.is_open())
	{
		printErrorCode(100);
		return false;
	}

	string temp;

	if (strcmp(filename, "graph_L.txt") == 0)
	{
		int vertexs = -1; //vertex의 개수(두 번째 줄의 값)

		int start_vertex = -1;
		int end_vertex = -1;
		int weight = -1;

		//종류 확인(L)
		getline(fdata, temp);
		if (temp != "L")
		{
			return false;
		}

		//정점 개수 입력 받기
		getline(fdata, temp);
		if (!temp.empty())
		{
			vertexs = stoi(temp);
		}

		//그래프 객체 생성
		graph = new ListGraph(false, vertexs);
		int index = -1; //vertex index

		while (getline(fdata, temp))
		{
			if (index >= vertexs)
			{
				break;
			}

			//공백이 없는 줄=정점 번호만 적힌 줄=출발 정점
			if (temp.find(" ") == string::npos)
			{
				if (!temp.empty())
				{
					start_vertex = stoi(temp);
					//입력 파일의 정점 번호가 연속 증가(i = 0부터 시작)하지 않으면 잘못된 파일
					if (index + 1 != start_vertex)
					{
						graph = nullptr;
						delete graph; // Delete graph information
						return false; // Output error code
					}
				}
				index++;
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
		int weight=-1;

		getline(fdata, temp);
		if (temp != "M")
		{
			return false;
		}

		//정점 개수 입력 받기 
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
	else //어차피 위에서 걸리니까 굳이? 싶지만?
	{
		return false;
	}
	
	return true;
}

bool Manager::PRINT()	
{
	//저장된 그래프가 없는 경우 에러코드 출력
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
	fout<<"========ERROR======="<<endl;
	fout<<n<<endl;
	fout<<"===================="<<endl << endl;
}

