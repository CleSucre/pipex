

===============TO FIX==================

valgrind --track-fds=yes --trace-children=yes ./pipex Makefile "cat" "wc -l" ou
valgrind

./pipex Makefile "cat /dev/urandom" "ls" ou

===============FIXED==================

./pipex Makefile "/bin/ls" "ls" ou
chemin absolut

./pipex Makefile "" "wc -l" ou
boucle infinie

env -i ./pipex Makefile "cat" "wc -l" ou
pas de message derreur
