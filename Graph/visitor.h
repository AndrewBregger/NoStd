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
#ifndef VISITOR_H
#define VISITOR_H

namespace nostd {
  template<typename Visitor_Data>
  class base_visitor {
    public:
      /// @name Constructor
      /// @{
      visitor(Visitor_Data& _vis): m_vis(_vis) {}
      /// @}
        

      /// @name Actions
      /// @{
      template<typename V, typename Graph> 
      void initialize_vertex(V _vert, Graph& _graph) = 0;

      template<typename V, typename Graph> 
      void discover_vertex(V _vert, Graph& _graph) = 0;   

      template<typename V, typename Graph> 
      void examine_vertex(V _vert, Graph& _graph) = 0;

      template<typename E, typename Graph> 
      void examine_edge(E _edge, Graph& _graph) = 0;

      template<typename E, typename Graph> 
      void tree_edge(E _edge, Graph& _graph) = 0;

      template<typename E, typename Graph> 
      void non_tree_edge(E _edge, Graph& _graph) = 0;

      template<typename E, typename Graph> 
      void grey_target(E _edge, Graph& _graph) = 0;

      template<typename E, typename Graph> 
      void black_target(E _edge, Graph& _graph) = 0;

      template<typename V, typename Graph> 
      void finish_vertex(V _vert, Graph& _graph) = 0;

      /// @}
    private:
      Visitor_Data m_vis;
  };

};

#endif
