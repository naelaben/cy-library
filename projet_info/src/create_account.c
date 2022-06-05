#include "library.h"

/*
Returns 0 if the username is valid:
- If its size is >= 2.
- If it does not exist in the database.
Parameter:
- username -> buffer to the username.
*/
int check_username_validity(char *username)
{
	if (strlen(username) < 2)
		return (-1);
	else
		return(user_exist(username));	
}

/*
Defines the validity rules of the password:
- If its size is > 8.
- Contains upper case letters, lower case letters, special characters, numbers.
- No spaces.
*/
int check_password_validity(char *password)
{
	int i;
	int lower = 0;
	int upper = 0;
	int digit = 0;
	int punct = 0;

	i = 0;
	if (strlen(password) < 8)
		return (-1);
	while (password[i])
	{
		if (islower(password[i]))
			lower = 1; 
		else if (isupper(password[i]))
			upper = 1;
		else if (isdigit(password[i]))
			digit = 1; 
		else if (ispunct(password[i]))
			punct = 1;
		else if (isspace(password[i]))
			return (-1);
		i++;
	}
	if (lower == 1 && upper == 1 && digit == 1 && punct == 1)
		return (0);
	else 
		return (-1);
}

/*
Check the validity of the roles:
1 -> Student
2 -> Teacher
3 -> Quit
Paramter:
- buffer -> buffer to the user's answer
*/
int check_role(char *buffer)
{

	if (strlen(buffer) > 1)
		return (-1);
	else if(buffer[0] != '1' && buffer[0] != '2' && buffer[0] != '3')
		return (-1);
	else if (buffer[0] == '3')
		return (-2);
	return (0);
}

/*
Adds the new user to the file.
*/
int add_user_in_file(char username[20], char password[20], char role[20])
{
	long length;
	FILE *users_file;

	users_file = open_user_data("r");
	fseek(users_file, 0, SEEK_END);
   	length = ftell(users_file);
	char ch;
   	fseek(users_file, (length - 1), SEEK_SET);
	ch = fgetc(users_file);
	fclose(users_file);
	users_file = open_user_data("a");
	if (ch == '\n')
		fprintf(users_file, "%s %s %s\n", username, password, role);
	else
		fprintf(users_file, "\n%s %s %s\n", username, password, role);
	fclose(users_file);
	return (0);
}

/*
Returns 0 if the user exists or -1 otherwise
Parameter:
- username -> buffer to the username.
*/
int user_exist(char *username)
{
	FILE *user_data;
	char user[21];
	char password[21];
	char role[2];
	int ret;

	user_data = open_user_data("r");
	fscanf(user_data, "%s", user);
	fscanf(user_data, "%s", password);
	fscanf(user_data, "%s", role);
	while (strcmp(username, user) != 0)
	{
		fscanf(user_data, "%s", user);
		fscanf(user_data, "%s", password);
		ret = fscanf(user_data, "%s", role);
		if (ret == EOF)
			break;
	}
	fclose(user_data);
	if (ret == EOF && strcmp(username, user) != 0)
		return (0);
	else
		return (-1);
}

/*
Main function for creating an account.
- user -> Pointer to the user structure
- books -> Pointer to the books structure
*/
int create_account(t_user *user, t_books *books)
{
	if (get_user_info(user) == 0)
		user_menu(user, books);
	return (0);
}

/*
Get all the information to create an account.
Parameter:
- user -> Pointer to the user structure.
*/
int get_user_info(t_user *user)
{
	char username[20];
	char password[20];
	char password_confirm[20];
	char role[20];

	printf("CREER UN COMPTE\n");
	printf("Loggin (tapez 'q' pour revenir au menu principal): ");
	scanf(" %[^\n]s", username);
	if (check_user_quit(username) == 0)
		return (-1);
	while(check_username_validity(username) == -1)
	{
		printf("Nom d'utilisateur deja utilise\n");
		printf("Loggin (tapez 'q' pour revenir au menu principal): ");
		scanf("%s", username);
		if (check_user_quit(username) == 0)
			return (-1);
	}
	printf("\nMot de passe (tapez 'q' pour revenir au menu principal): ");
	scanf("%s", password);
	if (check_user_quit(password) == 0)
		return (-1);
	while(check_password_validity(password) == -1)
	{
		printf("Mot de passe invalide.\n");
		printf("Le mot de passe doit contenir au moins: \n- une lettre majuscule\n- une lettre minusule\n- un caractere special\n- 8 caracteres\n- pas d'espaces\n- un chiffre\n");
		printf("Mot de passe (tapez 'q' pour revenir au menu principal): ");
		scanf("%s", password);
		if (check_user_quit(password) == 0)
			return (-1);
	}
	printf("Confirmer mot de passe (tapez 'q' pour revenir au menu principal): ");
	scanf("%s", password_confirm);
	if (check_user_quit(password_confirm) == 0)
		return (-1);
	while (strcmp(password, password_confirm) != 0)
	{
		printf("Les mots de passe ne concordent pas.\n");
		printf("Confirmer mot de passe (tapez 'q' pour revenir au menu principal): ");
		scanf("%s", password_confirm);
		if (check_user_quit(password_confirm) == 0)
			return (-1);
	}
	printf("Quel est votre role ?\n");
	printf("1. Etudiant\n");
	printf("2. Professeur\n");
	printf("3. Retour au menu principal\n");
	printf("-> ");
	scanf("%s", role);
	if (check_role(role) == -2)
		return (-1);
	while (check_role(role) != 0)
	{
		printf("Entree non reconnu.\n");
		printf("-> ");
		scanf("%s", role);
		if (check_user_quit(role) == 0)
			return (-1);
	}
	user->username = malloc(sizeof(char) * strlen(username));
	strcpy(user->username, username);
	user->role = atoi(role);
	add_user_in_file(username, password, role);
	return (0);
}