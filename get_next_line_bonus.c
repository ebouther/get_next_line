#include "get_next_line.h"

static t_read	*ft_search_lst(t_read **lst, int fd)
{
	t_read	*tmp;

	tmp = *lst;
	while (tmp)
	{
		if (tmp->fd == fd)
			break ;
		tmp = tmp->next;
	}
	return (tmp);
}

static t_read	*ft_newlst(char	*buf, int fd)
{
	t_read	*list;

	if (!(list = (t_read *)malloc(sizeof(*list))))
		return (NULL);
	if (!buf)
	{
		list->buf = NULL;
		list->fd = 0;
	}
	else
	{
		if (!(buf))
		{
			if (!(list->buf = ft_strnew(0)))
				return (NULL);
		}
		list->buf = buf;
		list->fd = fd;
	}
	list->next = NULL;
	return (list);
}

static t_read	*ft_ret_lst(int fd, char **line, t_read **lst)
{
	t_read *new;

	if (fd < 0 || !line)
		return (NULL);
	if (!(new = ft_search_lst(lst, fd)))
	{
		new = ft_newlst(ft_strnew(0), fd);
		new->next = *lst;
		*lst = new;
	}
	return (new);
}

static char	*ft_read_file(int *ret, t_read *lst, const int fd)
{
	char	buffer[BUFF_SIZE + 1];
	char	*tmp;

	*ret = read(fd, buffer, BUFF_SIZE);
	buffer[*ret] = '\0';
	if ((lst->buf)[0] != '\0')
		tmp = ft_strjoin(lst->buf, buffer);
	else
		tmp = ft_strdup(buffer);
	ft_strclr(lst->buf);
	return (tmp);
}

int			get_next_line(int const fd, char **line)
{
	static t_read	*lst;
	t_read			*tmp;
	int				ret;
	char			*cpy;
	char			*str;

	if ((tmp = ft_ret_lst(fd, line, &lst)) == NULL)
		return (-1);
	*line = ft_strnew(0);
	ret = 1;
	while (ret > 0)
	{
		cpy = ft_read_file(&ret, tmp, fd);
		if ((str = ft_strchr(cpy, '\n')) != NULL)
		{
			*str = '\0';
			tmp->buf = ft_strdup(str + 1);
			*line = ft_strjoin(*line, cpy);
			return (1);
		}
		*line = ft_strjoin(*line, cpy);
	}
	return (ret);
}