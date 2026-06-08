#pragma once
#include <cstdint>
#include <string>

struct ElGamalPublicKey {
    uint64_t prostoeP;
    uint64_t generatorG;
    uint64_t otkritiyY;
};

void runElGamalMenu();
void encryptFileElGamal(const std::string& fajlIn, const std::string& fajlOut, ElGamalPublicKey pub);
void decryptFileElGamal(const std::string& fajlIn, const std::string& fajlOut, uint64_t zakritiyX, uint64_t modulP);