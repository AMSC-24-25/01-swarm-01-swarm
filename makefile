# Nome del compilatore
CXX = g++

# Flags per il compilatore
CXXFLAGS = -std=c++20 -Wall -Wextra -O2

# Nome del file eseguibile
TARGET = main

# File sorgenti
SRCS = main.cpp particle.hpp coordinate.hpp

# File oggetto (generati dai sorgenti)
OBJS = $(SRCS:.cpp=.o)

# Regola principale: compila l'eseguibile
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regola per compilare i file .cpp in .o
%.o: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Dipendenze specifiche (aggiungi altre se necessario)
main.o: particle.hpp
particle.o: particle.hpp coordinate.hpp
position.o: coordinate.hpp

# Regola per pulire i file generati
clean:
	rm -f $(OBJS) $(TARGET)
