#ifndef MONITOR_H
# define MONITOR_H

# define _GNU_SOURCE	//required by strcasestr function, must be defined before includes

# include <stdio.h>
# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <ctype.h>
# include <time.h>
# include <netdb.h>

# define DELIMITER_CHAR ","
# define BUF_SIZE 4096
# define REQUEST_SIZE 512

/*
** Flag struct of web monitor.
*/
typedef struct			s_flag
{
	int					log_val;
	int					delay;
	int					verbose;
	double				timer;
}						t_flag;

/*
** Log info struct.
*/
typedef struct			s_parse
{
	char 				*less;
	char				*status;
	char				*found;
}						t_parse;

/*
** URL struct
** Each link holds the host, url, content requirements and read html data from the page.
*/
typedef struct			s_monitor
{
	char				*host;
	char				*page;
	char				*content;
	char				*data;
	uint64_t			response;
	struct s_monitor	*next;
}						t_monitor;

/*
** Initialization, usage, flags, file reading, init.
*/
void		print_usage(void);
int			handle_flags(t_flag *flags, int argc, char **argv);
int			read_file(char *file, t_monitor **mntr);
t_parse 	*init_info(void);
t_flag	 	*init_flags(void);

/*
** GET request, list functions
*/
int			http_get_loop(t_monitor **mntr, t_flag *flags);
int			url_append(t_monitor **head, char *host, char *page, char *content);

/*
** Log functions and log parsing
*/
void		monitor_log(t_monitor *cur, char *status, char *less, int log_val, char *found);
void		error_log(char *data, char *msg);
char		*get_status(char *data);
char		*less_log(char *data, char *less);
void		check_content(char *data, char *content, t_parse *info);

/*
** Free functions
*/
void		free_list(t_monitor *mntr);
void		free_cycle_data(t_monitor *cur, t_parse *info);
void		free_info(t_parse *info);

/*
** Utility functions
*/
char 		*strjoin(const char* s1, const char* s2);

#endif
