#include "monitor.h"

/*
** Joins the read data.
** Getline reads BUF_SIZE amount of bytes, need to join them into data.
*/
static int	strjoin_data(t_monitor *cur, char buf[BUF_SIZE])
{
	char	*temp;

	temp = NULL;
	if (cur->data == NULL)
	{
		cur->data = (char*)malloc(sizeof(char));
		if (!cur->data)
		{
			fprintf(stderr, "Allocation of data string failed\n");
			exit(EXIT_FAILURE);
		}
		bzero(cur->data, sizeof(char));
	}
	temp = strjoin(cur->data, buf);
	free(cur->data);
	cur->data = temp;
	return (1);
}

/*
** Reads data from socket pointed by sockfd.
** Snprintf formats GET request string into request.
** Timespec structs are for taking the time from writing to server until we are ablo to read response.
** Keep reading in loop until there is no more data in socket.
*/
static int	http_get_read(t_monitor *cur, int sockfd)
{
	int				i;
	char			request[REQUEST_SIZE];
	char			buf[BUF_SIZE];
	struct timespec	send;
	struct timespec	end;

	cur->response = 0;
	snprintf(request, REQUEST_SIZE,
			"GET %s HTTP/1.1\r\n"
			"Host: %s\r\n"
			"Connection: close\r\n\r\n",
			cur->page, cur->host);
	clock_gettime(CLOCK_MONOTONIC_RAW, &send);
	i = write(sockfd, request, strlen(request));
	if (i < 0)
	{	
		error_log(cur->host, "Error writing to socket");
		return (0);
	}
	while (1)
	{
		bzero(buf, BUF_SIZE);
		i = read(sockfd, buf, 4095);
		if (i < 0)
		{
			error_log(cur->host, "Error reading from socket");
			break;
		}
		if (!cur->response)
		{
			clock_gettime(CLOCK_MONOTONIC_RAW, &end);
			cur->response = (end.tv_sec - send.tv_sec) * 1000000 + (end.tv_nsec - send.tv_nsec) / 1000;
		}
		if (i == 0)
			break;
		strjoin_data(cur, buf);
	}
	return (1);
}

/*
** GET request preparation.
** Initialize addrinfo values.
** Attempt to get addrinfo from HTTP port 80 of host address.
** Cycle through addrinfo structs attempting to connect.
** Call http_get_read to send the request and read data.
*/
static int	http_get_request(t_monitor *cur)
{
	struct addrinfo	hints;
	struct addrinfo	*result;
	struct addrinfo	*rp;
	int				sockfd;
	int				i;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;		//both IPv4 and IPv6 are allowed
	hints.ai_socktype = SOCK_STREAM;	//tcp connection (connect-converse-terminate)
	hints.ai_flags = 0;					//no flags
	hints.ai_protocol = 0;				//any protocol

	i = getaddrinfo(cur->host, "80", &hints, &result);
	if (i != 0)
	{
		error_log(cur->host, (char*)gai_strerror(i));
		return (0);
	}
	for (rp = result; rp != NULL; rp = rp->ai_next)
	{
		sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sockfd == -1)	//failed to connect
			continue;
		if (connect(sockfd, rp->ai_addr, rp->ai_addrlen) != -1)		//success
			break;
		close(sockfd);
	}
	if (rp == NULL)
	{
		error_log(cur->host, "Error, could not connect");
		close(sockfd);
		return (0);
	}
	http_get_read(cur, sockfd);
	freeaddrinfo(result);
	close(sockfd);
	return (0);
}

/*
** Loop to call periodic get requests.
** Initialises info struct.
** Loops until time is up or user interrupts with ctrl-c/cmd-c.
** Calls get_request, then parses server data, checks if content requirement is within data,
** logs data, sleeps delay amount, frees current cycle data and repeats.
*/
int			http_get_loop(t_monitor **mntr, t_flag *flags)
{
	t_monitor		*cur;
	t_parse			*info;
	struct timespec start;
	struct timespec elapsed;
	int				total;

	cur = *mntr;
	info = init_info();
	clock_gettime(CLOCK_MONOTONIC_RAW, &start);
	total = 0;
	while (cur && total <= flags->timer)
	{
		if (flags->verbose)
			printf("Sending GET request to host: %s\n", cur->host);
		http_get_request(cur);
		info->status = get_status(cur->data);
		if (flags->log_val == 2 && cur->data != NULL)
			info->less = less_log(cur->data, info->less);
		check_content(cur->data, cur->content, info);
		monitor_log(cur, info->status, info->less, flags->log_val, info->found);
		if (flags->verbose)
			printf("Status: %s\nResponse time: %lums\n\n", info->status, cur->response / 1000);
		sleep(flags->delay);
		free_cycle_data(cur, info);
		cur = cur->next;
		clock_gettime(CLOCK_MONOTONIC_RAW, &elapsed);
		total = elapsed.tv_sec - start.tv_sec;
		if (cur == NULL)
			cur = *mntr;
	}
	free(info);
	return (1);
}
