#include "library.h"

/*
Retrieves all the information of a book from one line of the corresponding file.
Parameters:
- books -> Pointer to the books structure
- i -> index of the arrays
- buffer -> buffer to the line of the file
*/
int parse_line(t_books *books, int i, char *buffer)
{
	char **tab;
	int j = 0;

	tab = my_split(buffer, '"');
	books->names[i] = strdup(tab[0]);
	books->authors[i] = strdup(tab[2]);
	books->id[i] = strdup(tab[4]);
	books->genre[i] = strdup(tab[6]);
	j = 0;
	while (j < 7)
	{
		free(tab[j]);
		j++;
	}
	free(tab);
	return (0);
}

/*
Retrieves all the information of the borrowed books from one line of the corresponding file.
Parameters:
- line -> Buffer to the line of the file
- books -> Pointer to the books structure
*/
int parse_line_borrowed_books(char *line, t_books *books)
{
	char **tab;
	int x;

	x = 0;
	tab = my_split(line, ' ');
	while (x < books->count)
	{
		if (strcmp(books->id[x], tab[1]) == 0)
		{
			books->borrow_name[x] = strdup(tab[0]);
			books->is_borrow[x] = 1;
			books->borrow_hour[x] = strdup(tab[2]);
			break;
		}
		x++;
	}
	x = 0;
	while (x < 3)
	{
		free(tab[x]);
		x++;
	}
	free(tab);
	return (0);
}

/*
Browse the file of borrowed books and call the functions to fill in the corresponding arrays.
Parameter:
books -> Pointer to the books structure
*/
int parse_borrowed_books(t_books *books)
{
	FILE *borrowed_books;
	char buffer[256];

	borrowed_books = open_borrowed_data("r");
	while(fgets(buffer, 256, borrowed_books)) 
	{
		buffer[strcspn(buffer, "\n")] = 0;
		parse_line_borrowed_books(buffer, books);
	}
	fclose(borrowed_books);
	return (0);
}

/*
Browse the book file and call the functions to fill in the corresponding arrays.
Parameter:
books -> Pointer to the books structure
*/
int parse_books(t_books *books)
{
	FILE *books_data;
	int c;
	int i;
    long count = 0;
	char buffer[256];

	books_data = open_books_data("r");
	if (books_data == NULL)
		return (-1);
	while ((c = getc(books_data)) != EOF)
	{
    	if (c == '\n')
        	count++;
	}
	books->count = count;
	books->names = malloc(sizeof(char *) * books->count);
	books->authors = malloc(sizeof(char *) * books->count);
	books->id = malloc(sizeof(char *) * books->count);
	books->genre = malloc(sizeof(char *) * books->count);
	books->is_borrow = malloc(sizeof(int) * books->count);
	books->borrow_name = malloc(sizeof(char *) * books->count);
	books->borrow_hour = malloc(sizeof(char *) * books->count);
	if (books->names == NULL ||
		books->authors == NULL ||
		books->id == NULL ||
		books->genre == NULL ||
		books->is_borrow == NULL ||
		books->borrow_name == NULL ||
		books->borrow_hour == NULL)
	{
		return (-1);
	}
	i = 0;
	while (i < books->count)
	{
		books->borrow_name[i] = NULL;
		books->names[i] = NULL;
		books->authors[i] = NULL;
		books->id[i] = NULL;
		books->genre[i] = NULL;
		books->borrow_hour[i] = NULL;
		i++;
	}
	bzero(books->is_borrow, sizeof(int) * books->count);
	fseek(books_data, 0, SEEK_SET);
	i = 0;
	while(fgets(buffer, 256, books_data)) 
	{
		buffer[strcspn(buffer, "\n")] = 0;
		parse_line(books, i, buffer);
		i++;
	}
	fclose(books_data);
	parse_borrowed_books(books);
	return (0);
}