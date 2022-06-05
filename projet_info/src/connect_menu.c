#include "library.h"

/*
Check if the user's choice is correct among the possible answers.
Parameter:
- buffer -> buffer to the user's response
*/
int check_buffer(char *buffer)
{
	if (strlen(buffer) > 1)
		return (-1);
	else if(buffer[0] != '1' && buffer[0] != '2' && buffer[0] != '3')
		return (-1);
	return (0);
}

/*
Displays the title of the application on the screen.
*/
void print_program_title()
{
	printf("+---------------------------------+\n");
	printf("|    Application CY-biblioTECH    |\n");
	printf("+---------------------------------+\n");
}

/*
Start menu.
Redirects to the login or account creation screen or quit the application.
Parameters:
- user -> Pointer to the user structure
- books -> Pointer to the books structure
*/
int user_auth_choice(t_user *user, t_books *books)
{
	char buffer[20];
	int value;
	int quit;

	quit = 0;
	print_program_title();
	while (quit == 0)
	{
		printf("\n  CONNEXION\n");
		printf("-------------\n");
		printf("1. Se connecter\n");
		printf("2. Nouvel utilisateur ?\n");
		printf("3. Quitter le programme\n");
		printf("-> ");
		scanf("%s", buffer);
		while (check_buffer(buffer) != 0)
		{
			printf("Entree non reconnu.\n");
			printf("-> ");
			scanf("%s", buffer);
		}
		value = atoi(buffer);
		if (value == USER_AUTH)
			user_authentification(user, books);
		else if (value == CREATE_ACCOUNT)
			create_account(user, books);
		else if (value == 3)
			quit = 1;
		value = 0;
		bzero(buffer, 20);
	}
	return (0);
}



