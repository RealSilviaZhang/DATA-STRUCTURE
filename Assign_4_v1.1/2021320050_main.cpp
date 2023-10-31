/**
 * Assignment 4 for COSE213 Data Structures
 *
 * Won-Ki Jeong (wkjeong@korea.ac.kr)
 *
 * 2022. 5. 22
 *
 */

#include <time.h> /* clock_t, clock, CLOCKS_PER_SEC */
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "AVLTree.h"
#include "LinkedBinaryTree.h"
#include "SearchTree.h"

using namespace std;

int main() {
  typedef Entry<int, float> EntryType;
  std::srand(std::time(0));  // use current time as seed for random generator
  // basic Test
  {
    SearchTree<EntryType> st;
    AVLTree<EntryType> avl;
    int N = 50;
    int *key = new int[N];
    float *val = new float[N];
    for (int i = 0; i < N; i++) {
      key[i] = std::rand();
      val[i] = (float)std::rand() / RAND_MAX * 20000;
    }
    //
    // Search Tree Test
    //
    std::cout << "Search Tree Basic Test" << std::endl;
    if (!st.empty()) {
      printf("Should be empty.\n");
    }

    for (int i = 0; i < N; i++) {
      st.insert(key[i], val[i]);
    }

    if (st.empty()) {
      printf("Shouldn't be empty.\n");
    }

    if (st.size() != N) {
      printf("The size should be %d but got %d\n", N, st.size());
    }

    for (int i = 0; i < 0.1 * N; i++) {
      st.erase(key[i * 10]);
    }

    for (auto iter = st.begin(); iter != st.end(); ++iter) {
      std::cout << "key :" << (*iter).key() << ", value :" << (*iter).value() << std::endl;
    }

    if (st.size() != 0.9 * N) {
      printf("After delete, the size should be %d but got %d\n", int(0.9 * N), st.size());
    }

    for (int i = 0; i < N; i++) {
      auto iter = st.find(key[i]);
      if (i % 10 != 0) {
        if (iter == st.end()) {
          printf("ERROR: Key [%d] %d should exist.\n", i, key[i]);
        } else if ((*iter).value() != val[i]) {
          printf("ERROR: key [%d] %d, got %f, expected %f.\n", i, key[i], (*iter).value(), val[i]);
        }
      } else {
        if (iter != st.end()) {
          printf("Key [%d] %d shouldn't exist.\n", i, key[i]);
        }
      }
    }
    //
    // AVL Tree Test
    //
    std::cout << "AVL Tree Basic Test" << std::endl;
    if (!avl.empty()) {
      printf("Should be empty.\n");
    }

    for (int i = 0; i < N; i++) {
      avl.insert(key[i], val[i]);
    }

    if (avl.empty()) {
      printf("Shouldn't be empty.\n");
    }

    if (avl.size() != N) {
      printf("The size should be N but got %d\n", avl.size());
    }

    for (int i = 0; i < 0.1 * N; i++) {
      avl.erase(key[i * 10]);
    }

    for (auto iter = avl.begin(); iter != avl.end(); ++iter) {
      std::cout << "key :" << (*iter).key() << ", value :" << (*iter).value() << std::endl;
    }

    if (avl.size() != 0.9 * N) {
      printf("After delete, the size should be 900 but got %d\n", avl.size());
    }

    for (int i = 0; i < N; i++) {
      auto iter = avl.find(key[i]);
      if (i % 10 != 0) {
        if (iter == avl.end()) {
          printf("ERROR: Key [%d] %d should exist.\n", i, key[i]);
        } else if ((*iter).value() != val[i]) {
          printf("ERROR: key [%d] %d, got %f, expected %f.\n", i, key[i], (*iter).value(), val[i]);
        }
      } else {
        if (iter != avl.end()) {
          printf("Key %d shouldn't exist.\n", key[i]);
        }
      }
    }
    delete[] key;
    delete[] val;
  }

  // performance test
  for (int flag = 0; flag <= 1; flag++) {
    if (flag == 0) {
      std::cout << "======== random test =========" << std::endl;
    } else {
      std::cout << "======== skewed test =========" << std::endl;
    }
    int max = 1000000;
    if (flag == 1) {
      max /= 10;
    }
    for (int nElem = 1000; nElem <= max; nElem *= 10) {
      SearchTree<EntryType> st;
      AVLTree<EntryType> avl;
      std::cout << "Elements num: " << nElem << std::endl;
      int *key = new int[nElem * 2];
      float *val = new float[nElem * 2];

      // initialize
      for (int i = 0; i < nElem * 2; i++) {
        if (flag == 0) {
          key[i] = std::rand();
        } else {
          key[i] = i;
        }
        val[i] = (float)std::rand() / RAND_MAX * 20000;
      }

      //
      // Search Tree Insert test
      //
      clock_t tm;
      tm = clock();
      for (int i = 0; i < nElem; i++) {
        st.insert(key[i], val[i]);
      }
      tm = clock() - tm;
      printf("Search Tree Insert Test takes %f seconds.\n", ((float)tm) / (float)CLOCKS_PER_SEC);

      //
      // Search Tree Find test
      //
      tm = clock();
      for (int i = nElem; i < nElem * 2; i++) {
        auto iter = st.find(key[i]);
        if (i < nElem) {
          if ((*iter).value() != val[i]) {
            printf("ERROR: key [%d] %d, got %f, expected %f.\n", i, key[i], (*iter).value(),
                   val[i]);
          }
        }
      }
      tm = clock() - tm;
      printf("Search Tree Find Test takes %f seconds.\n", ((float)tm) / (float)CLOCKS_PER_SEC);

      //
      // AVL tree Insert test
      //
      tm = clock();
      for (int i = 0; i < nElem; i++) {
        avl.insert(key[i], val[i]);
      }
      tm = clock() - tm;
      printf("AVL tree Insert test takes %f seconds.\n", ((float)tm) / (float)CLOCKS_PER_SEC);
      //
      // AVL tree Find test
      //
      tm = clock();
      for (int i = nElem; i < nElem * 2; i++) {
        auto iter = avl.find(key[i]);
        if (i < nElem) {
          if ((*iter).value() != val[i]) {
            printf("ERROR: key [%d] %d, got %f, expected %f.\n", i, key[i], (*iter).value(),
                   val[i]);
          }
        }
      }
      tm = clock() - tm;
      printf("AVL tree Find test takes %f seconds.\n", ((float)tm) / (float)CLOCKS_PER_SEC);
      delete[] key;
      delete[] val;
    }
  }
  return 0;
}