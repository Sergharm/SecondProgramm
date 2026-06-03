CXX = g++
CXXFLAGS = -std=c++17 -O2 -fPIC
LDFLAGS = -ldl

all: cryptum_app libaffine.so libplayfair.so

cryptum_app: main.cpp ui.cpp library_loader.cpp file_processor.cpp key_generator.cpp
	$(CXX) $(CXXFLAGS) main.cpp ui.cpp library_loader.cpp file_processor.cpp key_generator.cpp $(LDFLAGS) -o cryptum_app

libaffine.so: affine_cipher.cpp crypto_interface.h
	$(CXX) $(CXXFLAGS) -shared affine_cipher.cpp -o libaffine.so

libplayfair.so: playfair_cipher.cpp crypto_interface.h
	$(CXX) $(CXXFLAGS) -shared playfair_cipher.cpp -o libplayfair.so

clean:
	rm -f cryptum_app libaffine.so libplayfair.so

.PHONY: all clean