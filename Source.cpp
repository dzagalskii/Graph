#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <queue>
#include <stack>
#include <vector>
#include <string>
#include <climits>
#include <algorithm>
#include <io.h>  
#include <windows.h>

int get_graph();
using namespace std;

class Graph
{
public:
	~Graph();
	//1
	string name;
	int* adjacency_matrix;//������� ���������
	int* incidence_matrix;//������� �������������
	vector<vector<int>> adjacency_list;//������ ���������
	bool orgraph;		//���� �����������������
	int vertex_count;	//���������� ������
	int edge_count;	//���������� �����
	int loop_count; //���������� ������
	bool flag_count;
	bool flag_success;
	vector<int> degree_sequence;//������� ������ �����
	vector<int> halfdegree_sequence_out;//����������� ������� ������ �����
	vector<int> halfdegree_sequence_in;//����������� ������� ������ �����

	vector<int> leafs;
	vector<int> isolated;
	vector<int> sources;
	vector<int> drains;
	vector<vector<int>> distance_matrix;
	vector<int> eccentricity;
	int diameter;
	int radius;
	bool flag_connected;

	void print_edges_count();	//����� ���������� �����
	void print_vertex_count();	//����� ���������� ������
	void get_adjacency_matrix(char* file_name);	//���������� � ����� ������� ���������
	void get_incidence_matrix(char* file_name);	//���������� � ����� ������� �������������
	void get_adjacency_list(char* file_name);	//���������� � ����� ������� ���������
	void print_adjacency_matrix();	//����� ������� ���������
	void print_incidence_matrix();	//����� ������� �������������
	void print_adjacency_list();	//����� ������ ���������
	void adjacency_matrix_to_incidence_matrix();	//�������������� ������� ��������� � ������� ��������������
	void incidence_matrix_to_adjacency_matrix();	//�������������� ������� �������������� � ������� ���������
	void adjacency_list_to_adjacency_matrix();	//�������������� ������ ��������� � ������� ���������
	void adjacency_matrix_to_adjacency_list();	//�������������� ������� ��������� � ������ ���������
	void get_edges_count();	//������� ���������� �����/���
	void get_degree_sequence();	//������� �������� ������
	void get_halfdegree_sequence();	//������� ������������ ������ ��� ������
	void print_vertex_degree(int V);	//����� ������� �������
	void print_vertex_halfdegree(int V);	//����� ������������ ������ � ������ �������
	void print_halfdegree_sequence();	//����� ������������� ������������������
	void print_degree_sequence();	//����� ��������� ������������������
	void add_vertex();	//���������� ������� � ����
	void remove_vertex(int V);	//�������� ������� �� �����
	void add_edge(int from, int to);	//���������� ����� � ����
	void remove_edge(int from, int to);	//�������� ����� �� �����
	void get_leaf_and_isolated_count();	//������� ������� � ������������� ������
	void get_sources_and_drains_count();	//������� ������ � �������
	Graph complement_graph();	//����������� ���������� �����
	void get_eccentricity();	//���������� ��������������� ������
	void get_diameter_and_radius();	//������� �������� � ������� �����
	void print_vertex_eccentricity(int V);	//����� ��������������� �������
	void get_distance_matrix();	//����������� ������� ����������
	void subdivision_edge(int from, int to);	//������������ �����
	void graph_contraction();	//���������� �����
	void duplicate_vertex(int V, bool mult);	//������������ � ����������� �������
	Graph disjoint_union(Graph G);	//����������� ����������� ������
	Graph connect_graphs(Graph G);	//���������� ������
	Graph multiplication_graphs(Graph G);	//������������ ������
	void print_diameter();	//����� �������� �����
	void print_radius();	//����� ������� �����
	void print_centres();	//����� ������� �����
	void print_peripheral_vertex();	//����� ������������ ������ �����
	void get_distance(int V1, int V2);

	//2
	stack <int> help_stack;
	vector <int> walk_list;
	vector <vector <int>> prima_list;
	vector <vector <int>> help_list;
	deque <int> help_queue;

	void walk_in_depth(int vertex);
	void walk_in_wide(int vertex);

	void get_weight_matrix(char* file_name);
	void print_weight_matrix();
	int* weight_matrix;
	void prima(int vertex);
	vector <int> deleted;
	int prima_weight = 0;
};

vector<Graph> Graphs;

Graph* choose_graph()
{
	int n = 0;
	printf("> ����� ������� �����:\n");
	scanf("%d", &n);
	while (n > Graphs.size() || n < 1)
	{
		fflush(stdin);
		n = 0;
		printf("> ������������ ����\n");
		scanf("%d", &n);
	}
	return &(Graphs[n - 1]);
}

Graph::~Graph()
{
	adjacency_list.clear();
	adjacency_matrix = nullptr;
	incidence_matrix = nullptr;
}

//��������
void Graph::print_edges_count()
{
	printf("> ���������� �����: %d\n", edge_count);
	return;
}

//��������
void Graph::print_vertex_count()
{
	printf("> ���������� ������: %d\n", vertex_count);
	return;
}

//22222222222222222222

//
void Graph::get_weight_matrix(char* file_path)
{
	vertex_count = 0;
	int i = 0, j;
	char buf[100];
	FILE* file;
	if ((file = fopen(file_path, "r")) == NULL)
	{
		printf("> ������ �������� �����\n");
		return;
	}
	orgraph = false;
	fgets(buf, 100, file);
	while (buf[i] != '\n')
	{
		if (buf[i] != 32 && buf[i] != '-')
			vertex_count++;
		i++;
	}
	adjacency_matrix = new int[(vertex_count + 1) * (vertex_count + 1)];
	weight_matrix = new int[(vertex_count + 1) * (vertex_count + 1)];
	memset(adjacency_matrix, 0, (vertex_count + 1) * (vertex_count + 1) * sizeof(int));
	memset(weight_matrix, 0, (vertex_count + 1) * (vertex_count + 1) * sizeof(int));
	for (i = 1; i < vertex_count + 1; i++)
	{
		adjacency_matrix[i] = i;
		adjacency_matrix[i * (vertex_count + 1)] = i;
		weight_matrix[i] = i;
		weight_matrix[i * (vertex_count + 1)] = i;
	}

	print_adjacency_matrix();
	print_weight_matrix();

	fseek(file, 0, SEEK_SET);
	while (!feof(file))
	{
		for (i = 1; i < vertex_count + 1; i++)
		{
			for (j = 1; j < vertex_count + 1; j++)
			{
				fscanf_s(file, "%d", &weight_matrix[i * (vertex_count + 1) + j]);
				if (weight_matrix[i * (vertex_count + 1) + j] != 0)
					adjacency_matrix[i * (vertex_count + 1) + j] = 1;
			}
		}
	}
	fclose(file);
	adjacency_matrix_to_adjacency_list();
	adjacency_matrix_to_incidence_matrix();
	if (orgraph)
		get_halfdegree_sequence();
	else
		get_degree_sequence();
	return;
}

