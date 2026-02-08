CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = xorlock
SRC = xorlock.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

test: $(TARGET)
	@echo "Test content" > test.txt
	./$(TARGET) -e test.txt password
	@echo ""
	@echo "Testing wrong password warning:"
	./$(TARGET) -d test.txt.enc wrongpassword
	rm -f test.txt test.txt.enc
	@echo "Test passed"
