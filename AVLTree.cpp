//AVLTree.cpp

#include "AVLTree.h"
#include <string>
#include "AVLNode.h"
#include <iostream>
using namespace std;

AVLTree::AVLTree() { root = NULL; }

AVLTree::~AVLTree() {
  delete root;
  root = NULL;
}

// insert finds a position for x in the tree and places it there, rebalancing
// as necessary.
void AVLTree::insert(const string& x) {
  if(root == NULL) {
    root = new AVLNode();
    root->value = x;
  }
  else {
    insert(x, root);
  }
}

void AVLTree::insert(const string& x, AVLNode * & curNode) {
  if(curNode == NULL) {
    curNode = new AVLNode();
    curNode->value = x;
  }
  else if (x.compare(curNode->value) < 0)
    insert(x, curNode->left);
  else if (x.compare(curNode->value) > 0)
    insert(x, curNode->right);
  else
    ;
  curNode->height = 1 + max(height(curNode->left), height(curNode->right));
  balance(curNode);
}

// remove finds x's position in the tree and removes it, rebalancing as
// necessary.
void AVLTree::remove(const string& x) { root = remove(root, x); }

// pathTo finds x in the tree and returns a string representing the path it
// took to get there.
string AVLTree::pathTo(const string& x) const {
  if(root == NULL)
    return "";
  else if(find(x))
    return pathTo(x, root);
  else
    return "";
}

string AVLTree::pathTo(const string& x, AVLNode * curNode) const {
  if(curNode == NULL)
    return "";
  else if (x.compare(curNode->value) < 0)
    return curNode->value + " " + pathTo(x, curNode->left);
  else if (x.compare(curNode->value) > 0)
    return curNode->value + " " + pathTo(x, curNode->right);
  else
    return curNode->value;
}

// find determines whether or not x exists in the tree.
bool AVLTree::find(const string& x) const {
  if(root == NULL) {
    return false;
  }
  else {
    return find(x, root);
  }
}

bool AVLTree::find(const string& x, AVLNode * curNode) const {
  if(curNode == NULL)
    return false;
  else if (x.compare(curNode->value) < 0)
    return find(x, curNode->left);
  else if (x.compare(curNode->value) > 0)
    return find(x, curNode->right);
  else
    return true;
}

// numNodes returns the total number of nodes in the tree.
int AVLTree::numNodes() const {
  if(root == NULL)
    return 0;
  else
    return numNodes(root);
}

int AVLTree::numNodes(AVLNode * bn) const {
  int c = 1;
  
  if(bn->right != NULL)
    c += numNodes(bn->right);
  if(bn->left != NULL)
    c += numNodes(bn->left);
  
  return c;
}


// balance makes sure that the subtree with root n maintains the AVL tree
// property, namely that the balance factor of n is either -1, 0, or 1.
void AVLTree::balance(AVLNode*& n) {
  if(balanceFactor(n) == 2) {
    if(balanceFactor(n->right) < 0) {
      n->right = rotateRight(n->right);
    }
    n = rotateLeft(n);
  }
  else if(balanceFactor(n) == -2) {
    if(balanceFactor(n->left) > 0) {
      n->left = rotateLeft(n->left);
    }
    n = rotateRight(n);
  }
}
int AVLTree::balanceFactor(AVLNode * an) {
  if(an == NULL) {
    return 0;
  }
  else {
    return height(an->right)-height(an->left);
  }
}

// rotateLeft performs a single rotation on node n with its right child.
AVLNode* AVLTree::rotateLeft(AVLNode*& n) {
  AVLNode * an = n->right;
  n->right = an->left;
  an->left = n;
  n->height = 1 + max(height(n->left), height(n->right));
  an->height = 1 + max(height(n->left), height(an->right));
  return an;
}

// rotateRight performs a single rotation on node n with its left child.
AVLNode* AVLTree::rotateRight(AVLNode*& n) {
  AVLNode * an = n->left;
  n->left = an->right;
  an->right = n;
  n->height = 1 + max(height(n->left), height(n->right));
  an->height = 1 + max(height(n->left), height(an->right));
  return an;
}

// private helper for remove to allow recursion over different nodes. returns
// an AVLNode* that is assigned to the original node.
AVLNode* AVLTree::remove(AVLNode*& n, const string& x) {
  if (n == NULL) {
    return NULL;
  }
  // first look for x
  if (x == n->value) {
    // found
    // no children
    if (n->left == NULL && n->right == NULL) {
      delete n;
      n = NULL;
      return NULL;
    }
    // single child
    if (n->left == NULL) {
      AVLNode* temp = n->right;
      n->right = NULL;
      delete n;
      n = NULL;
      return temp;
    }
    if (n->right == NULL) {
      AVLNode* temp = n->left;
      n->left = NULL;
      delete n;
      n = NULL;
      return temp;
    }
    // two children -- tree may become unbalanced after deleting n
    string sr = min(n->right);
    n->value = sr;
    n->right = remove(n->right, sr);
  } else if (x < n->value) {
    n->left = remove(n->left, x);
  } else {
    n->right = remove(n->right, x);
  }
  n->height = 1 + max(height(n->left), height(n->right));
  balance(n);
  return n;
}

// min finds the string with the smallest value in a subtree.
string AVLTree::min(AVLNode* node) const {
  // go to bottom-left node
  if (node->left == NULL) {
    return node->value;
  }
  return min(node->left);
}

// height returns the value of the height field in a node. If the node is
// null, it returns -1.
int AVLTree::height(AVLNode* node) const {
  if (node == NULL) {
    return -1;
  }
  return node->height;
}

// max returns the greater of two integers.
int max(int a, int b) {
  if (a > b) {
    return a;
  }
  return b;
}

// Helper function to print branches of the binary tree
void showTrunks(Trunk* p) {
  if (p == NULL) return;
  showTrunks(p->prev);
  cout << p->str;
}

// Recursive function to print binary tree
// It uses inorder traversal
void AVLTree::printTree(AVLNode* root, Trunk* prev, bool isLeft) {
  if (root == NULL) return;

  string prev_str = "    ";
  Trunk* trunk = new Trunk(prev, prev_str);

  printTree(root->left, trunk, true);

  if (!prev)
    trunk->str = "---";
  else if (isLeft) {
    trunk->str = ".---";
    prev_str = "   |";
  } else {
    trunk->str = "`---";
    prev->str = prev_str;
  }

  showTrunks(trunk);
  cout << root->value << endl;

  if (prev) prev->str = prev_str;
  trunk->str = "   |";

  printTree(root->right, trunk, false);
}

void AVLTree::printTree() { printTree(root, NULL, false); }
