#include "library.h"

/*
Free all data from the structures.
- user -> Pointer to the user structure
- books -> Pointer to the books structure
*/
void free_data(t_user *user, t_books *books)
{
	int i;
	
	i = 0;
	while (i < books->count)
	{
		if (books->names[i])
			free(books->names[i]);
		if (books->authors[i])
			free(books->authors[i]);
		if (books->id[i])
			free(books->id[i]);
		if (books->genre[i])
			free(books->genre[i]);
		if (books->borrow_name[i] != NULL)
			free(books->borrow_name[i]);
		if (books->borrow_hour[i] != NULL)
			free(books->borrow_hour[i]);
		i++;
	}
	if (books->names)
		free(books->names);
	if (books->authors)
		free(books->authors);
	if (books->id)
		free(books->id);
	if (books->genre)
		free(books->genre);
	if (books->borrow_name)
		free(books->borrow_name);
	if (books->borrow_hour)
		free(books->borrow_hour);
	if (books->is_borrow)
		free(books->is_borrow);
	if (user->username != NULL)
		free(user->username);
	free(books);
	free(user);
}