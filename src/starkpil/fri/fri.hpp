#ifndef FRI_HPP
#define FRI_HPP

#include "stark_info.hpp"
#include "proof_stark.hpp"
#include <cassert>
#include <vector>
#include "ntt_goldilocks.hpp"
#include "merklehash_goldilocks.hpp"
#include "merkleTreeGL.hpp"
#include "merkleTreeBN128.hpp"

template <typename ElementType>
class FRI
{
public:
    using MerkleTreeType = std::conditional_t<std::is_same<ElementType, Goldilocks::Element>::value, MerkleTreeGL, MerkleTreeBN128>;

    static void fold(uint64_t step, FRIProof<ElementType> &proof, Polinomial &friPol, Polinomial& challenge, StarkInfo starkInfo, MerkleTreeType** treesFRI);
    static void proveQueries(uint64_t* friQueries, FRIProof<ElementType> &fproof, MerkleTreeType **trees, MerkleTreeType **treesFRI, StarkInfo starkInfo);
private:
    static void queryPol(FRIProof<ElementType> &fproof, MerkleTreeType *trees[], uint64_t nTrees, uint64_t idx, uint64_t treeIdx);
    static void queryPol(FRIProof<ElementType> &fproof, MerkleTreeType *tree, uint64_t idx, uint64_t treeIdx);
    static void polMulAxi(Polinomial &pol, Goldilocks::Element init, Goldilocks::Element acc);
    static void evalPol(Polinomial &res, uint64_t res_idx, Polinomial &p, Polinomial &x);
    static void getTransposed(Polinomial &aux, Polinomial &pol, uint64_t trasposeBits);
};

template class FRI<RawFr::Element>;
template class FRI<Goldilocks::Element>;

#endif