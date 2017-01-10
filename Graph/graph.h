///////////////////////////////////////////////////////////////////////////////
/// @name Graph
/// @group Graph
///
/// @note This graph can be interacted with by either vertex and edge pointers
///       or with vertex and edge descriptors. Using the pointers interface is
///       default. To use the descriptor interface:
///           #define DESCRIPTOR_GRAPH
///       before including the graph header file.
///
///       All of the algorithms interfaces are the same; however, instead of
///       taking a vertex or edge pointer a descriptor is taken.
///
///////////////////////////////////////////////////////////////////////////////
#ifndef GRAPH_H
#define GRAPH_H
#include <cstdio>
#include <cstdlib>
#include <set>
#include <utility>

namespace nostd {

  template <typename VertProp, typename EdgeProp>
  class graph {
    public:
      class vertex;
      class edge;

      /////////////////////////////////////////////////////////////////////////
      /// @name Graph Typedefs
      /// @{
      typedef typename std::set<vertex*>::iterator vertex_iterator;
      typedef typename std::set<edge*>::iterator edge_iterator;    
      
      typedef typename std::set<vertex*>::const_iterator const_vertex_iterator;
      typedef typename std::set<edge*>::const_iterator const_edge_iterator;    

      typedef VertProp vertex_type;
      typedef EdgeProp edge_type;

#ifdef DESCRIPTOR_GRAPH
      typedef size_t vertex_descriptor;
      typedef std::pair<vertex_descriptor, vertex_descriptor> edge_descriptor;
      static const vertex_descriptor INVALID_VERTEX = size_t(-1);

      static inline vertex_descriptor get_opposite(edge_descriptor _edge, 
                                            vertex_descriptor _vert) {
        if(_edge.first == _vert)
          return _edge.second;
        else
          return _edge.first;
      }

      typedef typename std::set<edge_descriptor>::iterator adj_iterator;
      typedef typename std::set<edge_descriptor>::const_iterator const_adj_iterator;

#else
      typedef typename std::set<edge*>::iterator adj_iterator;
      typedef typename std::set<edge*>::const_iterator const_adj_iterator;
#endif

      /// @}
      /// @name constructors
      /// @{
     
#ifdef DESCRIPTOR_GRAPH
      graph() {m_count = 0;}
#else
      graph() {}
#endif

      /// @}
      /// @name Graph Manipulation
      /// @{
#ifdef DESCRIPTOR_GRAPH
      vertex_iterator find_vertex(vertex_descriptor _vert) {
        vertex temp = vertex(VertProp(), _vert);
        return m_vertex.find(&temp);
      }

      const_vertex_iterator find_vertex(vertex_descriptor _vert) const {
        vertex temp = vertex(VertProp(), _vert);
        return m_vertex.find(&temp);
      }

      edge_iterator find_edge(vertex_descriptor _source, vertex_descriptor _target) {
        edge temp = edge(_source, _target, EdgeProp());
        return m_edge.find(&temp);
      }

      const_edge_iterator find_edge(vertex_descriptor _source,
                                    vertex_descriptor _target) const {
        edge temp = edge(_source, _target, EdgeProp());
        return m_edge.find(&temp);
      }

      edge_iterator find_edge(edge_descriptor _edge) {
        return find_edge(_edge.first, _edge.second);
      }
      
      const_edge_iterator find_edge(edge_descriptor _edge) const {
        return find_edge(_edge.first, _edge.second);
      }

      vertex_descriptor insert_vertex(const VertProp& _prop) {
        vertex* temp = new vertex(_prop, m_count);
        m_vertex.insert(temp);
        return m_count++;
      }

      edge_descriptor insert_edge(vertex_descriptor _source,
                                  vertex_descriptor _target, 
                                  const EdgeProp& _prop) {
        auto source = find_vertex(_source);
        auto target = find_vertex(_target);

        edge* temp = new edge(_source, _target, _prop);
        this->m_edge.insert(temp);
        (*source)->add_outedge(temp->descriptor());
        (*target)->add_inedge(temp->descriptor());
        return temp->descriptor();
      }

      void insert_undirected(vertex_descriptor _source,
                             vertex_descriptor _target, 
                             const EdgeProp& _prop) {

        insert_edge(_source, _target, _prop);
        insert_edge(_target, _source, _prop);
      }
      
      void erase_vertex(vertex_descriptor _vert) {
        auto vert = find_vertex(_vert);
        auto in_iter  = (*vert)->in_begin();
        auto out_iter = (*vert)->out_begin();

        for(; in_iter != (*vert)->in_end(); ++in_iter) {
          auto iter = find_edge(*in_iter);
          (*iter)->source()->remove_edge(*in_iter);
          m_edge.erase(*in_iter);
        }

        for(;out_iter != (*vert)->out_end(); ++out_iter) {
          auto iter = find_edge(*out_iter);
          (*iter)->target()->remove_edge(*out_iter);
          m_edge.erase(*out_iter);
        }
        m_vertex.erase(vert);
      }
      
