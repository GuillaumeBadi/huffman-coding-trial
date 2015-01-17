#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct			s_tree
{
	char				*data;
	int					frequency;
	struct s_tree		*left;
	struct s_tree		*right;
}						t_tree;

typedef struct			node_list
{
	t_tree				*tree;
	struct node_list	*next;
}						t_node;

int			ft_strcmp(const char *s1, const char *s2)
{
	int		i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0')
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int				ft_strequ(char const *s1, char const *s2)
{
	return (!ft_strcmp(s1, s2));
}

int						node_len(t_node *node)
{
	int					len;
	t_node				*current;

	len = 0;
	current = node;
	while (current != NULL)
	{
		len++;
		current = current->next;
	}
	return (len);
}

char			*ft_strjoinc(char *s, char c, size_t len)
{
	char		*d;

	d = (char *)malloc(sizeof(char) * len + 2);
	strcpy(d, s);
	d[len] = c;
	d[len + 1] = '\0';
	return (d);
}

char			*ft_strrev(char const *s)
{
	char		*str;
	int			len;
	int			i;

	i = 0;
	len = strlen(s);
	str = (char *)malloc(sizeof(char) * len + 1);
	while (len)
		str[i++] = s[--len];
	str[i] = '\0';
	return (str);
}

char		*base(unsigned long int number, char *str, char *set, int b)
{
	int			r;

	if (!number && !str)
		return ("0");
	if (!str)
	{
		if ((str = (char *)malloc(sizeof(char) * 2)) == NULL)
		{
			//wtf cette ligne
			str[0] = 'a';
			return (NULL);
		}
	}
	if (!number)
		return (ft_strrev(str));
	else
	{
		r = number % (unsigned long int)b;
		return (base(number / (unsigned long int)b, ft_strjoinc(str, set[r], strlen(str)), set, b));
	}
}

char					*ft_strjoin(char *s1, char *s2)
{
	char				*s;
	int					size;

	size = strlen(s1) + strlen(s2) + 1;
	s = (char *)malloc(sizeof(char) * size);
	s[size - 1] = '\0';
	s = strcpy(s, s1);
	s = strcat(s, s2);
	return (s);
}

t_tree					*branch(char *data, int freq, t_tree *left, t_tree *right)
{
	t_tree				*new;

	new = (t_tree *)malloc(sizeof(t_tree));
	new->right = right;
	new->left = left;
	if (data)
		new->data = strdup(data);
	else
		new->data = 0;
	new->frequency = freq;
	return (new);
}

t_node					*new_node(t_tree *tree)
{
	t_node				*list;

	list = (t_node *)malloc(sizeof(t_node));
	list->next = NULL;
	list->tree = tree;
	return (list);
}

int						get_freq(char c, char *text)
{
	int					freq;
	int					i;

	freq = 0;
	i = 0;
	while (text[i])
	{
		if (text[i] == c)
			freq++;
		i++;
	}
	return (freq);
}

t_node					*push_node(t_node *node, char *s, int freq, t_tree *left, t_tree *right)
{
	t_node				*current;
	t_tree				*tree;

	current = node;
	tree = branch(s, freq, left, right);
	while (current->next != NULL)
		current = current->next;
	current->next = new_node(tree);
	return (node);
}

void					print_node(t_node *node)
{
	t_node				*current;

	current = node;
	while (current != NULL)
	{
		printf("%s:%d ", current->tree->data, current->tree->frequency);
		current = current->next;
	}
	printf("\n");
}

t_node					*get_node(char *text)
{
	int					i;
	int					j;
	char				save[27];
	t_node				*node;
	char				s[2];

	i = 0;
	j = 0;
	save[26] = '\0';
	s[1] = '\0';
	node = new_node(0);
	while (text[i])
	{
		if (strchr(save, text[i]) == NULL)
		{
			save[j] = text[i];
			s[0] = text[i];
			node = push_node(node, s, get_freq(text[i], text), NULL, NULL);
			j++;
		}
		i++;	
	}
	node = node->next;
	return (node);
}

t_node					*node_reduce(t_node *node)
{
	t_node				*min1;
	t_node				*min2;
	t_node				*current;
	t_node				*new;
	t_tree				*tree;

	new = new_node(0);
	tree = branch(0, 0, 0, 0);
	min1 = new_node(tree);
	min2 = new_node(tree);
	current = node;
	while (current != NULL)
	{
		if (!min1->tree->frequency || min1->tree->frequency > current->tree->frequency)
			min1 = current;
		current = current->next;
	}
	current = node;
	while (current != NULL)
	{
		if ((!(min2->tree->frequency) || min2->tree->frequency > current->tree->frequency) && strcmp(min1->tree->data, current->tree->data))
			min2 = current;
		current = current->next;
	}
	current = node;
	while (current != NULL)
	{
		if (strcmp(current->tree->data, min1->tree->data) && strcmp(current->tree->data, min2->tree->data))
			new = push_node(new, current->tree->data, current->tree->frequency, current->tree->left, current->tree->right);
		current = current->next;
	}
	new = push_node(new, ft_strjoin(min2->tree->data, min1->tree->data), min2->tree->frequency + min1->tree->frequency, min2->tree, min1->tree);
	new = new->next;
	return (new);
}

char					*get_path(char *s, t_tree *tree)
{
	t_tree				*current;
	char				*path;

	current = tree;
	path = "";
	while (current->left != NULL && current->right != NULL)
	{
		if (strchr(current->left->data, *s) != NULL)
		{
			current = current->left;
			path = ft_strjoin(path, "0");
		}
		else if (strchr(current->right->data, *s) != NULL)
		{
			current = current->right;
			path = ft_strjoin(path, "1");
		}
		else
		{
			dprintf(1, "%s\n", "Error on path");
			return (0);
		}
	}
	return (path);
}

t_tree					*get_tree(char *text, char **compression)
{
	t_node				*node;
	t_tree				*tree;

	node = get_node(text);
	print_node(node);
	*compression = "";
	while (node_len(node) != 1)
		node = node_reduce(node);
	tree = node->tree;
	while (*text)
	{
		*compression = ft_strjoin(*compression, get_path(text, tree));
		text++;
	}
	return (tree);
}

char					*str_to_h(char *s)
{
	char				*dest;
	int					i;

	dest = "";
	i = 0;
	while (s[i])
	{
		dest = ft_strjoin(dest, base(s[i], NULL, "0123456789abcdef", 16));
		i++;
	}
	return (dest);
}

char					*str_to_b(char *s)
{
	char				*dest;
	int					i;

	dest = "";
	i = 0;
	while (s[i])
	{
		dest = ft_strjoin(dest, base(s[i], NULL, "01", 2));
		i++;
	}
	return (dest);
}

char					*decode(char *buffer, t_tree *tree)
{
	char				*dest;
	t_tree				*current;

	dest = "";
	current = tree;
	while (*buffer)
	{
		if (*buffer == '1')
			current = current->right;
		else if (*buffer == '0')
			current = current->left;
		if (current->left == NULL && current->right == NULL)
		{
			dest = ft_strjoin(dest, current->data);
			current = tree;
		}
		buffer++;
	}
	return (dest);
}

int						main(void)
{
	char				*compression;
	char				*set;
	char				*tr;

	set = str_to_h("guillaume");
	t_tree *tree = get_tree(set, &compression);
	tr = decode(compression, tree);
	dprintf(1, "control return ok = %d\n", ft_strequ(tr, set));
	return (0);
}