//��������
void Graph::get_adjacency_matrix(char* file_path)
{
	vertex_count = 0;
	int i = 0, j;
	char buf[100];
	FILE* file;
	if ((file = fopen(file_path, "r")) == NULL)
	{
		printf("> ������ �������� �����\n");
		return;
	}
	fscanf_s(file, "%d", &j);
	if (j == 0)
		orgraph = false;
	else
		orgraph = true;
	j = fgetc(file);
	fgets(buf, 100, file);
	while (buf[i] != '\n')
	{
		if (buf[i] != 32)
			vertex_count++;
		i++;
	}
	adjacency_matrix = new int[(vertex_count + 1) * (vertex_count + 1)];
	adjacency_matrix[0] = 0;
	for (i = 1; i < vertex_count + 1; i++)
	{
		adjacency_matrix[i] = i;
		adjacency_matrix[i * (vertex_count + 1)] = i;
	}
	fseek(file, 2, SEEK_SET);
	while (!feof(file))
	{
		for (i = 1; i < vertex_count + 1; i++)
		{
			for (j = 1; j < vertex_count + 1; j++)
			{
				fscanf_s(file, "%d", &adjacency_matrix[i * (vertex_count + 1) + j]);
			}
		}
	}
	fclose(file);
	adjacency_matrix_to_adjacency_list();
	adjacency_matrix_to_incidence_matrix();
	if (orgraph)
		get_halfdegree_sequence();
	else
		get_degree_sequence();
	return;
}

//��������
void Graph::get_incidence_matrix(char* file_path)
{
	int i = 0, j, c, N = 0;
	char buf[100];
	vertex_count = 0;
	edge_count = edge_count = 0;
	FILE* file;
	if ((file = fopen(file_path, "r")) == NULL)
	{
		printf("> ������ �������� �����\n");
		return;
	}
	fscanf_s(file, "%d", &c);
	if (c == 0)
		orgraph = false;
	else orgraph = true;
	c = fgetc(file);
	fgets(buf, 100, file);
	while (buf[i] != '\n')
	{
		if (buf[i] != 32 && buf[i] != 45)
			N++;
		i++;
	}
	if (orgraph) edge_count = N;
	else edge_count = N;
	vertex_count++;
	while (fgets(buf, 100, file))
		vertex_count++;
	incidence_matrix = new int[(N + 1) * vertex_count];
	for (i = 0; i < vertex_count; i++)
	{
		incidence_matrix[i * (N + 1)] = i + 1;
	}
	fseek(file, 2, SEEK_SET);
	while (!feof(file))
	{
		for (i = 0; i < vertex_count; i++)
		{
			for (j = 1; j < N + 1; j++)
			{
				fscanf_s(file, "%d", &incidence_matrix[i * (N + 1) + j]);
			}
		}
	}
	fclose(file);
	if (orgraph)
		get_halfdegree_sequence();
	else
		get_degree_sequence();
	incidence_matrix_to_adjacency_matrix();
	return;
}

//��������
void Graph::get_adjacency_list(char* file_path)
{
	int c = -1, line = 0;
	FILE* file;
	if ((file = fopen(file_path, "r")) == NULL)
	{
		printf("> ������ �������� �����\n");
		return;
	}
	while ((c = fgetc(file)) != EOF)
	{
		if (c > 0)
		{
			adjacency_list.push_back(vector<int>());
			fscanf_s(file, "%d", &c);
			(*(adjacency_list.end() - 1)).push_back(c);
			while (c != '}')
			{
				fscanf_s(file, "%d", &c);
				adjacency_list[line].push_back(c);
				c = fgetc(file);
			}
			line++;
		}
	}
	fclose(file);
	vertex_count = adjacency_list.size();
	adjacency_list_to_adjacency_matrix();
	if (orgraph)
		get_halfdegree_sequence();
	else
		get_degree_sequence();
	get_distance_matrix();
	return;
}

//��������
void Graph::print_incidence_matrix()
{
	int i, j, N;
	N = edge_count;
	printf("> ������� �������������:\n");
	for (i = 0; i < vertex_count; i++)
	{
		for (j = 0; j < N + 1; j++)
		{
			printf("%d  ", incidence_matrix[i * (N + 1) + j]);
		}
		printf("\n");
	}
	return;
}

//��������
void Graph::print_adjacency_list()
{
	unsigned int i, j;
	printf("> ������ ���������:\n");
	for (i = 0; i < adjacency_list.size(); i++)
	{
		printf("{%d", adjacency_list[i][0]);
		for (j = 1; j < adjacency_list[i].size(); j++)
		{
			printf(" %d", adjacency_list[i][j]);
		}
		printf("}\n");
	}
	return;
}

//��������
void Graph::print_adjacency_matrix()
{
	int i, j;
	printf("> ������� ���������:\n");
	for (i = 0; i < vertex_count + 1; i++)
	{
		for (j = 0; j < vertex_count + 1; j++)
		{
			printf("%d ", adjacency_matrix[i * (vertex_count + 1) + j]);
		}
		printf("\n");
	}
	return;
}

//��������
void Graph::print_weight_matrix()
{
	printf("> ������� �����:\n");
	for (int i = 0; i < vertex_count + 1; i++)
	{
		for (int j = 0; j < vertex_count + 1; j++)
			printf("%d ", weight_matrix[i * (vertex_count + 1) + j]);
		printf("\n");
	}
	return;
}

//��������
void Graph::adjacency_list_to_adjacency_matrix()
{
	adjacency_matrix = new int[(vertex_count + 1) * (vertex_count + 1)];
	memset(adjacency_matrix, 0, (vertex_count + 1) * (vertex_count + 1) * sizeof(int));
	//������ ������
	for (int i = 1; i < vertex_count + 1; i++)
	{
		adjacency_matrix[i] = adjacency_matrix[i * (vertex_count + 1)] = adjacency_list[i - 1][0];
	}
	for (int i = 0; i < adjacency_list.size(); i++)
	{
		for (int j = 1; j < adjacency_list[i].size(); j++)
		{
			//�����
			if (adjacency_list[i][0] == adjacency_list[i][j])
			{
				adjacency_matrix[(i + 1) * (vertex_count + 1) + i + 1] = 2;
			}
			//������
			else
			{
				adjacency_matrix[(i + 1) * (vertex_count + 1) + (adjacency_list[i][j])] = 1;
				adjacency_matrix[(adjacency_list[i][j]) * (vertex_count + 1) + i + 1] = 1;
			}
		}
	}
	get_edges_count();
	adjacency_matrix_to_incidence_matrix();
	return;
}

//��������
void Graph::adjacency_matrix_to_adjacency_list()
{
	for (int i = 1; i < vertex_count + 1; i++)
	{
		adjacency_list.push_back(vector<int>());
		//�������
		adjacency_list[i - 1].push_back(adjacency_matrix[i]);
		//������� � ���
		for (int j = 1; j < vertex_count + 1; j++)
		{
			if (adjacency_matrix[i * (vertex_count + 1) + j] == 1)
				adjacency_list[i - 1].push_back(adjacency_matrix[j]);
		}
	}
	get_distance_matrix();
	return;
}

