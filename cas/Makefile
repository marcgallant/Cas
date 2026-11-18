CC = gcc
CFLAGS = -fPIC -Wall -Wextra -Werror -std=c11 -g -I utils

SRC = utils/dlist/dlist.c \
      utils/arg_parser/argument.c \
      utils/arg_parser/parser.c \
      utils/arg_parser/formatter.c \
      utils/cJSON/cJSON.c \
      configschema.c \
      plugins/build.c \
      plugins/checkout.c \
      plugins/clean.c \
      plugins/diff.c \
      plugins/dump.c \
      plugins/for_all_repos.c \
      plugins/lock.c \
      plugins/menu.c \
      plugins/shell.c \
      cas.c

OBJ = $(SRC:.c=.o)
BIN = cas

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) $(LDLIBS) -o $(BIN)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(BIN) $(OBJ)

.PHONY : all clean

