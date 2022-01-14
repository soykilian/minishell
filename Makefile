NAME	= minishell

CC		= gcc

CFLAGS	= -Wall -Werror -Wextra -D BUFFER_SIZE=32 -g3 -fsanitize=address

SRCS_MAIN	= main.c

SRCS_PARSER	= tokens.c classify_tokens.c utils.c

SRCS	= $(SRCS_MAIN) \
		  $(addprefix parser/, $(SRCS_PARSER))

OBJS	= ${SRCS:.c=.o}

SRCS_DIR = src
OBJS_DIR = obj

OBJS_DIRS = $(OBJS_DIR) \
			$(addprefix $(OBJS_DIR)/, parser)

LIBFT_NAME	= libft.a

LIBFT	= lib/libft/

SRCS_PATHS 	= $(addprefix $(SRCS_DIR)/, $(SRCS))

OBJS_PATHS 	= $(addprefix $(OBJS_DIR)/, $(OBJS))

RM		= rm -rf

OS		:= $(shell uname -s)

LIB_LNK	= -L ${LIBFT}

INCLUDES = -I ./${LIBFT}inc -I ./inc

all: ${NAME}

run: all
	./minishell

${OBJS_DIR}/%.o: ${SRCS_DIR}/%.c
	${CC} ${CFLAGS} ${INCLUDES} -c $< -o $@ 

$(NAME): ${LIBFT_NAME} ${OBJS_DIRS} ${OBJS_PATHS} 
	${CC} ${CFLAGS} ${LIB_LNK} ${OBJS_PATHS} -o ${NAME} -lft -lreadline

${LIBFT_NAME} :
	make -C ${LIBFT}
	cp ${LIBFT}/${LIBFT_NAME} .

$(OBJS_DIRS):
	@mkdir -p $(OBJS_DIRS) 2> /dev/null

clean:
		@${RM} *.a ${OBJS_DIR}/*.o *.dSYM

fclean:		clean
		@${RM} ${NAME}

re:		fclean all

.PHONY:	clean re fclean all
