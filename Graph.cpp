#include <iostream>
#include <unordered_map>
#include <list>
#include <queue>
#include <stack>
#include <vector>
using namespace std;

class Graph 
{
    public:
    unordered_map<int, list<int>> adjList;
    void addVertex(int vertex) 
    {
        if (adjList.find(vertex) == adjList.end()) 
        {
            adjList[vertex] = list<int>();
        }
    }

    void addEdge(int source, int destination, bool isDirected = false) 
    {
        adjList[source].push_back(destination);
        if (!isDirected) 
        {
            adjList[destination].push_back(source);
        }
    }

    void printGraph() 
    {
        //printing the adjacency list of the graph for each node
        for (auto& i : adjList) 
        {
            cout << i.first << " -> ";
            for (int j : i.second) 
            {
                cout << j << " ";
            }
            cout << endl;
        }
    }

    void bfs(int start) 
    {
        unordered_map<int, bool> visited;
        queue<int> q;

        visited[start] = true;
        q.push(start);

        cout << "BFS Traversal: ";
        while (!q.empty()) {
            int current = q.front();
            q.pop();
            cout << current << " ";

            for (int neighbor : adjList[current]) 
            {
                if (!visited[neighbor]) 
                {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }
        cout << endl;
    }

    void dfs(int start) 
    {
        unordered_map<int, bool> visited;
        stack<int> s;

        s.push(start);

        cout << "DFS Traversal: ";
        while (!s.empty()) 
        {
            int current = s.top();
            s.pop();

            if (!visited[current]) 
            {
                visited[current] = true;
                cout << current << " ";

                for (auto it = adjList[current].rbegin(); it != adjList[current].rend(); ++it)
                {
                    if (!visited[*it]) 
                    {
                        s.push(*it);
                    }
                }
            }
        }
        cout << endl;
    }

};


