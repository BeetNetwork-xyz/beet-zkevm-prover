#include "merkleTreeGL.hpp"
#include <cassert>
#include <algorithm> // std::max


MerkleTreeGL::MerkleTreeGL(uint64_t _height, uint64_t _width, Goldilocks::Element *_source) : height(_height), width(_width), source(_source)
{

    if (source == NULL)
    {
        source = (Goldilocks::Element *)calloc(height * width, sizeof(Goldilocks::Element));
        isSourceAllocated = true;
    }
    numNodes = getNumNodes(height);
    nodes = (Goldilocks::Element *)calloc(numNodes, sizeof(Goldilocks::Element));
    isNodesAllocated = true;
};

MerkleTreeGL::MerkleTreeGL(Goldilocks::Element *tree)
{
    width = Goldilocks::toU64(tree[0]);
    height = Goldilocks::toU64(tree[1]);
    source = &tree[2];
    numNodes = getNumNodes(height);
    nodes = &tree[2 + height * width];
    isNodesAllocated = false;
    isSourceAllocated = false;
};

MerkleTreeGL::~MerkleTreeGL()
{
    if (isSourceAllocated)
    {
        free(source);
    }
    if (isNodesAllocated)
    {
        free(nodes);
    }
}

uint64_t MerkleTreeGL::getNumSiblings() 
{
    return (arity - 1) * elementSize;
}

uint64_t MerkleTreeGL::getMerkleTreeWidth() 
{
    return width;
}

uint64_t MerkleTreeGL::getMerkleProofLength() {
    if(height > 1) {
        return (uint64_t)ceil(log10(height) / log10(arity));
    } 
    return 0;
}

uint64_t MerkleTreeGL::getMerkleProofSize() {
    return getMerkleProofLength() * elementSize;
}

uint64_t MerkleTreeGL::getNumNodes(uint64_t height) 
{
    return height * elementSize + (height - 1) * elementSize;
}

void MerkleTreeGL::getRoot(Goldilocks::Element *root)
{
    std::memcpy(root, &nodes[numNodes - elementSize], elementSize * sizeof(Goldilocks::Element));
    zklog.info("MerkleTree root: [ " + Goldilocks::toString(root[0]) + ", " + Goldilocks::toString(root[1]) + ", " + Goldilocks::toString(root[2]) + ", " + Goldilocks::toString(root[3]) + " ]");
}

void MerkleTreeGL::copySource(Goldilocks::Element *_source)
{
    std::memcpy(source, _source, height * width * sizeof(Goldilocks::Element));
}


Goldilocks::Element MerkleTreeGL::getElement(uint64_t idx, uint64_t subIdx)
{
    assert((idx > 0) || (idx < width));
    return source[idx * width + subIdx];
};

void MerkleTreeGL::getGroupProof(Goldilocks::Element *proof, uint64_t idx) {
    assert(idx < height);

    Goldilocks::Element v[width];
#pragma omp parallel for
    for (uint64_t i = 0; i < width; i++) {
        v[i] = getElement(idx, i);
    }
    std::memcpy(proof, &v[0], width * sizeof(Goldilocks::Element));

    Goldilocks::Element *mp = (Goldilocks::Element *)calloc(getMerkleProofSize(), sizeof(Goldilocks::Element));
    genMerkleProof(mp, idx, 0, height);

    std::memcpy(proof + width, mp, getMerkleProofSize() * sizeof(Goldilocks::Element));
    free(mp);
}

void MerkleTreeGL::genMerkleProof(Goldilocks::Element *proof, uint64_t idx, uint64_t offset, uint64_t n)
{
    if (n <= elementSize) return;
    
    uint64_t nextIdx = idx >> 1;
    uint64_t si = (idx ^ 1) * elementSize;

    std::memcpy(proof, &nodes[offset + si], elementSize * sizeof(Goldilocks::Element));

    uint64_t nextN = (std::floor((n - 1) / 8) + 1) * elementSize;
    genMerkleProof(&proof[elementSize], nextIdx, offset + nextN * 2, nextN);
}

void MerkleTreeGL::merkelize()
{
#ifdef __AVX512__
    PoseidonGoldilocks::merkletree_avx512(nodes, source, width, height);
#else
    PoseidonGoldilocks::merkletree_avx(nodes, source, width, height);
#endif
}