OUT := pcf2bmf

SRC_DIR := ./src
INCLUDE_DIR := $(SRC_DIR)/include
INCLUDES := $(wildcard $(INCLUDE_DIR)/*.h)
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst %.c,%.o,$(SRCS))

CC := cc

LFLAGS := -lc

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDES)
	$(CC) -c -I$(INCLUDE_DIR) -o $@ $<
	
$(OUT): $(OBJS)
	$(CC) -o $@ $< $(LFLAGS)
	
all: $(OUT)
	 
.PHONY: clean

clean:
	rm -f $(OBJS)
	rm -f $(OUT)