TARGET = my_ls

SOURCES = my_ls.c \
       ls_Rec/ls_Rec.c \
       ls_d/ls_d.c \
       basic_ls/basic_ls.c \
       ls_a/ls_a.c \
       ls_Alm/ls_Alm.c \
       ls_t/ls_t.c \
       ls_r/ls_r.c \
       ls_l/ls_l.c \
       ls_1/ls_1.c \
       functions/flag_management.c \
       functions/common_functions.c

OBJS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	gcc -o $@ $(OBJS) -lm -Wall -Wextra -Werror

%.o: %.c
	gcc -c $< -o $@ -Wall -Wextra -Werror

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(TARGET)

re: fclean all

#one line: gcc -g -o my_ls my_ls.c ls_Rec/ls_Rec.c ls_d/ls_d.c basic_ls/basic_ls.c ls_a/ls_a.c ls_Alm/ls_Alm.c ls_t/ls_t.c ls_r/ls_r.c ls_l/ls_l.c ls_1/ls_1.c ./functions/flag_management.c ./functions/common_functions.c -lm -Wall -Wextra -Werror