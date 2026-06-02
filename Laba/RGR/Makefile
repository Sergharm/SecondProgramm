# Makefile (адаптированный фрагмент для Windows/MinGW)
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -pedantic
# В Windows DLL не требуют префикса lib, но наш код в main.cpp это учитывает через макросы
LDFLAGS = 

TARGET_MAIN = cryptum.exe
TARGET_TEST = crypto_test.exe
LIB_AFFINE = affine.dll
LIB_PLAYFAIR = playfair.dll

SRC_MAIN = main.cpp
SRC_TEST = test_runner.cpp
SRC_AFFINE = affine_cipher.cpp
SRC_PLAYFAIR = playfair_cipher.cpp

all: $(LIB_AFFINE) $(LIB_PLAYFAIR) $(TARGET_MAIN) $(TARGET_TEST)

# Компиляция DLL
$(LIB_AFFINE): $(SRC_AFFINE) crypto_interface.h
	$(CXX) $(CXXFLAGS) -shared -o $@ $<

$(LIB_PLAYFAIR): $(SRC_PLAYFAIR) crypto_interface.h
	$(CXX) $(CXXFLAGS) -shared -o $@ $<

# Компиляция EXE
$(TARGET_MAIN): $(SRC_MAIN) crypto_interface.h
	$(CXX) $(CXXFLAGS) -o $@ $(SRC_MAIN)

$(TARGET_TEST): $(SRC_TEST) crypto_interface.h
	$(CXX) $(CXXFLAGS) -o $@ $(SRC_TEST)

clean:
	del /Q *.exe *.dll 2>nul || rm -f *.exe *.dll

.PHONY: all clean