#include "graph.h"
#include "unit_test.h"
#include <set>
#include <cassert>

using nostd::graph;

class graph_test : public test_class {

  void test() {
    vertex_insert();
    edge_insert();
    vertex_remove();
    edge_remove();
    edge_opposite();
    find_adj_edge();
  }

  void build_graph(graph<int, int>& _g) {
    auto v1 = _g.insert_vertex(1);
    auto v2 = _g.insert_vertex(2);
    auto v3 = _g.insert_vertex(3);
    auto v4 = _g.insert_vertex(4);

    _g.insert_edge(v1, v2, 1);
    _g.insert_edge(v2, v4, 2);
    _g.insert_edge(v3, v4, 3);
  }

  void vertex_insert() {
    graph<int, int> g;
    auto v = g.insert_vertex(1);

    assert(v->property() == 1 && g.num_vertices() == 1);
  }

  void edge_insert() {
    graph<int, int> g;

    auto v1 = g.insert_vertex(1);
    auto v2 = g.insert_vertex(2);
    auto e = g.insert_edge(v1, v2, 10);

    assert(e->property() == 10 && g.num_edges() == 1 &&
        v1->degree() == 1 && v2->degree() == 1);
  }

  void vertex_remove() {
    graph<int, int> g;

    auto v1 = g.insert_vertex(1);
    auto v2 = g.insert_vertex(2);
    auto v3 = g.insert_vertex(3);
    auto v4 = g.insert_vertex(4);

    g.insert_edge(v1, v2, 1);
    g.insert_edge(v2, v4, 2);
    g.insert_edge(v3, v4, 3);
    
    g.erase_vertex(v2);
    assert(g.num_edges() == 1 && g.num_vertices() == 3);
  }

  void edge_remove() {
    graph<int, int> g;

    auto v1 = g.insert_vertex(1);
    auto v2 = g.insert_vertex(2);

    auto e = g.insert_edge(v1, v2, 1);

    g.erase_edge(e);

    assert(g.num_edges() == 0);
  }

  void find_edge() {
    graph<int, int> g;

    auto v1 = g.insert_vertex(1);
    auto v2 = g.insert_vertex(2);
    auto v3 = g.insert_vertex(3);
    auto v4 = g.insert_vertex(4);

    auto e = g.insert_edge(v1, v2, 1);
    g.insert_edge(v2, v4, 2);
    g.insert_edge(v3, v4, 3);
    
    auto e1 = g.find_edge(v1, v2);
    assert(*e1 == e);
  }

  void edge_opposite() {
    graph<int, int> g;

    auto v1 = g.insert_vertex(1);
    auto v2 = g.insert_vertex(2);
    auto v3 = g.insert_vertex(3);
    auto v4 = g.insert_vertex(4);

    auto e = g.insert_edge(v1, v2, 1);
    g.insert_edge(v2, v4, 2);
    g.insert_edge(v3, v4, 3);

    auto v5 = e->opposite(v1);

    assert(v5 == v2);

  }

  void find_adj_edge() {

    graph<int, int> g;

    auto v1 = g.insert_vertex(1);
    auto v2 = g.insert_vertex(2);
    auto v3 = g.insert_vertex(3);
    auto v4 = g.insert_vertex(4);

    auto e = g.insert_edge(v1, v2, 1);
    g.insert_edge(v2, v4, 2);
    g.insert_edge(v3, v4, 3);

    auto e1 = v1->find(v2);

    assert(e1 == e);
  }

  void dfs() {

  }

  void bfs() {

  }

};

int main() {
  graph_test gtest;
  if(gtest.run())
    std::cout << "Test Successful\n";
  return 0;
}
