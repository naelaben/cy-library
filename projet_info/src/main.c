#include "library.h"


int main()
{
	t_user *current_user;
	t_books *books;

	current_user = malloc(sizeof(t_user));
	books = malloc(sizeof(t_books));
	if (current_user == NULL || books == NULL)
	{
		printf("Error with malloc\n");
		return (1);
	}
	books->count = 0;
	current_user->username = NULL;
	current_user->number_of_books = 0;
	if (parse_books(books) == 0)
		user_auth_choice(current_user, books);
	free_data(current_user, books);
	return (0);
}