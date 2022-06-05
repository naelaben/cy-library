#include "library.h"

/*
Count the number of words after split.
Paramters:
- s -> the string to split.
- c -> the split's char.
*/
static int			count_words(char const *s, char c)
{
	int words;
	int i;

	words = 0;
	i = 0;
	while (s[i])
	{
		if (!s[i + 1] || (s[i] == c && s[i + 1] != c) ||
				(s[i + 1] == c && !s[i + 1]))
			words++;
		i++;
	}
	if (words == 0)
		words++;
	return (words);
}

/*
Get the size before the char c.
Parameters:
- s -> the string to split.
- c -> the split's char.
*/
static int			len_stop(char const *s, char c)
{
	int i;

	i = 0;
	while (c != s[i] && s[i])
		i++;
	return (i);
}

/*
Free the tab
Paramter:
- tab -> the tab to free.
*/
static void			*free_tab(char **tab)
{
	size_t i;

	i = 0;
	while (tab[i])
		free(tab[i++]);
	free(tab);
	return (NULL);
}

/*
Split a string into an array of strings, split according to the char c.
Parameters:
- s -> the string to split.
- c -> the split's char.
*/
char				**my_split(char const *s, char c)
{
	int		i;
	int		j;
	char	**split;

	i = 0;
	j = 0;
	if (!s)
		return (NULL);
	if (!(split = (char**)malloc((count_words(s, c) + 1) * sizeof(char *))))
		return (NULL);
	while (j < count_words(s, c))
	{
		while (s[i] == c && s[i])
			i++;
		if (!s[i])
			break ;
		split[j] = strndup(&s[i], len_stop(&s[i], c));
		if (!split[j])
			return (free_tab(split));
		j++;
		i += len_stop(&s[i], c);
	}
	split[j] = 0;
	return (split);
}
