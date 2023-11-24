#include <stdio.h>
#include "starks.hpp"
#include "proof2zkinStark.hpp"
#include "allSteps.hpp"

int main()
{

    Config config;
    config.runFileGenBatchProof = true; // So that starkInfo is created
    config.mapConstPolsFile = false;
    config.mapConstantsTreeFile = false;
    
    string constPols = "test/prover/all.const";
    string constTree = "test/prover/all.consttree";
    string starkInfoFile = "test/prover/all.starkinfo.json";
    string commitPols = "test/prover/all.commit";
    string verkey = "test/prover/all.verkey.json";

    StarkInfo starkInfo(config, starkInfoFile);

    uint64_t polBits = starkInfo.starkStruct.steps[starkInfo.starkStruct.steps.size() - 1].nBits;
    FRIProof fproof((1 << polBits), FIELD_EXTENSION, starkInfo.starkStruct.steps.size(), starkInfo.evMap.size(), starkInfo.nPublics);

    void *pCommit = copyFile(commitPols, starkInfo.nCommitments * sizeof(Goldilocks::Element) * (1 << starkInfo.starkStruct.nBits));
    void *pAddress = (void *)calloc(starkInfo.mapTotalN + (starkInfo.mapSectionsN.section[eSection::cm1_n] * (1 << starkInfo.starkStruct.nBits) * FIELD_EXTENSION ), sizeof(uint64_t));

    Starks starks(config, {constPols, config.mapConstPolsFile, constTree, starkInfoFile}, pAddress);

    starks.nrowsStepBatch = 4;

    uint64_t N = (1 << starkInfo.starkStruct.nBits);
    #pragma omp parallel for
    for (uint64_t i = 0; i < N; i += 1)
    {
        std::memcpy((uint8_t*)pAddress + i*starkInfo.nCm1*sizeof(Goldilocks::Element), (uint8_t*)pCommit + i*starkInfo.nCommitments*sizeof(Goldilocks::Element), starkInfo.nCommitments*sizeof(Goldilocks::Element));
    }

    Goldilocks::Element publicInputs[3] = {
        Goldilocks::fromU64(1),
        Goldilocks::fromU64(2),
        Goldilocks::fromU64(74469561660084004),
    };

    json publicStarkJson;
    for (int i = 0; i < 3; i++)
    {
        publicStarkJson[i] = Goldilocks::toString(publicInputs[i]);
    }
    AllSteps allSteps;

    json allVerkeyJson;
    file2json(verkey, allVerkeyJson);
    Goldilocks::Element allVerkey[4];
    allVerkey[0] = Goldilocks::fromU64(allVerkeyJson["constRoot"][0]);
    allVerkey[1] = Goldilocks::fromU64(allVerkeyJson["constRoot"][1]);
    allVerkey[2] = Goldilocks::fromU64(allVerkeyJson["constRoot"][2]);
    allVerkey[3] = Goldilocks::fromU64(allVerkeyJson["constRoot"][3]);

    starks.genProof(fproof, &publicInputs[0], allVerkey, &allSteps);

    nlohmann::ordered_json jProof = fproof.proofs.proof2json();
    nlohmann::json zkin = proof2zkinStark(jProof);
    // Generate publics
    jProof["publics"] = publicStarkJson;
    zkin["publics"] = publicStarkJson;

    json2file(publicStarkJson, "runtime/output/publics.json");
    json2file(zkin, "runtime/output/zkin.json");
    json2file(jProof, "runtime/output/jProof.json");

    return 0;
}