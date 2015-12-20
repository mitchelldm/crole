# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

CC := cc

C_FILES := $(wildcard src/*.c)
OBJ_FILES := $(addprefix obj/,$(notdir $(C_FILES:.c=.o)))

CC_FLAGS := -MMD -I../include --std=c11 -Wall -Wpedantic -Wextra -O0 -g
LD_FLAGS := -L../ -lcrole

$(shell mkdir -p obj)

all: test-crole

obj/%.o: src/%.c
	$(CC) $(CC_FLAGS) -c -o $@ $<

test-crole: $(OBJ_FILES)
	$(CC) $^ $(LD_FLAGS) -o $@

.PHONY: clean
clean:
	-rm obj/*.*
	-rm test-crole

-include $(OBJ_FILES:.o=.d)
