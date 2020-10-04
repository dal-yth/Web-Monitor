#include "monitor.h"

/*
** Usage
*/
void	print_usage()
{
	printf("Usage: ./web_monitor -log -t -delay -v file\n");
	printf("-log [number]\tLogging options:\n");
	printf("\t\t1 - basic, logs url, status, response time and content status (turned on by default)\n");
	printf("\t\t2 - less, in addition to basic, logs all server data\n");
	printf("\t\t3 - full, logs all information\n");
	printf("\t\tLog data is saved to logs folder.\n\n");
	printf("-t [number]\tTime in seconds how long the monitor runs (default 60s)\n\n");
	printf("-delay [number]\tDelay between requests in seconds\n\n");
	printf("-v\t\tVerbose mode\n\n");
	printf("file\t\tFile with monitored urls:\n");
	printf("\t\tEach line with one entry: url, content requirement\n");
	printf("\t\tFile format example: http://www.example.com/page, example text\n");
	exit(0);
}

/*
** simple strjoin function.
*/
char	*strjoin(const char* s1, const char* s2)
{
	int	i;

	i = strlen(s1) + strlen(s2) + 1;
    char *result = (char*)malloc(sizeof(char) * i);
	bzero(result, i);
	if (!result)
	{
		fprintf(stderr, "Error allocating data");
		exit(EXIT_FAILURE);
	}
    if (result)
    {
        strcpy(result, s1);
        strcat(result, s2);
    }
    return (result);
}

/*
** Allocate and initialize t_parse *info struct.
*/
t_parse	*init_info(void)
{
	t_parse	*info;

	info = (t_parse*)malloc(sizeof(t_parse));
	if (!info)
	{
		fprintf(stderr, "Allocation of info struct failed\n");
		exit(EXIT_FAILURE);
	}
	info->found = NULL;
	info->status = NULL;
	info->less = NULL;
	return (info);
}

/*
** Allocate and initialize t_flag *flags struct.
*/
t_flag	*init_flags(void)
{
	t_flag	*flags;

	flags = (t_flag*)malloc(sizeof(t_flag));
	if (!flags)
	{
		fprintf(stderr, "Allocation of flags failed\n");
		exit(EXIT_FAILURE);
	}
	flags->log_val = 1;
	flags->delay = 1;
	flags->timer = 60.0;
	return (flags);
}

/*
** Flag handling
** Cycle through **argv to find matches for -log, -v and -delay
*/
int		handle_flags(t_flag *flags, int argc, char **argv)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (!strcmp("-log", argv[i]))
		{
			flags->log_val = atoi(argv[i + 1]);
			if (flags->log_val < 1 || flags->log_val > 3)
			{
				fprintf(stderr, "Incorrect usage of -log flag\n");
				exit(EXIT_FAILURE);
			}
		}
		else if (!strcmp("-delay", argv[i]))
		{
			flags->delay = atoi(argv[i + 1]);
			if (flags->delay < 0)
			{
				fprintf(stderr, "Incorrect usage of -delay flag\n");
				exit(EXIT_FAILURE);
			}
		}
		else if (!strcmp("-t", argv[i]))
		{
			flags->timer = atof(argv[i + 1]);
			if (flags->timer <= 0)
			{
				fprintf(stderr, "Incorrect usage of -timer flag\n");
				exit(EXIT_FAILURE);
			}
		}
		else if (!strcmp("-v", argv[i]))
			flags->verbose = 1;
		i += 1;
	}
	return (1);
}
