

#include <string>
#include <iostream>
#include "proof2zkinStark.hpp"
using namespace std;

ordered_json proof2zkinStark(ordered_json &proof, StarkInfo starkInfo)
{
    uint64_t friSteps = starkInfo.starkStruct.steps.size();
    uint64_t nQueries = starkInfo.starkStruct.nQueries;
    uint64_t nStages = 3; // TODO
    uint64_t nSubAirValues = 0; // TODO
   
    ordered_json zkinOut = ordered_json::object();

    zkinOut["root1"] = proof["root1"];
    for(uint64_t i = 0; i < nStages - 1; i++) {
        uint64_t stage = i + 2;
        zkinOut["root" + to_string(stage)] = proof["root" + to_string(stage)];
    }
    zkinOut["root4"] = proof["root4"];
    zkinOut["evals"] = proof["evals"];

    ordered_json friProof = proof["fri"];
    for (uint64_t i = 1; i < friSteps; i++)
    {
        zkinOut["s" + std::to_string(i) + "_root"] = friProof[i]["root"];
        zkinOut["s" + std::to_string(i) + "_vals"] = ordered_json::array();
        zkinOut["s" + std::to_string(i) + "_siblings"] = ordered_json::array();
        for (uint q = 0; q < nQueries; q++)
        {
            zkinOut["s" + std::to_string(i) + "_vals"][q] = friProof[i]["polQueries"][q][0];
            zkinOut["s" + std::to_string(i) + "_siblings"][q] = friProof[i]["polQueries"][q][1];
        }
    }
  
    zkinOut["s0_valsC"] = ordered_json::array();
    zkinOut["s0_siblingsC"] = ordered_json::array();
  
    zkinOut["s0_vals4"] = ordered_json::array();
    zkinOut["s0_siblings4"] = ordered_json::array();

    for(uint64_t i = 0; i < nStages; ++i) {
        uint64_t stage = i + 1;
        if (friProof[0]["polQueries"][0][i][0].size()) {
            zkinOut["s0_siblings" + to_string(stage)] = ordered_json::array();
            zkinOut["s0_vals" + to_string(stage)] = ordered_json::array();
        }
    }

    for (uint64_t i = 0; i < nQueries; i++) {
        for (uint64_t j = 0; j < nStages; ++j) {
            uint64_t stage = j + 1;
            if (friProof[0]["polQueries"][i][j][0].size()) {
                zkinOut["s0_vals" + to_string(stage)][i] = friProof[0]["polQueries"][i][j][0];
                zkinOut["s0_siblings" + to_string(stage)][i] = friProof[0]["polQueries"][i][j][1];
            }
        }

        zkinOut["s0_vals4"][i] = friProof[0]["polQueries"][i][nStages][0];
        zkinOut["s0_siblings4"][i] = friProof[0]["polQueries"][i][nStages][1];

        zkinOut["s0_valsC"][i] = friProof[0]["polQueries"][i][nStages + 1][0];
        zkinOut["s0_siblingsC"][i] = friProof[0]["polQueries"][i][nStages + 1][1];
    }

    zkinOut["finalPol"] = friProof[friSteps];

    if (nSubAirValues > 0) {
        // zkin.subAirValues = proof.subAirValues;
    }
    
    return zkinOut;
};

