#include <cstdio>
#include <cassert>
#include <utility>
#include <algorithm>

using namespace std;

struct BSTNode;

struct Node {
  Node * parent;
  Node * left;
  Node * right;

  Node * link;

  size_t size;
  BSTNode * bst;

  Node(BSTNode * bst)
    : parent(NULL)
    , left(NULL)
    , right(NULL)
    , link(NULL)
    , size(1)
    , bst(bst)
  {}
};

size_t size(Node * node) {
  return node == NULL ? 0 : node -> size;
}

void set_parent(Node * node, Node * parent) {
  if (node != NULL) {
    node -> parent = parent;
  }
}

void fix(Node * node) {
  set_parent(node -> left, node);  
  set_parent(node -> right, node);
  node -> size = size(node -> left) + size(node -> right) + 1;
}

Node * rotate(Node * child) {
  Node * parent = child -> parent;
  assert(parent != NULL);
  Node * gparent = parent -> parent;
  if (gparent != NULL) {
    if (gparent -> left == parent) {
      gparent -> left = child;
    } else {
      gparent -> right = child;
    }
  }

  if (parent -> left == child) {
    Node * right = child -> right;
    child -> right = parent;
    parent -> left = right;
  } else { // parent -> right == child
    Node * left = child -> left;
    child -> left = parent;
    parent -> right = left;
  }
  fix(parent);
  fix(child);
  child -> parent = gparent;

  return child;
}

Node * splay(Node * v) {
  if (v -> parent == NULL) {
    return v;
  }
  Node * parent  = v -> parent;
  Node * gparent = parent -> parent;
  if (gparent == NULL) {
    return rotate(v);
  }
  bool zigzig = (gparent -> left == parent) == (parent -> left == v);
  if (zigzig) {
    rotate(parent);
    rotate(v);
  } else {
    rotate(v);
    rotate(v);
  }
  return splay(v);
}

Node * find(size_t kth, Node * v) {
  assert(v != NULL);  // <- To choose or not to choose
  size_t left_size = size(v -> left); 
  if (kth < left_size) {
    return find(kth, v -> left);    
  } else if (kth == left_size) {
    return splay(v);
  } else {
    return find(kth - left_size - 1, v -> right); 
  }
}

Node * merge(Node * left, Node * right) {
  if (left == NULL)  { return right; }
  if (right == NULL) { return left; }
  right = find(0, right);
  right -> left = left;
  fix(right);
  return right;
}

pair<Node *, Node *> split(size_t left_size, Node * v) {
  assert(left_size <= size(v));
  if (v == NULL) {
    return make_pair(NULL, NULL);
  }
  if (left_size == v -> size) {
    return make_pair(v, NULL);
  }
  v = find(left_size, v);
  Node * left = v -> left;
  set_parent(left, NULL);
  v -> left = NULL;
  fix(v);
  return make_pair(left, v);
}

struct BSTNode {
  int key;
  BSTNode * left;
  BSTNode * right;
  
  Node * node;

  BSTNode(int key) 
    : key(key)
    , left(NULL)
    , right(NULL)
    , node(new Node(this))
  {}
};

Node * disconnect(Node * v) {
  splay(v);
  pair< Node *, Node * > result = split(size(v -> left) + 1, v);
  Node * right = find(0, result.second);
  right -> link = v;
  return result.first;
}

Node * find_root(Node * v) {
  v = disconnect(v);
  for (v = find(0, v); v -> link != NULL; v = find(0, v)) {
    Node * link = v -> link;
    v -> link = NULL;       
    v = merge(disconnect(v -> link), v);
  }
  return v;
}

Node * link(Node * child, Node * parent) {
  child -> link = parent;
  return find_root(child);
}

void cut(Node * child, Node * parent) {
  find_root(parent);
  child -> link = NULL;
// ? splay(child);
}

struct BST {
  map< int, BSTNode * > map_;
  typedef map< int, BSTNode* > :: iterator mit;

  bool find(int key, int & cost) {
    bool found = map_.find(key) != map_.end();
    if (found) {
      cost = height(map_[key]);
    } else {
      mit it_l = map_.lower_bound(key);
      cost = -1;
      if (it_l != map_.end()) {
        cost = max(cost, height(it_l -> second));
      }
      if (it_l != map_.begin()) {
        cost = max(cost, height(--it_l -> second));
      }
    }
    return found;
  }

  bool add(int key, int & cost) {
    bool found = map_.find(key) != map_.end();
    if (found) {
      cost = height(map_[key]);
    } else {
     /* mit it_l = map_.lower_bound(key);
      cost = -1;
      if (it_l != map_.end()) {
        cost = max(cost, height(it_l -> second));
      }
      if (it_l != map_.begin()) {
        cost = max(cost, height(--it_l -> second));
      } TODO TADAM */
    } 
       
  }  
  
  int height(BSTNode * bnode) {
    Node * node = bnode -> node;
    node = find_root(node);
    return (int)node -> size;
  }

};