//��������
void Graph::adjacency_matrix_to_incidence_matrix()
{
	get_edges_count();
	int N, i = 0, j, k = 1;
	N = edge_count;
	incidence_matrix = new int[vertex_count * (N + 1)];
	memset(incidence_matrix, 0, vertex_count * (N + 1) * (sizeof(int)));
	for (i = 0; i < (unsigned int)vertex_count; i++)
	{
		incidence_matrix[i * (N + 1)] = adjacency_list[i][0];
	}
	for (i = 1; i < vertex_count + 1; i++)
	{
		for (j = i; j < vertex_count + 1; j++)
		{
			//��� �������
			if ((adjacency_matrix[i * (vertex_count + 1) + j] == 0) && (adjacency_matrix[j * (vertex_count + 1) + i]) == 1)
			{
				incidence_matrix[(i - 1) * (N + 1) + k] = -1;
				incidence_matrix[(j - 1) * (N + 1) + k] = 1;
				k++;
			}
			//��� ������
			else if ((adjacency_matrix[i * (vertex_count + 1) + j] == 1) && (adjacency_matrix[j * (vertex_count + 1) + i]) == 0)
			{
				incidence_matrix[(i - 1) * (N + 1) + k] = 1;
				incidence_matrix[(j - 1) * (N + 1) + k] = -1;
				k++;
			}
			//��� �����
			else if ((i == j) && (adjacency_matrix[i * (vertex_count + 1) + i]) == 2)
			{
				incidence_matrix[(i - 1) * (N + 1) + k] = 2;
				k++;
			}
			//������� ������
			else if ((adjacency_matrix[i * (vertex_count + 1) + j] == 1) && (adjacency_matrix[j * (vertex_count + 1) + i]) == 1)
			{
				//��� �������
				if (orgraph)
				{
					incidence_matrix[(i - 1) * (N + 1) + k] = 1;
					incidence_matrix[(j - 1) * (N + 1) + k] = -1;
					k++;
					incidence_matrix[(i - 1) * (N + 1) + k] = -1;
					incidence_matrix[(j - 1) * (N + 1) + k] = 1;
					k++;
				}
				//��� ��������
				else
				{
					incidence_matrix[(i - 1) * (N + 1) + k] = 1;
					incidence_matrix[(j - 1) * (N + 1) + k] = 1;
					k++;
				}
			}
			if (k == (N + 1))
			{
				break;
			}
		}
	}
	return;
}

//��������
void Graph::incidence_matrix_to_adjacency_matrix()
{
	int N = edge_count;
	adjacency_matrix = new int[(vertex_count + 1) * (vertex_count + 1)];
	memset(adjacency_matrix, 0, (vertex_count + 1) * (vertex_count + 1) * (sizeof(int)));
	//������
	for (int i = 1; i < vertex_count + 1; i++)
	{
		adjacency_matrix[i] = adjacency_matrix[i * (vertex_count + 1)] = incidence_matrix[(i - 1) * (N + 1)];
	}
	//�������
	if (!orgraph)
	{
		for (int i = 0; i < vertex_count; i++)
		{
			for (int j = 1; j < N + 1; j++)
			{
				//������ �������
				if (incidence_matrix[i * (N + 1) + j] == 1)
				{
					for (int z = i + 1; z < vertex_count; z++)
					{
						//������ �������
						if (incidence_matrix[z * (N + 1) + j] == 1)
						{
							adjacency_matrix[(z + 1) * (vertex_count + 1) + i + 1] = 1;
							adjacency_matrix[(i + 1) * (vertex_count + 1) + z + 1] = 1;
						}
					}
				}
				//�����
				else if (incidence_matrix[i * (N + 1) + j] == 2)
				{
					adjacency_matrix[(i + 1) * (vertex_count + 1) + i + 1] = 2;
				}
			}
		}
	}
	//������
	else
	{
		for (int i = 0; i < vertex_count; i++)
		{
			for (int j = 1; j < N + 1; j++)
			{
				//������ ������� ����
				if (incidence_matrix[i * (N + 1) + j] == 1)
				{
					for (int z = i + 1; z < vertex_count; z++)
					{
						//������ ������� �������
						if (incidence_matrix[z * (N + 1) + j] == -1)
						{
							adjacency_matrix[(i + 1) * (vertex_count + 1) + z + 1] = 1;
						}
					}
				}
				//������ ������� �������
				else if (incidence_matrix[i * (N + 1) + j] == -1)
				{
					for (int z = i + 1; z < vertex_count; z++)
					{
						//������ ������� ����
						if (incidence_matrix[z * (N + 1) + j] == 1)
						{
							adjacency_matrix[(z + 1) * (vertex_count + 1) + i + 1] = 1;
						}
					}
				}
				//�����
				else if (incidence_matrix[i * (N + 1) + j] == 2)
				{
					adjacency_matrix[(i + 1) * (vertex_count + 1) + i + 1] += 2;
				}
			}
		}
	}
	adjacency_matrix_to_adjacency_list();
	return;
}

//��������
void Graph::get_edges_count()
{
	edge_count = 0;
	loop_count = 0;
	for (int i = 1; i < vertex_count + 1; i++)
	{
		for (int j = 1; j < vertex_count + 1; j++)
		{
			if (adjacency_matrix[i * (vertex_count + 1) + j] == 1)
			{
				edge_count++;
			}
			else if (adjacency_matrix[i * (vertex_count + 1) + j] == 2)
			{
				loop_count++;
			}
		}
	}
	if (!orgraph)
		edge_count /= 2;
	edge_count += loop_count;
	return;
}

//��������
void Graph::get_degree_sequence()
{
	for (int i = 0; i < vertex_count; i++)
	{
		degree_sequence.push_back(0);
		for (int j = 1; j < edge_count + 1; j++)
		{
			if (incidence_matrix[i * (edge_count + 1) + j])
				degree_sequence[i] += incidence_matrix[i * (edge_count + 1) + j];
		}
	}
	get_leaf_and_isolated_count();
	return;
}

//��������
void Graph::print_vertex_degree(int vertex)
{
	int num = 0;
	while (num < adjacency_list.size() && adjacency_list[num][0] != vertex)
	{
		num++;
	}
	if (num == adjacency_list.size())
	{
		printf("> ���� ������� �� ����������!\n");
		return;
	}
	else
	{
		printf("> �������: %d\n", degree_sequence[vertex - 1]);
	}
	return;
}

//��������
void Graph::print_degree_sequence()
{
	printf("> ��������� ������������������:\n");
	for (int i = 0; i < vertex_count; i++)
		printf("> %d - %d\n", incidence_matrix[i * (edge_count + 1)], degree_sequence[i]);
	return;
}

//��������
void Graph::get_halfdegree_sequence()
{
	for (int i = 0; i < vertex_count; i++)
	{
		halfdegree_sequence_out.push_back(0);
		halfdegree_sequence_in.push_back(0);
		for (int j = 1; j < edge_count + 1; j++)
		{
			if (incidence_matrix[i * (edge_count + 1) + j] < 0)
				halfdegree_sequence_out[i]++;
			else if (incidence_matrix[i * (edge_count + 1) + j] > 0)
				halfdegree_sequence_in[i]++;
		}
	}
	get_leaf_and_isolated_count();
	return;
}

//��������
void Graph::print_vertex_halfdegree(int vertex)
{
	int num = 0;
	while (num < adjacency_list.size() && adjacency_list[num][0] != vertex)
	{
		num++;
	}
	if (num == adjacency_list.size())
	{
		printf("> ������� �� ����������\n");
		return;
	}
	else
	{
		printf("> ����������� ������ ������� %d: %d\t����������� ������ ������� %d: %d\n", vertex, halfdegree_sequence_out[vertex - 1], vertex, halfdegree_sequence_in[vertex - 1]);
	}
	return;
}

//��������
void Graph::print_halfdegree_sequence()
{
	printf("> \t����������� ������\n");
	for (int i = 0; i < vertex_count; i++)
		printf("> %d\t%d\n", i + 1, halfdegree_sequence_out[i]);
	printf("> \t����������� ������\n");
	for (int i = 0; i < vertex_count; i++)
		printf("> %d\t%d\n", i + 1, halfdegree_sequence_in[i]);
	return;
}

