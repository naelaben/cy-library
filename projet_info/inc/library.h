#ifndef LIBRARY_H
#define LIBRARY_H

#include "includes.h"
#include "macro.h"

/*
Structure representing a user with his:
- username
- the number of books borrowed
- and his role.
*/
typedef struct s_user
{
	char *username;
	int number_of_books;
	int role;
} t_user;

/*
Structure containing the entire data of the library, which are retrieved from the files.
Allows to have in real time the books and the borrowed books.
*/
typedef struct s_books
{
	char **names;
	char **authors;
	char **id;
	char **genre;
	int *is_borrow;
	char **borrow_name;
	char **borrow_hour;
	int count;
} t_books;

int user_authentification(t_user *user, t_books *books);
FILE *open_user_data(char *mode);
FILE *open_borrowed_data(char *mode);
FILE *open_books_data(char *mode);
int user_connect(t_user *user);
char *get_password(char *username, t_user *user);
int user_auth_choice(t_user *user, t_books *books);
int user_exist(char *username);
int create_account(t_user *user, t_books *books);
int check_user_quit(char *str);
int user_menu(t_user *user, t_books *books);
int get_user_info(t_user *user);
void get_borrowed_books(t_user *user, t_books *books);
int check_buffer(char *buffer);
int search_menu(t_user *user, t_books *books);
int parse_books(t_books *books);
char **my_split(char const *s, char c);
void free_data(t_user *user, t_books *books);
void add_book_menu(t_books *books);
int borrow_validity(t_user *user, t_books *books);
int borrow_by_title(t_books *books, t_user *user);
int borrow_by_id(t_books *books, t_user *user);
void return_book_menu(t_user *user, t_books *books);
void print_borrowed_books(t_user *user, t_books *books, int id);

#endif