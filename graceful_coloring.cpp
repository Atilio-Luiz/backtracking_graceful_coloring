/**
 * @file graceful_coloring.cpp
 * @author Atilio Gomes Luiz
 * @brief Program that generates a k-graceful coloring of a graph with minimum k
 * @version 0.2
 * @date 2023
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <set>
#include <list>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <iostream>
using namespace std;

// a graph is implemented as a unordered_map
#define Graph unordered_map<int, list<int>>

// -------------------------------------------------------
// prototypes
void create_graph_from_file(Graph&, const string&);
void print_adj_list(const Graph&);
bool generate_aedg_labeling(Graph& graph, int max_label);
int backtrack(Graph&, int, vector<int>&, int);
int backtrack_all(Graph& G, int max_label, vector<int>& labeling, int index);
int is_a_solution(const vector<int>&);
void print_solution(const vector<int>&);
int label_is_safe(int, Graph&, int, vector<int>&);
// -------------------------------------------------------

int main() 
{
    fstream afile;
    afile.open("edges.txt", ios::out | ios::in );
    if(!afile.is_open()) {
        cout << "file could not be opened.\n";
        return 0;
    }
    int v, m;
    while(afile >> v >> m) {
        Graph graph;
        for(int i = 1; i <= m; ++i) {
            int v1, v2;
            afile >> v1 >> v2;
            graph[v1].push_back(v2);
            graph[v2].push_back(v1);
        }
        cout << "order of G: " << graph.size() << endl;
        //print_adj_list(graph);
        int max_degree {-1};
        for(int i = 0; i < (int)graph.size(); ++i) {
            if(max_degree < (int)graph[i].size()) {
                max_degree = graph[i].size();
            }
        }
        max_degree++;
        while(true) {
            if(!generate_aedg_labeling(graph, max_degree)) {
                max_degree++;
            }
            else {
                cout << "max label = " << max_degree << "\n\n";
                break;
            }
        }
    }
    afile.close();
}

void create_graph_from_file(Graph& graph, const string& filename) 
{
    fstream afile;
    afile.open(filename, ios::out | ios::in );
    int v1, v2;
    while(afile >> v1 >> v2) {
        graph[v1].push_back(v2);
        graph[v2].push_back(v1);
    }
    afile.close();
}

void print_adj_list(const Graph& graph) 
{
    for(auto p : graph) {
        cout << p.first << ": ";
        for(auto v : p.second) {
            cout << v << " ";
        }
        cout << endl;
    }
}

bool generate_aedg_labeling(Graph& graph, int max_label) 
{
    vector<int> labeling(graph.size(), -1); // solution vector
    return backtrack(graph, max_label, labeling, 0);
}

int backtrack(Graph& G, int max_label, vector<int>& labeling, int index) 
{
    if(is_a_solution(labeling)) {
        print_solution(labeling);
        return true;
    }
    for(int l = 1; l <= max_label; ++l) {
        if(label_is_safe(l, G, index, labeling)) {
            labeling[index] = l;

            if(backtrack(G, max_label, labeling, index + 1))
                return true;
            else
                labeling[index] = -1;
        }
    }
    return false;
}

int backtrack_all(Graph& G, int max_label, vector<int>& labeling, int index) 
{
    if(is_a_solution(labeling)) {
        print_solution(labeling);
        return true;
    }
    for(int l = 0; l <= max_label; ++l) {
        if(label_is_safe(l, G, index, labeling)) {
            labeling[index] = l;

            backtrack_all(G, max_label, labeling, index + 1);

            labeling[index] = -1;
        }
    }
    return false;
}

int is_a_solution(const vector<int>& labeling) 
{
    for(int v : labeling) {
        if(v == -1) {
            return false;
        }
    }
    return true;
}

void print_solution(const vector<int>& labeling) 
{
    cout << "[";
    for(int i{0}; i < (int)labeling.size()-1; ++i) {
        cout << labeling[i] << ",";
    }
    cout << labeling[labeling.size()-1] << "]\n";
}

int label_is_safe(int l, Graph& G, int index, vector<int>& labeling) 
{
    set<int> auxList;
    for(const int& neighbor : G[index]) {
        if(labeling[neighbor] != -1) {
            if(labeling[neighbor] == l) {
                return false;
            }
            int edge_label1 = abs(l-labeling[neighbor]);
            for(const int& nd : G[neighbor]) {
                if(labeling[nd] != -1 && nd != index) {
                    int edge_label2 = abs(labeling[neighbor]-labeling[nd]);
                    if(edge_label1 == edge_label2) {
                        return false;
                    }
                }
            }
            if(auxList.count(edge_label1) > 0) {
                return false;
            }
            auxList.insert(edge_label1);
        }
    }
    return true;
}
