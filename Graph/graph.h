#ifndef GRAPH_H
#define GRAPH_H
#include <cstdlib>
#include <set>

namespace nostd {


  template<typename VertProp, typename EdgeProp, 
    template <typename... A> class VertCont, 
    template <typename... B> class EdgeCont, 
    template <typename... C> class EdgeListCont>
  class graph_generic {
    public:
      class vertex_generic;
      class edge_generic;
      /// @name Graph Typedefs
      /// @{
        typedef typename VertCont<vertex_generic*>::iterator vertex_iterator;
        typedef typename EdgeCont<edge_generic*>::iterator edge_iterator;    
        typedef typename EdgeListCont<edge_generic*>::iterator adj_iterator;

        typedef typename VertCont<vertex_generic*>::const_iterator const_vertex_iterator;
        typedef typename EdgeCont<edge_generic*>::const_iterator const_edge_iterator;    
        typedef typename EdgeListCont<edge_generic*>::const_iterator const_adj_iterator;

        typedef vertex_generic vertex;
        typedef edge_generic edge;

      /// @}
      
      virtual vertex_iterator find_vertex(vertex_generic* _vert) = 0;
      virtual edge_iterator find_edge(edge_generic* _edge) = 0;

      virtual const_vertex_iterator find_vertex(vertex_generic* _vert) const = 0; const_edge_iterator find_edge(edge_generic* _edge) const = 0;

      virtual vertex_generic* insert_vertex(const VertProp& _prop) = 0;

      virtual edge_generic* insert_edge(vertex_generic* _source, vertex_generic* _target, 
          const EdgeProp& _prop) = 0;

      virtual void insert_undirected(vertex_generic* _source, vertex_generic* _target, 
          const EdgeProp& _prop) = 0;
      
      virtual void erase_vertex(vertex_generic* _vert) = 0;
      virtual void erase_edge(edge_generic* _edge) = 0;
      
      size_t num_vertices() { return m_vertex.size(); }
      size_t num_edges() { return m_edge.size(); } 

      void clear() {
        for(auto& i : m_edges)
          delete i;
        for(auto& i : m_vertex)
          delete i;
        

        m_edges.clear();
        m_vertex.clear();
      }

      vertex_iterator begin() { return m_vertex.begin(); }
      vertex_iterator end() { return m_vertex.end(); }

      const_vertex_iterator begin() const { return m_vertex.begin(); }
      const_vertex_iterator end() const { return m_vertex.end(); }

      class vertex_generic {
        public:
          /// @name constructors
          /// @{
          
          vertex_generic(const VertProp& _prop): m_property(_prop) {}

          /// @}
          /// @name Object Access and Manipulation
          /// @{
      
          VertProp& property() { return m_property; }
          const VertProp& property() const { return m_property; }
          
          virtual edge_generic* add_inedge(edge_generic* _edge) = 0;        // adds an inbound edge
          virtual edge_generic* add_outedge(edge_generic* _edge) = 0;       // add an outbound edge
          virtual edge_generic* find(vertex_generic* _vert) = 0;            // finds an vertex (source or target)
                                                                    // if an edge is contained within it

          adj_iterator in_begin() { return m_inedgelist.begin(); }
          adj_iterator in_end() { return m_inedgelist.end(); }

          const_adj_iterator in_begin() const { return m_inedgelist.begin(); }
          const_adj_iterator in_end() const { return m_inedgelist.end(); }

          adj_iterator out_begin() { return m_outedgelist.begin(); }
          adj_iterator out_end() { return m_outedgelist.end(); }

          const_adj_iterator out_begin() const { return m_outedgelist.begin(); }
          const_adj_iterator out_end() const { return m_outedgelist.end(); }

          size_t degee() { return m_inedgelist.size() + m_outedgelist.size(); }

          /// @}
          
           
        protected:
          VertProp m_property;                        // property of the vertex
          EdgeListCont<edge_generic*> m_inedgelist;   // list of in adjacent edges
          EdgeListCont<edge_generic*> m_outedgelist;  // list of out adjacent edges
      };

      class edge_generic {
        public:
          //////////////////////////////////////////////////////////////////////
          /// @name constructors
          /// @{
          
          edge_generic(vertex_generic* _source, vertex_generic* _target, 
              const EdgeProp& _prop): m_property(_prop), m_source(_source), m_target(_target) {}
          
          /// @}
          /// @name Object Access and Manipulation
          /// @{
          
          EdgeProp& property() { return m_property; }
          const EdgeProp& property() const { return m_property; }

          vertex_generic* source() { return m_source; }
          const vertex_generic* target() const { return m_target; }

          vertex_generic* target() { return m_target; }
          const vertex_generic* source() const { return m_source; }

          /// @}
          
        protected:
          EdgeProp m_property;                      // property of the edge
          vertex_generic* m_source;                 // source vertex of the edge
          vertex_generic* m_target;                 // target vertex of the edge
      };

    private:
    protected:
      VertCont<vertex_generic*> m_vertex;
      EdgeCont<edge_generic*> m_edge;

  };

  template <typename VertProp, typename EdgeProp>
  class graph : public 
                graph_generic<VertProp, EdgeProp, 
                std::set, std::set, std::set> {
    public:
      /////////////////////////////////////////////////////////////////////////
      ///
      typedef typename graph_generic<VertProp, EdgeProp, 
                std::set, std::set, std::set> gen_graph;
      typedef typename gen_graph::iterator vertex_iterator;
      typedef typename gen_graph::iterator edge_iterator;    
      typedef typename gen_graph::iterator adj_iterator;
      typedef typename gen_graph::const_iterator const_vertex_iterator;
      typedef typename gen_graph::const_iterator const_edge_iterator;    
      typedef typename gen_graph::const_iterator const_adj_iterator;

      typedef typename gen_graph::vertex_generic vertex;
      typedef typename gen_graph::edge_generic edge;

     
      graph() {}
      vertex_iterator find_vertex(vertex* _vert) {
        return m_vertex.find(_vert);
      }

      edge_iterator find_edge(edge* _edge) {
        return m_edge.find(_edge);
      }

      const_vertex_iterator find_vertex(vertex* _vert) const {
        return m_vertex.find(_vert);
      }
      
      const_edge_iterator find_edge(edge* _edge) const {
        return m_edge.find(_edge);
      }

      vertex* insert_vertex(const VertProp& _prop) {
        vertex* temp = new vertex(_prop);
        m_vertex.insert(temp);
        return temp;
      }

      edge* insert_edge(vertex* _source, vertex* _target, 
          const EdgeProp& _prop) {
        edge* temp = new edge(_source, _target, _prop);
        m_edge.insert(temp);
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

      }
      
      void erase_edge(edge* _edge) {

      }

      class vertex_generic {
        public:
          virtual edge_generic* add_inedge(edge_generic* _edge) {

          }
          virtual edge_generic* add_outedge(edge_generic* _edge) {

          }
          virtual edge_generic* find(vertex_generic* _vert) {

          } 
          
      };
  }
}

#endif // GRAPH_H
