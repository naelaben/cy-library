#include "library.h"

/*
Browse the books borrowed by the user and check the current time with the return time.
Displays the books that need to be returned.
Parameters:
- books -> Pointer to the books structure
- user -> Pointer to the user structure
*/
int check_borrowed_books(t_books *books, t_user *user)
{
	int		i;
	char	**borrow_time;
	int		borrow_hour;
	int		borrow_minutes;
	time_t	now;
	struct tm *local;
	int		hours;
	int		minutes;
	int ret;

	i = 0;
	ret = 0;
	time(&now);
	local = localtime(&now);
	hours = local->tm_hour;
    minutes = local->tm_min;
	while (i < books->count)
	{
		if (books->is_borrow[i] == 1)
		{
			if(strcmp(books->borrow_name[i], user->username) == 0)
			{
				borrow_time = my_split(books->borrow_hour[i], 'h');
				borrow_hour = atoi(borrow_time[0]);
				borrow_minutes = atoi(borrow_time[1]);
				if (borrow_hour < hours || (borrow_hour == hours && borrow_minutes < minutes))
				{
					if (ret == 0)
						printf("\nVous devez rendre le(s) livre(s) suivant: \n");
					printf("%s - %s\n", books->names[i], books->authors[i]);
					ret = -1;
				}
				free(borrow_time[0]);
				free(borrow_time[1]);
				free(borrow_time);
			}
		}
		i++;
	}
	return (ret);
}

/*
Check the borrowed books according to the rules:
- A student can take a maximum of 3 books
- A teacher can take 5 books maximum
- Books must be returned within the allotted time
Parameters:
- user -> Pointer to the user structure
- books -> Pointer to the books structure
*/
int borrow_validity(t_user *user, t_books *books)
{
	if (user->role == 1 && user->number_of_books == 3)
		return (-1);
	else if(user->role && user->number_of_books == 5)
		return (-1);
	else if (check_borrowed_books(books, user) == -1)
		return (-2);
	return (0);
}

/*
Counts the number of books borrowed by a user.
Parameters:
- user -> Pointer to the user structure
- books -> Pointer to the books structure
*/
void get_borrowed_books(t_user *user, t_books *books)
{
	int i;

	i = 0;
	user->number_of_books = 0;
	while (i < books->count)
	{
		if (books->is_borrow[i] == 1)
		{
			if (strcmp(books->borrow_name[i], user->username) == 0)
				user->number_of_books++;
		}
		i++;
	}
}

/*
Retrieves the book index from the database according to the search and according `to_search`.
`to_search` allows to generalize the search and to search by title, author or id.
Parameters:
- books -> Pointer to the books structure
- to_search -> The table in which to search
- buffer -> the string to search
*/
int get_books_index(t_books *books, char **to_search, char *buffer)
{
	int i;

	i = 0;
	while(i < books->count)
	{
		if (strcmp(to_search[i], buffer) == 0)
		{
			if (books->is_borrow[i] == 1)
				return (-2);
			else
				return (i);
		}
		i++;
	}
	return (-1);
}

/*
Adds a borrowed book to the corresponding file.
Parameters:
- user -> Pointer to the user structure
- books -> Pointer to the books structure
- index -> The index of the book in the array
*/
int add_borrowed_in_file(t_books *books, t_user *user, int index)
{
	long length;
	FILE *borrow_file;
	char ch;

	borrow_file = open_borrowed_data("r");
	if (borrow_file == NULL)
		return (-1);
	fseek(borrow_file, 0, SEEK_END);
   	length = ftell(borrow_file);
   	fseek(borrow_file, (length - 1), SEEK_SET);
	ch = fgetc(borrow_file);
	fclose(borrow_file);
	borrow_file = open_borrowed_data("a");
	if (borrow_file == NULL)
		return (-1);
	if (ch == '\n')
		fprintf(borrow_file, "%s %s %s\n", user->username, books->id[index], books->borrow_hour[index]);
	else
		fprintf(borrow_file, "\n%s %s %s\n", user->username, books->id[index], books->borrow_hour[index]);
	fclose(borrow_file);
	return (0);
}

/*
Borrow a book by its ID.
- user -> Pointer to the user structure
- books -> Pointer to the books structure
*/
int borrow_by_id(t_books *books, t_user *user)
{
	char buffer[256];
	int index;
	char *date;
	time_t	now;
	struct tm *local;
	int		hours;
	int		minutes;

	time(&now);
	local = localtime(&now);
	hours = local->tm_hour;
    minutes = local->tm_min;
	printf("ID du livre: ");
	scanf(" %[^\n]s", buffer);
	index = get_books_index(books, books->id, buffer);
	if (index == -1)
	{
		printf("Pas de resultat pour l'id `%s`\n\n", buffer);
		return (-1);
	}
	else if (index == -2)
	{
		printf("Livre deja emprunte !\n\n");
		return (-1);
	}
	else
	{
		books->is_borrow[index] = 1;
		books->borrow_name[index] = strdup(user->username);
		date = malloc(sizeof(char) * 6);
		date[5] = '\0';
		if (user->role == 1)
			sprintf(date, "%dh%d", hours, minutes + 2);
		else
			sprintf(date, "%dh%d", hours, minutes + 3);
		books->borrow_hour[index] = date;
		add_borrowed_in_file(books, user, index);
		get_borrowed_books(user, books);
	}
	return (0);
}

/*
Borrow a book by its title.
- user -> Pointer to the user structure
- books -> Pointer to the books structure
*/
int borrow_by_title(t_books *books, t_user *user)
{
	char buffer[256];
	int index;
	char *date;
	time_t	now;
	struct tm *local;
	int		hours;
	int		minutes;

	time(&now);
	local = localtime(&now);
	hours = local->tm_hour;
    minutes = local->tm_min;
	printf("Titre du livre: ");
	scanf(" %[^\n]s", buffer);
	index = get_books_index(books, books->names, buffer);
	if (index == -1)
	{
		printf("Pas de resultat pour le titre `%s`\n\n", buffer);
		return (-1);
	}
	else if (index == -2)
	{
		printf("Livre deja emprunte !\n\n");
		return (-1);
	}
	else
	{
		books->is_borrow[index] = 1;
		books->borrow_name[index] = strdup(user->username);
		date = malloc(sizeof(char) * 6);
		date[5] = '\0';
		if (user->role == 1)
			sprintf(date, "%dh%d", hours, minutes + 2);
		else
			sprintf(date, "%dh%d", hours, minutes + 3);
		books->borrow_hour[index] = date;
		add_borrowed_in_file(books, user, index);
		get_borrowed_books(user, books);
	}
	return (0);
}