CC = gcc
CFLAGS = -Wall -I./src -I./src/utils -g
SRC = src/bplus.c src/utils/visual.c
OBJ = $(SRC:.c=.o)
TEST_SRC = tests/test_main.c
TARGET = run_tests
all: $(TARGET)
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(TEST_SRC) $(OBJ)
clean:
	rm -f src/*.o src/utils/*.o $(TARGET)
test: $(TARGET)
	./$(TARGET)
