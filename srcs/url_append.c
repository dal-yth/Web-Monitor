#include "monitor.h"

/*
** Allocates the new link and duplicates url and req strings to link.
*/
static t_monitor	*allocate_url(char *host, char *page, char *content)
{
	t_monitor	*new;

	new = (t_monitor*)malloc(sizeof(t_monitor));
	if (!new)
	{
		fprintf(stderr, "Error allocating new link\n");
		exit(EXIT_FAILURE);
	}
	if (host)
		new->host = strdup(host);
	else
		new->host = NULL;
	if (page)
		new->page = strdup(page);
	else
		new->page = NULL;
	if (content)
		new->content = strdup(content);
	else
		new->content = NULL;
	new->data = NULL;
	new->next = NULL;
	new->response = 0;
	return (new);
}

/*
** Append new link at the end of linked list.
** Gets head reference of list, url and req strings.
** If there is no head yet, allocate head, else cycle through the links
** and allocate new link to be last.
*/
int					url_append(t_monitor **head, char *host, char *page, char *content)
{
	t_monitor	*last;

	last = *head;
	if (*head == NULL)
		*head = allocate_url(host, page, content);
	else
	{
		while (last->next)
			last = last->next;
		last->next = allocate_url(host, page, content);
	}
	return (1);
}