      void erase_edge(edge_descriptor _edge) {
        auto source = find_vertex(_edge.first);
        auto target = find_vertex(_edge.second);

        (*source)->remove_edge(_edge);
        (*target)->remove_edge(_edge);
        
        m_edge.erase(find_edge(_edge));
      }

#else 
      vertex_iterator find_vertex(vertex* _vert) {
        return m_vertex.find(_vert);
      }

      const_vertex_iterator find_vertex(vertex* _vert) const {
        return m_vertex.find(_vert);
      }

      edge_iterator find_edge(vertex* _source, vertex* _target) {
        edge temp = edge(_source, _target, EdgeProp());
        return m_edge.find(&temp);
      }

      const_edge_iterator find_edge(vertex* _source, vertex* _target) const {
        edge temp = edge(_source, _target, EdgeProp());
        return m_edge.find(&temp);
      }

      edge_iterator find_edge(edge* _edge) {
        return this->m_edge.find(_edge);
      }
      
      const_edge_iterator find_edge(edge* _edge) const {
        return this->m_edge.find(_edge);
      }

      vertex* insert_vertex(const VertProp& _prop) {
        vertex* temp = new vertex(_prop);
        this->m_vertex.insert(temp);
        return temp;
      }

      edge* insert_edge(vertex* _source, vertex* _target, 
          const EdgeProp& _prop) {

        edge* temp = new edge(_source, _target, _prop);
        this->m_edge.insert(temp);
        _source->add_outedge(temp);
        _target->add_inedge(temp);
        return temp;
      }

      void insert_undirected(vertex* _source, vertex* _target, 
          const EdgeProp& _prop) {
        insert_edge(_source, _target, _prop);
        insert_edge(_target, _source, _prop);
      }
      
      void erase_vertex(vertex* _vert) {
        auto in_iter = _vert->in_begin();
        auto out_iter = _vert->out_begin();
        for(; in_iter != _vert->in_end(); ++in_iter) {
          (*in_iter)->source()->remove_edge(*in_iter);
          m_edge.erase(*in_iter);
        }

        for(;out_iter != _vert->out_end(); ++out_iter) {
          (*out_iter)->target()->remove_edge(*out_iter);
          m_edge.erase(*out_iter);
        }
        m_vertex.erase(_vert);
      }
      
      void erase_edge(edge* _edge) {

        _edge->source()->remove_edge(_edge);
        _edge->target()->remove_edge(_edge);
        
        m_edge.erase(_edge);
      }
#endif     
      /// @}
      /// @name Graph Statistics
      /// @{

      size_t num_vertices() { return m_vertex.size(); }
      size_t num_edges() { return m_edge.size(); } 

      void clear() {
        for(auto& i : m_edge)
          delete i;
        for(auto& i : m_vertex)
          delete i;
        
        m_edge.clear();
        m_vertex.clear();
      }
      
      /// @}
      /// @name Iterators 
      /// @{

      vertex_iterator begin() { return m_vertex.begin(); }
      vertex_iterator end() { return m_vertex.end(); }

      const_vertex_iterator begin() const { return m_vertex.begin(); }
      const_vertex_iterator end() const { return m_vertex.end(); }

      /// @}
      /// @name Internal Structures
      /// @{


      class vertex {
        public:
          /// @name constructors
          /// @{
#ifdef DESCRIPTOR_GRAPH       
          vertex(const VertProp& _prop = VertProp(), 
                 vertex_descriptor _desc = INVALID_VERTEX):
                 m_property(_prop), m_descriptor(_desc) {}
#else
          vertex(const VertProp& _prop = VertProp()): m_property(_prop) {}
#endif
          /// @}
          /// @name Object Access and Manipulation
          /// @{
      
          VertProp& property() { return m_property; }
          const VertProp& property() const { return m_property; }

#ifdef DESCRIPTOR_GRAPH
          edge_descriptor add_inedge(edge_descriptor _edge) { 
            m_inedgelist.insert(_edge);
            return _edge; 
          }

          edge_descriptor add_outedge(edge_descriptor _edge) { 
            m_outedgelist.insert(_edge);
            return _edge; 
          }

          edge_descriptor find(vertex_descriptor _vert) { 
            for(auto& i : m_inedgelist) {
              auto op = get_opposite(i, _vert);
              if(op == _vert)
               return i;
            }
            for(auto& i : m_outedgelist) {
              auto op = get_opposite(i, _vert);
              if(op == _vert)
               return i;
            }

            return INVALID_VERTEX; 
          }

