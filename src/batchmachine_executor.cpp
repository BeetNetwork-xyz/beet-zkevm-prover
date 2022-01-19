
#include <iostream>
#include <fstream>
#include "batchmachine_executor.hpp"
#include "poseidon_opt/poseidon_opt.hpp"
#include "scalar.hpp"
#include "compare_fe.hpp"
#include "utils.hpp"
#include "config.hpp"
#include "merkle_group.hpp"
#include "merkle_group_multipol.hpp"
#include "fft/fft.hpp"

void BatchMachineExecutor::execute (Mem &mem, json &proof)
{
    TimerStart(BME_PROGRAM_EXECUTION);

    Poseidon_opt poseidon;
    Merkle M(MERKLE_ARITY);

    for (uint64_t i = 0; i < script.program.size(); i++)
    {
        Program program = script.program[i];
        if (i%100==0 || i==script.program.size()-1)
        {
            cout << "Program line: " << i << " operation: " << op2string(program.op) << " result: " << program.result << endl;
        }

        switch (program.op)
        {
        case op_field_set:
        {
            zkassert(mem[program.result].type == rt_field);

            mem[program.result].fe = program.value;
#ifdef LOG_BME
            printReference(fr, mem[program.result]);
#endif
            break;
        }
        case op_field_add:
        {
            zkassert(mem[program.result].type == rt_field);
            zkassert(program.values.size() == 2);
            zkassert(mem[program.values[0]].type == rt_field);
            zkassert(mem[program.values[1]].type == rt_field);

            fr.add(mem[program.result].fe, mem[program.values[0]].fe, mem[program.values[1]].fe);

#ifdef LOG_BME
            printReference(fr, mem[program.result]);
#endif
            break;
        }
        case op_field_sub:
        {
            zkassert(mem[program.result].type == rt_field);
            zkassert(program.values.size() == 2);
            zkassert(mem[program.values[0]].type == rt_field);
            zkassert(mem[program.values[1]].type == rt_field);

            fr.sub(mem[program.result].fe, mem[program.values[0]].fe, mem[program.values[1]].fe);

#ifdef LOG_BME
            printReference(fr, mem[program.result]);
#endif
            break;
        }
        case op_field_neg:
        {
            zkassert(mem[program.result].type == rt_field);
            zkassert(program.values.size() == 1);
            zkassert(mem[program.values[0]].type == rt_field);

            fr.neg(mem[program.result].fe, mem[program.values[0]].fe);

#ifdef LOG_BME
            printReference(fr, mem[program.result]);
#endif
            break;
        }
        case op_field_mul:
        {
            zkassert(mem[program.result].type == rt_field);
            zkassert(program.values.size() == 2);
            zkassert(mem[program.values[0]].type == rt_field);
            zkassert(mem[program.values[1]].type == rt_field);

            fr.mul(mem[program.result].fe, mem[program.values[0]].fe, mem[program.values[1]].fe);

#ifdef LOG_BME
            printReference(fr, mem[program.result]);
#endif
            break;
        }
        case op_pol_add:
        {
            zkassert(mem[program.result].type == rt_pol);
            zkassert(program.values.size() == 2);
            zkassert(mem[program.values[0]].type == rt_pol);
            zkassert(mem[program.values[1]].type == rt_pol);
            zkassert(mem[program.result].N == mem[program.values[0]].N);
            zkassert(mem[program.result].N == mem[program.values[1]].N);

            for (uint64_t j = 0; j < program.N; j++)
            {
                fr.add(mem[program.result].pPol[j], mem[program.values[0]].pPol[j], mem[program.values[1]].pPol[j]);
            }

#ifdef LOG_BME
            printReference(fr, mem[program.result]);
#endif
            break;
        }
        case op_pol_sub:
        {
            zkassert(mem[program.result].type == rt_pol);
            zkassert(program.values.size() == 2);
            zkassert(mem[program.values[0]].type == rt_pol);
            zkassert(mem[program.values[1]].type == rt_pol);
            zkassert(mem[program.result].N == mem[program.values[0]].N);
            zkassert(mem[program.result].N == mem[program.values[1]].N);

            for (uint64_t j = 0; j < program.N; j++)
            {
                fr.sub(mem[program.result].pPol[j], mem[program.values[0]].pPol[j], mem[program.values[1]].pPol[j]);
            }

#ifdef LOG_BME
            printReference(fr, mem[program.result]);
#endif
            break;
        }
        case op_pol_neg:
        {
            zkassert(mem[program.result].type == rt_pol);
            zkassert(program.values.size() == 1);
            zkassert(mem[program.values[0]].type == rt_pol);
            zkassert(mem[program.result].N == mem[program.values[0]].N);

            for (uint64_t j = 0; j < program.N; j++)
            {
                fr.neg(mem[program.result].pPol[j], mem[program.values[0]].pPol[j]);
            }

#ifdef LOG_BME
            printReference(fr, mem[program.result]);
#endif
            break;
        }
        case op_pol_mul:
        {
            zkassert(mem[program.result].type == rt_pol);
            zkassert(program.values.size() == 2);
            zkassert(mem[program.values[0]].type == rt_pol);
            zkassert(mem[program.values[1]].type == rt_pol);
            zkassert(mem[program.result].N == mem[program.values[0]].N);
            zkassert(mem[program.result].N == mem[program.values[1]].N);

            for (uint64_t j = 0; j < program.N; j++)
            {
                fr.mul(mem[program.result].pPol[j], mem[program.values[0]].pPol[j], mem[program.values[1]].pPol[j]);
            }
#ifdef LOG_BME
            printReference(fr, mem[program.result]);
#endif
            break;
        }
        case op_pol_addc:
        {
            zkassert(mem[program.result].type == rt_pol);
            zkassert(program.values.size() == 1);
            zkassert(mem[program.values[0]].type == rt_pol);
            zkassert(mem[program.result].N == mem[program.values[0]].N);

            for (uint64_t j = 0; j < program.N; j++)
            {
                fr.add(mem[program.result].pPol[j], mem[program.values[0]].pPol[j], mem[program.constant].fe);
            }

#ifdef LOG_BME
            printReference(fr, mem[program.result]);
#endif
            break;
        }
        case op_pol_mulc:
        {
            zkassert(mem[program.result].type == rt_pol);
            zkassert(program.values.size() == 1);
            zkassert(mem[program.values[0]].type == rt_pol);
            zkassert(mem[program.result].N == mem[program.values[0]].N);

            for (uint64_t j = 0; j < program.N; j++)
            {
                fr.mul(mem[program.result].pPol[j], mem[program.values[0]].pPol[j], mem[program.constant].fe);
            }

#ifdef LOG_BME
            printReference(fr, mem[program.result]);
#endif
            break;
        }
        case op_pol_grandProduct:
        {
            zkassert(mem[program.result].type == rt_pol);
            zkassert(program.values.size() == 1);
            zkassert(mem[program.values[0]].type == rt_pol);
            zkassert(mem[program.result].N == mem[program.values[0]].N);

            mem[program.result].pPol[0] = fr.one();
            for (uint64_t j = 1; j < program.N; j++)
            {
                fr.mul(mem[program.result].pPol[j], mem[program.values[0]].pPol[j - 1], mem[program.result].pPol[j - 1]);
            }

#ifdef LOG_BME
            printReference(fr, mem[program.result]);
#endif
            break;
        }
        case op_pol_batchInverse:
        {
            zkassert(mem[program.result].type == rt_pol);
            zkassert(program.values.size() == 1);
            zkassert(mem[program.values[0]].type == rt_pol);
            zkassert(mem[program.result].N == mem[program.values[0]].N);

            batchInverse(fr, mem[program.values[0]], mem[program.result]);

#ifdef LOG_BME
            printReference(fr, mem[program.result]);
#endif
            break;
        }
        case op_pol_rotate:
        {
            zkassert(mem[program.result].type == rt_pol);
            zkassert(program.values.size() == 1);
            zkassert(mem[program.values[0]].type == rt_pol);
            zkassert(mem[program.result].N == mem[program.values[0]].N);

            for (uint64_t j = 0; j < program.N; j++)
            {
                mem[program.result].pPol[j] = mem[program.values[0]].pPol[(j + program.shift) % program.N];
            }

#ifdef LOG_BME
            printReference(fr, mem[program.result]);
#endif
            break;
        }
        case op_pol_extend:
        {
            uint32_t extendBits = program.extendBits;
            uint32_t length = 1 << NBITS;
            uint32_t extensionLength = (length << extendBits) - length;
            FFT fft(&fr, length);
            FFT fft_extended(&fr, length + (length << extendBits) - length);
            for (uint32_t i = 0; i < program.values.size(); i++)
            {

                RawFr::Element aux[length + extensionLength] = {fr.zero()};
                std::memcpy(&aux, mem[program.values[i]].pPol, mem[program.values[i]].memSize);
                std::memcpy(mem[program.result].pPol, &aux, mem[program.values[i]].memSize + extensionLength * sizeof(RawFr::Element));

                fft.ifft(mem[program.result].pPol, length);

                RawFr::Element r = fr.one();
                RawFr::Element shift;
                fr.fromUI(shift, 25);
                for (uint j = 0; j < length; j++) // TODO: Pre-compute r and parallelize
                {
                    fr.mul(mem[program.result].pPol[j], mem[program.result].pPol[j], r);
                    fr.mul(r, r, shift);
                }

                fft_extended.fft(mem[program.result].pPol, length + extensionLength);
            }
#ifdef LOG_BME
            printReference(fr, mem[program.result]);
#endif
            break;
        }
        case op_pol_getEvaluation:
        {
            zkassert(mem[program.result].type == rt_field);
            zkassert(mem[program.p].type == rt_pol);
            zkassert(program.idx < mem[program.p].N);

            mem[program.result].fe = mem[program.p].pPol[program.idx];

#ifdef LOG_BME
            printReference(fr, mem[program.result]);
#endif
            break;
        }
        case op_treeGroupMultipol_extractPol:
        {
            MerkleGroupMultiPol MGP(&M, program.nGroups, program.groupSize, program.nPols);
            uint32_t N = program.nGroups * program.groupSize;
#pragma omp parallel for
            for (uint32_t j = 0; j < N; j++)
            {
                mem[program.result].pPol[j] = MGP.getElement(mem[program.tree].pTreeGroupMultipol, program.polIdx, j);
            }
#ifdef LOG_BME
            printReference(fr, mem[program.result]);
#endif
            break;
        }
        case op_treeGroupMultipol_merkelize:
        {
            MerkleGroupMultiPol MGP(&M, program.nGroups, program.groupSize, program.nPols);
            vector<vector<RawFr::Element>> pols;
            for (uint32_t j = 0; j < program.nPols; j++)
            {
                std::vector<RawFr::Element> aux((RawFr::Element *)mem[program.pols[j]].pPol, mem[program.pols[j]].pPol + mem[program.pols[j]].N);
                pols.push_back(aux);
            }
            MGP.merkelize((RawFr::Element *)mem[program.result].pTreeGroupMultipol, pols);
#ifdef LOG_BME
            printReference(fr, mem[program.result]);
#endif
            break;
        }
        case op_treeGroupMultipol_root:
        {
            MerkleGroupMultiPol MGP(&M, program.nGroups, program.groupSize, program.nPols);

            // It needs the mainTree pointer, not the tree. TODO: change
            mem[program.result].fe = MGP.root(&mem[program.tree].pTreeGroupMultipol[program.nGroups * (MGP.groupProofSize + program.groupSize * MGP.polsProofSize)]);
#ifdef LOG_BME
            printReference(fr, mem[program.result]);
#endif
            break;
        }
        case op_treeGroupMultipol_getGroupProof:
        {
            MerkleGroupMultiPol MGP(&M, program.nGroups, program.groupSize, program.nPols);
            MGP.getGroupProof(mem[program.tree].pTreeGroupMultipol, mem[program.idx].integer, mem[program.result].pTreeGroupMultipol_groupProof);
#ifdef LOG_BME
            printReference(fr, mem[program.result]);
#endif

            break;
        }
        case op_treeGroup_merkelize:
        {
            MerkleGroup MG(&M, program.nGroups, program.groupSize);
            MG.merkelize(mem[program.result].pTreeGroup, mem[program.pol].pPol);
#ifdef LOG_BME
            printReference(fr, mem[program.result]);
#endif
            break;
        }
        case op_treeGroup_root:
        {
            MerkleGroup MG(&M, program.nGroups, program.groupSize);
            mem[program.result].fe = MG.root(mem[program.tree].pTreeGroup);
#ifdef LOG_BME
            printReference(fr, mem[program.result]);
#endif
            break;
        }
        case op_treeGroup_getElementProof:
        {
            MerkleGroup MG(&M, program.nGroups, program.groupSize);
            MG.getElementsProof(mem[program.tree].pTreeGroup, mem[program.idx].integer, mem[program.result].pTreeGroup_elementProof);
#ifdef LOG_BME
            printReference(fr, mem[program.result]);
#endif
            break;
        }
        case op_treeGroup_getGroupProof:
        {
            MerkleGroup MG(&M, program.nGroups, program.groupSize);
            MG.getGroupProof(mem[program.tree].pTreeGroup, mem[program.idx].integer, mem[program.result].pTreeGroup_groupProof);
#ifdef LOG_BME
            printReference(fr, mem[program.result]);
#endif
            break;
        }
        case op_idxArrayFromFields:
        {
            zkassert(program.fields.size() > 0);
            zkassert(mem[program.result].type == rt_idxArray);

            vector<vector<uint8_t>> fields;
            for (uint64_t j = 0; j < program.fields.size(); j++)
            {
                zkassert(mem[program.fields[j]].type == rt_field);

                mpz_class s;
                fe2scalar(fr, s, mem[program.fields[j]].fe);
                vector<uint8_t> bits;
                scalar2bits(s, bits);
                fields.push_back(bits);
            }

            uint64_t curField = 0;
            uint64_t curBit = 0;
            for (uint64_t i = 0; i < program.n; i++)
            {
                uint32_t a = 0;
                for (uint64_t j = 0; j < program.nBits; j++)
                {
                    if (fields[curField][curBit])
                        a = a + (1 << j);
                    curBit++;
                    if (curBit == 253)
                    {
                        curBit = 0;
                        curField++;
                    }
                }
                mem[program.result].pIdxArray[i] = a;
            }

#ifdef LOG_BME
            printReference(fr, mem[program.result]);
#endif
            break;
        }
        case op_idxArray_get:
        {
            zkassert(mem[program.result].type == rt_int);
            zkassert(mem[program.idxArray].type == rt_idxArray);
            zkassert(program.pos <= mem[program.idxArray].N);

            mem[program.result].integer = mem[program.idxArray].pIdxArray[program.pos];

#ifdef LOG_BME
            printReference(fr, mem[program.result]);
#endif
            break;
        }
        case op_idx_addMod:
        {
            zkassert(mem[program.result].type == rt_int);
            zkassert(mem[program.idx].type == rt_int);

            mem[program.result].integer = (uint32_t)((uint64_t(mem[program.idx].integer) + program.add) % program.mod);

#ifdef LOG_BME
            printReference(fr, mem[program.result]);
#endif
            break;
        }
        case op_calculateH1H2:
        {
            calculateH1H2(mem[program.f], mem[program.t], mem[program.resultH1], mem[program.resultH2]);
#ifdef LOG_BME
            printReference(fr, mem[program.resultH1]);
            printReference(fr, mem[program.resultH2]);
#endif
            break;
        }
        case op_friReduce:
        {
            RawFr::Element acc;
            fr.fromString(acc, program.shiftInv);

            uint32_t nX = 1 << (program.reduceBits);
            uint32_t pol2N = program.N / nX;

            FFT fft(&fr, nX);
            for (uint32_t g = 0; g < pol2N; g++)
            {
                RawFr::Element ppar[nX];
                for (uint32_t i = 0; i < nX; i++)
                {
                    ppar[i] = mem[program.pol].pPol[(i * pol2N) + g];
                }

                fft.ifft(ppar, nX);
                polMulAxi(ppar, nX, fr.one(), acc);
                evalPol(ppar, nX, mem[program.specialX].fe, mem[program.result].pPol[g]);
                fr.mul(acc, acc, program.w);
            }
#ifdef LOG_BME
            printReference(fr, mem[program.result]);
#endif
            break;
        }
        case op_hash:
        {
            zkassert(program.values.size() > 0)
                zkassert(mem[program.result].type == rt_field);

            vector<RawFr::Element> keyV;
            for (uint64_t j = 0; j < program.values.size(); j++)
            {
                zkassert(mem[program.values[j]].type == rt_field);
                keyV.push_back(mem[program.values[j]].fe);
            }
            poseidon.hash(keyV, &mem[program.result].fe);

#ifdef LOG_BME
            printReference(fr, mem[program.result]);
#endif
            break;
        }
        case op_log:
        {
            zkassert(program.msg.size() > 0);
            cout << "BME log: " << program.msg << endl;
            /*      if (typeof(l.refId)!= "undefined") {  TODO: Ask Jordi is we need to support this (no refID occurrences in script)
                    const o = refToObject(F, mem, l.ref);
                    console.log(JSON.stringify(o, null, 1));
                }*/
            break;
        }
        default:
        {
            cerr << "Error: batchMachineExecutor() found unsupported operation: " << program.op << " at program line: " << i << endl;
            exit(-1);
        }
        }
    }

    TimerStopAndLog(BME_PROGRAM_EXECUTION);

    TimerStart(BME_GENERATE_STARK_JSON);
    proof = dereference(mem, script.output);
    TimerStopAndLog(BME_GENERATE_STARK_JSON);
}

