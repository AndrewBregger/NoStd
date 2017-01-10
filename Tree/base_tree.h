///////////////////////////////////////////////////////////////////////////////
/// @name Base Tree
/// @group Tree 
///
/// @note Generic binary tree implementation with no ordering of the nodes.
///       
///
///////////////////////////////////////////////////////////////////////////////
#ifndef BASE_TREE_H
#define BASE_TREE_H

namespace nostd {
  
  template<typename NodeType> 
  class base_tree {
    public:
      class tree_node;
      class iterator;

      /// @name Base Tree Typedefs
      /// @{
        typedef NodeType value_type;
      /// @}

      base_tree() {
        m_root = new tree_node(NodeType());
        m_root->m_right = new tree_node(NodeType());
      }
      
      
      iterator add(tree_node* _parent, const NodeType& _data) {
        auto temp = new tree_node(_data);
        if(_parent->m_left == nullptr)
          _parent->set_left(temp);
        else
          _parent->set_right(temp);
        ++m_count;
        return iterator(temp);
      }
  
      template <typename... T>
      iterator emplace(tree_node* _parent, T... _args) {
        NodeType temp(_args...);
        return add(_parent, temp);
      }
  
      // replaces the removed node to the left child
      iterator remove(tree_node* _node) {
        tree_node* left = _node->m_left;
        tree_node* parent = _node->m_parent;
        if(parent->m_left == _node) {
          parent->m_left = left;
          
        }
        else {
          parent->m_right = left;
       
        }

      }
      
      iterator begin() { return iterator(m_root->m_left); } 
      iterator end() { return iterator(m_root->m_right); }
      
      
      class tree_node {
        public:
          tree_node(const NodeType& _data,
                    tree_node* _left = nullptr, 
                    tree_node* _right = nullptr):
                    m_data(_data), m_left(_left),
                    m_right(_right) {}

          void set_parent(tree_node* _p) { m_parent = _p; }
          
          void set_left(tree_node* _l) { 
            _l->set_parent(this);
            m_left = _l; 
          }

          void set_right(tree_node* _r) { 
            _r->set_parent(this);
            m_right = _r; 
          }

          tree_node* left() { return m_left; }
          tree_node* right() { return m_right; } 
          tree_node* parent() { return m_parent; }

        private:
          NodeType m_data;

          tree_node* m_left;
          tree_node* m_right;

          tree_node* m_parent;

          friend class base_tree;
      };

      class iterator {
        public:
          iterator(tree_node* _node): m_n(_node) {}

          iterator operator++ () {}
          iterator operator++ (int) {}

          iterator operator-- () {}
          iterator operator-- (int) {}

        private:
          tree_node* m_n;
      };

    private:
      tree_node* m_root{nullptr}; //< a super root
      size_t counter{}; //< why not?
  };
}

#endif
