#ifndef PADDING_KKBIT_EXECUTOR_HPP
#define PADDING_KKBIT_EXECUTOR_HPP

#include <vector>
#include "definitions.hpp"
#include "sm/pols_generated/commit_pols.hpp"
#include "sm/bits2field/bits2field_executor.hpp"
#if __ZKEVM_SM__
#include "zkevm_sm.h"
#include "zkevm_api.hpp"
#endif

USING_PROVER_FORK_NAMESPACE;

using namespace std;

class PaddingKKBitExecutorInput
{
public:

    /* If connected, this means that this is not the first block of a keccak message to hash,
       and then the state of the previous block is used as initial state of this block */
    bool connected;

    /* Input keccak block data: 136 bytes = 1088 bits */
    uint8_t data[136];

    /* Constructor */
    PaddingKKBitExecutorInput() : connected(false) {};
};

class PaddingKKBitExecutor
{
private:

    /* Goldilocks reference */
    Goldilocks &fr;

    /* Constant values */
    const uint64_t N;
    const uint64_t slotSize;
    const uint64_t nSlots;

public:

    /* Constructor */
    PaddingKKBitExecutor(Goldilocks &fr) :
        fr(fr),
        N(PROVER_FORK_NAMESPACE::PaddingKKBitCommitPols::pilDegree()),
        slotSize(155286),
        nSlots(44*((N-1)/slotSize)) {};

    /* Executor */
    void execute (vector<PaddingKKBitExecutorInput> &input, PROVER_FORK_NAMESPACE::PaddingKKBitCommitPols &pols, vector<Bits2FieldExecutorInput> &required);
    inline void execute(vector<PaddingKKBitExecutorInput> &input, Goldilocks::Element * pAddress,
    void *pRequests){
        PROVER_FORK_NAMESPACE::PaddingKKBitCommitPols pols(pAddress, N);
        vector<Bits2FieldExecutorInput> required;
        execute(input, pols, required);
#ifdef __ZKEVM_SM__
        add_bits_2_field_inputs(pRequests, (void *)required.data(), (uint64_t) required.size());
#endif
    }
    
};

#endif