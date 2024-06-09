TESTS_EXEC = $(TESTS_DIR)/unit_tests

DEBUG = -g

TESTS_CFLAGS = -I$(INC_DIR) -Wall -Wextra -Werror -pedantic-errors -std=c99 $(DEBUG) 
TESTS_LDFLAGS = -L$(LIB_DIR) -lcunit

# Source and object files of project that are used for unit tests
USED_PROJECT_SRC= $(SRC_DIR)/eFile.c
USED_PROJECT_OBJ= $(USED_PROJECT_SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Source and object files of unit tests
TESTS_SRC = $(wildcard $(TESTS_DIR)/*.c)
TESTS_OBJ = $(TESTS_SRC:$(TESTS_DIR)/%.c=$(TESTS_DIR)/%.o)


# first target
tests: $(TESTS_EXEC)

$(TESTS_EXEC): $(TESTS_OBJ)
	$(CC) -o $@ $^ $(USED_PROJECT_OBJ) $(TESTS_LDFLAGS)

$(TESTS_DIR)/%.o: $(TESTS_DIR)/%.c
	$(CC) -c $< -o $@ $(TESTS_CFLAGS)

# TODO: Add .h dependencies

clean_tests:
	rm $(TESTS_OBJ) $(TESTS_EXEC)
