#include <vector>
#include <random>
#include <climits>
#include <algorithm>
#include <functional>
#include "sha256_executor.hpp"
#include "sha256_executor_test.hpp"
#include "sha256_gate.hpp"
#include "timer.hpp"

void SHA256SMTest(Goldilocks &fr, Sha256Executor &executor)
{
	void *pAddress = malloc(CommitPols::pilSize());
	if (pAddress == NULL)
	{
		zklog.error("SHA256SMTest() failed calling malloc() of size=" + to_string(CommitPols::pilSize()));
		exitProcess();
	}
	CommitPols cmPols(pAddress, CommitPols::pilDegree());

	// How this test works:
	// 1. We generate numberOfSlots test vectors, each is randomByteCount random bytes.
	// 2. We hash these test vectors using the standard SHA256 reference implementation.
	// 3. We also pack them into numberOfSlots "slots" to feed the SHA256 executor and hash the slots too.
	// 4. We compare the hashes.

	const uint64_t numberOfSlots = ((SHA256GateConfig.polLength - 1) / SHA256GateConfig.slotSize);
	const uint64_t randomByteCount = 32;

	string *pHash = new string[numberOfSlots];
	string *pHashGate = new string[numberOfSlots];
	std::vector<std::vector<Goldilocks::Element>> pInput(numberOfSlots);

	cout << "Starting FE " << numberOfSlots << " slots test..." << endl;

	for (uint64_t slot = 0; slot < numberOfSlots; slot++)
	{
		// Generate the randomByteCount random bytes for the test vector.
		uint8_t randomTestVector[randomByteCount];
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, 255);
		for (size_t i = 0; i < randomByteCount; ++i)
		{
			// randomTestVector[i] = static_cast<uint8_t>(dis(gen));
			randomTestVector[i] = 0;
		}

		// Calculate the reference hash through a reference implementation of SHA256.
		std::string hashString = "";
		SHA256(randomTestVector, randomByteCount, hashString);
		pHash[slot] = hashString;

		// Create an input slot for the SHA256 executor of size SHA256GateConfig.sinRefNumber.
		std::vector<Goldilocks::Element> pInputSlot(SHA256GateConfig.sinRefNumber);

		// padded data = randomTestVector[randomByteCount] + 0x80 + paddedZeros*0x00 + randomByteCount[8]
		uint64_t paddedSizeInBitsMin = randomByteCount * 8 + 1 + 64;
		uint64_t paddedSizeInBits = ((paddedSizeInBitsMin / 512) + 1) * 512;
		uint64_t paddedSize = paddedSizeInBits / 8;
		uint64_t paddedZeros = (paddedSizeInBits - paddedSizeInBitsMin) / 8;
		uint8_t padding[64] = {0};
		u642bytes(randomByteCount * 8, &padding[56], true);
		uint64_t onePosition = 64 - 8 - paddedZeros - 1;
		padding[onePosition] = 0x80;
		uint8_t randomTestVectorPadded[paddedSize];
		for (uint64_t i = 0; i < randomByteCount; i++)
		{
			randomTestVectorPadded[i] = randomTestVector[i];
		}
		for (uint64_t i = 0; i < (paddedSize - randomByteCount); i++)
		{
			randomTestVectorPadded[randomByteCount + i] = padding[onePosition + i];
		}

		GateState S(SHA256GateConfig);
		// SHA256Gate(S, randomTestVectorPadded);

		for (uint64_t word = 0; word < 16; word++)
		{
			uint32_t wordBuffer;
			bytes2u32(randomTestVectorPadded + 4 * word, wordBuffer, true);
			vector<uint8_t> bits;
			u322bits(wordBuffer, bits);
			for (uint64_t bit = 0; bit < 32; bit++)
			{
				// Fill the bits normally from the test vector, with each bit being replaced with an equivalent
				// bit initialized as a Goldilocks finite field element.
				pInputSlot[(word * 32) + bit] = (bits[bit] != 0) ? fr.one() : fr.zero();
			}
		}
		uint32_t h[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
		for (uint64_t word = 0; word < 8; word++)
		{
			vector<uint8_t> bits;
			u322bits(h[word], bits);
			for (uint64_t bit = 0; bit < 32; bit++)
			{
				pInputSlot[512 + (word * 32) + bit] = (bits[bit] != 0) ? fr.one() : fr.zero();
			}
		}

		// Push the exector input slot into the vector of numberOfSlots slots.
		pInput[slot] = pInputSlot;
	}

	// Run the executor.
	TimerStart(SHA256_SM_EXECUTOR_FE);
	executor.execute(pInput, cmPols.Sha256);
	TimerStopAndLog(SHA256_SM_EXECUTOR_FE);

	for (uint64_t slot = 0; slot < numberOfSlots; slot++)
	{
		// For each slot, we must extract the output of the executor bit by bit.
		// Each bit is represented by a polynomial. We must get these bits by specifying their position in
		// the executor's output buffer for this slot, which starts at the distance of soutRef0.
		// Each output bit is separated by a multiple of SHA256GateConfig.soutRefDistance.
		// Once we get the polynomial, we AND it with 1 to extract the bit.
		uint32_t h[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
		for (uint64_t i = 0; i < 8; i++)
		{
			vector<uint8_t> bits;
			for (int j = 0; j < 32; j++)
			{
				uint64_t bitIndex = SHA256GateConfig.relRef2AbsRef(SHA256GateConfig.soutRef0 + SHA256GateConfig.soutRefDistance * ((i * 32) + j), slot);
				uint64_t pol = executor.getPol(cmPols.Sha256.output, bitIndex);
				bits.push_back(((pol & uint64_t(1)) == 0) ? 0 : 1);
			}
			h[i] = h[i] + bits2u32(bits);
		}

		mpz_class hashScalar;
		hashScalar = h[0];
		hashScalar = hashScalar << 32;
		hashScalar += h[1];
		hashScalar = hashScalar << 32;
		hashScalar += h[2];
		hashScalar = hashScalar << 32;
		hashScalar += h[3];
		hashScalar = hashScalar << 32;
		hashScalar += h[4];
		hashScalar = hashScalar << 32;
		hashScalar += h[5];
		hashScalar = hashScalar << 32;
		hashScalar += h[6];
		hashScalar = hashScalar << 32;
		hashScalar += h[7];

		std::string hashScalarString = "0x" + hashScalar.get_str(16);

		// Compare the reference hash to the executor's hash,
		// both represented as hex strings.
		if (hashScalarString == pHash[slot].substr(0, 64))
		{
			cout << "Pass: slot=" << slot << " Sout=" << hashScalarString << endl;
		}
		else
		{
			cerr << "RESULT: slot=" << slot << " Sout=" << hashScalarString << " = " << pHash[slot] << " (" << pHashGate[slot] << ")" << endl;
		}
	}
	free(pAddress);
}

uint64_t SHA256SMExecutorTest(Goldilocks &fr, const Config &config)
{
	cout << "SHA256SMExecutorTest() starting" << endl;

	Sha256Executor executor(fr, config);
	SHA256SMTest(fr, executor);

	cout << "SHA256SMExecutorTest() done" << endl;
	return 0;
}
