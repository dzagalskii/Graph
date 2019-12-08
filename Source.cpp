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
	int* adjacency_matrix;//Матрица смежности
	int* incidence_matrix;//Матрица инцидентности
	vector<vector<int>> adjacency_list;//Список смежности
	bool orgraph;		//Флаг ориентированности
	int vertex_count;	//Количество вершин
	int edge_count;	//Количество ребер
	int loop_count; //Количество петель
	bool flag_count;
	bool flag_success;
	vector<int> degree_sequence;//Степени вершин графа
	vector<int> halfdegree_sequence_out;//Полустепени исходов вершин графа
	vector<int> halfdegree_sequence_in;//Полустепени заходов вершин графа

	vector<int> leafs;
	vector<int> isolated;
	vector<int> sources;
	vector<int> drains;
	vector<vector<int>> distance_matrix;
	vector<int> eccentricity;
	int diameter;
	int radius;
	bool flag_connected;

	void print_edges_count();	//Вывод количества ребер
	void print_vertex_count();	//Вывод количества вершин
	void get_adjacency_matrix(char* file_name);	//Считывание с файла матрицы смежности
	void get_incidence_matrix(char* file_name);	//Считывание с файла матрицы инцидентности
	void get_adjacency_list(char* file_name);	//Считывание с файла списков смежности
	void print_adjacency_matrix();	//Вывод матрицы смежности
	void print_incidence_matrix();	//Вывод матрицы инцидентности
	void print_adjacency_list();	//Вывод списка смежности
	void adjacency_matrix_to_incidence_matrix();	//Преобразование матрицы смежности в матрицу индцидентности
	void incidence_matrix_to_adjacency_matrix();	//Преобразование матрицы индцидентности в матрицу смежности
	void adjacency_list_to_adjacency_matrix();	//Преобразование списка смежности в матрицу смежности
	void adjacency_matrix_to_adjacency_list();	//Преобразование матрицы смежности в список смежности
	void get_edges_count();	//Подсчет количества ребер/дуг
	void get_degree_sequence();	//Подсчет степеней вершин
	void get_halfdegree_sequence();	//Подсчет полустепеней вершин для оргафа
	void print_vertex_degree(int V);	//Вывод степени вершины
	void print_vertex_halfdegree(int V);	//Вывод полустепеней захода и исхода вершины
	void print_halfdegree_sequence();	//Вывод полустепенной последовательности
	void print_degree_sequence();	//Вывод степенной последовательности
	void add_vertex();	//Добавление вершины в граф
	void remove_vertex(int V);	//Удаление вершины из графа
	void add_edge(int from, int to);	//Добавление ребра в граф
	void remove_edge(int from, int to);	//Удаление ребра из графа
	void get_leaf_and_isolated_count();	//Подсчет висячих и изолированных вершин
	void get_sources_and_drains_count();	//Подсчет стоков и истоков
	Graph complement_graph();	//Определение дополнения графа
	void get_eccentricity();	//Вычисление эксцентриситета вершин
	void get_diameter_and_radius();	//Подсчет диаметра и радиуса графа
	void print_vertex_eccentricity(int V);	//Вывод эксцентриситета вершины
	void get_distance_matrix();	//Определение матрицы расстояний
	void subdivision_edge(int from, int to);	//Подразбиение ребра
	void graph_contraction();	//Стягивание графа
	void duplicate_vertex(int V, bool mult);	//Дублирование и размножение вершины
	Graph disjoint_union(Graph G);	//Дизъюнктное объединение графов
	Graph connect_graphs(Graph G);	//Соединение графов
	Graph multiplication_graphs(Graph G);	//Произведение графов
	void print_diameter();	//Вывод диаметра графа
	void print_radius();	//Вывод радиуса графа
	void print_centres();	//Вывод центров графа
	void print_peripheral_vertex();	//Вывод перефирийных вершин графа
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
	printf("> Номер второго графа:\n");
	scanf("%d", &n);
	while (n > Graphs.size() || n < 1)
	{
		fflush(stdin);
		n = 0;
		printf("> Некорректный ввод\n");
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

//работает
void Graph::print_edges_count()
{
	printf("> Количество ребер: %d\n", edge_count);
	return;
}

//работает
void Graph::print_vertex_count()
{
	printf("> Количество вершин: %d\n", vertex_count);
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
		printf("> Ошибка открытия файла\n");
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

//работает
void Graph::get_adjacency_matrix(char* file_path)
{
	vertex_count = 0;
	int i = 0, j;
	char buf[100];
	FILE* file;
	if ((file = fopen(file_path, "r")) == NULL)
	{
		printf("> Ошибка открытия файла\n");
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

//работает
void Graph::get_incidence_matrix(char* file_path)
{
	int i = 0, j, c, N = 0;
	char buf[100];
	vertex_count = 0;
	edge_count = edge_count = 0;
	FILE* file;
	if ((file = fopen(file_path, "r")) == NULL)
	{
		printf("> Ошибка открытия файла\n");
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

//работает
void Graph::get_adjacency_list(char* file_path)
{
	int c = -1, line = 0;
	FILE* file;
	if ((file = fopen(file_path, "r")) == NULL)
	{
		printf("> Ошибка открытия файла\n");
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

//работает
void Graph::print_incidence_matrix()
{
	int i, j, N;
	N = edge_count;
	printf("> Матрица инцидентности:\n");
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

//работает
void Graph::print_adjacency_list()
{
	unsigned int i, j;
	printf("> Список смежности:\n");
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

//работает
void Graph::print_adjacency_matrix()
{
	int i, j;
	printf("> Матрица смежности:\n");
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

//работает
void Graph::print_weight_matrix()
{
	printf("> Матрица весов:\n");
	for (int i = 0; i < vertex_count + 1; i++)
	{
		for (int j = 0; j < vertex_count + 1; j++)
			printf("%d ", weight_matrix[i * (vertex_count + 1) + j]);
		printf("\n");
	}
	return;
}

//работает
void Graph::adjacency_list_to_adjacency_matrix()
{
	adjacency_matrix = new int[(vertex_count + 1) * (vertex_count + 1)];
	memset(adjacency_matrix, 0, (vertex_count + 1) * (vertex_count + 1) * sizeof(int));
	//номера вершин
	for (int i = 1; i < vertex_count + 1; i++)
	{
		adjacency_matrix[i] = adjacency_matrix[i * (vertex_count + 1)] = adjacency_list[i - 1][0];
	}
	for (int i = 0; i < adjacency_list.size(); i++)
	{
		for (int j = 1; j < adjacency_list[i].size(); j++)
		{
			//петля
			if (adjacency_list[i][0] == adjacency_list[i][j])
			{
				adjacency_matrix[(i + 1) * (vertex_count + 1) + i + 1] = 2;
			}
			//обычно
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

//работает
void Graph::adjacency_matrix_to_adjacency_list()
{
	for (int i = 1; i < vertex_count + 1; i++)
	{
		adjacency_list.push_back(vector<int>());
		//вершина
		adjacency_list[i - 1].push_back(adjacency_matrix[i]);
		//смежные с ней
		for (int j = 1; j < vertex_count + 1; j++)
		{
			if (adjacency_matrix[i * (vertex_count + 1) + j] == 1)
				adjacency_list[i - 1].push_back(adjacency_matrix[j]);
		}
	}
	get_distance_matrix();
	return;
}

//работает
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
			//для орграфа
			if ((adjacency_matrix[i * (vertex_count + 1) + j] == 0) && (adjacency_matrix[j * (vertex_count + 1) + i]) == 1)
			{
				incidence_matrix[(i - 1) * (N + 1) + k] = -1;
				incidence_matrix[(j - 1) * (N + 1) + k] = 1;
				k++;
			}
			//для ографа
			else if ((adjacency_matrix[i * (vertex_count + 1) + j] == 1) && (adjacency_matrix[j * (vertex_count + 1) + i]) == 0)
			{
				incidence_matrix[(i - 1) * (N + 1) + k] = 1;
				incidence_matrix[(j - 1) * (N + 1) + k] = -1;
				k++;
			}
			//для петли
			else if ((i == j) && (adjacency_matrix[i * (vertex_count + 1) + i]) == 2)
			{
				incidence_matrix[(i - 1) * (N + 1) + k] = 2;
				k++;
			}
			//обычный случай
			else if ((adjacency_matrix[i * (vertex_count + 1) + j] == 1) && (adjacency_matrix[j * (vertex_count + 1) + i]) == 1)
			{
				//для орграфа
				if (orgraph)
				{
					incidence_matrix[(i - 1) * (N + 1) + k] = 1;
					incidence_matrix[(j - 1) * (N + 1) + k] = -1;
					k++;
					incidence_matrix[(i - 1) * (N + 1) + k] = -1;
					incidence_matrix[(j - 1) * (N + 1) + k] = 1;
					k++;
				}
				//для обычного
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

//работает
void Graph::incidence_matrix_to_adjacency_matrix()
{
	int N = edge_count;
	adjacency_matrix = new int[(vertex_count + 1) * (vertex_count + 1)];
	memset(adjacency_matrix, 0, (vertex_count + 1) * (vertex_count + 1) * (sizeof(int)));
	//номера
	for (int i = 1; i < vertex_count + 1; i++)
	{
		adjacency_matrix[i] = adjacency_matrix[i * (vertex_count + 1)] = incidence_matrix[(i - 1) * (N + 1)];
	}
	//обычный
	if (!orgraph)
	{
		for (int i = 0; i < vertex_count; i++)
		{
			for (int j = 1; j < N + 1; j++)
			{
				//первая вершина
				if (incidence_matrix[i * (N + 1) + j] == 1)
				{
					for (int z = i + 1; z < vertex_count; z++)
					{
						//вторая вершина
						if (incidence_matrix[z * (N + 1) + j] == 1)
						{
							adjacency_matrix[(z + 1) * (vertex_count + 1) + i + 1] = 1;
							adjacency_matrix[(i + 1) * (vertex_count + 1) + z + 1] = 1;
						}
					}
				}
				//петля
				else if (incidence_matrix[i * (N + 1) + j] == 2)
				{
					adjacency_matrix[(i + 1) * (vertex_count + 1) + i + 1] = 2;
				}
			}
		}
	}
	//орграф
	else
	{
		for (int i = 0; i < vertex_count; i++)
		{
			for (int j = 1; j < N + 1; j++)
			{
				//первая вершина туда
				if (incidence_matrix[i * (N + 1) + j] == 1)
				{
					for (int z = i + 1; z < vertex_count; z++)
					{
						//вторая вершина обратно
						if (incidence_matrix[z * (N + 1) + j] == -1)
						{
							adjacency_matrix[(i + 1) * (vertex_count + 1) + z + 1] = 1;
						}
					}
				}
				//первая вершина обратно
				else if (incidence_matrix[i * (N + 1) + j] == -1)
				{
					for (int z = i + 1; z < vertex_count; z++)
					{
						//вторая вершина туда
						if (incidence_matrix[z * (N + 1) + j] == 1)
						{
							adjacency_matrix[(z + 1) * (vertex_count + 1) + i + 1] = 1;
						}
					}
				}
				//петля
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

//работает
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

//работает
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

//работает
void Graph::print_vertex_degree(int vertex)
{
	int num = 0;
	while (num < adjacency_list.size() && adjacency_list[num][0] != vertex)
	{
		num++;
	}
	if (num == adjacency_list.size())
	{
		printf("> Этой вершины не существует!\n");
		return;
	}
	else
	{
		printf("> Степень: %d\n", degree_sequence[vertex - 1]);
	}
	return;
}

//работает
void Graph::print_degree_sequence()
{
	printf("> Степенная последовательность:\n");
	for (int i = 0; i < vertex_count; i++)
		printf("> %d - %d\n", incidence_matrix[i * (edge_count + 1)], degree_sequence[i]);
	return;
}

//работает
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

//работает
void Graph::print_vertex_halfdegree(int vertex)
{
	int num = 0;
	while (num < adjacency_list.size() && adjacency_list[num][0] != vertex)
	{
		num++;
	}
	if (num == adjacency_list.size())
	{
		printf("> Вершины не существует\n");
		return;
	}
	else
	{
		printf("> Полустепень исхода вершины %d: %d\tПолустепень захода вершины %d: %d\n", vertex, halfdegree_sequence_out[vertex - 1], vertex, halfdegree_sequence_in[vertex - 1]);
	}
	return;
}

//работает
void Graph::print_halfdegree_sequence()
{
	printf("> \tПолустепени исхода\n");
	for (int i = 0; i < vertex_count; i++)
		printf("> %d\t%d\n", i + 1, halfdegree_sequence_out[i]);
	printf("> \tПолустепени захода\n");
	for (int i = 0; i < vertex_count; i++)
		printf("> %d\t%d\n", i + 1, halfdegree_sequence_in[i]);
	return;
}

//работает
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
		if (k3 % (vertex_count + 1) == 0 && k3 > (vertex_count + 1))//пропускаем последний элемент в новой матрице, чтобы там был 0
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

//работает
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
		printf("> Вершины не существует\n");
		flag_success = false;
		return;
	}

	num++;
	for (i = num; i < vertex_count; i++)//сдвиг по горизонтали
	{
		for (j = 0; j < vertex_count + 1; j++)
		{
			adjacency_matrix[i * (vertex_count + 1) + j] = adjacency_matrix[(i + 1) * (vertex_count + 1) + j];
		}
	}


	for (i = num; i < vertex_count; i++)//вертикаль
	{
		for (j = 0; j < vertex_count; j++)
		{
			adjacency_matrix[j * (vertex_count + 1) + i] = adjacency_matrix[j * (vertex_count + 1) + i + 1];
		}
	}

	vertex_count--;
	int k = 0; int k2 = 0;//сдвигаем все в массиве (в массиве остались дублированными последние веришны)
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

//работает
void Graph::add_edge(int from_vertex, int to_vertex)
{
	flag_success = true;
	int from = 0, to = 0;
	//ищем
	for (int i = 1; i < vertex_count + 1; i++)
	{
		if (adjacency_matrix[i] == from_vertex)
			from = i;
		if (adjacency_matrix[i] == to_vertex)
			to = i;
	}
	//добавляем
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

//работает
void Graph::remove_edge(int from_vertex, int to_vertex)
{
	flag_success = true;
	int from = 0, to = 0;
	//ищем
	for (int i = 1; i < vertex_count + 1; i++)
	{
		if (adjacency_matrix[i] == from_vertex)
			from = i;
		if (adjacency_matrix[i] == to_vertex)
			to = i;
	}
	//проверяем направление
	if (orgraph)
	{
		if (adjacency_matrix[from * (vertex_count + 1) + to] != 0 && adjacency_matrix[to * (vertex_count + 1) + from] != 1)
			printf("> Ребра (дуги) не существует\n");
		flag_success = false;
		return;
	}
	//проверяем существование
	else
	{
		if (adjacency_matrix[from * (vertex_count + 1) + to] != 1 && adjacency_matrix[to * (vertex_count + 1) + from] != 1)
		{
			printf("> Ребра (дуги) не существует\n");
			flag_success = false;
			return;
		}
	}
	//удаляем
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

//работает
Graph Graph::complement_graph()
{
	Graph Z;
	Z.flag_count = false;
	Z.name = "Дополнение графа " + this->name;
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

//работает
void Graph::get_leaf_and_isolated_count()
{
	if (orgraph)
	{
		//для орграфа
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
		//обычного
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

//работает
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
	//номера
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
			v = q.front();//1 элемент в очереди
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
	/*printf("> \nМатрица расстояний:\n");
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
		printf("> Неверные вершины\n");
		flag_success = false;
		return;
	}
	int num1 = 0, num2 = 0, i, j;
	while (num1 < adjacency_list.size() && adjacency_list[num1++][0] != vertex_1);
	num1++;
	while (num2 < adjacency_list.size() && adjacency_list[num2++][0] != vertex_2);
	num2++;

	if (num1 != num2 && !distance_matrix[num1][num2])
		printf("> Не существует маршрута\n");
	else
		printf("> Расстояние: %d\n", distance_matrix[num1][num2]);
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

//работает
void Graph::print_vertex_eccentricity(int vertex)
{
	int num = 0, i, j;
	while (num < adjacency_list.size() && adjacency_list[num][0] != vertex)
	{
		num++;
	}
	if (num == adjacency_list.size())
	{
		printf("> Вершины с номером %d не существует\n", vertex);
		return;
	}
	printf("> Эксцентриситет вершины %d: %d\n", vertex, eccentricity[num]);
	return;
}

//работает
void Graph::get_diameter_and_radius()
{
	diameter = radius = eccentricity[0];
	for (int i = 1; i < vertex_count; i++)
	{
		//максимальное
		if (eccentricity[i] > diameter)
		{
			diameter = eccentricity[i];
		}
		//минимальное
		if (eccentricity[i] < radius)
		{
			radius = eccentricity[i];
		}
	}
	return;
}

//работает
void Graph::print_diameter()
{
	printf("> Диаметр: %d\n", diameter);
}

//работает
void Graph::print_radius()
{
	printf("> Радиус: %d\n", radius);
}

//работает
void Graph::print_centres()
{
	int i;
	printf("> Центры: \n");
	for (i = 0; i < vertex_count; i++)
	{
		if (eccentricity[i] == radius) printf(">  %d,", adjacency_matrix[i + 1]);
	}
	printf("> \n");
}

//работает
void Graph::print_peripheral_vertex()
{
	int i;
	printf("> Периферийные вершины: \n");
	for (i = 0; i < vertex_count; i++)
	{
		if (eccentricity[i] == diameter) printf(">  %d,", adjacency_matrix[i + 1]);
	}
	printf("> \n");
}

//работает ПРОВЕРИТЬ!
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
		printf("> Вершины не существует\n");
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
		printf("> Такого ребра (дуги) нет\n");
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

//работает ПРОВЕРИТЬ!
void Graph::graph_contraction()
{
	flag_success = true;
	int s;
	vector<int> V;
	//добавляем в список
	printf("> Введите вершины для стягивания: >\n");
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
			printf("> Такой вершины не существует\n");
			flag_success = false;
			return;
		}
		p = 0;
	}
	int j2 = 0;
	int new_ed[150] = { 0 };
	int count_new = 0;
	//запоминаем ребра
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
	//добавляем ребра
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

//работает ПРОВЕРИТЬ!
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
		printf("> Вершины не существует\n");
		flag_success = false;
		return;
	}
	else
	{
		num++;
		add_vertex();
		new_vertex_index = vertex_count;
		//добавляем ребра
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

//работает
Graph Graph::disjoint_union(Graph G)
{
	Graph Z;
	int i = 0, j = 0;
	for (i = 0; i < this->adjacency_list.size(); i++)
	{
		Z.adjacency_list.push_back(this->adjacency_list[i]);//сначал первый граф
	}
	for (i; i < G.adjacency_list.size() + this->adjacency_list.size(); i++)
	{
		Z.adjacency_list.push_back(G.adjacency_list[j]);// новые веришны - ыторой
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

//работает
Graph Graph::connect_graphs(Graph G)
{
	//объединить
	Graph Z = this->disjoint_union(G);
	Z.flag_count = false;
	//соединить
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

//работает
Graph Graph::multiplication_graphs(Graph G)
{
	Graph Z = (*this);
	Z.flag_count = false;
	int i, j, k;
	for (i = 0; i < G.vertex_count - 1; i++)
	{
		Z = Z.disjoint_union(*this);
	}
	//printf("## G.vertex_count %d\n", G.vertex_count); //на который умножаем
	//printf("## Z.vertex_count %d\n", Z.vertex_count); //который получаем
	//printf("## this->vertex_count %d\n", this->vertex_count); //который умножаем
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

//РАБОТАЕТ
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

//РАБОТАЕТ
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

//РАБОТАЕТ
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
	//ищем минимальный вес
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

//работает
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
	printf("> 1 - Ввести новый граф\n");
	printf("> 2 - Количество ребер\n");
	printf("> 3 - Количество вершин\n");
	printf("> 4 - Матрица смежности\n");
	printf("> 5 - Матрица индцидентности\n");
	printf("> 6 - Список смежности\n");
	printf("> 7 - Степень вершины\n");
	printf("> 8 - Полустепенная(Степенная) последовательность орграфа\n");
	printf("> 9 - Добавить вершину\n");
	printf("> 10 - Удалить вершину\n");
	printf("> 11 - Добавить ребро(дугу)\n");
	printf("> 12 - Удалить ребро(дугу)\n");
	printf("> 13 - Вывести стоки и истоки\n");
	printf("> 14 - Дополнение графа\n");
	printf("> 15 - Подразбиение ребра\n");//
	printf("> 16 - Стягивание\n");
	printf("> 17 - Отождествление 2 вершин\n");
	printf("> 18 - Дублирование вершины\n");//
	printf("> 19 - Размножение вершины\n");//
	printf("> 20 - Дизъюнктное объединение\n");
	printf("> 21 - Соединение\n");
	printf("> 22 - Произведение\n");
	printf("> 23 - Висячие вершины\n");
	printf("> 24 - Изолированные вершины\n");
	if (!Graphs[0].orgraph)
	{
		printf("> 25 - Эксцентриситет вершины\n");
		printf("> 26 - Диаметр\n");
		printf("> 27 - Радиус\n");
		printf("> 28 - Центры\n");
		printf("> 29 - Периферийные вершины\n");
		printf("> 30 - Расстояние между вершинами\n");
	}
	printf("> 31 Обход в глубину\n");
	printf("> 32 Обход в ширину\n");
	printf("> 33 Матрица весов\n");

	printf("> 0 - Выход\n");
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
				printf("> Успех\n");
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
			printf("> Укажите две вершины:\n> ");
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
			printf("> Успех!\n");
		}
		else if (s == 10)
		{
			int vertex;
			printf("> Укажите вершину:\n> ");
			scanf("%d", &vertex);
			Graphs[0].remove_vertex(vertex);
			if (Graphs[0].flag_success)
				printf("> Успех\n");
		}
		else if (s == 11)
		{
			int i, V[2];
			printf("> Укажите две вершины:\n> ");
			for (i = 0; i < 2; i++)
				scanf("%d", &V[i]);
			Graphs[0].add_edge(V[0], V[1]);
			if (Graphs[0].flag_success)
			{
				printf("> Успех\n");
			}
		}
		else if (s == 12)
		{
			int i, V[2];
			printf("> Укажите две вершины:\n> ");
			for (i = 0; i < 2; i++)
				scanf("%d", &V[i]);
			Graphs[0].remove_edge(V[0], V[1]);
			if (Graphs[0].flag_success)
				printf("> Успех\n");
		}
		else if (s == 13)
		{
			if (Graphs[0].orgraph)
			{
				int i;
				printf("> \nИстоки: \n");
				for (i = 0; i < Graphs[0].sources.size(); i++)
				{
					printf("> %d  ", Graphs[0].sources[i]);
				}
				printf("> \nСтоки: \n");
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
					printf("> Успех!\n");
				}
			}
		}
		else if (s == 15)
		{
			if (!Graphs[0].orgraph)
			{
				int i, V[2];
				printf("> Укажите две вершины:\n> ");
				for (i = 0; i < 2; i++)
					scanf("%d", &V[i]);
				Graphs[0].subdivision_edge(V[0], V[1]);
				if (Graphs[0].flag_success)
					printf("> Успех\n");
			}
		}
		else if (s == 16)
		{
			if (!Graphs[0].orgraph)
			{
				Graphs[0].graph_contraction();
				if (Graphs[0].flag_success)
					printf("> Успех!\n");
			}
		}
		else if (s == 17)
		{
			Graphs[0].graph_contraction();
			if (Graphs[0].flag_success)
				printf("> Успех!\n");
		}
		else if (s == 18)
		{
			int V;
			printf("> Укажите вершину:\n> ");
			scanf("%d", &V);
			Graphs[0].duplicate_vertex(V, false);
			if (Graphs[0].flag_success)
				printf("> Успех!\n");
		}
		else if (s == 19)
		{
			int V;
			printf("> Укажите вершину:\n> ");
			scanf("%d", &V);
			Graphs[0].duplicate_vertex(V, true);
			if (Graphs[0].flag_success)
				printf("> Успех!\n");
		}
		else if (s == 20)
		{
			if (Graphs.size() < 2)
			{
				printf("> Сначала введите еще один граф!\n");
			}
			else
			{
				Graph* Z = choose_graph();
				Graphs.push_back(Graphs[0].disjoint_union(*Z));
				printf("> Успех!\n");
			}
		}
		else if (s == 21)
		{
			if (Graphs.size() < 2)
			{
				printf("> Сначала введите еще один граф!\n");
			}
			else
			{
				printf("> Введите имя второго графа:\n> ");
				Graph* Z = choose_graph();
				Graphs.push_back(Graphs[0].connect_graphs(*Z));
				printf("> Успех!\n");
			}
		}
		else if (s == 22)
		{
			if (Graphs.size() < 2)
			{
				printf("> Сначала введите еще один граф\n");
			}
			else
			{
				printf("> Введите имя второго графа:\n> ");
				Graph* Z = choose_graph();
				Graphs.push_back(Graphs[0].multiplication_graphs(*Z));
				printf("> Успех!\n");
			}
		}
		else if (s == 23)
		{
			if (Graphs[0].leafs.size() == 0)
				printf("> Висячие вершины отсутствуют\n");
			else
			{
				printf("> Висячие вершины: \n");
				for (int i = 0; i < Graphs[0].leafs.size(); i++)
					printf("> %d  ", Graphs[0].leafs[i]);
				printf("\n");
			}
		}
		else if (s == 24)
		{
			if (Graphs[0].isolated.size() == 0)
				printf("> Изолированные вершины отсутствуют\n");
			else
			{
				printf("> Изолированные вершины: \n");
				for (int i = 0; i < Graphs[0].isolated.size(); i++)
					printf("> %d  ", Graphs[0].isolated[i]);
				printf("\n");
			}
		}
		else if (s == 25)
		{
			int vertex;
			printf("> Укажите вершину:\n> ");
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
			printf("> Укажите две вершины:\n> ");
			for (i = 0; i < 2; i++)
				scanf("%d", &V[i]);
			Graphs[0].get_distance(V[0], V[1]);
		}
		else if (s == 31)
		{
			//начинаем с первой вершины в списке смежности
			Graphs[0].walk_in_depth(0);
			printf("> ");
			for (int i = 0; i < Graphs[0].walk_list.size(); i++)
				printf("%d ", Graphs[0].walk_list[i]);
			printf("\n");
			Graphs[0].walk_list.clear();
		}
		else if (s == 32)
		{
			//начинаем с первой вершины в списке смежности
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
			printf("> Некорректный ввод.\n");
		}
	} while (s != 0);
}

//работает
int get_graph()
{
	int s;
	string name;
	printf("> Введите имя графа\n> ");
	fflush(stdin);
	cin >> name;
	for (int i = 0; i < Graphs.size(); i++)
	{
		if (name == Graphs[i].name)
		{
			printf("> Граф с таким именем уже есть!\n");
			return 0;
		}
	}
	Graph G;
	G.name = name;
	printf("> Выберите способ задания графа\n> 1 - Матрица смежности\n> 2 - Матрица инцидентности\n> 3 - Список смежности 4 - Матрица весов\n> ");
	fflush(stdin);
	scanf("%d", &s);
	if (s > 4 || s < 0)
	{
		printf("> Такой команды нет!\n");
		return 0;
	}
	fflush(stdin);
	if (s == 1)
		G.get_adjacency_matrix((char*)"МатрицаСмежности.txt");
	else if (s == 2)
		G.get_incidence_matrix((char*)"МатрицаИнцидентности.txt");
	else if (s == 3)
		G.get_adjacency_list((char*)"СписокСмежности.txt");
	else if (s == 4)
		G.get_weight_matrix((char*)"МатрицаВесов.txt");
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
	printf("> Введите имя графа\n> ");
	getline(cin, name);
	Graph G1;
	G1.name = name;
	G1.flag_count = false;
	system("cls");
	printf("> Выберите способ ввода графа:\n> 1 - Матрица смежности\n> 2 - Матрица инцидентности\n> 3 - Список смежности\n> 4 - Матрица весов\n> 0 - Выход\n> ");
	scanf("%d", &s);
	if (s == 1)
	{
		G1.get_adjacency_matrix((char*)"МатрицаСмежности.txt");
		Graphs.push_back(G1);
	}
	else if (s == 2)
	{
		G1.get_incidence_matrix((char*)"МатрицаИнцидентности.txt");
		Graphs.push_back(G1);
	}
	else if (s == 3)
	{
		G1.get_adjacency_list((char*)"СписокСмежности.txt");
		Graphs.push_back(G1);
	}
	else if (s == 4)
	{
		G1.get_weight_matrix((char*)"МатрицаВесов.txt");
		Graphs.push_back(G1);
	}
	else if (s == 0)
		return;
	work_with_graph();
	Graphs.clear();
	return;
}