//��������
void Graph::add_vertex()
{
	int* temp = new int[(vertex_count + 2) * (vertex_count + 2)];
	int k = 0, k3 = 0, k2 = 0;
	memset(temp, 0, (vertex_count + 2) * (vertex_count + 2) * sizeof(int));

	for (int i = 0; i < (vertex_count + 1) * (vertex_count + 1); i++)
	{
		if (k2 == vertex_count + 1)
		{
			temp[k] = vertex_count + 1;
			k++;
		}
		temp[k] = adjacency_matrix[i];
		k++;
		k2++;
		k3++;
		if (k3 % (vertex_count + 1) == 0 && k3 > (vertex_count + 1))//���������� ��������� ������� � ����� �������, ����� ��� ��� 0
			k++;
	}

	temp[k] = vertex_count + 1;
	k++;

	delete[] adjacency_matrix;
	adjacency_matrix = nullptr;

	vertex_count++;
	adjacency_matrix = new int[(vertex_count + 1) * (vertex_count + 1)];
	memset(adjacency_matrix, 0, (vertex_count + 1) * (vertex_count + 1) * sizeof(int));
	memcpy(adjacency_matrix, temp, (vertex_count + 1) * (vertex_count + 1) * sizeof(int));

	adjacency_list.clear();
	delete[] incidence_matrix;
	incidence_matrix = nullptr;

	adjacency_matrix_to_adjacency_list();
	adjacency_matrix_to_incidence_matrix();
	if (orgraph)
		get_halfdegree_sequence();
	else
		get_degree_sequence();
	return;
}

//��������
void Graph::remove_vertex(int vertex)
{
	flag_success = true;
	int num = 1, i, j;
	while (num < adjacency_list.size() && adjacency_list[num][0] != vertex)
	{
		num++;
	}
	if (num == adjacency_list.size())
	{
		printf("> ������� �� ����������\n");
		flag_success = false;
		return;
	}

	num++;
	for (i = num; i < vertex_count; i++)//����� �� �����������
	{
		for (j = 0; j < vertex_count + 1; j++)
		{
			adjacency_matrix[i * (vertex_count + 1) + j] = adjacency_matrix[(i + 1) * (vertex_count + 1) + j];
		}
	}


	for (i = num; i < vertex_count; i++)//���������
	{
		for (j = 0; j < vertex_count; j++)
		{
			adjacency_matrix[j * (vertex_count + 1) + i] = adjacency_matrix[j * (vertex_count + 1) + i + 1];
		}
	}

	vertex_count--;
	int k = 0; int k2 = 0;//�������� ��� � ������� (� ������� �������� �������������� ��������� �������)
	for (i = 0; i < (vertex_count + 1) * (vertex_count + 1); i++)
	{
		adjacency_matrix[i] = adjacency_matrix[k];
		k++; k2++;
		if (k2 == vertex_count + 1)
		{
			k2 = 0;
			k++;
		}
	}

	adjacency_list.clear();
	delete[] incidence_matrix;
	incidence_matrix = nullptr;

	adjacency_matrix_to_adjacency_list();
	adjacency_matrix_to_incidence_matrix();
	if (orgraph)
		get_halfdegree_sequence();
	else
		get_degree_sequence();
	return;
}

//��������
void Graph::add_edge(int from_vertex, int to_vertex)
{
	flag_success = true;
	int from = 0, to = 0;
	//����
	for (int i = 1; i < vertex_count + 1; i++)
	{
		if (adjacency_matrix[i] == from_vertex)
			from = i;
		if (adjacency_matrix[i] == to_vertex)
			to = i;
	}
	//���������
	if (!orgraph)
	{
		adjacency_matrix[from * (vertex_count + 1) + to] = 1;
		adjacency_matrix[to * (vertex_count + 1) + from] = 1;
	}
	else
	{
		adjacency_matrix[from * (vertex_count + 1) + to] = 0;
		adjacency_matrix[to * (vertex_count + 1) + from] = 1;
	}
	adjacency_list.clear();
	delete[] incidence_matrix;
	incidence_matrix = nullptr;

	adjacency_matrix_to_adjacency_list();
	adjacency_matrix_to_incidence_matrix();
	if (orgraph)
		get_halfdegree_sequence();
	else
		get_degree_sequence();
	return;
}

//��������
void Graph::remove_edge(int from_vertex, int to_vertex)
{
	flag_success = true;
	int from = 0, to = 0;
	//����
	for (int i = 1; i < vertex_count + 1; i++)
	{
		if (adjacency_matrix[i] == from_vertex)
			from = i;
		if (adjacency_matrix[i] == to_vertex)
			to = i;
	}
	//��������� �����������
	if (orgraph)
	{
		if (adjacency_matrix[from * (vertex_count + 1) + to] != 0 && adjacency_matrix[to * (vertex_count + 1) + from] != 1)
			printf("> ����� (����) �� ����������\n");
		flag_success = false;
		return;
	}
	//��������� �������������
	else
	{
		if (adjacency_matrix[from * (vertex_count + 1) + to] != 1 && adjacency_matrix[to * (vertex_count + 1) + from] != 1)
		{
			printf("> ����� (����) �� ����������\n");
			flag_success = false;
			return;
		}
	}
	//�������
	if (!orgraph)
	{
		adjacency_matrix[from * (vertex_count + 1) + to] = 0;
		adjacency_matrix[to * (vertex_count + 1) + from] = 0;
	}
	else
	{
		adjacency_matrix[to * (vertex_count + 1) + from] = 0;
	}
	edge_count--;
	adjacency_list.clear();
	delete[] incidence_matrix;
	incidence_matrix = nullptr;

	adjacency_matrix_to_adjacency_list();
	adjacency_matrix_to_incidence_matrix();
	if (orgraph)
		get_halfdegree_sequence();
	else
		get_degree_sequence();
	return;
}

//��������
Graph Graph::complement_graph()
{
	Graph Z;
	Z.flag_count = false;
	Z.name = "���������� ����� " + this->name;
	Z.adjacency_matrix = this->adjacency_matrix;
	Z.orgraph = this->orgraph;
	Z.vertex_count = this->vertex_count;
	for (int i = 1; i < vertex_count + 1; i++)
	{
		for (int j = 1; j < vertex_count + 1; j++)
		{
			if (i != j)
			{
				if (Z.adjacency_matrix[i * (vertex_count + 1) + j] == 0)
				{
					Z.adjacency_matrix[i * (vertex_count + 1) + j] = 1;
				}
				else
				{
					Z.adjacency_matrix[i * (vertex_count + 1) + j] = 0;
				}
			}
		}
	}
	Z.adjacency_matrix_to_adjacency_list();
	Z.adjacency_matrix_to_incidence_matrix();
	Z.print_adjacency_matrix();
	return Z;
}

//��������
void Graph::get_leaf_and_isolated_count()
{
	if (orgraph)
	{
		//��� �������
		for (int i = 0; i < vertex_count; i++)
		{
			if ((halfdegree_sequence_in[i] + halfdegree_sequence_out[i]) == 1)
			{
				leafs.push_back(i + 1);
			}
			if ((halfdegree_sequence_in[i] + halfdegree_sequence_out[i]) == 0)
			{
				isolated.push_back(i + 1);
			}
		}
		get_sources_and_drains_count();
	}
	else
	{
		//��������
		for (int i = 0; i < vertex_count; i++)
		{
			if (degree_sequence[i] == 1)
				leafs.push_back(i + 1);
			if (degree_sequence[i] == 0)
				isolated.push_back(i + 1);
		}
	}
	return;
}

//��������
void Graph::get_sources_and_drains_count()
{
	for (int i = 0; i < vertex_count; i++)
	{
		if (halfdegree_sequence_in[i] && (halfdegree_sequence_out[i] == 0))
		{
			drains.push_back(i + 1);
		}
		if ((halfdegree_sequence_in[i] == 0) && halfdegree_sequence_out[i])
		{
			sources.push_back(i + 1);
		}
	}
	return;
}

