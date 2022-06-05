#include "library.h"

/*
Checks the validity of the user's choice in the menu.
Parameter:
- buffer -> the user's choice.
*/
int check_buffer_search(char *buffer)
{
	if (strlen(buffer) > 1)
		return (-1);
	else if (buffer[0] != '1' &&
			 buffer[0] != '2' &&
			 buffer[0] != '3' &&
			 buffer[0] != '4' &&
			 buffer[0] != '5' &&
			 buffer[0] != '6')
		return (-1);
	return (0);
}

/*
Recovers the maximum size of title and author from search results to make the display more elegant.
Parameters:
- books -> Pointer to the books structure
- to_search -> array to search in
- buffer -> the buffer of the search
- max_length_names -> Pointer to retrieve the maximum len of the titles.
- max_length_author -> Pointer to retrieve the maximum len of the authors.
- count -> Pointer to record the number of results.
*/
void get_max_length_search_results(t_books *books, char **to_search, char *buffer, int *max_length_names, int *max_length_author, int *count)
{
	int i;
	int length_author;
	int length_name;

	i = 0;
	while (i < books->count)
	{
		if (strstr(to_search[i], buffer) != NULL)
		{
			length_name = strlen(books->names[i]);
			length_author = strlen(books->authors[i]);
			if (length_name > *max_length_names)
				*max_length_names = length_name;
			if (length_author > *max_length_author)
				*max_length_author = length_author;
			(*count)++;
		}
		i++;
	}
}

/*
Displays the results of the search.
Parameters:
- books -> Pointer to the books structure.
- index -> array that contains all the indexes of the books targeted by the search.
- count -> The number of results.
- max_length_names -> The maximum len of the titles.
- max_length_author -> The maximum len of the authors.
*/
void print_results(t_books *books, int *index, int count, int max_length_names, int max_length_author)
{
	int i;

	i = 0;
	printf("%d resultats:\n\n", count);
	while (i < count)
	{
		if (books->is_borrow[index[i]] == 0)
			printf("%-*s - %-*s - id: %s\n", max_length_names, books->names[index[i]], max_length_author, books->authors[index[i]], books->id[index[i]]);
		else
			printf("%-*s - %-*s - id: %s - Emprunte par: %s\n", max_length_names, books->names[index[i]], max_length_author, books->authors[index[i]], books->id[index[i]], books->borrow_name[index[i]]);
		i++;
	}
	printf("\n");
}

/*
Sort results alphabetically.
Parameters:
- books -> Pointer to the books structure.
- to_search -> The table in which to search
- buffer -> the buffer of the search
- count -> The number of results
*/
int *sort_results(t_books *books, char **to_search, char *buffer, int count)
{
	int i;
	int j;
	int temp;
	int *index;

	i = 0;
	j = 0;
	index = malloc(sizeof(int) * count);
	while (i < books->count)
	{
		if (strstr(to_search[i], buffer) != NULL)
		{
			index[j] = i;
			j++;
		}
		i++;
	}
	i = 0;
	j = 0;
	while (i < count)
	{
		j = i + 1;
		while (j < count)
		{
			if (strcmp(to_search[index[j]], to_search[index[i]]) < 0)
			{
				temp = index[i];
				index[i] = index[j];
				index[j] = temp;
			}
			j++;
		}
		i++;
	}
	return (index);
}

/*
Search by title.
Parameter:
- books -> Pointer to the books structure.
*/
int search_by_title(t_books *books)
{
	char buffer[256];
	int count;
	int max_length_author = 0;
	int max_length_names = 0;
	int *index;

	count = 0;
	printf("Titre: ");
	scanf(" %[^\n]s", buffer);
	printf("\n");
	get_max_length_search_results(books, books->names, buffer, &max_length_names, &max_length_author, &count);
	if (count == 0)
		printf("Pas de resultat pour la recherche `%s`\n\n", buffer);
	else
	{
		index = sort_results(books, books->names, buffer, count);
		print_results(books, index, count, max_length_names, max_length_author);
		free(index);
	}
	return (0);
}

/*
Search by author.
Parameter:
- books -> Pointer to the books structure.
*/
int search_by_author(t_books *books)
{
	char buffer[256];
	int count;
	int max_length_names = 0;
	int max_length_author = 0;
	int *index;

	count = 0;
	printf("RECHERCHE PAR AUTEUR\n\n");
	printf("Votre recherche: ");
	scanf(" %[^\n]s", buffer);
	printf("\n");
	get_max_length_search_results(books, books->authors, buffer, &max_length_names, &max_length_author, &count);
	if (count == 0)
		printf("Pas de resultat pour l'auteur: `%s`\n\n", buffer);
	else
	{
		index = sort_results(books, books->authors, buffer, count);
		print_results(books, index, count, max_length_names, max_length_author);
		free(index);
	}
	return (0);
}

/*
Search by type.
Parameter:
- books -> Pointer to the books structure.
*/
int search_by_type(t_books *books)
{
	char buffer[256];
	int count;
	int max_length_names = 0;
	int max_length_author = 0;
	int *index;

	count = 0;
	printf("RECHERCHE PAR GENRE\n\n");
	printf("Votre recherche: ");
	scanf(" %[^\n]s", buffer);
	printf("\n");
	get_max_length_search_results(books, books->genre, buffer, &max_length_names, &max_length_author, &count);
	if (count == 0)
		printf("Pas de resultat pour le genre: `%s`\n\n", buffer);
	else
	{
		index = sort_results(books, books->genre, buffer, count);
		print_results(books, index, count, max_length_names, max_length_author);
		free(index);
	}
	return (0);
}

/*
Main menu for borrowing a book. Allows you to search for books and borrow them.
Parameters:
- user -> Pointer to the user structure.
- books -> Pointer to the books structure.
*/
int search_menu(t_user *user, t_books *books)
{
	int quit = 0;
	char buffer[20];
	int value;

	(void)user;
	while (quit == 0)
	{
		printf("\n  EMPRUNTER UN LIVRE\n");
		printf("----------------------\n");
		printf("1. Recherche par titre.\n");
		printf("2. Recherche par auteur.\n");
		printf("3. Recherche par genre\n");
		printf("4. Emprunter par titre\n");
		printf("5. Emprunter par id\n");
		printf("6. Revenir au menu principal.\n");
		printf("-> ");
		scanf("%s", buffer);
		while (check_buffer_search(buffer) != 0)
		{
			printf("Entree non reconnu.\n");
			printf("-> ");
			scanf("%s", buffer);
		}
		value = atoi(buffer);
		if (value == GET_BACK)
			return (0);
		else if (value == SEARCH_TITLE)
			search_by_title(books);
		else if (value == SEARCH_AUTHOR)
			search_by_author(books);
		else if (value == SEARCH_GENRE)
			search_by_type(books);
		else if (value == BORROW_TITLE)
		{
			if (borrow_by_title(books, user) == 0)
			{
				printf("Livre emprunte !\n\n");
				return (0);
			}
		}
		else if (value == BORROW_ID)
		{
			if (borrow_by_id(books, user) == 0)
			{
				printf("Livre emprunte !\n\n");
				return (0);
			}
		}
	}
	return (0);
}