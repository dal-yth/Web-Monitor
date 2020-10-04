#include "monitor.h"

/*
** Free current cycle data.
** If data, status, less or found exist, free and set to NULL for next cycle.
*/
void	free_cycle_data(t_monitor *cur, t_parse *info)
{
	if (cur->data)
	{
		free(cur->data);
		cur->data = NULL;
	}
	if (info->status)
	{
		free(info->status);
		info->status = NULL;
	}
	if (info->less)
	{
		free(info->less);
		info->less = NULL;		
	}
	if (info->found)
	{
		free(info->found);
		info->found = NULL;		
	}
}

/*
** Free the linked list
** Frees allocated strings in a link and then the link itself.
** Repeats with every link.
*/
void	free_list(t_monitor *mntr)
{
	t_monitor	*temp;

	while (mntr)
	{
		temp = mntr;
		if (temp->host)
			free(temp->host);
		if (temp->page)
			free(temp->page);
		if (temp->content)
			free(temp->content);
		if (temp->data)
			free(temp->data);
		mntr = mntr->next;
		free(temp);
	}
}
