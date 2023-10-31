template <typename E> // constructor
SearchTree<E>::SearchTree() : T(), n(0) {
  T.addRoot();
  T.expandExternal(T.root());
} // create the super root

template <typename E> // get virtual root
typename SearchTree<E>::TPos SearchTree<E>::root() const {
  return T.root().left();
} // left child of super root

template <typename E> // iterator to first entry
typename SearchTree<E>::Iterator SearchTree<E>::begin() {
  TPos v = root(); // start at virtual root
  while (v.isInternal())
    v = v.left(); // find leftmost node
  return Iterator(v.parent());
}

template <typename E> // iterator to end entry
typename SearchTree<E>::Iterator SearchTree<E>::end() {
  return Iterator(T.root());
} // return the super root

template <typename E>                   // remove key k entry
void SearchTree<E>::erase(const K &k) { // throw(NonexistentElement) {
  TPos v = finder(k, root());           // search from virtual root
  if (v.isExternal())                   // not found?
    throw "Erase of nonexistent"; // NonexistentElement("Erase of nonexistent");
  eraser(v);                      // remove it
}

template <typename E> // erase entry at p
void SearchTree<E>::erase(const Iterator &p) {
  eraser(p.v);
}

template <typename E> // find entry with key k
typename SearchTree<E>::Iterator SearchTree<E>::find(const K &k) {
  TPos v = finder(k, root()); // search from virtual root
  if (v.isInternal())
    return Iterator(v); // found it
  else
    return end(); // didn't find it
}

template <typename E> // insert (k,x)
typename SearchTree<E>::Iterator SearchTree<E>::insert(const K &k, const V &x) {
  TPos v = inserter(k, x);
  return Iterator(v);
}

template <typename E> int SearchTree<E>::size() const { return n; }

//
// ToDo
//

template <typename E> bool SearchTree<E>::empty() const { return n == 0; }


template <typename E> // inorder successor
typename SearchTree<E>::Iterator &SearchTree<E>::Iterator::operator++() {
  TPos parent = v.parent();

  if (v.left().isInternal() || v.right().isInternal()) {
    // taversal of right subtree
    parent = v;
    v = v.right();
    if (v.isExternal()) {
      // no right subtree
      while (!v.isRoot()) {
        if (v == parent.left()) {
          v = parent;
          break;
        } else {
          v = parent;
          parent = parent.parent();
        }
      }
    } else {
      while (v.isInternal()) {
        v = v.left();
      }
      v = v.parent();
    }
  } else {
    if (v == parent.left()) {
      v = parent;
    } else {
      v = parent;
      parent = parent.parent();
      while (!v.isRoot()) {
        if (v == parent.left()) {
          v = parent;
          break;
        } else {
          v = parent;
          parent = parent.parent();
        }
      }
    }
  }
  return *this;
}

template <typename E> // remove utility
typename SearchTree<E>::TPos SearchTree<E>::eraser(TPos &v) {
  TPos ret;
  if (v.left().isExternal() || v.right().isExternal()) {
    // one of child is external node
    if (v.left().isInternal()) {
      ret = v.left();
      T.removeAboveExternal(v.right());
    } else if (v.right().isInternal()) {
      ret = v.right();
      T.removeAboveExternal(v.left());
    } else {
      ret = v.parent();
      T.removeAboveExternal(v.left());
    }
  } else {
    TPos pos = v.right();
    while (pos.isInternal()) {
      pos = pos.left();
    }
    pos = pos.parent();
    (*v).setKey((*pos).key());
    (*v).setValue((*pos).value());
    T.removeAboveExternal(pos.left());
    ret = v;
  }
  n--;
  return ret;
}

template <typename E> // find utility
typename SearchTree<E>::TPos SearchTree<E>::finder(const K &k, const TPos &v) {
  TPos pos = v;
  while (k != (*pos).key() && pos.isInternal()) {
    if (k < (*pos).key()) {
      pos = pos.left();
    } else {
      pos = pos.right();
    }
  }
  if (k == (*pos).key()) {
    return pos;
  }
  return T.root().right();
}

template <typename E> // insert utility
typename SearchTree<E>::TPos SearchTree<E>::inserter(const K &k, const V &x) {
  // empty tree
  if (root().isExternal()) {
    T.expandExternal(root());
    (*root()).setKey(k);
    (*root()).setValue(x);
    n++;
    return root();
  }
  // find the position
  TPos v = root();
  while (v.isInternal()) {
    if (k == (*v).key()) {
      (*v).setValue(x); // update value
      return v;
    }
    if (k < (*v).key()) {
      v = v.left();
    } else {
      v = v.right();
    }
  }

  TPos res;
  v = v.parent();
  // insert
  if (k < (*v).key()) {
    T.expandExternal(v.left());
    (*v.left()).setKey(k);
    (*v.left()).setValue(x);
    res = v.left();
  } else {
    T.expandExternal(v.right());
    (*v.right()).setKey(k);
    (*v.right()).setValue(x);
    res = v.right();
  }
  n++;
  return res;
}