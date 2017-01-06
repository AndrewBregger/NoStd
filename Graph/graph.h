#ifndef GRAPH_H
#define GRAPH_H
#include <cstdio>
#include <cstdlib>
#include <set>

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
      typedef typename std::set<edge*>::iterator adj_iterator;

      typedef typename std::set<vertex*>::const_iterator const_vertex_iterator;
      typedef typename std::set<edge*>::const_iterator const_edge_iterator;    
      typedef typename std::set<edge*>::const_iterator const_adj_iterator;

      /// @}
     
      graph() {}
      vertex_iterator find_vertex(vertex* _vert) {
        return this->m_vertex.find(_vert);
      }

      const_vertex_iterator find_vertex(vertex* _vert) const {
        return this->m_vertex.find(_vert);
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

      vertex_iterator begin() { return m_vertex.begin(); }
      vertex_iterator end() { return m_vertex.end(); }

      const_vertex_iterator begin() const { return m_vertex.begin(); }
      const_vertex_iterator end() const { return m_vertex.end(); }

      class vertex {
        public:
          /// @name constructors
          /// @{
          
          vertex(const VertProp& _prop = VertProp()): m_property(_prop) {}

          /// @}
          /// @name Object Access and Manipulation
          /// @{
      
          VertProp& property() { return m_property; }
          const VertProp& property() const { return m_property; }
          
          virtual edge* add_inedge(edge* _edge) { 
            m_inedgelist.insert(_edge);
            return _edge; 
          }

          virtual edge* add_outedge(edge* _edge) { 
            m_outedgelist.insert(_edge);
            return _edge; 
          }

          virtual edge* find(vertex* _vert) { 
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

          adj_iterator in_begin() { return m_inedgelist.begin(); }
          adj_iterator in_end() { return m_inedgelist.end(); }

          const_adj_iterator in_begin() const { return m_inedgelist.begin(); }
          const_adj_iterator in_end() const { return m_inedgelist.end(); }

          adj_iterator out_begin() { return m_outedgelist.begin(); }
          adj_iterator out_end() { return m_outedgelist.end(); }

          const_adj_iterator out_begin() const { return m_outedgelist.begin(); }
          const_adj_iterator out_end() const { return m_outedgelist.end(); }

          size_t degree() { return m_inedgelist.size() + m_outedgelist.size(); }

          void remove_edge(edge* _edge) {
            if(this == _edge->source())
              m_inedgelist.erase(_edge);
            else if(this == _edge->target())
              m_outedgelist.erase(_edge);
            else
              printf("Error Edge not on vertex\n");              
          }

          /// @}
          
           
        protected:
          VertProp m_property;                        // property of the vertex
          std::set<edge*> m_inedgelist;               // list of in adjacent edges
          std::set<edge*> m_outedgelist;              // list of out adjacent edges
          // maybe i need a list of all adj edges
          // maybe expand the class to allow for the
          // containers to be either vectors, sets, or unordered_sets
          // for now it will remain a set.
      };


      class edge {
        public:
          //////////////////////////////////////////////////////////////////////
          /// @name constructors
          /// @{
          
          edge(vertex* _source, vertex* _target, 
              const EdgeProp& _prop = EdgeProp()):
              m_property(_prop), m_source(_source), m_target(_target) {}
          
          /// @}
          /// @name Object Access and Manipulation
          /// @{
          
          EdgeProp& property() { return m_property; }
          const EdgeProp& property() const { return m_property; }

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

          /// @}
          
        protected:
          EdgeProp m_property;                      // property of the edge
          vertex* m_source;                 // source vertex of the edge
          vertex* m_target;                 // target vertex of the edge
      };

    protected:
      std::set<vertex*> m_vertex;
      std::set<edge*> m_edge;
  };
}

#endif // GRAPH_H