//!!!
void Graph::get_distance_matrix()
{
	int i, j, v, num;
	distance_matrix.clear();
	//������
	for (i = 0; i < vertex_count + 1; i++)
	{
		distance_matrix.push_back(vector<int>());
		for (j = 0; j < vertex_count + 1; j++)
		{
			distance_matrix[i].push_back(0);
		}
		distance_matrix[0][i] = distance_matrix[i][0] = adjacency_matrix[i];
	}
	//
	for (i = 1; i < vertex_count + 1; i++)
	{
		vector<int> flag;
		queue<int> q;

		flag.clear();
		for (j = 0; j < vertex_count; j++)
		{
			flag.push_back(0);
		}

		q.push(i - 1);
		flag[i - 1] = 1;
		distance_matrix[i][i] = 0;
		while (!q.empty())
		{
			v = q.front();//1 ������� � �������
			q.pop();
			for (j = 1; j < adjacency_list[v].size(); j++)
			{
				num = 1;
				while (num <= vertex_count && adjacency_matrix[num] != adjacency_list[v][j]) num++;
				num--;
				if (!flag[num])
				{
					q.push(num);
					distance_matrix[i][num + 1] = distance_matrix[i][v + 1] + 1;
					flag[num] = 1;
				}
			}
		}
	}
	/*printf("> \n������� ����������:\n");
	for (i = 0; i < vertex_count + 1; i++)
	{
		for (j = 0; j < vertex_count + 1; j++)
		{
			if ((i != j) && i && j && !distance_matrix[i][j]) printf("> %c  ", '-');
			else printf("%d  ", distance_matrix[i][j]);
		}
		printf("> \n");
	}*/
	get_eccentricity();
}

//!!!
void Graph::get_distance(int vertex_1, int vertex_2)
{
	flag_success = true;
	if (vertex_1 >= vertex_count || vertex_2 >= vertex_count)
	{
		printf("> �������� �������\n");
		flag_success = false;
		return;
	}
	int num1 = 0, num2 = 0, i, j;
	while (num1 < adjacency_list.size() && adjacency_list[num1++][0] != vertex_1);
	num1++;
	while (num2 < adjacency_list.size() && adjacency_list[num2++][0] != vertex_2);
	num2++;

	if (num1 != num2 && !distance_matrix[num1][num2])
		printf("> �� ���������� ��������\n");
	else
		printf("> ����������: %d\n", distance_matrix[num1][num2]);
	return;
}

//!!!
void Graph::get_eccentricity()
{
	int max;
	eccentricity.clear();
	for (int i = 0; i < vertex_count; i++)
	{
		max = distance_matrix[i][1];
		for (int j = 1; j < vertex_count; j++)
		{
			if (distance_matrix[i][j] > max)
			{
				max = distance_matrix[i][j];
			}
		}
		eccentricity.push_back(max);
	}
	get_diameter_and_radius();
	return;
}

//��������
void Graph::print_vertex_eccentricity(int vertex)
{
	int num = 0, i, j;
	while (num < adjacency_list.size() && adjacency_list[num][0] != vertex)
	{
		num++;
	}
	if (num == adjacency_list.size())
	{
		printf("> ������� � ������� %d �� ����������\n", vertex);
		return;
	}
	printf("> �������������� ������� %d: %d\n", vertex, eccentricity[num]);
	return;
}

//��������
void Graph::get_diameter_and_radius()
{
	diameter = radius = eccentricity[0];
	for (int i = 1; i < vertex_count; i++)
	{
		//������������
		if (eccentricity[i] > diameter)
		{
			diameter = eccentricity[i];
		}
		//�����������
		if (eccentricity[i] < radius)
		{
			radius = eccentricity[i];
		}
	}
	return;
}

//��������
void Graph::print_diameter()
{
	printf("> �������: %d\n", diameter);
}

//��������
void Graph::print_radius()
{
	printf("> ������: %d\n", radius);
}

//��������
void Graph::print_centres()
{
	int i;
	printf("> ������: \n");
	for (i = 0; i < vertex_count; i++)
	{
		if (eccentricity[i] == radius) printf(">  %d,", adjacency_matrix[i + 1]);
	}
	printf("> \n");
}

//��������
void Graph::print_peripheral_vertex()
{
	int i;
	printf("> ������������ �������: \n");
	for (i = 0; i < vertex_count; i++)
	{
		if (eccentricity[i] == diameter) printf(">  %d,", adjacency_matrix[i + 1]);
	}
	printf("> \n");
}

//�������� ���������!
void Graph::subdivision_edge(int from, int to)
{
	flag_success = true;
	int N, count_from = 0, count_to = 0;
	N = edge_count;
	for (int i = 0; i < vertex_count; i++)
	{
		if (incidence_matrix[count_from * (N + 1)] != from)
			count_from++;

		if (incidence_matrix[count_to * (N + 1)] != to)
			count_to++;
	}
	if (count_from == vertex_count || count_to == vertex_count)
	{
		printf("> ������� �� ����������\n");
		flag_success = false;
		return;
	}

	int check = 1;
	while (check < N + 1)
	{
		if (abs(incidence_matrix[count_to * (N + 1) + check]) == abs(incidence_matrix[count_from * (N + 1) + check])
			&& abs(incidence_matrix[count_to * (N + 1) + check]))
			break;
		else check++;
	}
	if (check == N + 1)
	{
		printf("> ������ ����� (����) ���\n");
		flag_success = false;
		return;
	}


	remove_edge(from, to);
	add_vertex();
	int new_vertex = adjacency_matrix[vertex_count]; //!!!!!!!!
	add_edge(from, new_vertex);
	add_edge(to, new_vertex);
	return;
}

//�������� ���������!
void Graph::graph_contraction()
{
	flag_success = true;
	int s;
	vector<int> V;
	//��������� � ������
	printf("> ������� ������� ��� ����������: >\n");
	while (1)
	{
		scanf("%d", &s);
		if (s == 0)
			break;
		else if (s > 0)
			V.push_back(s);
	}
	int p = 0;
	for (int i = 0; i < V.size(); i++)
	{
		for (int j = 0; j < adjacency_list.size(); j++)
		{
			if (adjacency_list[j][0] == V[i])
				p++;
		}
		if (p == 0) {
			printf("> ����� ������� �� ����������\n");
			flag_success = false;
			return;
		}
		p = 0;
	}
	int j2 = 0;
	int new_ed[150] = { 0 };
	int count_new = 0;
	//���������� �����
	for (int i = 0; i < V.size(); i++)
	{
		for (int j = 0; j < adjacency_list.size(); j++)
		{
			if (adjacency_list[j][0] == V[i])
			{
				for (j2 = 1; j2 < adjacency_list[j].size(); j2++)
				{
					new_ed[count_new] = adjacency_list[j][j2];
					count_new++;
				}
			}
		}
	}
	add_vertex();
	for (int i = 0; i < V.size(); i++)
	{
		remove_vertex(V[i]);
	}
	//��������� �����
	int new_vertex_index = vertex_count;
	for (int i = 0; i < count_new; i++)
	{
		for (int j = 1; j < vertex_count + 1; j++)
		{
			if (adjacency_matrix[j * (vertex_count + 1)] == new_ed[i])
			{
				adjacency_matrix[new_vertex_index * (vertex_count + 1) + j] = 1;
				adjacency_matrix[j * (vertex_count + 1) + new_vertex_index] = 1;
			}
		}
	}

	adjacency_list.clear();
	delete[] incidence_matrix;
	incidence_matrix = nullptr;

	adjacency_matrix_to_adjacency_list();
	adjacency_matrix_to_incidence_matrix();
	get_degree_sequence();
	return;
}

