//
// Implementation
//
template <typename E> // constructor
AVLTree<E>::AVLTree() : ST() {}

template <typename E> // node height utility
int AVLTree<E>::height(const TPos &v) const {
  return (v.isExternal() ? 0 : (*v).height());
}

template <typename E> // set height utility
void AVLTree<E>::setHeight(TPos v) {
  int hl = height(v.left());
  int hr = height(v.right());
  (*v).setHeight(1 + std::max(hl, hr)); // max of left & right
}

template <typename E> // is v balanced?
bool AVLTree<E>::isBalanced(const TPos &v) const {
  int bal = height(v.left()) - height(v.right());
  return ((-1 <= bal) && (bal <= 1));
}

template <typename E> // get tallest grandchild
typename AVLTree<E>::TPos AVLTree<E>::tallGrandchild(const TPos &z) const {
  TPos zl = z.left();
  TPos zr = z.right();
  if (height(zl) >= height(zr)) // left child taller
    if (height(zl.left()) >= height(zl.right()))
      return zl.left();
    else
      return zl.right();
  else // right child taller
    if (height(zr.right()) >= height(zr.left()))
      return zr.right();
    else
      return zr.left();
}

//
// ToDo
//

template <typename E> // remove key k entry
void AVLTree<E>::erase(const K &k) {
  TPos v = this->finder(k, this->root()); // search from virtual root
  if (v.isExternal())                     // not found?
    throw "Erase of nonexistent"; // NonexistentElement("Erase of nonexistent");
  v = this->eraser(v);

  // update height
  while (!v.isRoot()) {
    setHeight(v);
    if (!isBalanced(v)) {
      rebalance(v);
      break;
    }
    v = v.parent();
  }
}

template <typename E> // insert (k,x)
typename AVLTree<E>::Iterator AVLTree<E>::insert(const K &k, const V &x) {
  // insert
  TPos v = this->inserter(k, x);
  Iterator iter(v);
  // update height
  (*v).setHeight(1);
  while (!v.parent().isRoot()) {
    v = v.parent();
    setHeight(v);
    // need rebalance
    if (!isBalanced(v)) {
      rebalance(v);
      break;
    }
  }
  return iter;
}

template <typename E> // rebalancing utility
void AVLTree<E>::rebalance(const TPos &v) {
  TPos u = tallGrandchild(v);
  if (u == v.left().left()) {
    // right rotate
    restructure(v);
  } else if (u == v.left().right()) {
    // left rotate
    restructure(v.left());
    // right rotate
    restructure(v);
  } else if (u == v.right().right()) {
    // left rotate
    restructure(v);
  } else {
    // right rotate
    restructure(v.right());
    // left rotate
    restructure(v);
  }
}

template <typename E> // Binary Search Tree Rotation
typename AVLTree<E>::TPos AVLTree<E>::restructure(const TPos &v) {
  TPos left = v.left();
  TPos right = v.right();
  TPos parent = v.parent();
  if (height(v.left()) > height(v.right())) {
    // right rotate
    if (parent.left() == v) {
      parent.v->left = left.v;
      left.v->par = parent.v;
    } else {
      parent.v->right = left.v;
      left.v->par = parent.v;
    }
    v.v->left = left.v->right;
    left.v->right->par = v.v;

    left.v->right = v.v;
    v.v->par = left.v;
    setHeight(v);
    setHeight(left);
    return left;
  } else {
    // left rotate
    if (parent.left() == v) {
      parent.v->left = right.v;
      right.v->par = parent.v;
    } else {
      parent.v->right = right.v;
      right.v->par = parent.v;
    }
    v.v->right = right.v->left;
    right.v->left->par = v.v;

    right.v->left = v.v;
    v.v->par = right.v;
    setHeight(v);
    setHeight(right);
    return right;
  }
}