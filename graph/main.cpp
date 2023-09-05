#include <Graph.h>

int main() {
    Graph g = {
        {1, 4, 6},    // 0: 1->4->6
        {2, 6},       // 1: 2->6
        {4, 5},       // 2: 4->5
        {},           // 3: empty
        {1, 2, 3, 5}, // 4: 1->2->3->5
        {3},          // 5: 3
        {2, 3}        // 6: 2->3
    };

    VisitedSet visited(g.num_vertices());
    std::cout << "DFS traversal starting from vertex 0: ";
    dfs(g, 0, visited);
    std::cout << std::endl;

    return 0;
}