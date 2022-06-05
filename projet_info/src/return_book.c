#include "library.h"

/*
Check if the line is the right one by comparing the ids.
Parameters:
- buffer -> buffer to the current line of the file
- id -> the id to looking for.
*/
int match_line(char *buffer, char *id)
{
	char **data;
	int result;

	if (!buffer)
		return (-1);
	data = my_split(buffer, ' ');
	result = strcmp(data[1], id);
	free(data[0]);
	free(data[1]);
	free(data[2]);
	free(data);
	if (result == 0)
		return (0);
	return (-1);
}

/*
Copy all lines except the line to be deleted.
Parameters:
- src -> Pointer to the source file.
- temp -> Pointer to the temp file
- line -> index of the line to be not copy. 
*/
void delete_line(FILE *src, FILE *temp, int line)
{
	char buffer[256];
	int count = 0;

	while ((fgets(buffer, 256, src)) != NULL)
	{
		buffer[strcspn(buffer, "\n")] = 0;
		if (line != count)
		{
			fputs(buffer, temp);
			fputs("\n", temp);
		}
		count++;
	}
}

/*
Updates the file of borrowed books after a return.
We create a temporary file and we copy in it all the lines of the file of the borrowed books except the line of the returned book.
Parameters:
- books -> Pointer to the books structure
- index -> the index of the books to be return
*/
int update_borrowed_file(t_books *books, int index)
{
	FILE *borrowed_file;
	FILE *temp_borrowed_file;
	char buffer[256];
	int i;

	i = 0;
	borrowed_file = open_borrowed_data("r");
	temp_borrowed_file = fopen(BORROWED_PATH_FILE_TEMP, "w+");
	while (fgets(buffer, 256, borrowed_file))
	{
		buffer[strcspn(buffer, "\n")] = 0;
		if (match_line(buffer, books->id[index]) == 0)
			break;
		i++;
	}
	fclose(borrowed_file);
	borrowed_file = open_borrowed_data("r");
	delete_line(borrowed_file, temp_borrowed_file, i);
	fclose(borrowed_file);
	fclose(temp_borrowed_file);
	remove(BORROWED_PATH_FILE);
	rename(BORROWED_PATH_FILE_TEMP, BORROWED_PATH_FILE);
	return (0);
}

/*
Checks the validity of the user's choice in the menu.
Parameter:
- buffer -> the user's choice.
*/
int check_buffer_return_menu(char *buffer)
{
	if (strlen(buffer) > 1)
		return (-1);
	else if (buffer[0] != '1' && buffer[0] != '2' && buffer[0] != '3')
		return (-1);
	return (0);
}

/*
Retrieves borrowed books to check if the user has borrowed the book.
`to_search` allows to generalize the search and to search by title, author or id.
Parameters:
- user -> Pointer to the user structure
- books -> Pointer to the books structure
- to_search -> The table in which to search
- buffer -> buffer for the user's answer
*/
int search_borrow_books(t_user *user, t_books *books, char **to_search, char *buffer)
{
	int i;

	i = 0;
	while (i < books->count)
	{
		if (strcmp(to_search[i], buffer) == 0)
		{
			if (books->is_borrow[i] == 1)
			{
				if (strcmp(books->borrow_name[i], user->username) == 0)
					return (i);
				else
				{
					printf("Livre emprunte par %s.\n", books->borrow_name[i]);
					return (-1);
				}
			}
		}
		i++;
	}
	return (-1);
}

/*
Update the array after return a book.
Parameters:
- books -> Pointer to the books structure
- index -> the index of the book to be returned.
*/
void update_data(t_books *books, int index)
{
	books->is_borrow[index] = 0;
	free(books->borrow_hour[index]);
	free(books->borrow_name[index]);
	books->borrow_hour[index] = NULL;
	books->borrow_name[index] = NULL;
}

/*
Return a book by its title.
Parameters:
- user -> Pointer to the user structure
- books -> Pointer to the books structure
*/
int return_by_title(t_user *user, t_books *books)
{
	int index;
	char buffer[256];

	printf("Titre: ");
	scanf(" %[^\n]s", buffer);
	index = search_borrow_books(user, books, books->names, buffer);
	if (index == -1)
		return (-1);
	else
	{
		update_borrowed_file(books, index);
		update_data(books, index);
		printf("Livre rendu !\n\n");
	}
	return (0);
}

/*
Return a book by its id.
Parameters:
user -> Pointer to the user structure
books -> Pointer to the books structure
*/
int return_by_id(t_user *user, t_books *books)
{
	int index;
	char buffer[256];

	printf("ID: ");
	scanf(" %[^\n]s", buffer);
	index = search_borrow_books(user, books, books->id, buffer);
	if (index == -1)
		return (-1);
	else
	{
		update_borrowed_file(books, index);
		update_data(books, index);
		printf("Livre rendu !\n\n");
	}
	return (0);
}

/*
Main menu to return a book:
- by id
- by title
- return to the user menu
Parameters:
user -> Pointer to the user structure
books -> Pointer to the books structure
*/
void return_book_menu(t_user *user, t_books *books)
{
	char buffer[256];
	int value;

	printf("\nVous avez emprunte les livres suivant: \n");
	print_borrowed_books(user, books, 1);
	printf("\n");
	printf("\n  RENDRE UN LIVRE\n");
	printf("-------------------\n");
	printf("1. Rendre par titre.\n");
	printf("2. Rendre par ID.\n");
	printf("3. Revenir au menu principal.\n");
	printf("-> ");
	scanf(" %[^\n]s", buffer);
	while (check_buffer_return_menu(buffer) != 0)
	{
		printf("Entree non reconnu.\n");
		printf("-> ");
		scanf("%s", buffer);
	}
	value = atoi(buffer);
	if (value == 1)
		return_by_title(user, books);
	else if (value == 2)
		return_by_id(user, books);
	else if (value == 3)
		return;
}