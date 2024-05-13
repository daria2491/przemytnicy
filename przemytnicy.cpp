#include <iostream>
#include <fstream>
#include "Graph.hpp"
#include <limits.h>
#include <algorithm>

class DijkstraTable
{
    private:
        int size;   
        int vert;
    public:
        int** table;
        DijkstraTable(int size, int vert) : size(size), vert(vert)
        {
            table = new int*[size];
            for (int i = 0; i < size; ++i)
                table[i] = new int[3];
            
            for (int i = 0; i < size; i++)
            { // pozostałe koszty na nieskonczonosc, poprzednicy -1;
                table[i][0] = INT_MAX;
                table[i][1] = -1;
                table[i][2] = INT_MAX;
            }
            table[vert][0] = 0; //tablica do przechowywania zbioru Q
            table[vert][1] = -1; //tab. poprzednikow
            table[vert][2] = 0; //tablica kosztow dojscia
        }
};

void Dijkstra(GraphAdjList G)
{
    int *costs; //wektor bedzie zawieral po kolei: koszty dotarcia do kazdego wierzcholka z 1, koszty dotarcia do 1 z kazdego wierzcholka, koszty przejscia przez granice dla wierzcholkow od 2 do n. 
    costs =  new int[(G.size-1)*3];
	int vert_num = 0;
	
    for(int j = 0 ; j < G.size; j++){
        DijkstraTable table = DijkstraTable(G.size, j);
        for (int i = 0; i < G.size; i++)
        {
            int min = INT_MAX;
            for(int i = 0; i < G.size; i++)
            { 
                if(min > table.table[i][0]){
                    min = table.table[i][0];
                    vert_num = i; //i to numer wierzcholka minimalnego
                }
            }
            for (int i = 0; i < G.table[vert_num].size(); i++)  
            {                                      // przez wszystkie wierchołki połączone z danym
                Vertice w1 = G.table[vert_num][i]; // pobiera wierchołek sposrod sąsiadów
                int w = w1.num;
                int cost = w1.cost;
                if (table.table[w][2] > table.table[vert_num][2] + cost)
                {                                            // jesli koszt dostarcia do kooejnego wierzchołka jest wiekszy niz koszt dotarcia do obecnego to:
                    table.table[w][2] = table.table[vert_num][2] + cost; // zmieniamy koszt dotarcia, w tablicy do pamietania kosztow
                    table.table[w][1] = vert_num;                  // zmiana poprzednika w tablicy do przechowywania wierzcholkow
                    table.table[vert_num][0] = INT_MAX;             //w tablicy do przechowywania el. nieodwiedzonych zmieniamy wartosc przypisana do wiercholka odwiedzonego na nieskonczonosc
                    table.table[w][0] = table.table[vert_num][2] + cost; 
                }
            }
        }
        if(j == 0){
            for (int i = 1; i < G.size; i++){
                    costs[i-1] = table.table[i][2];  //wprowadzanie kosztow dotarcia do wiercholkow z w.1
                }
            } else {
                costs[(2*(j+1)) - j] = table.table[0][2];  //wprowadzanie do tablicy kosztow dotarcia do w.1 z pozostałych
            }
    }
    for(int i = 2*(G.size-1); i < 3*(G.size-1); i++){ //dodajemy koszty przejscia przez granice na koniec wektora 
    Vertice v = G.table[i-(G.size-1)*2+1][0];
        costs[i] = v.cost;
    }
    std::vector<int> sums;
    for(int i = 0; i < G.size-1; i++){
        int sum = costs[i] + costs[i+G.size-1] + costs[i+2*(G.size-1)];
        sums.push_back(sum);
    }
    std::vector<int>::iterator result = std::min_element(sums.begin(), sums.end()); 
    std::cout << "min element has value " << *result << std::endl;
}

int main(int argc, const char *argv[])
{
    std::fstream plik;
    plik.open("tekst.txt", std::ios::in | std::ios::out);
    if (plik.good() == true)
    {
        std::cout << "Uzyskano dostep do pliku!" << std::endl;

        int num;
        plik >> num;
        GraphAdjList G1(num);
        // na poczatku musimy dodac wirzcholki poczatkowe z prowadzacymi do nich samych sciezkami
        for (int i = 0; i < num; i++)
        {
            int cost;
            plik >> cost;
            Vertice v2 = Vertice(i, cost / 2);
            G1.addEdge(i, v2);
        }
        // potem dodajemy poszczegolne sciezki
        int num2;
        plik >> num2;
        for (int i = 0; i < num2; i++)
        {
            int v1, v2, cost;
            plik >> v1 >> v2 >> cost;
            Vertice v = Vertice(v2 - 1, cost);
            G1.addEdge(v1 - 1, v); // v1 - 1 z uwagi na numeracje w zadaniu od 1 a w macierzy od 0
        }
        G1.printVertices();
        Dijkstra(G1);
    }
    else
        std::cout << "Dostep do pliku zostal zabroniony!" << std::endl;

    plik.close();
    return 0;
}