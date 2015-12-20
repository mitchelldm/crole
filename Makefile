# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

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
	-$(MAKE) -C bench clean
   
-include $(OBJ_FILES:.o=.d)
