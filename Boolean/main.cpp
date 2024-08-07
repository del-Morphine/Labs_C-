#include <iostream>
#include <fstream>
#include "Boolean.h"

void topsort(Boolean_matrix adjacency_matrix, int num_vertex, int *arr);

int main(int argc, char const *argv[])
{
    ifstream graph_file("graph.txt");

    int num_vertices = 0;
    int vertex1, vertex2;
    
    graph_file >> num_vertices;       
    Boolean_matrix adjacency_matrix(num_vertices, num_vertices);
    while(!graph_file.eof())
    {
        graph_file >> vertex1 >> vertex2;
        adjacency_matrix[vertex1 - 1].Set1(vertex2 - 1);
    }    
    cout << adjacency_matrix << "\n\n";
    int *arr = new int[num_vertices];
    topsort(adjacency_matrix, num_vertices, arr);
    for(int i = 0; i < num_vertices; i++)
        cout << arr[i] << ' ';

    return 0;
}

void topsort(Boolean_matrix adjacency_matrix, int num_verticies, int *arr)
{
    Boolean_vector processed = Boolean_vector(num_verticies);
    Boolean_vector with_incoming_arcs = Boolean_vector(num_verticies);
    Boolean_vector left_to_process = Boolean_vector(num_verticies);
    int index = 0;

    while(index < num_verticies)
    {
        for(int row = 0; row < num_verticies - 1; row++)
            if(!processed[row])
                with_incoming_arcs = with_incoming_arcs | (adjacency_matrix[row] | adjacency_matrix[row + 1]);

        for(int vertex = 0; vertex < num_verticies; vertex++)
            if((~with_incoming_arcs)[vertex] && (~processed)[vertex])
            {
                arr[index] = vertex;
                processed.Set1(vertex);
                index++;
                for(int i = 0; i < num_verticies; i++)
                    adjacency_matrix[vertex].Set0(i);
            }
            else
                left_to_process.Set1(vertex);
                
        with_incoming_arcs = Boolean_vector(num_verticies);
    }
}