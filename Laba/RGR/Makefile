CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -pedantic -fPIC

TARGET_MAIN = cryptum
TARGET_TEST = crypto_test
LIB_AFFINE = libaffine.so
LIB_PLAYFAIR = libplayfair.so

all: $(LIB_AFFINE) $(LIB_PLAYFAIR) $(TARGET_MAIN) $(TARGET_TEST)

$(LIB_AFFINE): affine_cipher.cpp crypto_interface.h
	$(CXX) $(CXXFLAGS) -shared -o $@ $<

$(LIB_PLAYFAIR): playfair_cipher.cpp crypto_interface.h
	$(CXX) $(CXXFLAGS) -shared -o $@ $<

$(TARGET_MAIN): main.cpp crypto_interface.h
	$(CXX) $(CXXFLAGS) -o $@ $< -ldl

$(TARGET_TEST): test_runner.cpp crypto_interface.h
	$(CXX) $(CXXFLAGS) -o $@ $< -ldl

clean:
	rm -f $(TARGET_MAIN) $(TARGET_TEST) $(LIB_AFFINE) $(LIB_PLAYFAIR)

.PHONY: all clean