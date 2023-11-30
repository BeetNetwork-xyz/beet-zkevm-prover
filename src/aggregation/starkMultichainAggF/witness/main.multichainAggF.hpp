#ifndef ZKEVM_VERIFIER_MAIN_MULTICHAIN_AGG_F_HPP
#define ZKEVM_VERIFIER_MAIN_MULTICHAIN_AGG_F_HPP

#include <string>
#include <nlohmann/json.hpp>
#include "calcwit.multichainAggF.hpp"
#include "circom.multichainAggF.hpp"
#include "fr_goldilocks.hpp"

#include "timer.hpp"
#include <iostream>
#include <unistd.h>
#include "commit_pols_starks.hpp"
using namespace std;

namespace CircomMultichainAggF
{
    using json = nlohmann::json;
    Circom_Circuit *loadCircuit(std::string const &datFileName);
    void freeCircuit(Circom_Circuit *circuit);
    void loadJson(Circom_CalcWit *ctx, std::string filename);
    void loadJsonImpl(Circom_CalcWit *ctx, json &j);
    void writeBinWitness(Circom_CalcWit *ctx, std::string wtnsFileName);
    void getBinWitness(Circom_CalcWit *ctx, FrGElement *&pWitness, uint64_t &witnessSize);
    void getCommitedPols(CommitPolsStarks *commitPols, const std::string multichainAggFVerifier, const std::string execFile, nlohmann::json &zkin, uint64_t N, uint64_t nCols);
    bool check_valid_number(std::string &s, uint base);

}
#endif
