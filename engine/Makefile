CC      := gcc
CFLAGS  := -Wall -Wextra -Werror -g
OBJDIR  := build
SRCDIR  := src
BINDIR  := bin
TARGET  := $(BINDIR)/db
VFlags  := --leak-check=full

SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))
DEPENDS := $(OBJECTS:.o=.d)

all: $(TARGET)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(OBJECTS)  -o $@

$(OBJDIR) $(BINDIR):
	mkdir -p $@

memory:
	valgrind ${TARGET} ${VFlags}

run:
	./${TARGET}

clean:
	rm -rf $(OBJDIR) $(BINDIR)

-include $(DEPENDS)

.PHONY: all clean memory run
