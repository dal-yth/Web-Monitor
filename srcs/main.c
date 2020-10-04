#include "monitor.h"

/*
** Web monitor main
** Checks arguments and reads flags.
** Reads file and saves the data.
** Monitors HTTP web pages.
** Frees and exits.
*/
int		main (int argc, char **argv)
{
	t_monitor	*mntr;
	t_flag		*flags;

	mntr = NULL;
	flags = NULL;
	if (argc < 2)
		print_usage();
	flags = init_flags();
	handle_flags(flags, argc, argv);
	read_file(argv[argc - 1], &mntr);
	http_get_loop(&mntr, flags);
	free_list(mntr);
	free(flags);
	return (1);
}
