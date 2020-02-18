VERSION=0.1
CFLAGS=-Werror -Wno-ignored-qualifiers -Wno-missing-field-initializers -Wextra -Wall -O0 -ffunction-sections -fdata-sections -g
INCLUDES=-pthread -I./include/
LIBS=-lm -lrt
COMMON_OBJ=main.o
NAME=mgm

PREFIX?=/usr/local
INSTALL_BIN=$(PREFIX)/bin/


all: bin

clean:
	rm -f *.o
	rm -f $(NAME)

%.o: ./src/%.c
	$(CC) $(CFLAGS) $(LIB_INCLUDES) $(INCLUDES) -c -fPIC $<

bin: $(NAME)
$(NAME): $(COMMON_OBJ)
	$(CC) $(CLAGS) $(LIB_INCLUDES) $(INCLUDES) -o $(NAME) $^ $(LIBS)

install: all
	@mkdir -p $(INSTALL_BIN)
	@install $(NAME) $(INSTALL_LIB)$(NAME)
	@echo "$(NAME) installed to $(PREFIX) :^)."

uninstall:
	rm -rf $(INSTALL_BIN)$(NAME)
