CC       ?= gcc
CFLAGS   += -W -Wall -std=gnu99 -Wextra
CFLAGS   += -D _XOPEN_SOURCE
CFLAGS   += -D LOGS
NAME      = mplayer_server
SRC       = main.c callbacks.c escape.c logs.c

all: depend $(NAME)

depend: .depend

.depend: $(SRC)
	@$(RM) .depend
	@$(CC) $(CFLAGS) -MM $^ > .depend

include .depend

OBJ     = $(SRC:.c=.o)

$(NAME): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all depend clean fclean all re
