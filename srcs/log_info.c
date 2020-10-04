#include "monitor.h"

/*
** Find content requirement from read data.
** Do case insensitive needle from haystack search with strcasestr.
*/
void	check_content(char *data, char *content, t_parse *info)
{
	if (data && strcasestr(data, content))
		info->found = strdup("Yes");
	else
		info->found = strdup("No");
}

/*
** Extracts the HTTP status code
*/
char	*get_status(char *data)
{
	int		i;
	char	*status;

	i = 0;
	if (data)
	{
		while (data[i] != '\n' && data[i])
			i += 1;
		status = strndup(data, i - 1);
		return (status);
	}
	else
		return (NULL);
}

/*
** In addition to the basic data, also logs the additional server data received.
*/
char	*less_log(char *data, char *less)
{
	int		i;
	int		j;
	int		len;

	i = 0;
	j = 0;
	len = strlen(data);
	if (len == 0)
		return (NULL);
	less = (char*)malloc(sizeof(char) * len + 1);
	bzero(less, len + 1);
	while (i < len && j < len)
	{
		less[j] = data[i];
		//The condition is to find the first empty line, OR empty line on windows with carriage return in between.
		if (i > 1 && ((data[i] == '\n' && data[i - 1] == '\n') || (data[i] == '\n' && data[i - 1] == '\r' && data[i - 2] == '\n')))
			break;
		i += 1;
		j += 1;
	}
	return (less);
}