ordered_json joinzkin(ordered_json &zkin1, ordered_json &zkin2, ordered_json &verKey, uint64_t steps)
{
    ordered_json zkinOut = ordered_json::object();

    // Load oldStateRoot
    for (int i = 0; i < 8; i++)
    {
        zkinOut["publics"][i] = zkin1["publics"][i];
    }

    // Load oldAccInputHash0
    for (int i = 0; i < 8; i++)
    {
        zkinOut["publics"][i + 8] = zkin1["publics"][8 + i];
    }

    zkinOut["publics"][16] = zkin1["publics"][16]; // oldBatchNum

    zkinOut["publics"][17] = zkin1["publics"][17]; // chainId

    zkinOut["publics"][18] = zkin1["publics"][18]; // forkid

    // newStateRoot
    for (int i = 0; i < 8; i++)
    {
        zkinOut["publics"][19 + i] = zkin2["publics"][19 + i];
    }
    // newAccInputHash0
    for (int i = 0; i < 8; i++)
    {
        zkinOut["publics"][27 + i] = zkin2["publics"][27 + i];
    }
    // newLocalExitRoot
    for (int i = 0; i < 8; i++)
    {
        zkinOut["publics"][35 + i] = zkin2["publics"][35 + i];
    }

    zkinOut["publics"][43] = zkin2["publics"][43]; // oldBatchNum

    zkinOut["a_publics"] = zkin1["publics"];
    zkinOut["a_root1"] = zkin1["root1"];
    zkinOut["a_root2"] = zkin1["root2"];
    zkinOut["a_root3"] = zkin1["root3"];
    zkinOut["a_root4"] = zkin1["root4"];
    zkinOut["a_evals"] = zkin1["evals"];
    zkinOut["a_s0_vals1"] = zkin1["s0_vals1"];
    zkinOut["a_s0_vals3"] = zkin1["s0_vals3"];
    zkinOut["a_s0_vals4"] = zkin1["s0_vals4"];
    zkinOut["a_s0_valsC"] = zkin1["s0_valsC"];
    zkinOut["a_s0_siblings1"] = zkin1["s0_siblings1"];
    zkinOut["a_s0_siblings3"] = zkin1["s0_siblings3"];
    zkinOut["a_s0_siblings4"] = zkin1["s0_siblings4"];
    zkinOut["a_s0_siblingsC"] = zkin1["s0_siblingsC"];
    for (uint64_t i = 1; i < steps; i++)
    {
        zkinOut["a_s" + std::to_string(i) + "_root"] = zkin1["s" + std::to_string(i) + "_root"];
        zkinOut["a_s" + std::to_string(i) + "_siblings"] = zkin1["s" + std::to_string(i) + "_siblings"];
        zkinOut["a_s" + std::to_string(i) + "_vals"] = zkin1["s" + std::to_string(i) + "_vals"];
    }
    zkinOut["a_finalPol"] = zkin1["finalPol"];

    zkinOut["b_publics"] = zkin2["publics"];
    zkinOut["b_root1"] = zkin2["root1"];
    zkinOut["b_root2"] = zkin2["root2"];
    zkinOut["b_root3"] = zkin2["root3"];
    zkinOut["b_root4"] = zkin2["root4"];
    zkinOut["b_evals"] = zkin2["evals"];
    zkinOut["b_s0_vals1"] = zkin2["s0_vals1"];
    zkinOut["b_s0_vals3"] = zkin2["s0_vals3"];
    zkinOut["b_s0_vals4"] = zkin2["s0_vals4"];
    zkinOut["b_s0_valsC"] = zkin2["s0_valsC"];
    zkinOut["b_s0_siblings1"] = zkin2["s0_siblings1"];
    zkinOut["b_s0_siblings3"] = zkin2["s0_siblings3"];
    zkinOut["b_s0_siblings4"] = zkin2["s0_siblings4"];
    zkinOut["b_s0_siblingsC"] = zkin2["s0_siblingsC"];
    for (uint64_t i = 1; i < steps; i++)
    {
        zkinOut["b_s" + std::to_string(i) + "_root"] = zkin2["s" + std::to_string(i) + "_root"];
        zkinOut["b_s" + std::to_string(i) + "_siblings"] = zkin2["s" + std::to_string(i) + "_siblings"];
        zkinOut["b_s" + std::to_string(i) + "_vals"] = zkin2["s" + std::to_string(i) + "_vals"];
    }
    zkinOut["b_finalPol"] = zkin2["finalPol"];

    zkinOut["rootC"] = ordered_json::array();
    for (int i = 0; i < 4; i++)
    {
        zkinOut["rootC"][i] = to_string(verKey["constRoot"][i]);
    }

    return zkinOut;
}