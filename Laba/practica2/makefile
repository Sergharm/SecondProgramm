# Определение операционной системы
ifdef OS
   # Настройки для Windows
   RM = del /Q /S
   FIX_PATH = $(subst /,\,$1)
   TARGET_EXT = .exe
else
   # Настройки для Linux / macOS
   RM = rm -f
   FIX_PATH = $1
   TARGET_EXT =
endif

CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -Iinclude
LDFLAGS =

TARGET_APP = pr2_crypto$(TARGET_EXT)

# ДОБАВЛЕН ФАЙЛ src/utils.cpp В СПИСОК ИСХОДНИКОВ
SRCS = src/main.cpp src/menu.cpp src/modulo.cpp src/euclid.cpp src/elgamal.cpp src/fractions.cpp src/utils.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET_APP)

$(TARGET_APP): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LDFLAGS) -o $(TARGET_APP)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
# Вызываем команду удаления в зависимости от ОС
ifdef OS
	-$(RM) src\*.o $(TARGET_APP) 2>nul
else
	-$(RM) $(OBJS) $(TARGET_APP)
endif

<<<<<<< HEAD
.PHONY: all clean
=======
.PHONY: all clean
>>>>>>> d6e8ed2 (Изменения во второй практике)
