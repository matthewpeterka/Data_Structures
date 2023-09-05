#pragma once

#include <list>
#include <vector>
#include <queue>
#include <initializer_list>
#include <iostream>

class Graph {
    public:
        using iterator = typename std::vector<std::list<size_t>>::iterator;
        using const_iterator = typename std::vector<std::list<size_t>>::const_iterator;

    private:
        std::vector<std::list<size_t>> graph_structure;

    public:
        // Constructor
        Graph(size_t num_v): graph_structure(num_v) {}

        // Build Graph using initializer list
        /*
            Example:
                Graph g = 
                {
                    {1, 4, 6},    // 0: 1->4->6
                    {2, 6},       // 1: 2->6
                    {4, 5},       // 2: 4->5
                    {},           // 3: empty
                    {1, 2, 3, 5}, // 4: 1->2->3->5
                    {3},          // 5: 3
                    {2, 3}        // 6: 2->3

                };
        */
        Graph(std::initializer_list<std::list<size_t>> il)
            : graph_structure(il.size()) 
        {
            size_t i = 0;
            for (const std::list<size_t>& l: il) {
                graph_structure[i++] = l;
            }
        }

        // Get the adjacency list for a given vertex
        std::list<size_t> adj_list(size_t vertex) const {
            if (vertex >= graph_structure.size()) {
                throw std::invalid_argument("Vertex not in graph");
            }
            return graph_structure.at(vertex);
        }

        // Gets the number of vertices in the graph (they are numbered 0 through num_vertices() - 1)
        size_t num_vertices() const { return graph_structure.size(); }

        iterator begin() { return graph_structure.begin(); }
        iterator end() { return graph_structure.end(); }
        const_iterator begin() const { return graph_structure.begin(); }
        const_iterator end() const { return graph_structure.end(); }

};

class VisitedSet {
    public:
        using iterator = typename std::vector<size_t>::iterator;
        using const_iterator = typename std::vector<size_t>::const_iterator;

    private:
        std::vector<size_t> s;

    public:
        VisitedSet(size_t size): s(size, 0) {}
        void visit(size_t vertex) {
            if (vertex >= s.size()) {
                throw std::invalid_argument("Vertex not in graph");
            }

            s.at(vertex) = 1;
        }
        void process(size_t vertex) {
            if (vertex >= s.size()) {
                throw std::invalid_argument("Vertex not in graph");
            }
            if (s.at(vertex) == 0) {
                visit(vertex);
            }
            s.at(vertex) = 2;
        }
        bool has_visited(size_t vertex) { 
            if (vertex >= s.size()) {
                throw std::invalid_argument("Vertex not in graph");
            }
            return s.at(vertex);
        }
        bool has_processed(size_t vertex) {
            if (vertex >= s.size()) {
                throw std::invalid_argument("Vertex not in graph");
            }
            return s.at(vertex) == 2; 
        }
        size_t size() { return s.size(); }
        void clear() { s.clear(); }
        iterator begin() { return s.begin(); }
        iterator end() { return s.end(); }
        const_iterator begin() const { return s.begin(); }
        const_iterator end() const { return s.end(); }
};

void bfs(const Graph& graph, size_t start_vertex) {
    VisitedSet visited(graph.num_vertices());
    std::queue<size_t> q;

    visited.process(start_vertex);
    q.push(start_vertex);

    while (!q.empty()) {
        size_t current_vertex = q.front();
        q.pop();

        std::cout << current_vertex << " ";

        for (size_t neighbor : graph.adj_list(current_vertex)) {
            if (!visited.has_visited(neighbor)) {
                visited.process(neighbor);
                q.push(neighbor);
            }
        }
    }
}


void dfs(const Graph& graph, size_t start_vertex, VisitedSet& visited) {
    visited.process(start_vertex);
    std::cout << start_vertex << " ";

    for (size_t neighbor : graph.adj_list(start_vertex)) {
        if (!visited.has_visited(neighbor)) {
            dfs(graph, neighbor, visited);
        }
    }
}

