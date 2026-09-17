#include <iostream>
#include <fstream>

using namespace std;
const int MAXN = 100;
// доп строка гитхаб
// доп строка гитхаб 2
// dop stroka github 3
// dop stroka github 4
// Узел односвязного списка смежности
struct Node
{
    int data; // номер смежной вершины
    Node *next;
};

int n; // количество вершин
Node *adj[MAXN]; // массив указателей на списки смежности
int disc[MAXN];
int low[MAXN];
int parent[MAXN];
bool visited[MAXN];
bool ap[MAXN];
int timer;

// функция добавления ребра в список смежности (добавляем в начало)
void addEdge(int u, int v)
{
    Node *newNode = new Node; // выделяет память
    newNode->data = v; // записывает в узел номер смежной вершины v
    newNode->next = adj[u]; // новый узел вставляется в начало
    adj[u] = newNode; // голова списка вершины u теперь указывает на новый узел
}

// Освобождение памяти, выделенной под списки
void deleteLists()
{
    for (int i = 0; i < n; i++)
    {
        Node *current = adj[i];
        while (current)
        {
            Node *temp = current;
            current = current->next;
            delete temp;
        }
        adj[i] = nullptr;
    }
}

// функция для поиска точек сочленения в списке смежности
void dfs(int u)
{
    visited[u] = true;
    disc[u] = low[u] = ++timer;
    int children = 0;

    // перебираем всех соседей вершины u по списку смежности
    Node *cur = adj[u];
    while (cur)
    {
        int v = cur->data; // номер соседней вершины
        if (!visited[v])
        {
            children++;
            parent[v] = u;
            dfs(v);

            low[u] = min(low[u], low[v]); // передаём наверх минимальный disc из поддерева v

            // корень с более чем одним ребёнком
            if (parent[u] == -1 && children > 1)
                ap[u] = true;

            //некорневая вершина, из поддерева которой нет обратного ребра выше u
            if (parent[u] != -1 && low[v] >= disc[u])
                ap[u] = true;
        }
        else if (v != parent[u]) // если v уже посещён и v не является родителем u, то ребро (u,v) – обратное
        {
            low[u] = min(low[u], disc[v]);
        }
        cur = cur->next;
    }
}

int main()
{

    ifstream fin("graph_list.txt");
    if (!fin)
    {
        cerr << "Не удалось открыть файл graph_list.txt" << endl;
        return 1;
    }

    fin >> n;
    for (int i = 0; i < n; i++)
        adj[i] = nullptr; // обнуляем массив указателей adj

    // читаем списки смежности из файла
    for (int i = 0; i < n; i++)
    {
        int deg;   // Для каждой вершины i читаем её степень deg (количество смежных вершин)
        fin >> deg;
        for (int j = 0; j < deg; j++)
        {
            int v;
            fin >> v;
            addEdge(i, v);// Добавляем ребро в обоих направлениях (i -> v, потом v -> i)
            addEdge(v, i);
        }
    }
    fin.close();


   // Вывод списков смежности с указанием степени
    cout << "Списки смежности графа (" << n << " вершин):" << endl;
    for (int i = 0; i < n; i++)
    {
        // Сначала соберём уникальных соседей во временный массив
        bool seen[MAXN] = {false};
        int neighbors[MAXN];
        int count = 0;
        Node* cur = adj[i];
        while (cur)
        {
            int v = cur->data;
            if (!seen[v])
            {
                seen[v] = true;
                neighbors[count++] = v;
            }
            cur = cur->next;
        }
    // Выводим вершину, степень и соседей
    cout << i << ": " << count;
    for (int k = 0; k < count; k++)
        cout << " " << neighbors[k];
    cout << endl;
}
cout << endl;

    // Инициализация вспомогательных массивов
    for (int i = 0; i < n; i++)
    {
        visited[i] = false;
        parent[i] = -1; // нет родителя (-1)
        ap[i] = false;
    }
    timer = 0;

    // запускаем функцию для каждой компоненты связности
    for (int i = 0; i < n; i++) // Цикл по всем вершинам. Если вершина ещё не посещена, запускает поиск в глубину
    {
        if (!visited[i])
            dfs(i);
    }

    // Вывод результатов
    cout << "Точки сочленения (цеха, отключение которых приведет к проблемам):" << endl;
    bool found = false;
    for (int i = 0; i < n; i++) // перебираем все вершины. Если ap[i] истинно, выводим номер вершины и ставим флаг found
    {
        if (ap[i])
        {
            cout << i << " ";
            found = true;
        }
    }
    if (!found)
        cout << "Таких цехов нет.";
    cout << endl;

    deleteLists();
    cout << "Нажмите Enter для продолжения...";
    cin.get();
    return 0;
}
