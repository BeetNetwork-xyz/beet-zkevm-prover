#include "goldilocks_cubic_extension.hpp"
#include "zhInv.hpp"
#include "starks.hpp"
#include "constant_pols_starks.hpp"
#include "allSteps.hpp"
#include "all.chelpers.step1.parser.hpp"
#include <immintrin.h>

void AllSteps::step1_parser_first_avx(StepsParams &params, uint64_t nrows, uint64_t nrowsBatch)
{
#pragma omp parallel for
     for (uint64_t i = 0; i < nrows; i += nrowsBatch)
     {
          int i_args = 0;
          //__m256i *tmp1 = new __m256i[NTEMP1_];
          __m256i tmp1[NTEMP1_];
          uint64_t offsets1[4], offsets2[4];
          uint64_t numpols = params.pConstPols->numPols();

          for (int kk = 0; kk < NOPS_; ++kk)
          {
               switch (op1[kk])
               {
               case 0:
               {
                    Goldilocks::add_avx(tmp1[(args1[i_args])], tmp1[args1[i_args + 1]], tmp1[args1[i_args + 2]]);
                    i_args += 3;
                    break;
               }
               case 1:
               {
                    Goldilocks::add_avx(tmp1[(args1[i_args])], tmp1[args1[i_args + 1]], &params.pols[args1[i_args + 2] + i * args1[i_args + 3]], args1[i_args + 3]);
                    i_args += 4;
                    break;
               }
               case 2:
               {
                    Goldilocks::add_avx(tmp1[(args1[i_args])], tmp1[args1[i_args + 1]], Goldilocks::fromU64(args1[i_args + 2]));
                    i_args += 3;
                    break;
               }
               case 3:
               {
                    Goldilocks::add_avx(tmp1[(args1[i_args])], tmp1[args1[i_args + 1]], &params.pConstPols->getElement(args1[i_args + 2], i), numpols);
                    i_args += 3;
                    break;
               }
               case 4:
               {
                    Goldilocks::add_avx(tmp1[(args1[i_args])], &params.pols[args1[i_args + 1] + i * args1[i_args + 2]], &params.pols[args1[i_args + 3] + i * args1[i_args + 4]], args1[i_args + 2], args1[i_args + 4]);
                    i_args += 5;
                    break;
               }
               case 5:
               {
                    for (uint64_t j = 0; j < AVX_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args + 1] + (((i + j) + args1[i_args + 2]) % args1[i_args + 3]) * args1[i_args + 4];
                         offsets2[j] = args1[i_args + 5] + (((i + j) + args1[i_args + 6]) % args1[i_args + 7]) * args1[i_args + 8];
                    }
                    Goldilocks::add_avx(tmp1[(args1[i_args])], &params.pols[0], &params.pols[0], offsets1, offsets2);
                    i_args += 9;
                    break;
               }
               case 6:
               {
                    Goldilocks::add_avx(tmp1[(args1[i_args])], &params.pols[args1[i_args + 1] + i * args1[i_args + 2]], &params.pConstPols->getElement(args1[i_args + 3], i), args1[i_args + 2], numpols);
                    i_args += 4;
                    break;
               }
               case 7:
               {
                    Goldilocks::add_avx(tmp1[(args1[i_args])], &params.pols[args1[i_args + 1] + i * args1[i_args + 2]], Goldilocks::fromU64(args1[i_args + 3]), args1[i_args + 2]);
                    i_args += 4;
                    break;
               }
               case 8:
               {
                    Goldilocks::add_avx(tmp1[(args1[i_args])], &params.pConstPols->getElement(args1[i_args + 1], i), &params.pConstPols->getElement(args1[i_args + 2], i), numpols, numpols);
                    i_args += 3;
                    break;
               }
               case 9:
               {
                    for (uint64_t j = 0; j < AVX_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args + 1] + (((i + j) + args1[i_args + 2]) % args1[i_args + 3]) * numpols;
                         offsets2[j] = args1[i_args + 4] + (((i + j) + args1[i_args + 5]) % args1[i_args + 6]) * numpols;
                    }
                    Goldilocks::add_avx(tmp1[(args1[i_args])], &params.pConstPols->getElement(0, 0), &params.pConstPols->getElement(0, 0), offsets1, offsets2);
                    i_args += 7;
                    break;
               }
               case 10:
               {
                    Goldilocks::add_avx(tmp1[(args1[i_args])], &params.pConstPols->getElement(args1[i_args + 1], i), Goldilocks::fromU64(args1[i_args + 2]), numpols);
                    i_args += 3;
                    break;
               }
               case 11:
               {
                    for (uint64_t j = 0; j < AVX_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args + 1] + (((i + j) + args1[i_args + 2]) % args1[i_args + 3]) * numpols;
                    }
                    Goldilocks::add_avx(tmp1[(args1[i_args])], &params.pConstPols->getElement(0, 0), Goldilocks::fromU64(args1[i_args + 4]), offsets1);
                    i_args += 5;
                    break;
               }
               case 21:
               {
                    Goldilocks::sub_avx(tmp1[(args1[i_args])], tmp1[args1[i_args + 1]], tmp1[args1[i_args + 2]]);
                    i_args += 3;
                    break;
               }
               case 22:
               {
                    Goldilocks::sub_avx(tmp1[(args1[i_args])], tmp1[args1[i_args + 1]], &params.pols[args1[i_args + 2] + i * args1[i_args + 3]], args1[i_args + 3]);
                    i_args += 4;
                    break;
               }
               case 23:
               {
                    for (uint64_t j = 0; j < AVX_SIZE_; ++j)
                    {
                         offsets2[j] = args1[i_args + 2] + (((i + j) + args1[i_args + 3]) % args1[i_args + 4]) * args1[i_args + 5];
                    }
                    Goldilocks::sub_avx(tmp1[(args1[i_args])], tmp1[args1[i_args + 1]], &params.pols[0], offsets2);
                    i_args += 6;
                    break;
               }
               case 24:
               {
                    Goldilocks::sub_avx(tmp1[(args1[i_args])], &params.pols[args1[i_args + 1] + i * args1[i_args + 2]], tmp1[args1[i_args + 3]], args1[i_args + 2]);
                    i_args += 4;
                    break;
               }
               case 25:
               {

                    for (uint64_t j = 0; j < AVX_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args + 1] + (((i + j) + args1[i_args + 2]) % args1[i_args + 3]) * args1[i_args + 4];
                    }
                    Goldilocks::sub_avx(tmp1[(args1[i_args])], &params.pols[0], tmp1[args1[i_args + 5]], offsets1);
                    i_args += 6;
                    break;
               }
               case 26:
               {
                    Goldilocks::sub_avx(tmp1[(args1[i_args])], tmp1[args1[i_args + 1]], Goldilocks::fromU64(args1[i_args + 2]));
                    i_args += 3;
                    break;
               }
               case 27:
               {
                    Goldilocks::sub_avx(tmp1[(args1[i_args])], Goldilocks::fromU64(args1[i_args + 1]), tmp1[args1[i_args + 2]]);
                    i_args += 3;
                    break;
               }
               case 28:
               {

                    Goldilocks::sub_avx(tmp1[(args1[i_args])], &params.pols[args1[i_args + 1] + i * args1[i_args + 2]], Goldilocks::fromU64(args1[i_args + 3]), args1[i_args + 2]);
                    i_args += 4;
                    break;
               }
               case 29:
               {
                    for (uint64_t j = 0; j < AVX_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args + 1] + (((i + j) + args1[i_args + 2]) % args1[i_args + 3]) * args1[i_args + 4];
                    }
                    Goldilocks::sub_avx(tmp1[(args1[i_args])], &params.pols[0], Goldilocks::fromU64(args1[i_args + 5]), offsets1);
                    i_args += 6;
                    break;
               }
               case 30:
               {
                    Goldilocks::sub_avx(tmp1[(args1[i_args])], Goldilocks::fromU64(args1[i_args + 1]), &params.pols[args1[i_args + 2] + i * args1[i_args + 3]], args1[i_args + 3]);
                    i_args += 4;
                    break;
               }
               case 31:
               {
                    for (uint64_t j = 0; j < AVX_SIZE_; ++j)
                    {
                         offsets2[j] = args1[i_args + 2] + (((i + j) + args1[i_args + 3]) % args1[i_args + 4]) * args1[i_args + 5];
                    }
                    Goldilocks::sub_avx(tmp1[(args1[i_args])], Goldilocks::fromU64(args1[i_args + 1]), &params.pols[0], offsets2);
                    i_args += 6;
                    break;
               }
               case 32:
               {
                    Goldilocks::sub_avx(tmp1[(args1[i_args])], Goldilocks::fromU64(args1[i_args + 1]), &params.pConstPols->getElement(args1[i_args + 2], i), numpols);
                    i_args += 3;
                    break;
               }
               case 33:
               {
                    for (uint64_t j = 0; j < AVX_SIZE_; ++j)
                    {
                         offsets2[j] = args1[i_args + 2] + (((i + j) + args1[i_args + 3]) % args1[i_args + 4]) * numpols;
                    }
                    Goldilocks::sub_avx(tmp1[(args1[i_args])], Goldilocks::fromU64(args1[i_args + 1]), &params.pConstPols->getElement(0, 0), offsets2);
                    i_args += 5;
                    break;
               }
               case 34:
               {
                    Goldilocks::sub_avx(tmp1[(args1[i_args])], &params.pols[args1[i_args + 1] + i * args1[i_args + 2]], params.publicInputs[args1[i_args + 3]], args1[i_args + 2]);
                    i_args += 4;
                    break;
               }
               case 35:
               {
                    for (uint64_t j = 0; j < AVX_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args + 1] + (((i + j) + args1[i_args + 2]) % args1[i_args + 3]) * args1[i_args + 4];
                         offsets2[j] = args1[i_args + 5] + (i + j) * args1[i_args + 6];
                    }
                    Goldilocks::sub_avx(tmp1[(args1[i_args])], &params.pols[0], &params.pols[0], offsets1, offsets2);
                    i_args += 7;
                    break;
               }
               case 36:
               {
                    for (uint64_t j = 0; j < AVX_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args + 1] + (i + j) * args1[i_args + 2];
                         offsets2[j] = args1[i_args + 3] + (((i + j) + args1[i_args + 4]) % args1[i_args + 5]) * args1[i_args + 6];
                    }
                    Goldilocks::sub_avx(tmp1[(args1[i_args])], &params.pols[0], &params.pols[0], offsets1, offsets2);
                    i_args += 7;
                    break;
               }
               case 37:
               {
                    Goldilocks::sub_avx(tmp1[(args1[i_args])], &params.pols[args1[i_args + 1] + i * args1[i_args + 2]], &params.pols[args1[i_args + 3] + i * args1[i_args + 4]], args1[i_args + 2], args1[i_args + 4]);
                    i_args += 5;
                    break;
               }
               case 38:
               {
                    for (uint64_t j = 0; j < AVX_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args + 1] + (((i + j) + args1[i_args + 2]) % args1[i_args + 3]) * args1[i_args + 4];
                         offsets2[j] = args1[i_args + 5] + (((i + j) + args1[i_args + 6]) % args1[i_args + 7]) * args1[i_args + 8];
                    }
                    Goldilocks::sub_avx(tmp1[(args1[i_args])], &params.pols[0], &params.pols[0], offsets1, offsets2);
                    i_args += 9;
                    break;
               }
               case 39:
               {
                    Goldilocks::sub_avx(tmp1[(args1[i_args])], &params.pConstPols->getElement(args1[i_args + 1], i), &params.pols[args1[i_args + 2] + i * args1[i_args + 3]], numpols, args1[i_args + 3]);
                    i_args += 4;
                    break;
               }
               case 40:
               {
                    Goldilocks::sub_avx(tmp1[(args1[i_args])], tmp1[args1[i_args + 1]], &params.pConstPols->getElement(args1[i_args + 2], i), numpols);
                    i_args += 3;
                    break;
               }
               case 45:
               {
                    Goldilocks::mult_avx(tmp1[(args1[i_args])], tmp1[args1[i_args + 1]], tmp1[args1[i_args + 2]]);
                    i_args += 3;
                    break;
               }
               case 46:
               {
                    Goldilocks::mul_avx(tmp1[(args1[i_args])], Goldilocks::fromU64(args1[i_args + 1]), tmp1[args1[i_args + 2]]);
                    i_args += 3;
                    break;
               }
               case 47:
               {
                    Goldilocks::mul_avx(tmp1[(args1[i_args])], &params.pols[args1[i_args + 1] + i * args1[i_args + 2]], tmp1[args1[i_args + 3]], args1[i_args + 2]);
                    i_args += 4;
                    break;
               }
               case 48:
               {
                    for (uint64_t j = 0; j < AVX_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args + 1] + (((i + j) + args1[i_args + 2]) % args1[i_args + 3]) * args1[i_args + 4];
                    }
                    Goldilocks::mul_avx(tmp1[(args1[i_args])], &params.pols[0], tmp1[args1[i_args + 5]], offsets1);
                    i_args += 6;
                    break;
               }
               case 49:
               {
                    Goldilocks::mul_avx(tmp1[(args1[i_args])], tmp1[args1[i_args + 1]], &params.pConstPols->getElement(args1[i_args + 2], i), numpols);
                    i_args += 3;
                    break;
               }
               case 50:
               {
                    Goldilocks::mul_avx(tmp1[(args1[i_args])], &params.pols[args1[i_args + 1] + i * args1[i_args + 2]], &params.pols[args1[i_args + 3] + i * args1[i_args + 4]], args1[i_args + 2], args1[i_args + 4]);
                    i_args += 5;
                    break;
               }
               case 51:
               {
                    for (uint64_t j = 0; j < AVX_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args + 1] + (i + j) * args1[i_args + 2];
                         offsets2[j] = args1[i_args + 3] + (((i + j) + args1[i_args + 4]) % args1[i_args + 5]) * args1[i_args + 6];
                    }
                    Goldilocks::mul_avx(tmp1[args1[i_args]], &params.pols[0], &params.pols[0], offsets1, offsets2);
                    i_args += 7;
                    break;
               }
               case 52:
               {
                    for (uint64_t j = 0; j < AVX_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args + 1] + (((i + j) + args1[i_args + 2]) % args1[i_args + 3]) * args1[i_args + 4];
                         offsets2[j] = args1[i_args + 5] + (((i + j) + args1[i_args + 6]) % args1[i_args + 7]) * args1[i_args + 8];
                    }
                    Goldilocks::mul_avx(tmp1[args1[i_args]], &params.pols[0], &params.pols[0], offsets1, offsets2);
                    i_args += 9;
                    break;
               }
               case 53:
               {
                    Goldilocks::mul_avx(tmp1[(args1[i_args])], Goldilocks::fromU64(args1[i_args + 1]), &params.pols[args1[i_args + 2] + i * args1[i_args + 3]], args1[i_args + 3]);
                    i_args += 4;
                    break;
               }
               case 54:
               {
                    Goldilocks::mul_avx(tmp1[(args1[i_args])], &params.pols[args1[i_args + 1] + i * args1[i_args + 2]], &params.pConstPols->getElement(args1[i_args + 3], i), args1[i_args + 2], numpols);
                    i_args += 4;
                    break;
               }
               case 55:
               {
                    for (uint64_t j = 0; j < AVX_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args + 1] + (((i + j) + args1[i_args + 2]) % args1[i_args + 3]) * args1[i_args + 4];
                         offsets2[j] = args1[i_args + 5] + (i + j) * numpols;
                    }
                    Goldilocks::mul_avx(tmp1[(args1[i_args])], &params.pols[0], &params.pConstPols->getElement(0, 0), offsets1, offsets2);
                    i_args += 6;
                    break;
               }
               case 56:
               {
                    Goldilocks::mul_avx(tmp1[(args1[i_args])], tmp1[args1[i_args + 1]], &params.pols[args1[i_args + 2] + i * args1[i_args + 3]], args1[i_args + 3]);
                    i_args += 4;
                    break;
               }
               case 57:
               {
                    for (uint64_t j = 0; j < AVX_SIZE_; ++j)
                    {
                         offsets2[j] = args1[i_args + 2] + (((i + j) + args1[i_args + 3]) % args1[i_args + 4]) * args1[i_args + 5];
                    }
                    Goldilocks::mul_avx(tmp1[(args1[i_args])], tmp1[args1[i_args + 1]], &params.pols[0], offsets2);
                    i_args += 6;
                    break;
               }
               case 58:
               {
                    Goldilocks::mul_avx(tmp1[(args1[i_args])], &params.pConstPols->getElement(args1[i_args + 1], i), tmp1[args1[i_args + 2]], numpols);
                    i_args += 3;
                    break;
               }
               case 78:
               {
                    Goldilocks::copy_avx(tmp1[(args1[i_args])], tmp1[args1[i_args + 1]]);
                    i_args += 2;
                    break;
               }
               case 79:
               {
                    Goldilocks::copy_avx(tmp1[(args1[i_args])], &params.pols[args1[i_args + 1] + i * args1[i_args + 2]], args1[i_args + 2]);
                    i_args += 3;
                    break;
               }
               case 80:
               {
                    for (uint64_t j = 0; j < AVX_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args + 1] + (((i + j) + args1[i_args + 2]) % args1[i_args + 3]) * args1[i_args + 4];
                    }
                    Goldilocks::copy_avx(tmp1[args1[i_args]], &params.pols[0], offsets1);
                    i_args += 5;
                    break;
               }
               case 81:
               {
                    Goldilocks::copy_avx(tmp1[(args1[i_args])], Goldilocks::fromU64(args1[i_args + 1]));
                    i_args += 2;
                    break;
               }
               case 82:
               {
                    Goldilocks::copy_avx(tmp1[(args1[i_args])], &params.pConstPols->getElement(args1[i_args + 1], i), numpols);
                    i_args += 2;
                    break;
               }
               case 83:
               {
                    for (uint64_t j = 0; j < AVX_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args + 1] + (((i + j) + args1[i_args + 2]) % args1[i_args + 3]) * numpols;
                    }
                    Goldilocks::copy_avx(tmp1[(args1[i_args])], &params.pConstPols->getElement(0, 0), offsets1);
                    i_args += 4;
                    break;
               }
               case 84:
               {
                    for (uint64_t j = 0; j < AVX_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args + 2] + (((i + j) + args1[i_args + 3]) % args1[i_args + 4]) * args1[i_args + 5];
                    }
                    Goldilocks::add_avx(tmp1[(args1[i_args])], tmp1[args1[i_args + 1]], &params.pols[0], offsets1);
                    i_args += 6;
                    break;
               }
               case 85:
               {
                    for (uint64_t j = 0; j < AVX_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args + 1] + (((i + j) + args1[i_args + 2]) % args1[i_args + 3]) * args1[i_args + 4];
                    }
                    Goldilocks::mul_avx(tmp1[(args1[i_args])], &params.pols[0], Goldilocks::fromU64(args1[i_args + 5]), offsets1);
                    i_args += 6;
                    break;
               }
               case 86:
               {
                    Goldilocks::add_avx(&params.pols[args1[i_args] + i * args1[i_args + 1]], args1[i_args + 1], tmp1[args1[i_args + 2]], tmp1[args1[i_args + 3]]);
                    i_args += 4;
                    break;
               }
               case 87:
               {
                    Goldilocks::add_avx(&params.pols[args1[i_args] + i * args1[i_args + 1]], args1[i_args + 1], tmp1[args1[i_args + 2]], &params.pols[args1[i_args + 3] + i * args1[i_args + 4]], args1[i_args + 4]);
                    i_args += 5;
                    break;
               }
               case 91:
               {
                    Goldilocks::copy_avx(&params.pols[args1[i_args] + i * args1[i_args + 1]], args1[i_args + 1], &params.pConstPols->getElement(args1[i_args + 2], i), numpols);
                    i_args += 3;
                    break;
               }
               case 92:
               {
                    Goldilocks::sub_avx(&params.pols[args1[i_args] + i * args1[i_args + 1]], args1[i_args + 1], tmp1[args1[i_args + 2]], tmp1[args1[i_args + 3]]);
                    i_args += 4;
                    break;
               }
               case 93:
               {
                    Goldilocks::sub_avx(&params.pols[args1[i_args] + i * args1[i_args + 1]], args1[i_args + 1], Goldilocks::fromU64(args1[i_args + 2]), tmp1[args1[i_args + 3]]);
                    i_args += 4;
                    break;
               }
               case 94:
               {
                    Goldilocks::mul_avx(&params.pols[args1[i_args] + i * args1[i_args + 1]], args1[i_args + 1], tmp1[args1[i_args + 2]], tmp1[args1[i_args + 3]]);
                    i_args += 4;
                    break;
               }
               case 95:
               {
                    Goldilocks::mul_avx(&params.pols[args1[i_args] + i * args1[i_args + 1]], args1[i_args + 1], &params.pols[args1[i_args + 2] + i * args1[i_args + 3]], tmp1[args1[i_args + 4]], args1[i_args + 3]);
                    i_args += 5;
                    break;
               }
               case 96:
               {
                    Goldilocks::mul_avx(&params.pols[args1[i_args] + i * args1[i_args + 1]], args1[i_args + 1], tmp1[args1[i_args + 2]], &params.pConstPols->getElement(args1[i_args + 3], i), numpols);
                    i_args += 4;
                    break;
               }
               case 99:
               {
                    assert(0); // code not used
                    i_args += 4;
                    break;
               }
               case 100:
               {
                    Goldilocks::copy_avx(&params.pols[args1[i_args] + i * args1[i_args + 1]], args1[i_args + 1], tmp1[(args1[i_args + 2])]);
                    i_args += 3;
                    break;
               }
               case 101:
               {
                    for (uint64_t j = 0; j < AVX_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args] + (((i + j) + args1[i_args + 1]) % args1[i_args + 2]) * args1[i_args + 3];
                    }
                    Goldilocks::add_avx(&params.pols[0], offsets1, tmp1[args1[i_args + 4]], tmp1[args1[i_args + 5]]);
                    i_args += 6;
                    break;
               }
               case 102:
               {
                    for (uint64_t j = 0; j < AVX_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args] + (((i + j) + args1[i_args + 1]) % args1[i_args + 2]) * args1[i_args + 3];
                    }
                    Goldilocks::add_avx(&params.pols[0], offsets1, tmp1[args1[i_args + 4]], &params.pols[args1[i_args + 5] + i * args1[i_args + 6]], args1[i_args + 6]);
                    i_args += 7;
                    break;
               }
               case 106:
               {
                    for (uint64_t j = 0; j < AVX_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args] + (((i + j) + args1[i_args + 1]) % args1[i_args + 2]) * args1[i_args + 3];
                    }
                    Goldilocks::sub_avx(&params.pols[0], offsets1, tmp1[args1[i_args + 4]], tmp1[args1[i_args + 5]]);
                    i_args += 6;
                    break;
               }
               case 107:
               {
                    for (uint64_t j = 0; j < AVX_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args] + (((i + j) + args1[i_args + 1]) % args1[i_args + 2]) * args1[i_args + 3];
                    }
                    Goldilocks::sub_avx(&params.pols[0], offsets1, Goldilocks::fromU64(args1[i_args + 4]), tmp1[args1[i_args + 5]]);
                    i_args += 6;
                    break;
               }
               case 108:
               {
                    for (uint64_t j = 0; j < AVX_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args] + (((i + j) + args1[i_args + 1]) % args1[i_args + 2]) * args1[i_args + 3];
                    }
                    Goldilocks::mul_avx(&params.pols[0], offsets1, tmp1[args1[i_args + 4]], tmp1[args1[i_args + 5]]);
                    i_args += 6;
                    break;
               }
               case 109:
               {
                    for (uint64_t j = 0; j < AVX_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args] + (((i + j) + args1[i_args + 1]) % args1[i_args + 2]) * args1[i_args + 3];
                    }
                    Goldilocks::mul_avx(&params.pols[0], offsets1, &params.pols[args1[i_args + 4] + i * args1[i_args + 5]], tmp1[args1[i_args + 6]], args1[i_args + 5]);
                    i_args += 7;
                    break;
               }
               case 110:
               {
                    for (uint64_t j = 0; j < AVX_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args] + (((i + j) + args1[i_args + 1]) % args1[i_args + 2]) * args1[i_args + 3];
                    }
                    Goldilocks::mul_avx(&params.pols[0], offsets1, tmp1[args1[i_args + 4]], &params.pConstPols->getElement(args1[i_args + 5], i), numpols);
                    i_args += 6;
                    break;
               }
               case 111:
               {
                    for (uint64_t j = 0; j < AVX_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args] + (((i + j) + args1[i_args + 1]) % args1[i_args + 2]) * args1[i_args + 3];
                         offsets2[j] = args1[i_args + 4] + (((i + j) + args1[i_args + 5]) % args1[i_args + 6]) * numpols;
                    }
                    Goldilocks::mul_avx(&params.pols[0], offsets1, &params.pConstPols->getElement(0, 0), tmp1[args1[i_args + 7]], offsets2);
                    i_args += 8;
                    break;
               }
               case 113:
               {
                    for (uint64_t j = 0; j < AVX_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args] + (((i + j) + args1[i_args + 1]) % args1[i_args + 2]) * args1[i_args + 3];
                    }
                    Goldilocks::copy_avx(&params.pols[0], offsets1, tmp1[(args1[i_args + 4])]);
                    i_args += 5;
                    break;
               }
               case 114:
               {
                    for (uint64_t j = 0; j < AVX_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args] + (((i + j) + args1[i_args + 1]) % args1[i_args + 2]) * args1[i_args + 3];
                         offsets2[j] = args1[i_args + 5] + (((i + j) + args1[i_args + 6]) % args1[i_args + 7]) * args1[i_args + 8];
                    }
                    Goldilocks::add_avx(&params.pols[0], offsets1, tmp1[args1[i_args + 4]], &params.pols[0], offsets2);
                    i_args += 9;
                    break;
               }
               case 117:
               {
                    Goldilocks::copy_avx(&params.pols[args1[i_args] + i * args1[i_args + 1]], args1[i_args + 1], &params.pols[args1[i_args + 2] + i * args1[i_args + 3]], args1[i_args + 3]);
                    i_args += 4;
                    break;
               }
               default:
               {
                    std::cout << " Wrong operation in step42ns_first!" << std::endl;
                    exit(1);
               }
               }
          }
          if (i_args != NARGS_)
               std::cout << " " << i_args << " - " << NARGS_ << std::endl;
          assert(i_args == NARGS_);
          // delete (tmp1);
     }
}

