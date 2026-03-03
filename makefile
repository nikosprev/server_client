# Specify the source directory and get all C source files
CC= gcc
SRC_S = Server/src
SRCS_S = $(wildcard $(SRC_S)/*.c)

SRC_C = Client/src
SRCS_C = $(wildcard $(SRC_C)/*.c)

TARGET_S = poller
TARGET_C = pollSwayer

# Specify the object directory and generate object files
OBJ_S = Server/bin
OBJS_S = $(addprefix $(OBJ_S)/,$(notdir $(SRCS_S:.c=.o)))

OBJ_C = Client/bin
OBJS_C = $(addprefix $(OBJ_C)/,$(notdir $(SRCS_C:.c=.o)))

# Default rule to build the program
all: $(TARGET_S) $(TARGET_C)
server: $(TARGET_S)
client: $(TARGET_C)


# Rule to build the program
$(TARGET_S): $(OBJS_S)
	$(CC) -g -o $(TARGET_S) $(OBJS_S) -lpthread

# Rule to compile each source file
$(OBJ_S)/%.o: $(SRC_S)/%.c
	$(CC) -c $< -o $@


$(TARGET_C): $(OBJS_C)
	$(CC) -g -o $(TARGET_C) $(OBJS_C) -lpthread

# Rule to compile each source file
$(OBJ_C)/%.o: $(SRC_C)/%.c
	$(CC) -c $< -o $@

# Clean rule to remove object files and executable
clean:
	rm -f $(OBJS_S) $(TARGET_S) $(OBJS_C) $(TARGET_C)

cleanpoll:
	rm -f $(OBJS_S) $(TARGET_S) $(OBJS_C) $(TARGET_C) $(filter-out readme.txt cleanpoll,$(wildcard *.txt))
