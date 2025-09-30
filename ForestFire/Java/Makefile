# Makefile for Java ForestFire
# to build, enter: make

# Variables
TARGET = firestarter
LIBS = Tree Forest FireSimulator Graph

SRC = $(TARGET).java
LIB_SRCS = $(addsuffix .java, $(LIBS))

CC = javac
CLASSPATH = .

# Rules
.PHONY: Fire clean

Fire: $(TARGET).class

$(TARGET).class: $(SRC) $(LIB_SRCS)
	@echo "Compiling $(TARGET)..."
	@$(CC) -deprecation -cp $(CLASSPATH) $^ \
	|| { echo "Compilation failed for $(TARGET)"; exit 1; }

clean:
	rm -f *.class *~ *# 