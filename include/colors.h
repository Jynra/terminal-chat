#ifndef COLORS_H
# define COLORS_H

/* Codes couleurs ANSI */
# define RESET     "\033[0m"
# define BOLD      "\033[1m"
# define DIM       "\033[2m"

/* Couleurs de texte */
# define BLACK     "\033[30m"
# define RED       "\033[31m"
# define GREEN     "\033[32m"
# define YELLOW    "\033[33m"
# define BLUE      "\033[34m"
# define MAGENTA   "\033[35m"
# define CYAN      "\033[36m"
# define WHITE     "\033[37m"

/* Couleurs de fond */
# define BLACK_BG  "\033[40m"
# define RED_BG    "\033[41m"
# define GREEN_BG  "\033[42m"
# define YELLOW_BG "\033[43m"
# define BLUE_BG   "\033[44m"
# define MAGENTA_BG "\033[45m"
# define CYAN_BG   "\033[46m"
# define WHITE_BG  "\033[47m"

/* Couleurs plus vives (256 couleurs) */
# define PURPLE    "\033[38;5;135m"
# define ORANGE    "\033[38;5;208m"
# define PINK      "\033[38;5;205m"
# define LIGHT_BLUE "\033[38;5;51m"
# define GRAY      "\033[38;5;240m"

/* Caractères pour les bordures */
# define TOP_LEFT     "┌"
# define TOP_RIGHT    "┐"
# define BOTTOM_LEFT  "└"
# define BOTTOM_RIGHT "┘"
# define HORIZONTAL   "─"
# define VERTICAL     "│"

#endif