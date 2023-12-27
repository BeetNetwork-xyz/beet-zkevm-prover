#ifndef MAIN_EXECUTOR_HPP_fork_1
#define MAIN_EXECUTOR_HPP_fork_1

#include <string>
#include <nlohmann/json.hpp>
#include <pthread.h>
#include <semaphore.h>
#include "config.hpp"
#include "main_sm/fork_1/main/rom.hpp"
#include "main_sm/fork_1/main/context.hpp"
#include "main_sm/fork_1/pols_generated/commit_pols.hpp"
#include "main_sm/fork_1/main/main_exec_required.hpp"
#include "scalar.hpp"
#include "hashdb_factory.hpp"
#include "poseidon_goldilocks.hpp"
#include "counters.hpp"
#include "goldilocks_base_field.hpp"
#include "prover_request.hpp"

using namespace std;
using json = nlohmann::json;

namespace fork_1
{

class MainExecutor {
public:

    // Finite field data
    Goldilocks &fr; // Finite field reference

    // RawFec instance
    RawFec fec;

    // RawFnec instance
    RawFnec fnec;

    // Number of evaluations, i.e. polynomials degree
    const uint64_t N;

    // Number of evaluations when counters are not used
    const uint64_t N_NoCounters;

    // Poseidon instance
    PoseidonGoldilocks &poseidon;

    // ROM JSON file data:
    Rom rom;

    // Database server configuration, if any
    const Config &config;

    // ROM labels
    uint64_t finalizeExecutionLabel;
    uint64_t checkAndSaveFromLabel;

    // HashDB
    HashDBInterface *pHashDB;

    // When we reach this zkPC, state root (SR) will be consolidated (from virtual to real state root)
    const uint64_t consolidateStateRootZKPC = 4835;

    // Constructor
    MainExecutor(Goldilocks &fr, PoseidonGoldilocks &poseidon, const Config &config, int mpiRank_ = 0);


    // Destructor
    ~MainExecutor();

    void execute (ProverRequest &proverRequest, MainCommitPols &cmPols, MainExecRequired &required);

    // Initial and final evaluations/state management
    void initState(Context &ctx);
    void checkFinalState(Context &ctx);
    void assertOutputs(Context &ctx);
    void logError(Context &ctx, const string &message = "");
    void linearPoseidon(Context &ctx, const vector<uint8_t> &data, Goldilocks::Element (&result)[4]);

private:
    // Flush
    pthread_mutex_t flushMutex; // Mutex to protect the flush threads queue
    void flushLock(void) { pthread_mutex_lock(&flushMutex); };
    void flushUnlock(void) { pthread_mutex_unlock(&flushMutex); };
    vector<pthread_t> flushQueue;
    int mpiRank;
};

} // namespace

#endif