json BatchMachineExecutor::dereference (const Mem &mem, const Output &output)
{
    if (output.isArray())
    {
        json j = json::array();
        for (uint64_t i = 0; i < output.array.size(); i++)
        {
            j[i] = dereference(mem, output.array[i]);
        }
        return j;
    }
    else if (output.isObject())
    {
        json j = json::object();
        for (uint64_t i = 0; i < output.objects.size(); i++)
        {
            j[output.objects[i].name] = dereference(mem, output.objects[i]);
        }
        return j;
    }
    else
    {
        return refToObject(mem, output.ref);
    }
}


json BatchMachineExecutor::refToObject (const Mem &mem, const Reference &ref)
{
    zkassert(mem[ref.id].type == ref.type);

    json j;

    switch (ref.type)
    {
    case rt_int:
    {
        j = mem[ref.id].integer;
        break;
    }
    case rt_field:
    {
        j = fr.toString(mem[ref.id].fe);
        break;
    }
    case rt_pol:
    {
        for (uint64_t i = 0; i < ref.N; i++)
        {
            j.push_back(fr.toString(mem[ref.id].pPol[i]));
        }
        break;
    }
    case rt_treeGroup_groupProof:
    {
        /*
           groupProof = [ value , mp ]
        */
        uint64_t size = mem[ref.id].memSize / sizeof(RawFr::Element);

        json value, mp;
        uint64_t i = 0;
        for (; i < mem[ref.id].sizeValue; i++)
        {
            zkassert(i<size);
            value.push_back(fr.toString(mem[ref.id].pTreeGroup_groupProof[i]));
        }
        j.push_back(value);
        for (; i < mem[ref.id].sizeValue + mem[ref.id].sizeMp; i++)
        {
            zkassert(i<size);
            mp.push_back(fr.toString(mem[ref.id].pTreeGroup_groupProof[i]));
        }
        j.push_back(mp);
        break;
    }
    case rt_treeGroup_elementProof:
    {
        uint64_t size = mem[ref.id].memSize / sizeof(RawFr::Element);
        json mp(json::array()), mpL(json::array()), mpH(json::array());
        /*
           elementProof = [ value ,[ mpL , mpH ]]
        */
        uint64_t i = 0;
        for (; i < mem[ref.id].sizeValue; i++)
        {
            zkassert(i<size);
            j.push_back(fr.toString(mem[ref.id].pTreeGroup_elementProof[i])); // NEW
        }

        for (; i < mem[ref.id].sizeValue + mem[ref.id].sizeMpL; )
        {
            json aux;
            for (uint64_t j=0; j<16; j++, i++)
            {
                aux.push_back(fr.toString(mem[ref.id].pTreeGroup_elementProof[i]));
            }
            mpL.push_back(aux);
        }
        mp.push_back(mpL);
        for (; i < mem[ref.id].sizeValue + mem[ref.id].sizeMpL + mem[ref.id].sizeMpH; )
        {
            json aux;
            for (uint64_t j=0; j<16; j++, i++)
            {
                aux.push_back(fr.toString(mem[ref.id].pTreeGroup_elementProof[i]));
            }
            mpH.push_back(aux);
        }
        mp.push_back(mpH);
        j.push_back(mp);
        break;
    }
    case rt_treeGroupMultipol_groupProof:
    {
        /*
           groupProof = [ value , mp ]
        */
        uint64_t size = mem[ref.id].memSize / sizeof(RawFr::Element);
        json value, mp;
        uint64_t i = 0;
        for (; i < mem[ref.id].sizeValue; i++)
        {
            zkassert(i<size);
            value.push_back(fr.toString(mem[ref.id].pTreeGroupMultipol_groupProof[i]));
        }
        j.push_back(value);
        for (; i < mem[ref.id].sizeValue + mem[ref.id].sizeMp; i++)
        {
            zkassert(i<size);
            mp.push_back(fr.toString(mem[ref.id].pTreeGroupMultipol_groupProof[i]));
        }
        j.push_back(mp);
        break;
    }
    case rt_treeGroup:
    case rt_treeGroupMultipol:
    case rt_idxArray:
    default:
        cerr << "Error: refToObject cannot return JSON object of ref.type: " << ref.type << endl;
        exit(-1);
    }
    zkassert(!j.is_null());
    return j;
}


