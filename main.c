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

int						main(void)
{
	char				*compression;

	t_tree *tree = get_tree("aaabbabbabacccacadddcccdccabab", &compression);
	dprintf(1, "a = %s\n", get_path("a", tree));
	dprintf(1, "b = %s\n", get_path("b", tree));
	dprintf(1, "c = %s\n", get_path("c", tree));
	dprintf(1, "d = %s\n", get_path("d", tree));
	dprintf(1, "compression: %s\n", compression);
	return (0);
}
