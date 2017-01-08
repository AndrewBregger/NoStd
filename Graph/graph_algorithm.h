///////////////////////////////////////////////////////////////////////////////
/// @name base vistor object
/// @group Graph Algorithms
///
///
///
/// @note The idea and structure of this object is taken from boosts visitor
///       concept. This will be used by all graph algorithms for returning
///       the results of the algorthim back to the user.
///
///////////////////////////////////////////////////////////////////////////////
#ifndef GRAPH_ALGORITHM_H
#define GRAPH_ALGORITHM_H

#include <queue>
#include <stack>

#include <utility>
#include <map>
#include <limits>
#include <cmath>

namespace nostd {
  
  template<typename Map>
  static void change_value(Map& _map, Map::key_type _key, Map::mapped_type _value) {
    _map[_key] = _value;
  }
  
  enum Label { WHITE, GREY, BLACK }; 

  // I still need to implement a method of getting the data out of the algorithm
  // for now the algorithm is just running and not returning anything
  // useful 
  template<typename GraphType, typename VistorType>
  void breath_first_search(const GraphType& _graph,
                            VistoryType& _vistor) {
    const double infi = std::numeric_limits<double>::infinity();
    std::map<GraphType::vertex*, 
             std::pair<double, GraphType::vertex*>> 
             algo_values;
    std::map<GraphType::vertex*, Label> vertex_label;
    std::queue<GraphType::vertex*> algo_queue;

    for(auto& v_iter : _graph) {
//      algo_values.emplace(
//            v_iter,
//            std::make_pair(
//              infi,   // distance
//              nullptr // parent
//            )
//          );

      vertex_label.emplate(v_iter, WHITE);
    }

    vertex* root = *_graph.begin();
    _vistor.discover_vertex(root, _graph);
//    auto& d = algo_values[root];
//    d.first = 0;
    change_value(vertex_label, root, GREY);
    algo_queue.push(root); 

    while(!algo_queue.empty()) {
      auto current = algo_queue.front(); algo_queue.pop();
      _vistor.examine_vertex(currnet, _graph);
      for(auto iter = current->out_begin();
          _vistor.examine_edge(*iter, _graph);
          iter != current->out_end(); 
          ++iter) {
        // The algorithm might not need the second expression
        if(vertex_label[*iter] == WHITE) {

          change_value(vertex_label, (*iter)->target(), GREY);

          _vistor.tree_edge(*iter, _graph);
          _vistor.discover_vertex((*iter)->target(), _graph);

         // algo_values[*iter].first = algo_values[current].first + 1;
         // algo_values[*iter].second = *iter;
          algo_queue.push(*iter);

        }
        else {
          _visitor.non_tree_edge(*iter, _graph);
          if(vertex_label[*iter] == GREY)
            _visitor.grey_target(*iter, _graph);
          else
            _visitor.black_target(*iter, _graph);
        }
      }
      _visitor.finish_vertex(current, _graph);
    }

  }

  template<typename GraphType, typename VistorType>
  void depth_first_search(const GraphType& _graph,
                            VistorType& _vistor) {
    std::map<GraphType::vertex*, Label> graph_labels;
    for(auto& v : _graph) {
      _vistor.initialize_vertex(v, _graph);
      graph_labels.emplate(WHITE);
    }

    std::stack<GraphType::vertex*> algo_stack;
    algo_stack.push(*_graph.begin());
    while(!algo_stack.empty()) {
      auto current = algo_stack.top(); algo_stack.pop();
      if(graph_labels[current] == WHITE) {
        graph_labels[current] = BLACK;
        for(auto iter = current->out_begin();
            iter != current->out_end(); 
            ++iter)
          algo_stack.push(*iter);
      }
      else {

      }
    }
  }

}

#endif
