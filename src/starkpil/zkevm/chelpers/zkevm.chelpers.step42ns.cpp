#include "goldilocks_cubic_extension.hpp"
#include "zhInv.hpp"
#include "starks.hpp"
#include "constant_pols_starks.hpp"
#include "zkevmSteps.hpp"
#include "zkevm.chelpers.step42ns.hpp"

#define NARGS_ 68020
#define NOPS_ 19127
/*
#define TMP1 tmp1[args42[i_args++]]
#define POL1 params.pols[args42[i_args++] + i * args42[i_args++]]
#define POL1_PRIME params.pols[args42[i_args++] + ((i + args42[i_args++]) % args42[i_args++]) * args42[i_args++]]
#define POL3 (Goldilocks3::Element &)params.pols[args42[i_args++] + i * args42[i_args++]]
#define POL3_PRIME (Goldilocks3::Element &)params.pols[args42[i_args++] + ((i + args42[i_args++]) % args42[i_args++]) * args42[i_args++]]
#define NUMBER Goldilocks::fromU64(args42[i_args++])
#define CONST1 params.pConstPols2ns->getElement(args42[i_args++], i)
#define CONST1_PRIME params.pConstPols2ns->getElement(args42[i_args++], i)
#define CONST3 (Goldilocks3::Element &)params.pConstPols2ns->getElement(args42[i_args++], i)
#define TMP3 tmp3[args42[i_args++]]
#define CHALLENGE (Goldilocks3::Element &)*params.challenges[args42[i_args++]]
#define PUBLIC params.publicInputs[args42[i_args++]]
#define X (Goldilocks::Element &)*params.x_2ns[i]
#define zINV params.zi.zhInv(i)
#define q2NS (Goldilocks3::Element &)(params.q_2ns[i * 3])*/

