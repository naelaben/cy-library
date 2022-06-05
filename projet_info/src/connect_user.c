#include "library.h"

/*
Main function in user authentication. Redirects to the user menu if the connection was successful.
Otherwise redirects to the main menu.
Parameters:
- user -> Pointer to the user structure
- books -> Pointer to the books structure
*/
int user_authentification(t_user *user, t_books *books)
{
	if (user_connect(user) == 0)
		user_menu(user, books);
	return (0);
}

/*
Returns the user's password or NULL if the user does not exist.
Parameters:
- username -> buffer for the username
- current_user -> Pointer to the user structure.
*/
char *get_password(char *username, t_user *current_user)
{
	FILE *user_data;
	char *user_password;
	char buffer[256];
	char **user_info;
	int i;

	user_data = open_user_data("r");
	i = 0;
	while (fgets(buffer, 256, user_data))
	{
		buffer[strcspn(buffer, "\n")] = 0;
		user_info = my_split(buffer, ' ');
		if (strcmp(user_info[0], username) == 0)
			break;
		else
		{
			while (i < 3)
			{
				free(user_info[i]);
				i++;
			}
			free(user_info);
			user_info = NULL;
			i = 0;
		}
	}
	fclose(user_data);
	if (user_info != NULL)
	{
		user_password = strdup(user_info[1]);
		current_user->role = atoi(user_info[2]);
		i = 0;
		while (i < 3)
		{
			free(user_info[i]);
			i++;
		}
		free(user_info);
		return (user_password);
	}
	else
		return (NULL);
}

/*
Login menu.
Ask for loggin and password.
Paramter:
- user -> Pointer to the user structure.
*/
int user_connect(t_user *user)
{
	char loggin[21];
	char password[21];
	char *user_password;

	user_password = NULL;
	while (user_password == NULL)
	{
		printf("Loggin (tapez 'q' pour revenir au menu principal): ");
		scanf("%s", loggin);
		if (check_user_quit(loggin) == 0)
		{
			if (user_password != NULL)
				free(user_password);
			return (-1);
		}
		user_password = get_password(loggin, user);

		if (user_password == NULL)
			printf("Utilisateur non trouve dans la base de donne\n");
	}
	printf("\nMot de passe (tapez 'q' pour revenir au menu principal): ");
	scanf("%s", password);
	if (check_user_quit(password) == 0)
	{
		free(user_password);
		return (-1);
	}
	while (strcmp(password, user_password) != 0)
	{
		printf("Mot de passe incorrect\n");
		printf("Mot de passe (tapez 'q' pour revenir au menu principal): ");
		scanf("%s", password);
		if (check_user_quit(password) == 0)
		{
			free(user_password);
			return (-1);
		}
	}
	user->username = strdup(loggin);
	free(user_password);
	return (0);
}