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


#include <iostream>
using namespace std;

float CalcularPi () {
    float pi=0;
    float t=1;
    int s=1;

    while (abs(4*pi-3.141592) >=0.0000001) // Para 6 decimales 
    {
        pi+=s*(1/t); // Se van sumando valores a pi
        s*= -1; // Se va cambiando el signo
        t+= 2; // Se va sumando de a 2 el denominador, manteniendo la imparidad del mismo
        }
    return 4*pi;
    }

    int main () {
        float pi= CalcularPi();
        cout.precision (7);
        cout<<"El valor de pi es:"<<pi<<endl;
        return 0;
    }


include settings.mk

# Set compiler options
CC=gcc

# Set prerrequisites
SRCS_C += $(shell find src/ -iname "*.c")
SRCS_H += $(shell find src/ -iname "*.h")
DEPS = $(foreach STL,$(STATIC_LIBPATHS),$(STL:%=%/bin/lib$(notdir $(STL)).a))

# Set header paths to (-I)nclude
IDIRS += $(addsuffix /src,$(STATIC_LIBPATHS) .)

# Set library paths to (-L)ook
LIBDIRS = $(addsuffix /bin,$(STATIC_LIBPATHS))

# Set intermediate objects
OBJS = $(patsubst src/%.c,obj/%.o,$(SRCS_C))

# Set binary targets
BIN = bin/$(shell cd . && pwd | xargs basename)

.PHONY: all
all: debug

.PHONY: debug
debug: CFLAGS = $(CDEBUG)
debug: $(BIN)

.PHONY: release
release: CFLAGS = $(CRELEASE)
release: $(BIN)

.PHONY: clean
clean:
	-rm -rfv obj bin

$(BIN): $(OBJS) | $(dir $(BIN))
	$(CC) $(CFLAGS) -o "$@" $^ $(IDIRS:%=-I%) $(LIBDIRS:%=-L%) $(LIBS:%=-l%)

obj/%.o: src/%.c $(SRCS_H) $(DEPS) | $(dir $(OBJS))
	$(CC) $(CFLAGS) -c -o "$@" $< $(IDIRS:%=-I%)

.SECONDEXPANSION:
$(DEPS): $$(shell find $$(patsubst %bin/,%src/,$$(dir $$@)) -iname "*.c" -or -iname "*.h")
	make --no-print-directory -C $(patsubst %bin/,%,$(dir $@)) 3>&1 1>&2 2>&3 | sed -E 's,(src/)[^ ]+\.(c|h)\:,$(patsubst %bin/,%,$(dir $@))&,' 3>&2 2>&1 1>&3

$(sort $(dir $(BIN) $(OBJS))):
	mkdir -pv $@

.PHONY: start
start: debug
	valgrind --tool=none ./$(BIN) $(ARGS)

.PHONY: memcheck
memcheck: debug
	valgrind --leak-check=full $(MEMCHECK_FLAGS) ./$(BIN) $(ARGS)

.PHONY: helgrind
helgrind: debug
	valgrind --tool=helgrind $(HELGRIND_FLAGS) ./$(BIN) $(ARGS)
