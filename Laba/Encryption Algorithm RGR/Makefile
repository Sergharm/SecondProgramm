CXX = g++
CXXFLAGS = -std=c++17 -O2

# На Windows динамические библиотеки собираются как .dll, на Linux как .so
ifeq ($(OS),Windows_NT)
    TARGET = cryptum_app.exe
    LIB_AFFINE = libaffine.dll
    LIB_PLAYFAIR = libplayfair.dll
    LDFLAGS = 
    RM = del /q /f
else
    TARGET = cryptum_app
    LIB_AFFINE = libaffine.so
    LIB_PLAYFAIR = libplayfair.so
    LDFLAGS = -ldl
    RM = rm -f
endif

all: $(TARGET) $(LIB_AFFINE) $(LIB_PLAYFAIR)

$(TARGET): main.cpp ui.cpp library_loader.cpp file_processor.cpp key_generator.cpp
	$(CXX) $(CXXFLAGS) main.cpp ui.cpp library_loader.cpp file_processor.cpp key_generator.cpp $(LDFLAGS) -o $(TARGET)

$(LIB_AFFINE): affine_cipher.cpp crypto_interface.h
	$(CXX) $(CXXFLAGS) -shared -fPIC affine_cipher.cpp -o $(LIB_AFFINE)

$(LIB_PLAYFAIR): playfair_cipher.cpp crypto_interface.h
	$(CXX) $(CXXFLAGS) -shared -fPIC playfair_cipher.cpp -o $(LIB_PLAYFAIR)

clean:
	$(RM) $(TARGET) $(LIB_AFFINE) $(LIB_PLAYFAIR)

.PHONY: all clean
