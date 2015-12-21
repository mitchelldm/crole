# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

CC := cc
CXX := c++

C_FILES := $(wildcard src/*.c)
CXX_FILES := $(wildcard src/*.cpp)

BENCH_C := $(addprefix bin/,$(notdir $(C_FILES:.c=.benchc)))
BENCH_CXX := $(addprefix bin/,$(notdir $(CXX_FILES:.cpp=.benchcpp)))


C_FLAGS := -I../include -Wall -Wpedantic -Wextra -O3 -g
CC_FLAGS := --std=c11
CXX_FLAGS := --std=c++14
LD_FLAGS := -L../ -lcrole

$(shell mkdir -p bin)

all: $(BENCH_C) $(BENCH_CXX)

bin/%.benchc: src/%.c
	$(CC) $< $(LD_FLAGS) $(CC_FLAGS) $(C_FLAGS) -o $@

bin/%.benchcpp: src/%.cpp
	$(CXX) $(CXX_FLAGS) $(C_FLAGS) $(LD_FLAGS) -o $@ $<

.PHONY: clean
clean:
	-rm bin/*.*
