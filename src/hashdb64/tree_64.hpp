#ifndef TREE_64_HPP
#define TREE_64_HPP

#include <vector>
#include <map>
#include <gmpxx.h>

#include "poseidon_goldilocks.hpp"
#include "goldilocks_base_field.hpp"
#include "compare_fe.hpp"
#include "database_64.hpp"
#include "database_map.hpp"
#include "zkresult.hpp"
#include "persistence.hpp"
#include "smt_set_result.hpp"
#include "smt_get_result.hpp"
#include "tree_chunk.hpp"
#include "key_value.hpp"
#include "hash_value_gl.hpp"

using namespace std;

/*

A Tree (state) is made of a set of TreeChunks:

      /\
     /__\
        /\
       /__\
          /\
         /__\
        /\ 
       /__\

When we want to read [key, value] for a given root:
    - we call db.read(treeChunk.hash, treeChunk.data) starting from the root until we reach the [key, value] leaf node

When we want to write a new leaf node [key, newValue] on a given root and get the resulting newStateRoot
    - we calculate the new position of [key, newValue], creating new chunks if needed
    - we recalculate the hashes of all the modified and new chunks
    - we call db.write(treeChunk.hash, treeChunk.data) of all the modified and new chunks

Every time we write a [key, newValue] we are potentially creating a new Tree = SUM(TreeChunks) if newValue != oldValue
Every new Tree represents a newer version of the state
Many Trees, or states, coexist in the same Forest, or state history; the forest is stored in the database
Every executor.processBatch() can potentially create several new Trees (states), one per SMT.set()
The Forest takes note of the latest Tree hash to keep track of the current state:

     SR1      SR2      SR3      SR4
     /\       /\       /\       /\
    /__\     /__\     /__\     /__\
                /\       /\       /\
               /__\     /__\     /__\
                           /\       /\
                          /__\     /__\
                                  /\ 
                                 /__\

*/

class Tree64
{
    Goldilocks fr;
    PoseidonGoldilocks poseidon;
public:
    zkresult WriteTree (Database64 &db, const Goldilocks::Element (&oldRoot)[4], const vector<KeyValue> &keyValues, Goldilocks::Element (&newRoot)[4], const bool persistent);
    zkresult CalculateHash (Child &result, std::vector<TreeChunk *> &chunks, vector<DB64Query> &dbQueries, int idChunk, int level, vector<HashValueGL> *hashValues);
    zkresult ReadTree (Database64 &db, const Goldilocks::Element (&root)[4], vector<KeyValue> &keyValues, vector<HashValueGL> *hashValues);
};

extern Tree64 tree64;

#endif