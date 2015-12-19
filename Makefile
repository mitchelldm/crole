CC := cc

C_FILES := $(wildcard src/*.c)
OBJ_FILES := $(addprefix obj/,$(notdir $(C_FILES:.c=.o)))

CC_FLAGS := -MMD -Iinclude --std=c11 -Wall -Wpedantic -Wextra

ifeq ($(mode), release)
	CC_FLAGS += -O3
else
	CC_FLAGS += -O0
endif

$(shell mkdir -p obj)

all: libcrole.a

obj/%.o: src/%.c
	$(CC) $(CC_FLAGS) -c -o $@ $<

libcrole.a: $(OBJ_FILES)
	ar rcs $@ $^

.PHONY: clean
clean:
	-rm obj/*.*
	-rm libcrole.a
	-$(MAKE) -C test clean

.PHONY: test
test: all
	$(MAKE) -C test 
   
-include $(OBJ_FILES:.o=.d)