void BatchMachineExecutor::calculateH1H2 (Reference &f, Reference &t, Reference &h1, Reference &h2)
{
    zkassert(t.type == rt_pol);
    zkassert(f.type == rt_pol);
    zkassert(h1.type == rt_pol);
    zkassert(h2.type == rt_pol);
    zkassert(h1.N == f.N);
    zkassert(h2.N == f.N);

#ifdef LOG_BME
    printReference(fr, t);
#endif

    map<RawFr::Element, uint64_t, CompareFe> idx_t;
    multimap<RawFr::Element, uint64_t, CompareFe> s;
    multimap<RawFr::Element, uint64_t>::iterator it;
    uint64_t i = 0;

    for (uint64_t i = 0; i < (uint32_t)t.N; i++)
    {
        idx_t[t.pPol[i]] = i;
        s.insert(pair<RawFr::Element, uint64_t>(t.pPol[i], i));
    }

    for (uint64_t i = 0; i < f.N; i++)
    {
        if (idx_t.find(f.pPol[i]) == idx_t.end())
        {
            cerr << "Error: calculateH1H2() Number not included: " << fr.toString(f.pPol[i], 16) << endl;
            exit(-1);
        }
        uint64_t idx = idx_t[f.pPol[i]];
        s.insert(pair<RawFr::Element, uint64_t>(f.pPol[i], idx));
    }

    multimap<uint64_t, RawFr::Element> s_sorted;
    multimap<uint64_t, RawFr::Element>::iterator it_sorted;

    for (it = s.begin(); it != s.end(); it++)
    {
        s_sorted.insert(make_pair(it->second, it->first));
    }
    for (it_sorted = s_sorted.begin(); it_sorted != s_sorted.end(); it_sorted++, i++)
    {
        if ((i & 1) == 0)
        {
            h1.pPol[i / 2] = it_sorted->second;
        }
        else
        {
            h2.pPol[i / 2] = it_sorted->second;
        }
    }
}


