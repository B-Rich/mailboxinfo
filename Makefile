CFLAGS_LOCAL := -Wall -Wextra -O2 -g
LDLIBS_LOCAL := -lmailbox

ALLDEPS := $(MAKEFILE_LIST)

CC := gcc
RM := rm -f

COMPILE.c := $(CC) $(CFLAGS) $(EXTRACFLAGS) $(CFLAGS_LOCAL) $(CPPFLAGS) $(EXTRACPPFLAGS) $(TARGET_ARCH) -c

all: mailbox_test

mailbox_test: test.o $(ALLDEPS)
	$(LINK.o) $(OUTPUT_OPTION) test.o $(LOADLIBES) $(LDLIBS) $(LDLIBS_LOCAL)

test.o: $(ALLDEPS)

.PHONY: clean
clean:
	$(RM) mailbox_test
	$(RM) test.o
