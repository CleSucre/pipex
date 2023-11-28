ifeq ($(OS), Windows_NT)
	DIRSEP	= \\
	CP		= copy
	RM		= del -f
else
	DIRSEP	= /
	CP		= cp
	RM		= rm -f
endif

NAME		= pipex

SRCS		= main.c

OBJ_PATH	= obj${DIRSEP}

OBJ_NAME	= ${SRCS:%.c=%.o}

OBJS		= ${addprefix ${OBJ_PATH}, ${OBJ_NAME}}

CC			= gcc

HEAD		= .

LIBFT_DIR	= libft

CFLAGS		= -Wall -Wextra -Werror -g -I ${HEAD}

${OBJ_PATH}%.o : %.c
	mkdir -p ${@D} 2> ${DIRSEP}dev${DIRSEP}null || true
	${CC} ${CFLAGS} -o $@ -c $<

${NAME}: ${OBJS}
	make -C ${LIBFT_DIR}
	${CC} ${CFLAGS} -o ${NAME} ${OBJS} -L ${LIBFT_DIR} -lft

all: ${NAME}

clean:
	make -C ${LIBFT_DIR} clean
	${RM} -r ${OBJ_PATH} 2> ${DIRSEP}dev${DIRSEP}null || true

fclean:
	make -C ${LIBFT_DIR} fclean
	${RM} -r ${OBJ_PATH} 2> ${DIRSEP}dev${DIRSEP}null || true
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re