#include <iostream>
#include <vector>

class Vertice{ //wierzchołek to struktura która przechowuje numer wierzchołka i koszt dotarcia do niego
    public:
    int num;
    int cost;
    Vertice(int n, int k) : num(n), cost(k) {}

    friend std::ostream &operator<<(std::ostream &os, const Vertice &v) {
        os << v.num+1; 
        return os;
    }
};

class GraphAdjList{
    public:
    int size;
    std::vector<std::vector<Vertice>> table;
    bool directed;

    GraphAdjList(int n) : size(n), table(std::vector<std::vector<Vertice>> (size)) {}   

    void addEdge(int v1, Vertice v2){ //podaj wierzcholek z ktorego prowadzi krawedz (v1) i do ktorego prowadzi(v2)
        table[v1].push_back(v2);
    }
    void printVertices() {
        for(int i = 0; i < size; i++){
            std::cout << "Vertice " << i+1 << " has an edge to: " ;
            for(auto j : table[i]){
                std::cout << j << " ";
            }
            std::cout << std::endl;
        }
    }
};