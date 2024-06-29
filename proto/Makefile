PROTO_EXEC= $(PROTO_DIR)/prototype

DEBUG= -g 

PROTO_CFLAGS= -I$(INC_DIR) -std=gnu99 -Wall -Wextra -Werror -pedantic-errors $(DEBUG)
PROTO_LDFLAGS= -L$(LIB_DIR) -lncurses -lpanel

# Source and object files of project that are used for unit tests
USED_PROJECT_SRC= $(SRC_DIR)/eFile.c $(SRC_DIR)/eLine.c
USED_PROJECT_OBJ= $(USED_PROJECT_SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Sources and objects files
PROTO_SRC= $(wildcard $(PROTO_DIR)/*.c)
PROTO_OBJ= $(PROTO_SRC:%.c=%.o)

# first target
proto : $(PROTO_EXEC)

$(PROTO_EXEC) : $(PROTO_OBJ) $(USED_PROJECT_OBJ)
	$(CC) -o $@ $^ $(PROTO_LDFLAGS)

# %.o : %.c decomposose each .o in a target
$(PROTO_DIR)/%.o : $(PROTO_DIR)/%.c
	$(CC) -c $< -o $@ $(PROTO_CFLAGS) 

# TODO declare .h include for each c file

clean_proto:
	-rm -f $(PROTO_OBJ) $(PROTO_EXEC)
