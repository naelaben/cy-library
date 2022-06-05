#include "library.h"

/*
Checks if the user wants to return to the main menu.
Parameter:
- str -> the buffer of the user choice.
*/
int check_user_quit(char *str)
{
	if (str == NULL)
		return (-1);
	else if(str[0] != 'q')
		return (-1);
	else if (strlen(str) > 1)
		return (-1);
	return (0);
}

/*
Opens the users file in the defined mode
Parameter:
- mode -> the opening mode
*/
FILE *open_user_data(char *mode)
{
	FILE *users_data;

	users_data = fopen(USER_PATH_FILE, mode);
	return (users_data);
}

/*
Opens the borrowed books file in the defined mode
Parameter:
- mode -> the opening mode
*/
FILE *open_borrowed_data(char *mode)
{
	FILE *borrowed_data;

	borrowed_data = fopen(BORROWED_PATH_FILE, mode);
	return (borrowed_data);
}

/*
Opens the books file in the defined mode
Parameter:
- mode -> the opening mode
*/
FILE *open_books_data(char *mode)
{
	FILE *books_data;

	books_data = fopen(BOOKS_PATH_FILE, mode);
	return (books_data);
}