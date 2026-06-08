#pragma once
#include "types.h"
#include <string>

struct ElGamalPublicKey {
    uint64 prostoeP;
    uint64 generatorG;
    uint64 otkritiyY;
};

void runElGamalMenu();
void encryptFileElGamal(const std::string& fajlIn, const std::string& fajlOut, ElGamalPublicKey pub);
void decryptFileElGamal(const std::string& fajlIn, const std::string& fajlOut, uint64 zakritiyX, uint64 modulP);