#ifdef __AVX512__
void AllSteps::step1_parser_first_avx512(StepsParams &params, uint64_t nrows, uint64_t nrowsBatch)
{
#pragma omp parallel for
     for (uint64_t i = 0; i < nrows; i += nrowsBatch)
     {
          int i_args = 0;
          //__m256i *tmp1 = new __m256i[NTEMP1_];
          __m512i tmp1[NTEMP1_];
          uint64_t offsets1[AVX512_SIZE_], offsets2[AVX512_SIZE_];
          uint64_t numpols = params.pConstPols->numPols();

          for (int kk = 0; kk < NOPS_; ++kk)
          {
               switch (op1[kk])
               {
               case 0:
               {
                    Goldilocks::add_avx512(tmp1[(args1[i_args])], tmp1[args1[i_args + 1]], tmp1[args1[i_args + 2]]);
                    i_args += 3;
                    break;
               }
               case 1:
               {
                    Goldilocks::add_avx512(tmp1[(args1[i_args])], tmp1[args1[i_args + 1]], &params.pols[args1[i_args + 2] + i * args1[i_args + 3]], args1[i_args + 3]);
                    i_args += 4;
                    break;
               }
               case 2:
               {
                    Goldilocks::add_avx512(tmp1[(args1[i_args])], tmp1[args1[i_args + 1]], Goldilocks::fromU64(args1[i_args + 2]));
                    i_args += 3;
                    break;
               }
               case 3:
               {
                    Goldilocks::add_avx512(tmp1[(args1[i_args])], tmp1[args1[i_args + 1]], &params.pConstPols->getElement(args1[i_args + 2], i), numpols);
                    i_args += 3;
                    break;
               }
               case 4:
               {
                    Goldilocks::add_avx512(tmp1[(args1[i_args])], &params.pols[args1[i_args + 1] + i * args1[i_args + 2]], &params.pols[args1[i_args + 3] + i * args1[i_args + 4]], args1[i_args + 2], args1[i_args + 4]);
                    i_args += 5;
                    break;
               }
               case 5:
               {
                    for (uint64_t j = 0; j < AVX512_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args + 1] + (((i + j) + args1[i_args + 2]) % args1[i_args + 3]) * args1[i_args + 4];
                         offsets2[j] = args1[i_args + 5] + (((i + j) + args1[i_args + 6]) % args1[i_args + 7]) * args1[i_args + 8];
                    }
                    Goldilocks::add_avx512(tmp1[(args1[i_args])], &params.pols[0], &params.pols[0], offsets1, offsets2);
                    i_args += 9;
                    break;
               }
               case 6:
               {
                    Goldilocks::add_avx512(tmp1[(args1[i_args])], &params.pols[args1[i_args + 1] + i * args1[i_args + 2]], &params.pConstPols->getElement(args1[i_args + 3], i), args1[i_args + 2], numpols);
                    i_args += 4;
                    break;
               }
               case 7:
               {
                    Goldilocks::add_avx512(tmp1[(args1[i_args])], &params.pols[args1[i_args + 1] + i * args1[i_args + 2]], Goldilocks::fromU64(args1[i_args + 3]), args1[i_args + 2]);
                    i_args += 4;
                    break;
               }
               case 8:
               {
                    Goldilocks::add_avx512(tmp1[(args1[i_args])], &params.pConstPols->getElement(args1[i_args + 1], i), &params.pConstPols->getElement(args1[i_args + 2], i), numpols, numpols);
                    i_args += 3;
                    break;
               }
               case 9:
               {
                    for (uint64_t j = 0; j < AVX512_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args + 1] + (((i + j) + args1[i_args + 2]) % args1[i_args + 3]) * numpols;
                         offsets2[j] = args1[i_args + 4] + (((i + j) + args1[i_args + 5]) % args1[i_args + 6]) * numpols;
                    }
                    Goldilocks::add_avx512(tmp1[(args1[i_args])], &params.pConstPols->getElement(0, 0), &params.pConstPols->getElement(0, 0), offsets1, offsets2);
                    i_args += 7;
                    break;
               }
               case 10:
               {
                    Goldilocks::add_avx512(tmp1[(args1[i_args])], &params.pConstPols->getElement(args1[i_args + 1], i), Goldilocks::fromU64(args1[i_args + 2]), numpols);
                    i_args += 3;
                    break;
               }
               case 11:
               {
                    for (uint64_t j = 0; j < AVX512_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args + 1] + (((i + j) + args1[i_args + 2]) % args1[i_args + 3]) * numpols;
                    }
                    Goldilocks::add_avx512(tmp1[(args1[i_args])], &params.pConstPols->getElement(0, 0), Goldilocks::fromU64(args1[i_args + 4]), offsets1);
                    i_args += 5;
                    break;
               }
               case 21:
               {
                    Goldilocks::sub_avx512(tmp1[(args1[i_args])], tmp1[args1[i_args + 1]], tmp1[args1[i_args + 2]]);
                    i_args += 3;
                    break;
               }
               case 22:
               {
                    Goldilocks::sub_avx512(tmp1[(args1[i_args])], tmp1[args1[i_args + 1]], &params.pols[args1[i_args + 2] + i * args1[i_args + 3]], args1[i_args + 3]);
                    i_args += 4;
                    break;
               }
               case 23:
               {
                    for (uint64_t j = 0; j < AVX512_SIZE_; ++j)
                    {
                         offsets2[j] = args1[i_args + 2] + (((i + j) + args1[i_args + 3]) % args1[i_args + 4]) * args1[i_args + 5];
                    }
                    Goldilocks::sub_avx512(tmp1[(args1[i_args])], tmp1[args1[i_args + 1]], &params.pols[0], offsets2);
                    i_args += 6;
                    break;
               }
               case 24:
               {
                    Goldilocks::sub_avx512(tmp1[(args1[i_args])], &params.pols[args1[i_args + 1] + i * args1[i_args + 2]], tmp1[args1[i_args + 3]], args1[i_args + 2]);
                    i_args += 4;
                    break;
               }
               case 25:
               {

                    for (uint64_t j = 0; j < AVX512_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args + 1] + (((i + j) + args1[i_args + 2]) % args1[i_args + 3]) * args1[i_args + 4];
                    }
                    Goldilocks::sub_avx512(tmp1[(args1[i_args])], &params.pols[0], tmp1[args1[i_args + 5]], offsets1);
                    i_args += 6;
                    break;
               }
               case 26:
               {
                    Goldilocks::sub_avx512(tmp1[(args1[i_args])], tmp1[args1[i_args + 1]], Goldilocks::fromU64(args1[i_args + 2]));
                    i_args += 3;
                    break;
               }
               case 27:
               {
                    Goldilocks::sub_avx512(tmp1[(args1[i_args])], Goldilocks::fromU64(args1[i_args + 1]), tmp1[args1[i_args + 2]]);
                    i_args += 3;
                    break;
               }
               case 28:
               {

                    Goldilocks::sub_avx512(tmp1[(args1[i_args])], &params.pols[args1[i_args + 1] + i * args1[i_args + 2]], Goldilocks::fromU64(args1[i_args + 3]), args1[i_args + 2]);
                    i_args += 4;
                    break;
               }
               case 29:
               {
                    for (uint64_t j = 0; j < AVX512_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args + 1] + (((i + j) + args1[i_args + 2]) % args1[i_args + 3]) * args1[i_args + 4];
                    }
                    Goldilocks::sub_avx512(tmp1[(args1[i_args])], &params.pols[0], Goldilocks::fromU64(args1[i_args + 5]), offsets1);
                    i_args += 6;
                    break;
               }
               case 30:
               {
                    Goldilocks::sub_avx512(tmp1[(args1[i_args])], Goldilocks::fromU64(args1[i_args + 1]), &params.pols[args1[i_args + 2] + i * args1[i_args + 3]], args1[i_args + 3]);
                    i_args += 4;
                    break;
               }
               case 31:
               {
                    for (uint64_t j = 0; j < AVX512_SIZE_; ++j)
                    {
                         offsets2[j] = args1[i_args + 2] + (((i + j) + args1[i_args + 3]) % args1[i_args + 4]) * args1[i_args + 5];
                    }
                    Goldilocks::sub_avx512(tmp1[(args1[i_args])], Goldilocks::fromU64(args1[i_args + 1]), &params.pols[0], offsets2);
                    i_args += 6;
                    break;
               }
               case 32:
               {
                    Goldilocks::sub_avx512(tmp1[(args1[i_args])], Goldilocks::fromU64(args1[i_args + 1]), &params.pConstPols->getElement(args1[i_args + 2], i), numpols);
                    i_args += 3;
                    break;
               }
               case 33:
               {
                    for (uint64_t j = 0; j < AVX512_SIZE_; ++j)
                    {
                         offsets2[j] = args1[i_args + 2] + (((i + j) + args1[i_args + 3]) % args1[i_args + 4]) * numpols;
                    }
                    Goldilocks::sub_avx512(tmp1[(args1[i_args])], Goldilocks::fromU64(args1[i_args + 1]), &params.pConstPols->getElement(0, 0), offsets2);
                    i_args += 5;
                    break;
               }
               case 34:
               {
                    Goldilocks::sub_avx512(tmp1[(args1[i_args])], &params.pols[args1[i_args + 1] + i * args1[i_args + 2]], params.publicInputs[args1[i_args + 3]], args1[i_args + 2]);
                    i_args += 4;
                    break;
               }
               case 35:
               {
                    for (uint64_t j = 0; j < AVX512_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args + 1] + (((i + j) + args1[i_args + 2]) % args1[i_args + 3]) * args1[i_args + 4];
                         offsets2[j] = args1[i_args + 5] + (i + j) * args1[i_args + 6];
                    }
                    Goldilocks::sub_avx512(tmp1[(args1[i_args])], &params.pols[0], &params.pols[0], offsets1, offsets2);
                    i_args += 7;
                    break;
               }
               case 36:
               {
                    for (uint64_t j = 0; j < AVX512_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args + 1] + (i + j) * args1[i_args + 2];
                         offsets2[j] = args1[i_args + 3] + (((i + j) + args1[i_args + 4]) % args1[i_args + 5]) * args1[i_args + 6];
                    }
                    Goldilocks::sub_avx512(tmp1[(args1[i_args])], &params.pols[0], &params.pols[0], offsets1, offsets2);
                    i_args += 7;
                    break;
               }
               case 37:
               {
                    Goldilocks::sub_avx512(tmp1[(args1[i_args])], &params.pols[args1[i_args + 1] + i * args1[i_args + 2]], &params.pols[args1[i_args + 3] + i * args1[i_args + 4]], args1[i_args + 2], args1[i_args + 4]);
                    i_args += 5;
                    break;
               }
               case 38:
               {
                    for (uint64_t j = 0; j < AVX512_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args + 1] + (((i + j) + args1[i_args + 2]) % args1[i_args + 3]) * args1[i_args + 4];
                         offsets2[j] = args1[i_args + 5] + (((i + j) + args1[i_args + 6]) % args1[i_args + 7]) * args1[i_args + 8];
                    }
                    Goldilocks::sub_avx512(tmp1[(args1[i_args])], &params.pols[0], &params.pols[0], offsets1, offsets2);
                    i_args += 9;
                    break;
               }
               case 39:
               {
                    Goldilocks::sub_avx512(tmp1[(args1[i_args])], &params.pConstPols->getElement(args1[i_args + 1], i), &params.pols[args1[i_args + 2] + i * args1[i_args + 3]], numpols, args1[i_args + 3]);
                    i_args += 4;
                    break;
               }
               case 40:
               {
                    Goldilocks::sub_avx512(tmp1[(args1[i_args])], tmp1[args1[i_args + 1]], &params.pConstPols->getElement(args1[i_args + 2], i), numpols);
                    i_args += 3;
                    break;
               }
               case 45:
               {
                    Goldilocks::mult_avx512(tmp1[(args1[i_args])], tmp1[args1[i_args + 1]], tmp1[args1[i_args + 2]]);
                    i_args += 3;
                    break;
               }
               case 46:
               {
                    Goldilocks::mul_avx512(tmp1[(args1[i_args])], Goldilocks::fromU64(args1[i_args + 1]), tmp1[args1[i_args + 2]]);
                    i_args += 3;
                    break;
               }
               case 47:
               {
                    Goldilocks::mul_avx512(tmp1[(args1[i_args])], &params.pols[args1[i_args + 1] + i * args1[i_args + 2]], tmp1[args1[i_args + 3]], args1[i_args + 2]);
                    i_args += 4;
                    break;
               }
               case 48:
               {
                    for (uint64_t j = 0; j < AVX512_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args + 1] + (((i + j) + args1[i_args + 2]) % args1[i_args + 3]) * args1[i_args + 4];
                    }
                    Goldilocks::mul_avx512(tmp1[(args1[i_args])], &params.pols[0], tmp1[args1[i_args + 5]], offsets1);
                    i_args += 6;
                    break;
               }
               case 49:
               {
                    Goldilocks::mul_avx512(tmp1[(args1[i_args])], tmp1[args1[i_args + 1]], &params.pConstPols->getElement(args1[i_args + 2], i), numpols);
                    i_args += 3;
                    break;
               }
               case 50:
               {
                    Goldilocks::mul_avx512(tmp1[(args1[i_args])], &params.pols[args1[i_args + 1] + i * args1[i_args + 2]], &params.pols[args1[i_args + 3] + i * args1[i_args + 4]], args1[i_args + 2], args1[i_args + 4]);
                    i_args += 5;
                    break;
               }
               case 51:
               {
                    for (uint64_t j = 0; j < AVX512_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args + 1] + (i + j) * args1[i_args + 2];
                         offsets2[j] = args1[i_args + 3] + (((i + j) + args1[i_args + 4]) % args1[i_args + 5]) * args1[i_args + 6];
                    }
                    Goldilocks::mul_avx512(tmp1[args1[i_args]], &params.pols[0], &params.pols[0], offsets1, offsets2);
                    i_args += 7;
                    break;
               }
               case 52:
               {
                    for (uint64_t j = 0; j < AVX512_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args + 1] + (((i + j) + args1[i_args + 2]) % args1[i_args + 3]) * args1[i_args + 4];
                         offsets2[j] = args1[i_args + 5] + (((i + j) + args1[i_args + 6]) % args1[i_args + 7]) * args1[i_args + 8];
                    }
                    Goldilocks::mul_avx512(tmp1[args1[i_args]], &params.pols[0], &params.pols[0], offsets1, offsets2);
                    i_args += 9;
                    break;
               }
               case 53:
               {
                    Goldilocks::mul_avx512(tmp1[(args1[i_args])], Goldilocks::fromU64(args1[i_args + 1]), &params.pols[args1[i_args + 2] + i * args1[i_args + 3]], args1[i_args + 3]);
                    i_args += 4;
                    break;
               }
               case 54:
               {
                    Goldilocks::mul_avx512(tmp1[(args1[i_args])], &params.pols[args1[i_args + 1] + i * args1[i_args + 2]], &params.pConstPols->getElement(args1[i_args + 3], i), args1[i_args + 2], numpols);
                    i_args += 4;
                    break;
               }
               case 55:
               {
                    for (uint64_t j = 0; j < AVX512_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args + 1] + (((i + j) + args1[i_args + 2]) % args1[i_args + 3]) * args1[i_args + 4];
                         offsets2[j] = args1[i_args + 5] + (i + j) * numpols;
                    }
                    Goldilocks::mul_avx512(tmp1[(args1[i_args])], &params.pols[0], &params.pConstPols->getElement(0, 0), offsets1, offsets2);
                    i_args += 6;
                    break;
               }
               case 56:
               {
                    Goldilocks::mul_avx512(tmp1[(args1[i_args])], tmp1[args1[i_args + 1]], &params.pols[args1[i_args + 2] + i * args1[i_args + 3]], args1[i_args + 3]);
                    i_args += 4;
                    break;
               }
               case 57:
               {
                    for (uint64_t j = 0; j < AVX512_SIZE_; ++j)
                    {
                         offsets2[j] = args1[i_args + 2] + (((i + j) + args1[i_args + 3]) % args1[i_args + 4]) * args1[i_args + 5];
                    }
                    Goldilocks::mul_avx512(tmp1[(args1[i_args])], tmp1[args1[i_args + 1]], &params.pols[0], offsets2);
                    i_args += 6;
                    break;
               }
               case 58:
               {
                    Goldilocks::mul_avx512(tmp1[(args1[i_args])], &params.pConstPols->getElement(args1[i_args + 1], i), tmp1[args1[i_args + 2]], numpols);
                    i_args += 3;
                    break;
               }
               case 78:
               {
                    Goldilocks::copy_avx512(tmp1[(args1[i_args])], tmp1[args1[i_args + 1]]);
                    i_args += 2;
                    break;
               }
               case 79:
               {
                    Goldilocks::copy_avx512(tmp1[(args1[i_args])], &params.pols[args1[i_args + 1] + i * args1[i_args + 2]], args1[i_args + 2]);
                    i_args += 3;
                    break;
               }
               case 80:
               {
                    for (uint64_t j = 0; j < AVX512_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args + 1] + (((i + j) + args1[i_args + 2]) % args1[i_args + 3]) * args1[i_args + 4];
                    }
                    Goldilocks::copy_avx512(tmp1[args1[i_args]], &params.pols[0], offsets1);
                    i_args += 5;
                    break;
               }
               case 81:
               {
                    Goldilocks::copy_avx512(tmp1[(args1[i_args])], Goldilocks::fromU64(args1[i_args + 1]));
                    i_args += 2;
                    break;
               }
               case 82:
               {
                    Goldilocks::copy_avx512(tmp1[(args1[i_args])], &params.pConstPols->getElement(args1[i_args + 1], i), numpols);
                    i_args += 2;
                    break;
               }
               case 83:
               {
                    for (uint64_t j = 0; j < AVX512_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args + 1] + (((i + j) + args1[i_args + 2]) % args1[i_args + 3]) * numpols;
                    }
                    Goldilocks::copy_avx512(tmp1[(args1[i_args])], &params.pConstPols->getElement(0, 0), offsets1);
                    i_args += 4;
                    break;
               }
               case 84:
               {
                    for (uint64_t j = 0; j < AVX512_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args + 2] + (((i + j) + args1[i_args + 3]) % args1[i_args + 4]) * args1[i_args + 5];
                    }
                    Goldilocks::add_avx512(tmp1[(args1[i_args])], tmp1[args1[i_args + 1]], &params.pols[0], offsets1);
                    i_args += 6;
                    break;
               }
               case 85:
               {
                    for (uint64_t j = 0; j < AVX512_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args + 1] + (((i + j) + args1[i_args + 2]) % args1[i_args + 3]) * args1[i_args + 4];
                    }
                    Goldilocks::mul_avx512(tmp1[(args1[i_args])], &params.pols[0], Goldilocks::fromU64(args1[i_args + 5]), offsets1);
                    i_args += 6;
                    break;
               }
               case 86:
               {
                    Goldilocks::add_avx512(&params.pols[args1[i_args] + i * args1[i_args + 1]], args1[i_args + 1], tmp1[args1[i_args + 2]], tmp1[args1[i_args + 3]]);
                    i_args += 4;
                    break;
               }
               case 87:
               {
                    Goldilocks::add_avx512(&params.pols[args1[i_args] + i * args1[i_args + 1]], args1[i_args + 1], tmp1[args1[i_args + 2]], &params.pols[args1[i_args + 3] + i * args1[i_args + 4]], args1[i_args + 4]);
                    i_args += 5;
                    break;
               }
               case 91:
               {
                    //Goldilocks::copy_avx512(&params.pols[args1[i_args] + i * args1[i_args + 1]], args1[i_args + 1], &params.pConstPols->getElement(args1[i_args + 2], i));
                    i_args += 3;
                    break;
               }
               case 92:
               {
                    Goldilocks::sub_avx512(&params.pols[args1[i_args] + i * args1[i_args + 1]], args1[i_args + 1], tmp1[args1[i_args + 2]], tmp1[args1[i_args + 3]]);
                    i_args += 4;
                    break;
               }
               case 93:
               {
                    Goldilocks::sub_avx512(&params.pols[args1[i_args] + i * args1[i_args + 1]], args1[i_args + 1], Goldilocks::fromU64(args1[i_args + 2]), tmp1[args1[i_args + 3]]);
                    i_args += 4;
                    break;
               }
               case 94:
               {
                    Goldilocks::mul_avx512(&params.pols[args1[i_args] + i * args1[i_args + 1]], args1[i_args + 1], tmp1[args1[i_args + 2]], tmp1[args1[i_args + 3]]);
                    i_args += 4;
                    break;
               }
               case 95:
               {
                    Goldilocks::mul_avx512(&params.pols[args1[i_args] + i * args1[i_args + 1]], args1[i_args + 1], &params.pols[args1[i_args + 2] + i * args1[i_args + 3]], tmp1[args1[i_args + 4]], args1[i_args + 3]);
                    i_args += 5;
                    break;
               }
               case 96:
               {
                    Goldilocks::mul_avx512(&params.pols[args1[i_args] + i * args1[i_args + 1]], args1[i_args + 1], tmp1[args1[i_args + 2]], &params.pConstPols->getElement(args1[i_args + 3], i), numpols);
                    i_args += 4;
                    break;
               }
               case 99:
               {
                    assert(0); // code not used
                    i_args += 4;
                    break;
               }
               case 100:
               {
                    Goldilocks::copy_avx512(&params.pols[args1[i_args] + i * args1[i_args + 1]], args1[i_args + 1], tmp1[(args1[i_args + 2])]);
                    i_args += 3;
                    break;
               }
               case 101:
               {
                    for (uint64_t j = 0; j < AVX512_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args] + (((i + j) + args1[i_args + 1]) % args1[i_args + 2]) * args1[i_args + 3];
                    }
                    Goldilocks::add_avx512(&params.pols[0], offsets1, tmp1[args1[i_args + 4]], tmp1[args1[i_args + 5]]);
                    i_args += 6;
                    break;
               }
               case 102:
               {
                    for (uint64_t j = 0; j < AVX512_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args] + (((i + j) + args1[i_args + 1]) % args1[i_args + 2]) * args1[i_args + 3];
                    }
                    Goldilocks::add_avx512(&params.pols[0], offsets1, tmp1[args1[i_args + 4]], &params.pols[args1[i_args + 5] + i * args1[i_args + 6]], args1[i_args + 6]);
                    i_args += 7;
                    break;
               }
               case 106:
               {
                    for (uint64_t j = 0; j < AVX512_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args] + (((i + j) + args1[i_args + 1]) % args1[i_args + 2]) * args1[i_args + 3];
                    }
                    Goldilocks::sub_avx512(&params.pols[0], offsets1, tmp1[args1[i_args + 4]], tmp1[args1[i_args + 5]]);
                    i_args += 6;
                    break;
               }
               case 107:
               {
                    for (uint64_t j = 0; j < AVX512_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args] + (((i + j) + args1[i_args + 1]) % args1[i_args + 2]) * args1[i_args + 3];
                    }
                    Goldilocks::sub_avx512(&params.pols[0], offsets1, Goldilocks::fromU64(args1[i_args + 4]), tmp1[args1[i_args + 5]]);
                    i_args += 6;
                    break;
               }
               case 108:
               {
                    for (uint64_t j = 0; j < AVX512_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args] + (((i + j) + args1[i_args + 1]) % args1[i_args + 2]) * args1[i_args + 3];
                    }
                    Goldilocks::mul_avx512(&params.pols[0], offsets1, tmp1[args1[i_args + 4]], tmp1[args1[i_args + 5]]);
                    i_args += 6;
                    break;
               }
               case 109:
               {
                    for (uint64_t j = 0; j < AVX512_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args] + (((i + j) + args1[i_args + 1]) % args1[i_args + 2]) * args1[i_args + 3];
                    }
                    Goldilocks::mul_avx512(&params.pols[0], offsets1, &params.pols[args1[i_args + 4] + i * args1[i_args + 5]], tmp1[args1[i_args + 6]], args1[i_args + 5]);
                    i_args += 7;
                    break;
               }
               case 110:
               {
                    for (uint64_t j = 0; j < AVX512_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args] + (((i + j) + args1[i_args + 1]) % args1[i_args + 2]) * args1[i_args + 3];
                    }
                    Goldilocks::mul_avx512(&params.pols[0], offsets1, tmp1[args1[i_args + 4]], &params.pConstPols->getElement(args1[i_args + 5], i), numpols);
                    i_args += 6;
                    break;
               }
               case 111:
               {
                    for (uint64_t j = 0; j < AVX512_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args] + (((i + j) + args1[i_args + 1]) % args1[i_args + 2]) * args1[i_args + 3];
                         offsets2[j] = args1[i_args + 4] + (((i + j) + args1[i_args + 5]) % args1[i_args + 6]) * numpols;
                    }
                    Goldilocks::mul_avx512(&params.pols[0], offsets1, &params.pConstPols->getElement(0, 0), tmp1[args1[i_args + 7]], offsets2);
                    i_args += 8;
                    break;
               }
               case 113:
               {
                    for (uint64_t j = 0; j < AVX512_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args] + (((i + j) + args1[i_args + 1]) % args1[i_args + 2]) * args1[i_args + 3];
                    }
                    Goldilocks::copy_avx512(&params.pols[0], offsets1, tmp1[(args1[i_args + 4])]);
                    i_args += 5;
                    break;
               }
               case 114:
               {
                    for (uint64_t j = 0; j < AVX512_SIZE_; ++j)
                    {
                         offsets1[j] = args1[i_args] + (((i + j) + args1[i_args + 1]) % args1[i_args + 2]) * args1[i_args + 3];
                         offsets2[j] = args1[i_args + 5] + (((i + j) + args1[i_args + 6]) % args1[i_args + 7]) * args1[i_args + 8];
                    }
                    Goldilocks::add_avx512(&params.pols[0], offsets1, tmp1[args1[i_args + 4]], &params.pols[0], offsets2);
                    i_args += 9;
                    break;
               }
               default:
               {
                    std::cout << " Wrong operation in step42ns_first!" << std::endl;
                    exit(1);
               }
               }
          }
          if (i_args != NARGS_)
               std::cout << " " << i_args << " - " << NARGS_ << std::endl;
          assert(i_args == NARGS_);
          // delete (tmp1);
     }
}
#endif