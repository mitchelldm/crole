CC := cc

C_FILES := $(wildcard src/*.c)
OBJ_FILES := $(addprefix obj/,$(notdir $(C_FILES:.c=.o)))

CC_FLAGS := -MMD -Iinclude --std=c11 -Wall -pedantic

ifeq ($(mode), release)
	CC_FLAGS += -Ofast
else
	CC_FLAGS += -Og
endif

$(shell mkdir -p obj)

all: libcrole.a

obj/%.o: src/%.c
	$(CC) $(CC_FLAGS) -c -o $@ $<

libcrole.a: $(OBJ_FILES)
	ar rcs $@ $^

.PHONY: clean
clean:
	rm obj/*.*
	rm libcrole.a
   
-include $(OBJ_FILES:.o=.d)