void BatchMachineExecutor::batchInverse (RawFr &fr, Reference &source, Reference &result)
{
    zkassert(source.type == rt_pol);
    zkassert(result.type == rt_pol);
    zkassert(source.N == result.N);
    zkassert(source.N >= 2);

    uint64_t N = source.N;

    // Calculate the products: [a, ab, abc, ... abc..xyz]
    RawFr::Element *pProduct;
    pProduct = (RawFr::Element *)malloc(N * sizeof(RawFr::Element));
    if (pProduct == NULL)
    {
        cerr << "Error: batchInverse() failed calling malloc of bytes: " << N * sizeof(RawFr::Element) << endl;
        exit(-1);
    }
    pProduct[0] = source.pPol[0]; // a
    for (uint64_t i = 1; i < N; i++)
    {
        fr.mul(pProduct[i], pProduct[i - 1], source.pPol[i]);
    }

    // Calculate the inversions: [1/a, 1/ab, 1/abc, ... 1/abc..xyz]
    RawFr::Element *pInvert;
    pInvert = (RawFr::Element *)malloc(N * sizeof(RawFr::Element));
    if (pInvert == NULL)
    {
        cerr << "Error: batchInverse() failed calling malloc of bytes: " << N * sizeof(RawFr::Element) << endl;
        exit(-1);
    }
    fr.inv(pInvert[N - 1], pProduct[N - 1]);
    for (uint64_t i = N - 1; i > 0; i--)
    {
        fr.mul(pInvert[i - 1], pInvert[i], source.pPol[i]);
    }

    // Generate the output
    result.pPol[0] = pInvert[0];
    for (uint64_t i = 1; i < N; i++)
    {
        fr.mul(result.pPol[i], pInvert[i], pProduct[i - 1]);
    }

    // Free memory
    free(pProduct);
    free(pInvert);
}

