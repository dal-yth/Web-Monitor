#include "monitor.h"

/*
** Error log
** Gets error data and user message.
** Logs time, error data and message to the log_error.txt file.
*/
void	error_log(char *data, char *msg)
{
	int			fd;
	time_t		cur_time;
	struct tm	date;

	cur_time = time(NULL);
	date = *localtime(&cur_time);
	fd = open("logs/log_error.txt", O_CREAT | O_APPEND | O_WRONLY, 0766);
	dprintf(fd, "%d-%02d-%02d %02d:%02d:%02d\n", date.tm_year + 1900, date.tm_mon + 1, date.tm_mday, date.tm_hour, date.tm_min, date.tm_sec);
	write(fd, data, strlen(data));
	write(fd, "\n", 1);
	write(fd, msg, strlen(msg));
	write(fd, "\n\n", 2);
	close(fd);
}

/*
** Logging function
** Based on the log_val opens or creates the proper log file.
** Writes in the current time and basic information.
** If log_val is specified to be 2 or 3, writes additional information.
*/
void	monitor_log(t_monitor *cur, char *status, char *less, int log_val, char *found)
{
	int		fd;
	time_t	cur_time;
	struct	tm date;
	
	cur_time = time(NULL);
	date = *localtime(&cur_time);
	if (log_val == 1)
		fd = open("logs/log_basic.txt", O_CREAT | O_APPEND | O_WRONLY, 0766);
	else if (log_val == 2)
		fd = open("logs/log_less.txt", O_CREAT | O_APPEND | O_WRONLY, 0766);
	else if (log_val == 3)
		fd = open("logs/log_full.txt", O_CREAT | O_APPEND | O_WRONLY, 0766);
	dprintf(fd, "%d-%02d-%02d %02d:%02d:%02d\n", date.tm_year + 1900, date.tm_mon + 1, date.tm_mday, date.tm_hour, date.tm_min, date.tm_sec);
	dprintf(fd, "Host: %s\nStatus: %s\nResponse time: %lums\nUrl: %s\nContent requirement: %s\nContent found: %s\n\n", 
			cur->host, status, cur->response / 1000, cur->page, cur->content, found);
	if (log_val == 2 && less != NULL)
		write(fd, less, strlen(less));
	else if (log_val == 3 && cur->data != NULL)
		write(fd, cur->data, strlen(cur->data));
	close(fd);
}