//�������� ���������!
void Graph::duplicate_vertex(int V, bool mult)
{
	flag_success = true;
	int num = 0; int new_vertex_index;
	while (num < adjacency_list.size() && adjacency_list[num][0] != V)
	{
		num++;
	}
	if (num == adjacency_list.size())
	{
		printf("> ������� �� ����������\n");
		flag_success = false;
		return;
	}
	else
	{
		num++;
		add_vertex();
		new_vertex_index = vertex_count;
		//��������� �����
		for (int i = 0; i < vertex_count + 1; i++)
		{
			for (int j = i; j < vertex_count + 1; j++)
			{
				if (adjacency_matrix[num * (vertex_count + 1) + j] == 1)
				{
					adjacency_matrix[new_vertex_index * (vertex_count + 1) + j] = 1;
					adjacency_matrix[j * (vertex_count + 1) + new_vertex_index] = 1;
				}

			}
		}
	}
	if (mult == true)
	{
		adjacency_matrix[new_vertex_index * (vertex_count + 1) + num] = 1;
		adjacency_matrix[num * (vertex_count + 1) + new_vertex_index] = 1;
	}

	adjacency_list.clear();
	delete[] incidence_matrix;
	incidence_matrix = nullptr;

	adjacency_matrix_to_adjacency_list();
	adjacency_matrix_to_incidence_matrix();
	if (orgraph)
		get_halfdegree_sequence();
	else
		get_degree_sequence();
}

//��������
Graph Graph::disjoint_union(Graph G)
{
	Graph Z;
	int i = 0, j = 0;
	for (i = 0; i < this->adjacency_list.size(); i++)
	{
		Z.adjacency_list.push_back(this->adjacency_list[i]);//������ ������ ����
	}
	for (i; i < G.adjacency_list.size() + this->adjacency_list.size(); i++)
	{
		Z.adjacency_list.push_back(G.adjacency_list[j]);// ����� ������� - ������
		for (int j2 = 0; j2 < G.adjacency_list[j].size(); j2++)
		{
			//!!!!!!!!!!
			//Z.adjacency_list[i][j2] += this->adjacency_list.size();
			Z.adjacency_list[i][j2] += this->adjacency_list[this->adjacency_list.size() - 1][0];
		}
		j++;
	}

	Z.flag_count = false;
	Z.vertex_count = Z.adjacency_list.size();
	Z.adjacency_list_to_adjacency_matrix();
	Z.print_adjacency_matrix();
	return Z;
}

//��������
Graph Graph::connect_graphs(Graph G)
{
	//����������
	Graph Z = this->disjoint_union(G);
	Z.flag_count = false;
	//���������
	for (int i = 0; i < this->adjacency_list.size(); i++)
	{
		for (int j = this->adjacency_list.size(); j < Z.adjacency_list.size(); j++)
		{
			Z.adjacency_list[i].push_back(Z.adjacency_list[j][0]);
		}
	}
	for (int i = this->adjacency_list.size(); i < Z.adjacency_list.size(); i++)
	{
		for (int j = 0; j < this->adjacency_list.size(); j++)
		{
			Z.adjacency_list[i].push_back(Z.adjacency_list[j][0]);
		}
	}
	delete[] Z.adjacency_matrix;
	Z.adjacency_matrix = nullptr;
	delete[] Z.incidence_matrix;
	Z.incidence_matrix = nullptr;
	Z.flag_count = false;
	Z.vertex_count = Z.adjacency_list.size();
	Z.adjacency_list_to_adjacency_matrix();
	Z.print_adjacency_matrix();
	return Z;
}

//��������
Graph Graph::multiplication_graphs(Graph G)
{
	Graph Z = (*this);
	Z.flag_count = false;
	int i, j, k;
	for (i = 0; i < G.vertex_count - 1; i++)
	{
		Z = Z.disjoint_union(*this);
	}
	//printf("## G.vertex_count %d\n", G.vertex_count); //�� ������� ��������
	//printf("## Z.vertex_count %d\n", Z.vertex_count); //������� ��������
	//printf("## this->vertex_count %d\n", this->vertex_count); //������� ��������
	Z.name = "lox";
	for (i = 1; i < G.vertex_count; i++)
	{
		for (j = i + 1; j < G.vertex_count + 1; j++)
		{
			if (G.adjacency_matrix[i * (G.vertex_count + 1) + j] == 1)
			{
				for (k = 1; k < this->vertex_count + 1; k++)
				{
					Z.adjacency_matrix[((i - 1) * this->vertex_count + k) * (Z.vertex_count + 1) + (j - 1) * this->vertex_count + k] = 1;
					Z.adjacency_matrix[((j - 1) * this->vertex_count + k) * (Z.vertex_count + 1) + (i - 1) * this->vertex_count + k] = 1;
				}
			}
		}
	}
	Z.adjacency_list.clear();
	Z.adjacency_matrix_to_adjacency_list();
	delete[] Z.incidence_matrix;
	Z.incidence_matrix = nullptr;
	Z.adjacency_matrix_to_incidence_matrix();
	Z.print_adjacency_matrix();
	return Z;
}

/*222222222222222222*/

//��������
void Graph::walk_in_depth(int vertex)
{
	if (find(walk_list.begin(), walk_list.end(), vertex) != walk_list.end())
		return;
	int vertex_index = 0;
	for (int i = 0; i < adjacency_list.size(); i++)
		if (adjacency_list[i][0] == vertex)
		{
			vertex_index = i;
			break;
		}
	walk_list.push_back(adjacency_list[vertex_index][0]);
	for (int i = 1; i < adjacency_list[vertex_index].size(); i++)
		if (find(walk_list.begin(), walk_list.end(), adjacency_list[vertex_index][i]) == walk_list.end())
			help_stack.push(adjacency_list[vertex_index][i]);
	while (!help_stack.empty())
	{
		int next_vertex = help_stack.top();
		help_stack.pop();
		walk_in_depth(next_vertex);
	}
	return;
}

//��������
void Graph::walk_in_wide(int vertex)
{
	if (find(walk_list.begin(), walk_list.end(), vertex) != walk_list.end())
		return;
	int vertex_index = 0;
	for (int i = 0; i < adjacency_list.size(); i++)
		if (adjacency_list[i][0] == vertex)
		{
			vertex_index = i;
			break;
		}
	walk_list.push_back(adjacency_list[vertex_index][0]);
	for (int i = 1; i < adjacency_list[vertex_index].size(); i++)
		if (find(walk_list.begin(), walk_list.end(), adjacency_list[vertex_index][i]) == walk_list.end())
			help_queue.push_front(adjacency_list[vertex_index][i]);
	while (!help_queue.empty())
	{
		int next_vertex = help_queue.back();
		help_queue.pop_back();
		walk_in_wide(next_vertex);
	}
	return;
}