void BatchMachineExecutor::batchInverseTest (RawFr &fr)

{
    uint64_t N = 1000000;

    Reference source;
    source.type = rt_pol;
    source.N = N;
    source.memSize = source.N * sizeof(RawFr::Element);
    source.pPol = (RawFr::Element *)malloc(source.memSize);
    zkassert(source.pPol != NULL);

    for (uint64_t i = 0; i < source.N; i++)
        fr.fromUI(source.pPol[i], (i + 1) * 10);

    Reference result;
    result.type = rt_pol;
    result.N = N;
    result.memSize = result.N * sizeof(RawFr::Element);
    result.pPol = (RawFr::Element *)malloc(result.memSize);
    zkassert(result.pPol != NULL);

    Reference inverse;
    inverse.type = rt_pol;
    inverse.N = N;
    inverse.memSize = inverse.N * sizeof(RawFr::Element);
    inverse.pPol = (RawFr::Element *)malloc(inverse.memSize);
    zkassert(inverse.pPol != NULL);

    TimerStart(BATCH_INVERSE_TEST_MANUAL);
    for (uint64_t i = 0; i < source.N; i++)
        fr.inv(inverse.pPol[i], source.pPol[i]);
    TimerStopAndLog(BATCH_INVERSE_TEST_MANUAL);

    TimerStart(BATCH_INVERSE_TEST_BATCH);
    BatchMachineExecutor::batchInverse(fr, source, result);
    TimerStopAndLog(BATCH_INVERSE_TEST_BATCH);

    for (uint64_t i = 0; i < source.N; i++)
        zkassert(fr.eq(inverse.pPol[i], result.pPol[i]));

    free(source.pPol);
    free(result.pPol);
    free(inverse.pPol);
}


void BatchMachineExecutor::evalPol (RawFr::Element *pPol, uint64_t polSize, RawFr::Element &x, RawFr::Element &result)
{
    if (polSize == 0)
    {
        result = fr.zero();
        return;
    }
    result = pPol[polSize - 1];

    for (int64_t i = polSize - 2; i >= 0; i--)
    {
        fr.mul(result, result, x);
        fr.add(result, result, pPol[i]);
    }
}

void BatchMachineExecutor::polMulAxi (RawFr::Element *pPol, uint64_t polSize, RawFr::Element &init, RawFr::Element &acc)

{
    RawFr::Element r = init;
    for (uint64_t i = 0; i < polSize; i++)
    {
        fr.mul(pPol[i], pPol[i], r);
        fr.mul(r, r, acc);
    }
}