# Nome do executável
EXEC = jogo

# Compilador
CC = gcc

# Flags do compilador
CFLAGS = $(shell pkg-config --cflags allegro-5 allegro_dialog-5 allegro_primitives-5 allegro_font-5 allegro_ttf-5 allegro_image-5 allegro_audio-5)

# Bibliotecas necessárias
LDFLAGS = $(shell pkg-config --libs allegro-5 allegro_dialog-5 allegro_primitives-5 allegro_font-5 allegro_ttf-5 allegro_image-5 allegro_audio-5 allegro_acodec-5) -lm


# Arquivos-fonte
SRCS = main.c game.c boss.c bullet.c collision.c enemy.c player.c

# Arquivos objeto (gerados a partir dos arquivos-fonte)
OBJS = $(SRCS:.c=.o)

# Regra padrão
all: $(EXEC)

# Regra para criar o executável
$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Regra para compilar arquivos .c em .o
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

# Limpeza dos arquivos gerados
clean:
	rm -f $(OBJS) $(EXEC)

# Limpeza total (incluindo backups)
distclean: clean
	rm -f *~ *.bak

# Declara que all, clean e distclean não são arquivos
.PHONY: all clean distclean
