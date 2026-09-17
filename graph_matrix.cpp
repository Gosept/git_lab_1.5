#include <iostream>
#include <fstream>
#include <windows.h>
using namespace std;
const int MAXN = 100;
int n;
int graph[MAXN][MAXN];
int disc[MAXN];
int low[MAXN];
int parent[MAXN];
bool visited[MAXN];
bool ap[MAXN];
int timer;

// функция для поиска точек сочленения
void dfs(int u)
{
    visited[u] = true;
    disc[u] = low[u] = ++timer;
    int children = 0;

    for (int v = 0; v < n; v++) // перебираем все вершины v
    {
        if (graph[u][v]) // если есть ребро между "u" и "v", тогда посещаем непосещенных детей
        {
            if (!visited[v])
            {
                children++;// увеличиваем счётчик детей; установка родителя вершины v
                parent[v] = u;
                dfs(v);  // рекурсивный вызов функции обхода для вершины v

                low[u] = min(low[u], low[v]); // Передаём наверх минимальный disc из поддерева v


                if (parent[u] == -1 && children > 1)// проверка есть ли у корня более одного ребёнка
                    ap[u] = true;


                if (parent[u] != -1 && low[v] >= disc[u])// проверка существует ли ребёнок v, из поддерева которого нет обратного ребра выше u
                    ap[u] = true;
            }
            else if (v != parent[u]) // обратное ребро
            {
                low[u] = min(low[u], disc[v]);
            }
        }
    }
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    ifstream fin("graph_matrix.txt");
    if (!fin)
    {
        cerr << "Не удалось открыть файл graph_matrix.txt" << endl;
        return 1;
    }

    fin >> n;
    for (int i = 0; i < n; i++) // циклы для перебора строк и столбцов
    {
        for (int j = 0; j < n; j++)
        {
            fin >> graph[i][j]; // Заполняем матрицу данными из файла
        }
    }
    fin.close();

    // Вывод матрицы смежности
    cout << "Матрица смежности графа (" << n << " вершин):" << endl;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            cout << graph[i][j] << " ";
        cout << endl;
    }
    cout << endl;

    // вспомогательные массивы (инициализация)
    for (int i = 0; i < n; i++)
    {
        visited[i] = false;
        parent[i] = -1;
        ap[i] = false;
    }
    timer = 0; //счётчик времени входа в вершины

    // запускаем DFS для каждой компоненты связности
    for (int i = 0; i < n; i++)
    {
        if (!visited[i]) // начинаем поиск в глубину от вершины i
        {
            dfs(i);
        }
    }

    // вывод результатов
    cout << "Точки сочленения (цеха, отключение которых приведет к проблемам (-_-) ):" << endl;
    bool found = false;
    for (int i = 0; i < n; i++)
    {
        if (ap[i])
        {
            cout << i << " ";
            found = true;
        }
    }
    if (!found)
    {
        cout << "Таких цехов нет.";
    }
    cout << endl;

    system("pause");
    return 0;
}
