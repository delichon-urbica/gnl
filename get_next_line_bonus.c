/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avoronko <avoronko@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 13:54:20 by avoronko          #+#    #+#             */
/*   Updated: 2023/07/04 20:09:03 by avoronko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*ft_free(char **buffer)
{
	if (*buffer)
	{
		free(*buffer);
		*buffer = NULL;
	}
	return (NULL);
}

static void	ft_join(char **buffer, char *buffer_temp)
{
	char	*temp;

	temp = ft_strjoin(*buffer, buffer_temp);
	ft_free(buffer);
	*buffer = temp;
}

static char	*ft_line(char **buffer)
{
	size_t	length;
	char	*line;
	char	*temp;

	if (ft_strchr(*buffer, '\n'))
		length = ft_strchr(*buffer, '\n') - *buffer + 1;
	else
		length = ft_strlen(*buffer);
	line = malloc(length + 1);
	if (!line)
		return (NULL);
	ft_strlcpy(line, *buffer, length + 1);
	if (ft_strchr(*buffer, '\n'))
	{
		temp = ft_strjoin(ft_strchr(*buffer, '\n') + 1, "");
		ft_free(buffer);
		*buffer = temp;
	}
	else
		ft_free(buffer);
	return (line);
}

static char	*ft_read(char *buffer, int fd)
{
	int		bytes_read;
	char	*buffer_temp;

	buffer_temp = malloc(BUFFER_SIZE + 1);
	if (!buffer_temp)
		return (NULL);
	bytes_read = read(fd, buffer_temp, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		buffer_temp[bytes_read] = '\0';
		if (!buffer)
			buffer = ft_strjoin("", buffer_temp);
		else
			ft_join(&buffer, buffer_temp);
		if (ft_strchr(buffer, '\n'))
			break ;
		bytes_read = read(fd, buffer_temp, BUFFER_SIZE);
	}
	free(buffer_temp);
	if (bytes_read == -1 && buffer != 0)
		ft_free(&buffer);
	return (buffer);
}

char	*get_next_line(int fd)
{
	static char	*buffer[FD_MAX];
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || BUFFER_SIZE > INT_MAX - 1)
		return (NULL);
	buffer[fd] = ft_read(buffer[fd], fd);
	if (!buffer[fd] || !*buffer[fd])
	{
		ft_free(&buffer[fd]);
		return (NULL);
	}
	line = ft_line(&buffer[fd]);
	return (line);
}

/*
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int	main(void)
{
	int fd;
	char *line;
	int i;

	fd = open("read_error.txt", O_RDONLY);
	if (fd == -1)
	{
		write(1, "open error", 10);
		return (1);
	}
	i = 1;
	line = get_next_line(fd);
	while(line)
	{
		printf("line: %i >>> %s\n", i++, line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}

int main(void)
{
    char *line;

    while (1)
	{
        line = get_next_line(STDIN_FILENO);
        if (line) 
		{
            printf("line: %s\n", line);
            free(line);
        }
		else if(!line)
			break ;
    }
    
    return (0);
}*/