#include "monitor.h"

/*
** Simple url parser
** Checks for http:// and https://, if neither is found, start of hostname is assumed correct.
** Moves pointer to after http(s):// and starts looking for "/", if found, replaces with '\0'.
** If not found, address is assumed correct.
*/
static char	*extract_host(char *page)
{
	char	*endptr;
	char	*host;

	host = page;
	if (strstr(host, "http://") != NULL)
		host += 7;
	else if (strstr(host, "https://") != NULL)
		host += 8;
	if ((endptr = strchr(host, '/')))
		*endptr = '\0';
	return (host);
}

/*
** Formatting of url and req strings
** Idea is to remove possible starting and trailing spaces and new line at the end.
** Makes it easier later on the handle urls and requirements.
** -might have to deal with http:// stuff... can't use it with request url...
*/
static char	*reformat(char *str)
{
	char	*end;

	if (str != NULL)
	{
		while (isspace(*str))
			str += 1;
		end = str + (strlen(str)) -1;
		while (end > str && isspace(*end))
			end -= 1;
		end[1] = '\0';
	}
	return (str);
}

/*
** This and reformat could be one func...
** strtok splits the string based on delimiter string (",")
** however it leaves whitespace and new lines intact
** so we reformat the lines a little
** (currently there can be additional delimiter and stuff after but it gets ignored, add multiple requirements if time?)
*/
static int	save_buf(t_monitor **mntr, char *buf)
{
	char	*host;
	char	*page;
	char	*content;

	page = NULL;
	page = reformat(strtok(buf, DELIMITER_CHAR));
	if (page[0] != '\0')
	{
		content = reformat(strtok(NULL, DELIMITER_CHAR));
		host = extract_host(page);
		url_append(mntr, host, page, content);
	}
	return (1);
}

/*
** Reads given file, expected format is www.sitename.com, string data (whitespace shouldnt matter)
** Uses getline to loop through the opened file.
** Calls save_buf to split, reformat and save the urls and content requirement to linked list.
*/
int			read_file(char *file, t_monitor **mntr)
{
	char	*buf;
	size_t	bufsize;
	ssize_t	line_size;
	FILE	*fp;

	buf = NULL;
	bufsize = 0;
	fp = fopen(file, "r");
	if (!fp)
	{
		fprintf(stderr, "Could not open file\n");
		exit(EXIT_FAILURE);
	}
	line_size = getline(&buf, &bufsize, fp);
	while (line_size >= 0)
	{
		save_buf(mntr, buf);
		line_size = getline(&buf, &bufsize, fp);
	}
	if (buf)
		free(buf);
	fclose(fp);
	return (1);
}
