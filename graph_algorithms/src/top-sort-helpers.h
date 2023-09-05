#pragma once

#include <unordered_map>

#include "weighted-graph.hpp"
#include "graph-types.h"

template <typename T>
void computeIndegrees(const WeightedGraph<T>& graph, std::unordered_map<value_type<T>, int>& indegrees) {
    // TODO store the indegree for each vertex in the graph in the indegrees map

    
    for (auto it : graph) {
        indegrees[it.first] = 0;
    }

    for (auto adjList : graph) {
        for (auto vertex : adjList.second) {
            indegrees[vertex.first] += 1;
        }
    }


}