//��������
void Graph::prima(int vertex)
{
	//printf("vertex = %d\n", vertex);
	int vertex_index = 0;
	for (int i = 0; i < adjacency_list.size(); i++)
		if (adjacency_list[i][0] == vertex)
		{
			vertex_index = i;
			break;
		}
	for (int i = 1; i < adjacency_list[vertex_index].size(); i++)
	{
		int two_vertex_index = 0;
		for (int k = 0; k < adjacency_list.size(); k++)
			if (adjacency_list[vertex_index][i] == adjacency_list[k][0])
			{
				two_vertex_index = k;
				break;
			}
		if (weight_matrix[(vertex_count + 1) * (vertex_index + 1) + (two_vertex_index + 1)] == 0 ||
			find(deleted.begin(), deleted.end(), adjacency_list[two_vertex_index][0]) != deleted.end())
			continue;
		//printf("help_list_piece.push_back %d %d %d\n", adjacency_list[vertex_index][0], adjacency_list[two_vertex_index][0], weight_matrix[(vertex_count + 1) * (vertex_index + 1) + (two_vertex_index + 1)]);
		vector <int> help_list_piece;
		help_list_piece.push_back(adjacency_list[vertex_index][0]);
		help_list_piece.push_back(adjacency_list[two_vertex_index][0]);
		help_list_piece.push_back(weight_matrix[(vertex_count + 1) * (vertex_index + 1) + (two_vertex_index + 1)]);
		help_list.push_back(help_list_piece);
	}
	//���� ����������� ���
	int min_weight = 100500;
	int min_weigth_index = 0;
	for (int i = 0; i < help_list.size(); i++)
	{
		if (help_list[i][2] < min_weight && find(deleted.begin(), deleted.end(), help_list[i][1]) == deleted.end())
		{
			min_weight = help_list[i][2];
			min_weigth_index = i;
		}
	}
	if (min_weight == 100500)
		return;
	prima_weight += help_list[min_weigth_index][2];
	deleted.push_back(help_list[min_weigth_index][0]);
	deleted.push_back(help_list[min_weigth_index][1]);
	//printf("deleted.push_back %d %d\n", help_list[min_weigth_index][0], help_list[min_weigth_index][1]);
	//printf("min_weight = %d\n", min_weight);
	//printf("prima_list.push_back %d %d %d\n", help_list[min_weigth_index][0], help_list[min_weigth_index][1], help_list[min_weigth_index][2]);
	prima_list.push_back(help_list[min_weigth_index]);
	prima(help_list[min_weigth_index][1]);
	return;
}

//��������
void work_with_graph()
{
	int s = -1;
	vector<int> flag, prior;
	Graphs[0].flag_connected = true;
	for (int i = 0; i < Graphs[0].vertex_count; i++)
	{
		flag.push_back(0);
		prior.push_back(0);
	}
	printf("> 1 - ������ ����� ����\n");
	printf("> 2 - ���������� �����\n");
	printf("> 3 - ���������� ������\n");
	printf("> 4 - ������� ���������\n");
	printf("> 5 - ������� ��������������\n");
	printf("> 6 - ������ ���������\n");
	printf("> 7 - ������� �������\n");
	printf("> 8 - �������������(���������) ������������������ �������\n");
	printf("> 9 - �������� �������\n");
	printf("> 10 - ������� �������\n");
	printf("> 11 - �������� �����(����)\n");
	printf("> 12 - ������� �����(����)\n");
	printf("> 13 - ������� ����� � ������\n");
	printf("> 14 - ���������� �����\n");
	printf("> 15 - ������������ �����\n");//
	printf("> 16 - ����������\n");
	printf("> 17 - �������������� 2 ������\n");
	printf("> 18 - ������������ �������\n");//
	printf("> 19 - ����������� �������\n");//
	printf("> 20 - ����������� �����������\n");
	printf("> 21 - ����������\n");
	printf("> 22 - ������������\n");
	printf("> 23 - ������� �������\n");
	printf("> 24 - ������������� �������\n");
	if (!Graphs[0].orgraph)
	{
		printf("> 25 - �������������� �������\n");
		printf("> 26 - �������\n");
		printf("> 27 - ������\n");
		printf("> 28 - ������\n");
		printf("> 29 - ������������ �������\n");
		printf("> 30 - ���������� ����� ���������\n");
	}
	printf("> 31 ����� � �������\n");
	printf("> 32 ����� � ������\n");
	printf("> 33 ������� �����\n");

	printf("> 0 - �����\n");
	do
	{
		printf("> ");
		scanf("%d", &s);
		if (s == 0)
		{
			Graphs[0].~Graph();
		}
		else if (s == 1)
		{
			if (get_graph())
				printf("> �����\n");
		}
		else if (s == 2)
		{
			Graphs[0].print_edges_count();
		}
		else if (s == 3)
		{
			Graphs[0].print_vertex_count();
		}
		else if (s == 4)
		{
			Graphs[0].print_adjacency_matrix();
		}
		else if (s == 5)
		{
			Graphs[0].print_incidence_matrix();
		}
		else if (s == 6)
		{
			Graphs[0].print_adjacency_list();
		}
		else if (s == 7)
		{
			int s;
			printf("> ������� ��� �������:\n> ");
			scanf("%d", &s);
			if (Graphs[0].orgraph)
				Graphs[0].print_vertex_halfdegree(s);
			else
				Graphs[0].print_vertex_degree(s);
		}
		else if (s == 8)
		{
			if (Graphs[0].orgraph)
				Graphs[0].print_halfdegree_sequence();
			else
				Graphs[0].print_degree_sequence();
		}
		else if (s == 9)
		{
			Graphs[0].add_vertex();
			printf("> �����!\n");
		}
		else if (s == 10)
		{
			int vertex;
			printf("> ������� �������:\n> ");
			scanf("%d", &vertex);
			Graphs[0].remove_vertex(vertex);
			if (Graphs[0].flag_success)
				printf("> �����\n");
		}
		else if (s == 11)
		{
			int i, V[2];
			printf("> ������� ��� �������:\n> ");
			for (i = 0; i < 2; i++)
				scanf("%d", &V[i]);
			Graphs[0].add_edge(V[0], V[1]);
			if (Graphs[0].flag_success)
			{
				printf("> �����\n");
			}
		}
		else if (s == 12)
		{
			int i, V[2];
			printf("> ������� ��� �������:\n> ");
			for (i = 0; i < 2; i++)
				scanf("%d", &V[i]);
			Graphs[0].remove_edge(V[0], V[1]);
			if (Graphs[0].flag_success)
				printf("> �����\n");
		}
		else if (s == 13)
		{
			if (Graphs[0].orgraph)
			{
				int i;
				printf("> \n������: \n");
				for (i = 0; i < Graphs[0].sources.size(); i++)
				{
					printf("> %d  ", Graphs[0].sources[i]);
				}
				printf("> \n�����: \n");
				for (i = 0; i < Graphs[0].drains.size(); i++)
				{
					printf("> %d  ", Graphs[0].drains[i]);
				}
				printf("\n");
			}
		}
		else if (s == 14)
		{
			if (!Graphs[0].orgraph)
			{
				bool flag_kratn = false;
				for (int i = 1; i < Graphs[0].vertex_count + 1; i++)
				{
					for (int j = 1; j < Graphs[0].vertex_count + 1; j++)
						if (Graphs[0].adjacency_matrix[i * (Graphs[0].vertex_count + 1) + j] > 1)
						{
							flag_kratn = true;
							break;
						}
					if (flag_kratn)
						break;
				}
				if (!Graphs[0].loop_count && !flag_kratn)
				{
					Graphs.push_back(Graphs[0].complement_graph());
					printf("> �����!\n");
				}
			}
		}
		else if (s == 15)
		{
			if (!Graphs[0].orgraph)
			{
				int i, V[2];
				printf("> ������� ��� �������:\n> ");
				for (i = 0; i < 2; i++)
					scanf("%d", &V[i]);
				Graphs[0].subdivision_edge(V[0], V[1]);
				if (Graphs[0].flag_success)
					printf("> �����\n");
			}
		}
		else if (s == 16)
		{
			if (!Graphs[0].orgraph)
			{
				Graphs[0].graph_contraction();
				if (Graphs[0].flag_success)
					printf("> �����!\n");
			}
		}
		else if (s == 17)
		{
			Graphs[0].graph_contraction();
			if (Graphs[0].flag_success)
				printf("> �����!\n");
		}
		else if (s == 18)
		{
			int V;
			printf("> ������� �������:\n> ");
			scanf("%d", &V);
			Graphs[0].duplicate_vertex(V, false);
			if (Graphs[0].flag_success)
				printf("> �����!\n");
		}
		else if (s == 19)
		{
			int V;
			printf("> ������� �������:\n> ");
			scanf("%d", &V);
			Graphs[0].duplicate_vertex(V, true);
			if (Graphs[0].flag_success)
				printf("> �����!\n");
		}
		else if (s == 20)
		{
			if (Graphs.size() < 2)
			{
				printf("> ������� ������� ��� ���� ����!\n");
			}
			else
			{
				Graph* Z = choose_graph();
				Graphs.push_back(Graphs[0].disjoint_union(*Z));
				printf("> �����!\n");
			}
		}
		else if (s == 21)
		{
			if (Graphs.size() < 2)
			{
				printf("> ������� ������� ��� ���� ����!\n");
			}
			else
			{
				printf("> ������� ��� ������� �����:\n> ");
				Graph* Z = choose_graph();
				Graphs.push_back(Graphs[0].connect_graphs(*Z));
				printf("> �����!\n");
			}
		}
		else if (s == 22)
		{
			if (Graphs.size() < 2)
			{
				printf("> ������� ������� ��� ���� ����\n");
			}
			else
			{
				printf("> ������� ��� ������� �����:\n> ");
				Graph* Z = choose_graph();
				Graphs.push_back(Graphs[0].multiplication_graphs(*Z));
				printf("> �����!\n");
			}
		}
		else if (s == 23)
		{
			if (Graphs[0].leafs.size() == 0)
				printf("> ������� ������� �����������\n");
			else
			{
				printf("> ������� �������: \n");
				for (int i = 0; i < Graphs[0].leafs.size(); i++)
					printf("> %d  ", Graphs[0].leafs[i]);
				printf("\n");
			}
		}
		else if (s == 24)
		{
			if (Graphs[0].isolated.size() == 0)
				printf("> ������������� ������� �����������\n");
			else
			{
				printf("> ������������� �������: \n");
				for (int i = 0; i < Graphs[0].isolated.size(); i++)
					printf("> %d  ", Graphs[0].isolated[i]);
				printf("\n");
			}
		}
		else if (s == 25)
		{
			int vertex;
			printf("> ������� �������:\n> ");
			scanf("%d", &vertex);
			Graphs[0].print_vertex_eccentricity(vertex);
		}
		else if (s == 26)
		{
			Graphs[0].print_diameter();
		}
		else if (s == 27)
		{
			Graphs[0].print_radius();
		}
		else if (s == 28)
		{
			Graphs[0].print_centres();
		}
		else if (s == 29)
		{
			Graphs[0].print_peripheral_vertex();
		}
		else if (s == 30)
		{
			int i, V[2];
			printf("> ������� ��� �������:\n> ");
			for (i = 0; i < 2; i++)
				scanf("%d", &V[i]);
			Graphs[0].get_distance(V[0], V[1]);
		}
		else if (s == 31)
		{
			//�������� � ������ ������� � ������ ���������
			Graphs[0].walk_in_depth(0);
			printf("> ");
			for (int i = 0; i < Graphs[0].walk_list.size(); i++)
				printf("%d ", Graphs[0].walk_list[i]);
			printf("\n");
			Graphs[0].walk_list.clear();
		}
		else if (s == 32)
		{
			//�������� � ������ ������� � ������ ���������
			Graphs[0].walk_in_wide(0);
			printf("> ");
			for (int i = 0; i < Graphs[0].walk_list.size(); i++)
				printf("%d ", Graphs[0].walk_list[i]);
			printf("\n");
			Graphs[0].walk_list.clear();
			Graphs[0].help_queue.clear();
		}
		else if (s == 33)
		{
			Graphs[0].print_weight_matrix();
		}
		else if (s == 34)
		{
			Graphs[0].prima(1);
			for (int i = 0; i < Graphs[0].prima_list.size(); i++)
			{
				printf("%d -> %d\n", Graphs[0].prima_list[i][0], Graphs[0].prima_list[i][1]);
			}
			printf("prima_weight = %d\n", Graphs[0].prima_weight);
			for (int i = 0; i < Graphs[0].vertex_count + 1; i++)
			{
				for (int j = 0; j < Graphs[0].vertex_count + 1; j++)
				{
					if (i == 0)
					{
						printf("%d ", j);
						continue;
					}
					else if (j == 0)
					{
						printf("%d ", i);
						continue;
					}
					int k = 0;
					for (k = 0; k < Graphs[0].prima_list.size(); k++)
					{
						if ((i == Graphs[0].prima_list[k][0] && j == Graphs[0].prima_list[k][1]) || (j == Graphs[0].prima_list[k][0] && i == Graphs[0].prima_list[k][1]))
						{
							printf("1 ");
							break;
						}
					}
					if(k == Graphs[0].prima_list.size())
						printf("0 ");
				}
				printf("\n");
			}
			Graphs[0].prima_list.clear();
			Graphs[0].help_list.clear();
		}
		else
		{
			printf("> ������������ ����.\n");
		}
	} while (s != 0);
}

