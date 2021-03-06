# shamelessly stolen and slightly adapted from https://hiltmon.com/blog/2013/07/03/a-simple-c-plus-plus-project-structure/
# structure of makefile recommanded by user Hilton Lipschitz, indeed it is quite nice 10/10 would use again

CC := gcc
SRCDIR := src
BUILDDIR := build
TARGET := bin/tsp

SRCEXT := c
SOURCES := $(shell find $(SRCDIR) -type f)
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -Wall -g
INC := -I include

$(TARGET): $(OBJECTS) main.o
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET) "; $(CC) $^ -o $(TARGET) -lm


$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

main.o: main.c
	gcc -Wall -g -I include -c -o main.o main.c -lm

clean:
	echo " Cleaning... ";
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

cleantest:
	rm tests.o

.PHONY: clean cleantest