void ZkevmSteps::step42ns_first(StepsParams &params, uint64_t i)
{
     int i_args = 0;
     Goldilocks::Element tmp1[20000];
     Goldilocks3::Element tmp3[20000];

     for (int kk = 0; kk < NOPS_; ++kk)
     {
          switch (op42[kk])
          {
          case 0:
               Goldilocks::add(tmp1[args42[i_args]], tmp1[args42[i_args + 1]], tmp1[args42[i_args + 2]]);
               i_args += 3;
               break;
          case 1:
               Goldilocks::add(tmp1[args42[i_args]], tmp1[args42[i_args + 1]], params.pols[args42[i_args + 2] + i * args42[i_args + 3]]);
               i_args += 4;
               break;
          case 2:
               Goldilocks::add(tmp1[args42[i_args]], tmp1[args42[i_args + 1]], Goldilocks::fromU64(args42[i_args + 2]));
               i_args += 3;
               break;
          case 3:
               Goldilocks::add(tmp1[args42[i_args]], tmp1[args42[i_args + 1]], params.pConstPols2ns->getElement(args42[i_args + 2], i));
               i_args += 3;
               break;
          case 4:
               Goldilocks::add(tmp1[args42[i_args]], params.pols[args42[i_args + 1] + i * args42[i_args + 2]], params.pols[args42[i_args + 3] + i * args42[i_args + 4]]);
               i_args += 5;
               break;
          case 5:
               Goldilocks::add(tmp1[args42[i_args]], params.pols[args42[i_args + 1] + ((i + args42[i_args + 2]) % args42[i_args + 3]) * args42[i_args + 4]], params.pols[args42[i_args + 5] + ((i + args42[i_args + 6]) % args42[i_args + 7]) * args42[i_args + 8]]);
               i_args += 9;
               break;
          case 6:
               Goldilocks::add(tmp1[args42[i_args]], params.pols[args42[i_args + 1] + i * args42[i_args + 2]], params.pConstPols2ns->getElement(args42[i_args + 3], i));
               i_args += 4;
               break;
          case 7:
               Goldilocks::add(tmp1[args42[i_args]], params.pols[args42[i_args + 1] + i * args42[i_args + 2]], Goldilocks::fromU64(args42[i_args + 3]));
               i_args += 4;
               break;
          case 8:
               Goldilocks::add(tmp1[args42[i_args]], params.pConstPols2ns->getElement(args42[i_args + 1], i), params.pConstPols2ns->getElement(args42[i_args + 2], i));
               i_args += 3;
               break;
          case 9:
               Goldilocks::add(tmp1[args42[i_args]], params.pConstPols2ns->getElement(args42[i_args + 1], (i + args42[i_args + 2]) % args42[i_args + 3]), params.pConstPols2ns->getElement(args42[i_args + 4], (i + args42[i_args + 5]) % args42[i_args + 6]));
               i_args += 7;
               break;
          case 10:
               Goldilocks::add(tmp1[args42[i_args]], params.pConstPols2ns->getElement(args42[i_args + 1], i), Goldilocks::fromU64(args42[i_args + 2]));
               i_args += 3;
               break;
          case 11:
               Goldilocks::add(tmp1[args42[i_args]], params.pConstPols2ns->getElement(args42[i_args + 1], (i + args42[i_args + 2]) % args42[i_args + 3]), Goldilocks::fromU64(args42[i_args + 4]));
               i_args += 5;
               break;
          case 12:
               Goldilocks3::add(tmp3[args42[i_args]], tmp1[args42[i_args + 1]], tmp3[args42[i_args + 2]]);
               i_args += 3;
               break;
          case 13:
               Goldilocks3::add(tmp3[args42[i_args]], Goldilocks::fromU64(args42[i_args + 1]), (Goldilocks3::Element &)*params.challenges[args42[i_args + 2]]);
               i_args += 3;
               break;
          case 14:
               Goldilocks3::add(tmp3[args42[i_args]], tmp1[args42[i_args + 1]], (Goldilocks3::Element &)*params.challenges[args42[i_args + 2]]);
               i_args += 3;
               break;
          case 15:
               Goldilocks3::add(tmp3[args42[i_args]], params.pols[args42[i_args + 1] + i * args42[i_args + 2]], tmp3[args42[i_args + 3]]);
               i_args += 4;
               break;
          case 16:
               Goldilocks3::add(tmp3[args42[i_args]], params.pols[args42[i_args + 1] + i * args42[i_args + 2]], (Goldilocks3::Element &)*params.challenges[args42[i_args + 3]]);
               i_args += 4;
               break;
          case 17:
               Goldilocks3::add(tmp3[args42[i_args]], tmp3[args42[i_args + 1]], tmp3[args42[i_args + 2]]);
               i_args += 3;
               break;
          case 18:
               Goldilocks3::add(tmp3[args42[i_args]], tmp3[args42[i_args + 1]], (Goldilocks3::Element &)*params.challenges[args42[i_args + 2]]);
               i_args += 3;
               break;
          case 19:
               Goldilocks3::add(tmp3[args42[i_args]], (Goldilocks3::Element &)params.pols[args42[i_args + 1] + i * args42[i_args + 2]], tmp3[args42[i_args + 3]]);
               i_args += 4;
               break;
          case 20:
               Goldilocks3::add(tmp3[args42[i_args]], (Goldilocks3::Element &)params.pols[args42[i_args + 1] + i * args42[i_args + 2]], (Goldilocks3::Element &)*params.challenges[args42[i_args + 3]]);
               i_args += 4;
               break;
          case 21:
               Goldilocks::sub(tmp1[args42[i_args]], tmp1[args42[i_args + 1]], tmp1[args42[i_args + 2]]);
               i_args += 3;
               break;
          case 22:
               Goldilocks::sub(tmp1[args42[i_args]], tmp1[args42[i_args + 1]], params.pols[args42[i_args + 2] + i * args42[i_args + 3]]);
               i_args += 4;
               break;
          case 23:
               Goldilocks::sub(tmp1[args42[i_args]], tmp1[args42[i_args + 1]], params.pols[args42[i_args + 2] + ((i + args42[i_args + 3]) % args42[i_args + 4]) * args42[i_args + 5]]);
               i_args += 6;
               break;
          case 24:
               Goldilocks::sub(tmp1[args42[i_args]], params.pols[args42[i_args + 1] + i * args42[i_args + 2]], tmp1[args42[i_args + 3]]);
               i_args += 4;
               break;
          case 25:
               Goldilocks::sub(tmp1[args42[i_args]], params.pols[args42[i_args + 1] + ((i + args42[i_args + 2]) % args42[i_args + 3]) * args42[i_args + 4]], tmp1[args42[i_args + 5]]);
               i_args += 6;
               break;
          case 26:
               Goldilocks::sub(tmp1[args42[i_args]], tmp1[args42[i_args + 1]], Goldilocks::fromU64(args42[i_args + 2]));
               i_args += 3;
               break;
          case 27:
               Goldilocks::sub(tmp1[args42[i_args]], Goldilocks::fromU64(args42[i_args + 1]), tmp1[args42[i_args + 2]]);
               i_args += 3;
               break;
          case 28:
               Goldilocks::sub(tmp1[args42[i_args]], params.pols[args42[i_args + 1] + i * args42[i_args + 2]], Goldilocks::fromU64(args42[i_args + 3]));
               i_args += 4;
               break;
          case 29:
               Goldilocks::sub(tmp1[args42[i_args]], params.pols[args42[i_args + 1] + ((i + args42[i_args + 2]) % args42[i_args + 3]) * args42[i_args + 4]], Goldilocks::fromU64(args42[i_args + 5]));
               i_args += 6;
               break;
          case 30:
               Goldilocks::sub(tmp1[args42[i_args]], Goldilocks::fromU64(args42[i_args + 1]), params.pols[args42[i_args + 2] + i * args42[i_args + 3]]);
               i_args += 4;
               break;
          case 31:
               Goldilocks::sub(tmp1[args42[i_args]], Goldilocks::fromU64(args42[i_args + 1]), params.pols[args42[i_args + 2] + ((i + args42[i_args + 3]) % args42[i_args + 4]) * args42[i_args + 5]]);
               i_args += 6;
               break;
          case 32:
               Goldilocks::sub(tmp1[args42[i_args]], Goldilocks::fromU64(args42[i_args + 1]), params.pConstPols2ns->getElement(args42[i_args + 2], i));
               i_args += 3;
               break;
          case 33:
               Goldilocks::sub(tmp1[args42[i_args]], Goldilocks::fromU64(args42[i_args + 1]), params.pConstPols2ns->getElement(args42[i_args + 2], (i + args42[i_args + 3]) % args42[i_args + 4]));
               i_args += 5;
               break;
          case 34:
               Goldilocks::sub(tmp1[args42[i_args]], params.pols[args42[i_args + 1] + i * args42[i_args + 2]], params.publicInputs[args42[i_args + 3]]);
               i_args += 4;
               break;
          case 35:
               Goldilocks::sub(tmp1[args42[i_args]], params.pols[args42[i_args + 1] + ((i + args42[i_args + 2]) % args42[i_args + 3]) * args42[i_args + 4]], params.pols[args42[i_args + 5] + i * args42[i_args + 6]]);
               i_args += 7;
               break;
          case 36:
               Goldilocks::sub(tmp1[args42[i_args]], params.pols[args42[i_args + 1] + i * args42[i_args + 2]], params.pols[args42[i_args + 3] + ((i + args42[i_args + 4]) % args42[i_args + 5]) * args42[i_args + 6]]);
               i_args += 7;
               break;
          case 37:
               Goldilocks::sub(tmp1[args42[i_args]], params.pols[args42[i_args + 1] + i * args42[i_args + 2]], params.pols[args42[i_args + 3] + i * args42[i_args + 4]]);
               i_args += 5;
               break;
          case 38:
               Goldilocks::sub(tmp1[args42[i_args]], params.pols[args42[i_args + 1] + ((i + args42[i_args + 2]) % args42[i_args + 3]) * args42[i_args + 4]], params.pols[args42[i_args + 5] + ((i + args42[i_args + 6]) % args42[i_args + 7]) * args42[i_args + 8]]);
               i_args += 9;
               break;
          case 39:
               Goldilocks::sub(tmp1[args42[i_args]], params.pConstPols2ns->getElement(args42[i_args + 1], i), params.pols[args42[i_args + 2] + i * args42[i_args + 3]]);
               i_args += 4;
               break;
          case 40:
               Goldilocks::sub(tmp1[args42[i_args]], tmp1[args42[i_args + 1]], params.pConstPols2ns->getElement(args42[i_args + 2], i));
               i_args += 3;
               break;
          case 41:
               Goldilocks3::sub(tmp3[args42[i_args]], (Goldilocks3::Element &)params.pols[args42[i_args + 1] + i * args42[i_args + 2]], Goldilocks::fromU64(args42[i_args + 3]));
               i_args += 4;
               break;
          case 42:
               Goldilocks3::sub(tmp3[args42[i_args]], tmp3[args42[i_args + 1]], tmp3[args42[i_args + 2]]);
               i_args += 3;
               break;
          case 43:
               Goldilocks3::sub(tmp3[args42[i_args]], tmp3[args42[i_args + 1]], (Goldilocks3::Element &)*params.challenges[args42[i_args + 2]]);
               i_args += 3;
               break;
          case 44:
               Goldilocks3::sub(tmp3[args42[i_args]], tmp3[args42[i_args + 1]], (Goldilocks3::Element &)params.pols[args42[i_args + 2] + i * args42[i_args + 3]]);
               i_args += 4;
               break;
          case 45:
               Goldilocks::mul(tmp1[args42[i_args]], tmp1[args42[i_args + 1]], tmp1[args42[i_args + 2]]);
               i_args += 3;
               break;
          case 46:
               Goldilocks::mul(tmp1[args42[i_args]], Goldilocks::fromU64(args42[i_args + 1]), tmp1[args42[i_args + 2]]);
               i_args += 3;
               break;
          case 47:
               Goldilocks::mul(tmp1[args42[i_args]], params.pols[args42[i_args + 1] + i * args42[i_args + 2]], tmp1[args42[i_args + 3]]);
               i_args += 4;
               break;
          case 48:
               Goldilocks::mul(tmp1[args42[i_args]], params.pols[args42[i_args + 1] + ((i + args42[i_args + 2]) % args42[i_args + 3]) * args42[i_args + 4]], tmp1[args42[i_args + 5]]);
               i_args += 6;
               break;
          case 49:
               Goldilocks::mul(tmp1[args42[i_args]], tmp1[args42[i_args + 1]], params.pConstPols2ns->getElement(args42[i_args + 2], i));
               i_args += 3;
               break;
          case 50:
               Goldilocks::mul(tmp1[args42[i_args]], params.pols[args42[i_args + 1] + i * args42[i_args + 2]], params.pols[args42[i_args + 3] + i * args42[i_args + 4]]);
               i_args += 5;
               break;
          case 51:
               Goldilocks::mul(tmp1[args42[i_args]], params.pols[args42[i_args + 1] + i * args42[i_args + 2]], params.pols[args42[i_args + 3] + ((i + args42[i_args + 4]) % args42[i_args + 5]) * args42[i_args + 6]]);
               i_args += 7;
               break;
          case 52:
               Goldilocks::mul(tmp1[args42[i_args]], params.pols[args42[i_args + 1] + ((i + args42[i_args + 2]) % args42[i_args + 3]) * args42[i_args + 4]], params.pols[args42[i_args + 5] + ((i + args42[i_args + 6]) % args42[i_args + 7]) * args42[i_args + 8]]);
               i_args += 9;
               break;
          case 53:
               Goldilocks::mul(tmp1[args42[i_args]], Goldilocks::fromU64(args42[i_args + 1]), params.pols[args42[i_args + 2] + i * args42[i_args + 3]]);
               i_args += 4;
               break;
          case 54:
               Goldilocks::mul(tmp1[args42[i_args]], params.pols[args42[i_args + 1] + i * args42[i_args + 2]], params.pConstPols2ns->getElement(args42[i_args + 3], i));
               i_args += 4;
               break;
          case 55:
               Goldilocks::mul(tmp1[args42[i_args]], params.pols[args42[i_args + 1] + ((i + args42[i_args + 2]) % args42[i_args + 3]) * args42[i_args + 4]], params.pConstPols2ns->getElement(args42[i_args + 5], i));
               i_args += 6;
               break;
          case 56:
               Goldilocks::mul(tmp1[args42[i_args]], tmp1[args42[i_args + 1]], params.pols[args42[i_args + 2] + i * args42[i_args + 3]]);
               i_args += 4;
               break;
          case 57:
               Goldilocks::mul(tmp1[args42[i_args]], tmp1[args42[i_args + 1]], params.pols[args42[i_args + 2] + ((i + args42[i_args + 3]) % args42[i_args + 4]) * args42[i_args + 5]]);
               i_args += 6;
               break;
          case 58:
               Goldilocks::mul(tmp1[args42[i_args]], params.pConstPols2ns->getElement(args42[i_args + 1], i), tmp1[args42[i_args + 2]]);
               i_args += 3;
               break;
          case 59:
               Goldilocks3::mul(tmp3[args42[i_args]], tmp1[args42[i_args + 1]], (Goldilocks3::Element &)*params.challenges[args42[i_args + 2]]);
               i_args += 3;
               break;
          case 60:
               Goldilocks3::mul(tmp3[args42[i_args]], params.pConstPols2ns->getElement(args42[i_args + 1], i), tmp3[args42[i_args + 2]]);
               i_args += 3;
               break;
          case 61:
               Goldilocks3::mul(tmp3[args42[i_args]], tmp1[args42[i_args + 1]], tmp3[args42[i_args + 2]]);
               i_args += 3;
               break;
          case 62:
               Goldilocks3::mul(tmp3[args42[i_args]], params.pols[args42[i_args + 1] + i * args42[i_args + 2]], (Goldilocks3::Element &)*params.challenges[args42[i_args + 3]]);
               i_args += 4;
               break;
          case 63:
               Goldilocks3::mul(tmp3[args42[i_args]], params.pols[args42[i_args + 1] + ((i + args42[i_args + 2]) % args42[i_args + 3]) * args42[i_args + 4]], (Goldilocks3::Element &)*params.challenges[args42[i_args + 5]]);
               i_args += 6;
               break;
          case 64:
               Goldilocks3::mul(tmp3[args42[i_args]], params.pols[args42[i_args + 1] + i * args42[i_args + 2]], tmp3[args42[i_args + 3]]);
               i_args += 4;
               break;
          case 65:
               Goldilocks3::mul(tmp3[args42[i_args]], params.pols[args42[i_args + 1] + ((i + args42[i_args + 2]) % args42[i_args + 3]) * args42[i_args + 4]], tmp3[args42[i_args + 5]]);
               i_args += 6;
               break;
          case 66:
               Goldilocks3::mul(tmp3[args42[i_args]], Goldilocks::fromU64(args42[i_args + 1]), (Goldilocks3::Element &)*params.challenges[args42[i_args + 2]]);
               i_args += 3;
               break;
          case 67:
               Goldilocks3::mul(tmp3[args42[i_args]], (Goldilocks::Element &)*params.x_2ns[i], (Goldilocks3::Element &)*params.challenges[args42[i_args + 1]]);
               i_args += 2;
               break;
          case 68:
               Goldilocks3::mul(tmp3[args42[i_args]], (Goldilocks::Element &)*params.x_2ns[i], tmp3[args42[i_args + 1]]);
               i_args += 2;
               break;
          case 69:
               Goldilocks3::mul((Goldilocks3::Element &)(params.q_2ns[i * 3]), params.zi.zhInv(i), tmp3[args42[i_args]]);
               i_args += 1;
               break;
          case 70:
               Goldilocks3::mul(tmp3[args42[i_args]], (Goldilocks3::Element &)*params.challenges[args42[i_args + 1]], tmp3[args42[i_args + 2]]);
               i_args += 3;
               break;
          case 71:
               Goldilocks3::mul(tmp3[args42[i_args]], tmp3[args42[i_args + 1]], tmp3[args42[i_args + 2]]);
               i_args += 3;
               break;
          case 72:
               Goldilocks3::mul(tmp3[args42[i_args]], (Goldilocks3::Element &)params.pols[args42[i_args + 1] + i * args42[i_args + 2]], (Goldilocks3::Element &)params.pols[args42[i_args + 3] + i * args42[i_args + 4]]);
               i_args += 5;
               break;
          case 73:
               Goldilocks3::mul(tmp3[args42[i_args]], (Goldilocks3::Element &)params.pols[args42[i_args + 1] + ((i + args42[i_args + 2]) % args42[i_args + 3]) * args42[i_args + 4]], (Goldilocks3::Element &)*params.challenges[args42[i_args + 5]]);
               i_args += 6;
               break;
          case 74:
               Goldilocks3::mul(tmp3[args42[i_args]], (Goldilocks3::Element &)params.pols[args42[i_args + 1] + ((i + args42[i_args + 2]) % args42[i_args + 3]) * args42[i_args + 4]], tmp3[args42[i_args + 5]]);
               i_args += 6;
               break;
          case 75:
               Goldilocks3::mul(tmp3[args42[i_args]], (Goldilocks3::Element &)params.pols[args42[i_args + 1] + i * args42[i_args + 2]], tmp3[args42[i_args + 3]]);
               i_args += 4;
               break;
          case 76:
               Goldilocks3::mul(tmp3[args42[i_args]], (Goldilocks3::Element &)params.pols[args42[i_args + 1] + i * args42[i_args + 2]], (Goldilocks3::Element &)*params.challenges[args42[i_args + 3]]);
               i_args += 4;
               break;
          case 77:
               Goldilocks3::mul(tmp3[args42[i_args]], (Goldilocks3::Element &)params.pols[args42[i_args + 1] + ((i + args42[i_args + 2]) % args42[i_args + 3]) * args42[i_args + 4]], (Goldilocks3::Element &)params.pols[args42[i_args + 5] + i * args42[i_args + 6]]);
               i_args += 7;
               break;
          case 78:
               Goldilocks::copy(tmp1[args42[i_args]], tmp1[args42[i_args + 1]]);
               i_args += 2;
               break;
          case 79:
               Goldilocks::copy(tmp1[args42[i_args]], params.pols[args42[i_args + 1] + i * args42[i_args + 2]]);
               i_args += 3;
               break;
          case 80:
               Goldilocks::copy(tmp1[args42[i_args]], params.pols[args42[i_args + 1] + ((i + args42[i_args + 2]) % args42[i_args + 3]) * args42[i_args + 4]]);
               i_args += 5;
               break;
          case 81:
               Goldilocks::copy(tmp1[args42[i_args]], Goldilocks::fromU64(args42[i_args + 1]));
               i_args += 2;
               break;
          case 82:
               Goldilocks::copy(tmp1[args42[i_args]], params.pConstPols2ns->getElement(args42[i_args + 1], i));
               i_args += 2;
               break;
          case 83:
               Goldilocks::copy(tmp1[args42[i_args]], params.pConstPols2ns->getElement(args42[i_args + 1], (i + args42[i_args + 2]) % args42[i_args + 3]));
               i_args += 4;
               break;
          default:
               std::cout << " Wrong operation in step42ns_first!" << std::endl;
               exit(1); // rick, use execption
          }
     }
     assert(i_args == NARGS_);
}

void ZkevmSteps::step42ns_i(StepsParams &params, uint64_t i)
{
}

void ZkevmSteps::step42ns_last(StepsParams &params, uint64_t i)
{
}