//��������
int get_graph()
{
	int s;
	string name;
	printf("> ������� ��� �����\n> ");
	fflush(stdin);
	cin >> name;
	for (int i = 0; i < Graphs.size(); i++)
	{
		if (name == Graphs[i].name)
		{
			printf("> ���� � ����� ������ ��� ����!\n");
			return 0;
		}
	}
	Graph G;
	G.name = name;
	printf("> �������� ������ ������� �����\n> 1 - ������� ���������\n> 2 - ������� �������������\n> 3 - ������ ��������� 4 - ������� �����\n> ");
	fflush(stdin);
	scanf("%d", &s);
	if (s > 4 || s < 0)
	{
		printf("> ����� ������� ���!\n");
		return 0;
	}
	fflush(stdin);
	if (s == 1)
		G.get_adjacency_matrix((char*)"����������������.txt");
	else if (s == 2)
		G.get_incidence_matrix((char*)"��������������������.txt");
	else if (s == 3)
		G.get_adjacency_list((char*)"���������������.txt");
	else if (s == 4)
		G.get_weight_matrix((char*)"������������.txt");
	Graphs.push_back(G);
	return 1;
}

void main()
{
	Graphs.clear();
	fflush(stdin);
	setlocale(LC_ALL, ".1251");
	int s;
	string name;
	printf("> ������� ��� �����\n> ");
	getline(cin, name);
	Graph G1;
	G1.name = name;
	G1.flag_count = false;
	system("cls");
	printf("> �������� ������ ����� �����:\n> 1 - ������� ���������\n> 2 - ������� �������������\n> 3 - ������ ���������\n> 4 - ������� �����\n> 0 - �����\n> ");
	scanf("%d", &s);
	if (s == 1)
	{
		G1.get_adjacency_matrix((char*)"����������������.txt");
		Graphs.push_back(G1);
	}
	else if (s == 2)
	{
		G1.get_incidence_matrix((char*)"��������������������.txt");
		Graphs.push_back(G1);
	}
	else if (s == 3)
	{
		G1.get_adjacency_list((char*)"���������������.txt");
		Graphs.push_back(G1);
	}
	else if (s == 4)
	{
		G1.get_weight_matrix((char*)"������������.txt");
		Graphs.push_back(G1);
	}
	else if (s == 0)
		return;
	work_with_graph();
	Graphs.clear();
	return;
}