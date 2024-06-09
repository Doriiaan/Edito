PROJECT_EXEC= $(BUILD_DIR)/edito

DEBUG= -g

PROJECT_CFLAGS= -I$(INC_DIR) -std=c99 -Wall -Wextra -Werror -pedantic-errors $(DEBUG)
PROJECT_LDFLAGS= -L$(LIB_DIR) -lncurses

# Sources and objects files
PROJECT_SRC= $(wildcard $(SRC_DIR)/*.c)
PROJECT_OBJ= $(PROJECT_SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)


# first target
project : $(PROJECT_EXEC)

$(PROJECT_EXEC) : $(PROJECT_OBJ)
	$(CC) -o $@ $^ $(PROJECT_LDFLAGS)

# %.o : %.c decomposose each .o in a target
$(BUILD_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) -c $< -o $@ $(PROJECT_CFLAGS) 

# TODO declare .h include for each c file

clean_project:
	rm $(PROJECT_OBJ) $(PROJECT_EXEC)
