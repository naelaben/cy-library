#include "library.h"

/*
Checks if the user's answer is "oui" or "non".
Parameter :
- reponses -> buffer
*/
int check_response(char *response)
{
	if (strcmp(response, "oui") == 0)
		return (0);
	else if(strcmp(response, "non") == 0)
		return (1);
	else
		return (2);
}

/*
Checks the validity of the ID:
- No spaces
- Not used
Parameters:
- books -> Pointer to the book structure
- id -> string of the id
*/
int check_id(t_books *books, char *id)
{
	int i;

	i = 0;
	if (strlen(id) < 1)
		return (-1);
	while(id[i])
	{
		if (isspace(id[i]))
		{
			printf("Un ID ne peut pas contenir d'espaces\n");
			return (-1);
		}
		i++;
	}
	i = 0;
	while(i < books->count)
	{
		if (strcmp(books->id[i], id) == 0)
		{
			printf("ID deja utilise.\n");
			return (-1);
		}
		i++;
	}
	return (0);
}

/*
Check if the book is already in the database.
Proposes to the user to add it anyway, in case it is a different edition of the book for example.
Parameters:
- books -> Pointer to the book structure
- title -> buffer of the book's title
- author -> buffer of the book's author
*/
int book_exist(t_books *books, char *title, char *author)
{
	int i;
	int validity;
	char response[256];

	i = 0;
	while(i < books->count)
	{
		if (strcmp(books->names[i], title) == 0 && strcmp(books->authors[i], author) == 0)
		{
			printf("Ce livre est deja dans la base de donnee: \n");
			printf("%s - %s - id: %s\n", books->names[i], books->authors[i], books->id[i]);
			printf("\nSouhaitez vous l'ajouter malgres tout ?(oui/non)\n");
			printf("-> ");
			scanf("%s", response);
			validity = check_response(response);
			while(validity == 2)
			{
				printf("Reponse invalide.\n");
				printf("Souhaitez vous l'ajouter malgres tout ?(oui/non)\n");
				printf("-> ");
				scanf("%s", response);
				validity = check_response(response);
			}
			break;
		}
		i++;
	}
	return (validity);
}

/*
Allows you to add a book to the books structure.
The function must reallocate the previous table by one more cell.
If all the reallocations have worked well then we update the structure.
Parameters: 
- books -> Pointer to the books structure
- title -> buffer to book's title
- new_author -> buffer to book's author
- new_id -> buffer to book's id
- new_genre -> buffer to the books' genre
*/
int add_in_struct(t_books *books, char *title, char *new_author, char *new_id, char *new_genre)
{
	char	**names;
	char	**authors;
	char	**id;
	char	**genre;
	int 	*is_borrow;
	char	**borrow_name;
	char	**borrow_hour;	
	int count;

	count = books->count + 1;
	names = realloc(books->names, sizeof(char *) * count);
	names[count - 1] = strdup(title);
	authors = realloc(books->authors, sizeof(char *) * count);
	authors[count - 1] = strdup(new_author);
	id = realloc(books->id, sizeof(char *) * count);
	id[count - 1] = strdup(new_id);
	genre = realloc(books->genre, sizeof(char *) * count);
	genre[count - 1] = strdup(new_genre);
	is_borrow = realloc(books->is_borrow, sizeof(int) * count);
	is_borrow[count - 1] = 0;
	borrow_name = realloc(books->borrow_name, sizeof(char *) * count);
	borrow_name[count - 1] = NULL;
	borrow_hour = realloc(books->borrow_hour, sizeof(char *) * count);
	borrow_hour[count - 1] = NULL;

	if (names == NULL ||
		authors == NULL || 
		id == NULL ||
		genre == NULL ||
		is_borrow == NULL ||
		borrow_name == NULL ||
		borrow_hour == NULL )
	{
		return (-1);
	}
	else
	{
		books->count = count;
		books->names = names;
		books->authors = authors;
		books->id = id;
		books->genre = genre;
		books->is_borrow = is_borrow;
		books->borrow_name = borrow_name;
		books->borrow_hour = borrow_hour;
	}
	return (0);
}

/*
Allows you to add the new book to the file.
Parameters:
- title -> buffer to the book's title
- author -> buffer to the book's author
- id -> buffer to the book's id
- genre -> buffer to the book's genre
*/
int add_book_in_file(char *title, char *author, char *id, char *genre)
{
	long length;
	FILE *books_file;
	char ch;
	
	//Open the file on read mode
	books_file = open_books_data("r");
	if (books_file == NULL)
		return (-1);
	fseek(books_file, 0, SEEK_END);
   	length = ftell(books_file);
	//Allows to place the cursor at the end of the file
   	fseek(books_file, (length - 1), SEEK_SET);
	//Get the last char of the file
	ch = fgetc(books_file);
	fclose(books_file);
	//Open the file en append mode
	books_file = open_books_data("a");
	if (books_file == NULL)
		return (-1);
	if (ch == '\n')
		fprintf(books_file, "\"%s\" \"%s\" \"%s\" \"%s\"\n", title, author, id, genre);
	else
		fprintf(books_file, "\n\"%s\" \"%s\" \"%s\" \"%s\"\n", title, author, id, genre);
	fclose(books_file);
	return (0);
}

/*
Main menu of adding a book to the database.
Parameter:
- books -> Pointer to the books structure
*/
void add_book_menu(t_books *books)
{
	char title[256];
	char author[256];
	char id[256];
	char genre[256];
	int validity;
	int id_validity;

	id_validity = -1;
	validity = 0;
	printf("AJOUTER UN LIVRE A LA COLLECTION\n\n");
	printf("Titre: ");
	scanf(" %[^\n]s", title);
	printf("Auteur: ");
	scanf(" %[^\n]s", author);
	printf("Genre: ");
	scanf(" %[^\n]s", genre);
	validity = book_exist(books, title, author);
	if (validity == 1)
	{
		printf("Ajout annule.\n");
		return ;
	}
	else
	{
		while(id_validity == -1)
		{
			printf("ID: ");
			scanf(" %[^\n]s", id);
			id_validity = check_id(books, id);
		}
		if (add_in_struct(books, title, author, id, genre) == -1 || add_book_in_file(title, author, id, genre) == -1)
			printf("Erreur lors de l'ajout du livre.\n");
		else
			printf("Livre ajout avec succes !\n");
	}
}