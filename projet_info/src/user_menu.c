#include "library.h"

/*
Recovers the maximum size of title and author from borrowed books to make the display more elegant.
Parameters:
- user -> Pointer to the buser structure.
- books -> Pointer to the books structure
- max_length_names -> Pointer to retrieve the maximum len of the titles.
- max_length_author -> Pointer to retrieve the maximum len of the authors.
*/
void get_max_length(t_user *user, t_books *books, int *max_length_names, int *max_length_author)
{
	int i;
	int length_author;
	int length_name;

	i = 0;
	while (i < books->count)
	{
		if (books->is_borrow[i] == 1 && strcmp(books->borrow_name[i], user->username) == 0)
		{
			length_name = strlen(books->names[i]);
			length_author = strlen(books->authors[i]);
			if (length_name > *max_length_names)
				*max_length_names = length_name;
			if (length_author > *max_length_author)
				*max_length_author = length_author;
		}
		i++;
	}
}

/*
Display the books to be returned.
Parameters:
- user -> Pointer to the buser structure.
- books -> Pointer to the books structure.
- id -> If the parameter id is different from 0, the ids are also displayed.
*/
void print_borrowed_books(t_user *user, t_books *books, int id)
{
	int i;
	int max_length_names;
	int max_length_author;

	i = 0;
	max_length_names = 0;
	max_length_author = 0;
	get_max_length(user, books, &max_length_names, &max_length_author);
	while (i < books->count)
	{
		if (books->is_borrow[i] == 1 && strcmp(books->borrow_name[i], user->username) == 0)
		{
			if (id == 0)
				printf("%-*s - %-*s - a rendre pour: %s\n", max_length_names, books->names[i], max_length_author, books->authors[i], books->borrow_hour[i]);
			else
				printf("%-*s - %-*s - a rendre pour: %s - id: %s\n", max_length_names, books->names[i], max_length_author, books->authors[i], books->borrow_hour[i], books->id[i]);
		}
		i++;
	}
}

/*
Verifies that the user's response is consistent with the menu choices.
Parameters:
- buffer -> the user's choice.
- user -> Pointer to the buser structure.
*/
int check_buffer_user_menu(char *buffer, t_user *user)
{
	if (strlen(buffer) > 1)
		return (-1);
	else if (user->role == 1 && buffer[0] != '1' && buffer[0] != '2' && buffer[0] != '3' && buffer[0] != '4')
		return (-1);
	else if (user->role == 2 && buffer[0] != '1' && buffer[0] != '2' && buffer[0] != '3' && buffer[0] != '4')
		return (-1);
	return (0);
}

/*
User's main menu.
It varies according to the user's role.
In the case of a teacher it also shows the option to add a book to the library.
Parameters:
- user -> Pointer to the buser structure.
- books -> Pointer to the books structure.
*/
int user_menu(t_user *user, t_books *books)
{
	int quit = 0;
	char buffer[20];
	int value;
	int b_validity;

	printf("\n\tBienvenu %s !\n\n", user->username);
	while (quit == 0)
	{
		get_borrowed_books(user, books);
		if (user->number_of_books != 0)
		{
			printf("Vous avez emprunte les livres suivant: \n");
			print_borrowed_books(user, books, 0);
			printf("\n");
		}
		printf("\n  MENU PRINCIPAL\n");
		printf("------------------\n");
		printf("1. Emprunter un livre\n");
		printf("2. Rendre un livre\n");
		if (user->role == 1)
			printf("3. Se deconnecter\n");
		else if (user->role == 2)
		{
			printf("3. Ajouter un livre a la collection.\n");
			printf("4. Se deconnecter\n");
		}
		printf("-> ");
		scanf("%s", buffer);
		while (check_buffer_user_menu(buffer, user) != 0)
		{
			printf("Entree non reconnu.\n");
			printf("-> ");
			scanf("%s", buffer);
		}
		value = atoi(buffer);
		if (value == BORROW_BOOK)
		{
			b_validity = borrow_validity(user, books);
			if (b_validity == 0)
				search_menu(user, books);
			else if (b_validity == -1)
				printf("Vous avez emprunter le nombre maximal de livres.\n");
			else
				printf("\n");
		}
		else if (value == RETURN_BOOK)
		{
			if (user->number_of_books == 0)
				printf("Vous n'avez aucun livre a rendre.\n");
			else
				return_book_menu(user, books);
		}
		else if ((user->role == 1 && value == 3) || (user->role == 2 && value == 4))
		{
			printf("A bientot %s !\n\n", user->username);
			free(user->username);
			user->username = NULL;
			quit = 1;
		}
		else if ((user->role == 2 && value == 3))
			add_book_menu(books);
	}
	return (0);
}