          size_t degree() { return m_inedgelist.size() + m_outedgelist.size(); }

          void remove_edge(edge_descriptor _edge) {
            if(m_descriptor == _edge.first)
              m_inedgelist.erase(_edge);
            else if(m_descriptor == _edge.second)
              m_outedgelist.erase(_edge);
            else
              printf("Error Edge not on vertex\n");              
          }

          vertex_descriptor descriptor() {
            return m_descriptor;
          }

#else 
          edge* add_inedge(edge* _edge) { 
            m_inedgelist.insert(_edge);
            return _edge; 
          }

          edge* add_outedge(edge* _edge) { 
            m_outedgelist.insert(_edge);
            return _edge; 
          }

          edge* find(vertex* _vert) { 
            for(auto& i : m_inedgelist) {
              auto op = i->opposite(this);
              if(op == _vert)
               return i;
            }
            for(auto& i : m_outedgelist) {
              auto op = i->opposite(this);
              if(op == _vert)
               return i;
            }

            return nullptr; 
          }

          size_t degree() { return m_inedgelist.size() + m_outedgelist.size(); }

          void remove_edge(edge* _edge) {
            if(this == _edge->source())
              m_inedgelist.erase(_edge);
            else if(this == _edge->target())
              m_outedgelist.erase(_edge);
            else
              printf("Error Edge not on vertex\n");              
          }
#endif

          /// @}
          /// @name Iterators
          /// @{

          adj_iterator in_begin() { return m_inedgelist.begin(); }
          adj_iterator in_end() { return m_inedgelist.end(); }

          const_adj_iterator in_begin() const { return m_inedgelist.begin(); }
          const_adj_iterator in_end() const { return m_inedgelist.end(); }

          adj_iterator out_begin() { return m_outedgelist.begin(); }
          adj_iterator out_end() { return m_outedgelist.end(); }

          const_adj_iterator out_begin() const { return m_outedgelist.begin(); }
          const_adj_iterator out_end() const { return m_outedgelist.end(); }
          
          /// @}
           
        protected:
          VertProp m_property;                        // property of the vertex
#ifdef DESCRIPTOR_GRAPH
          vertex_descriptor m_descriptor; 
          std::set<edge_descriptor> m_inedgelist;
          std::set<edge_descriptor> m_outedgelist;
#else
          std::set<edge*> m_inedgelist;               // list of in adjacent edges
          std::set<edge*> m_outedgelist;              // list of out adjacent edges
#endif
          // maybe i need a list of all adj edges
          // maybe expand the class to allow for the
          // containers to be either vectors, sets, or unordered_sets
          // for now it will remain a set.
      };


      class edge {
        public:
          /// @name constructors
          /// @{
#ifdef DESCRIPTOR_GRAPH
          edge(vertex_descriptor _source, vertex_descriptor _target,
              const EdgeProp& _prop = EdgeProp()) :
              m_property(_prop), m_descriptor(std::make_pair(_source, _target)) {}
#else     
          edge(vertex* _source, vertex* _target, 
              const EdgeProp& _prop = EdgeProp()):
              m_property(_prop), m_source(_source), m_target(_target) {}
#endif      
          /// @}
          /// @name Object Access and Manipulation
          /// @{
          
          EdgeProp& property() { return m_property; }
          const EdgeProp& property() const { return m_property; }

#ifdef DESCRIPTOR_GRAPH
          vertex_descriptor source() { return m_descriptor.first; }
          vertex_descriptor target() { return m_descriptor.second; }

          edge_descriptor descriptor() {
            return m_descriptor;
          }

          vertex_descriptor opposite(vertex_descriptor _vert) {
            if(m_descriptor.first == _vert)
              return m_descriptor.second;
            else
              return m_descriptor.first;
          }
#else
          vertex* source() { return m_source; }
          const vertex* target() const { return m_target; }

          vertex* target() { return m_target; }
          const vertex* source() const { return m_source; }

          vertex* opposite(vertex* _vert) {
            if(m_source == _vert)
              return m_target;
            else
              return m_source;
          }
#endif

          /// @}
          
        protected:
          EdgeProp m_property;                      // property of the edge
#ifdef DESCRIPTOR_GRAPH
          edge_descriptor m_descriptor;
#else
          vertex* m_source;                 // source vertex of the edge
          vertex* m_target;                 // target vertex of the edge
#endif
      };

    protected:
#ifdef DESCRIPTOR_GRAPH
      vertex_descriptor m_count;
#endif
      std::set<vertex*> m_vertex;
      std::set<edge*> m_edge;
  };
}

#endif // GRAPH_H
