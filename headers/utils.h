/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 08:58:10 by craimond          #+#    #+#             */
/*   Updated: 2024/02/08 20:51:07 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

void    		ft_putstr_fd(char *s, int fd);
char    		*ft_strdup(char *s);
char			**ft_split(char *s, char c);
size_t			ft_strlen(char *s);
char    		*ft_strjoin(char *s1, char *s2);
char			*ft_strnstr(char *big, char *little, size_t len);
char    		*ft_strcpy(char *dst, char *src);
size_t	        ft_strlcpy(char *dst,  char *src, size_t size);
char 			*ft_strcat(char *dst, char *src);
int	    		ft_strncmp(char *s1, char *s2, size_t n);
int	            ft_strcmp(char *s1, char *s2);
char	        *ft_strchr(char *s, int c);

void    		*ft_calloc(size_t count, size_t size);
void			ft_freematrix(char **matrix);
size_t			ft_matrixsize(char **matrix);
int				ft_isdigit(int c);
unsigned int 	ft_atou(char *nptr);
char            *ft_itoa(int n);

t_list			*lstnew_p(void *content);
void	        lstadd_front(t_list **lst, t_list *new);
void            lstreverse(t_list **lst);
void			lstclear(t_list **lst, void (*del)(void *));
void			lstdelone(t_list *lst, void (*del)(void *));
void            lstremoveone(t_list **head, t_list *to_remove, void (*del)(void *));
void	        lstdelif(t_list **lst, bool (*f)(void *), void (*del)(void *));

t_tree			*treenew_p(int8_t type, t_cmd *cmd);
void 			treeadd_below(t_tree **head, t_tree *new);
t_tree			*tree_last(t_tree *head);
void	        treeclear(t_tree **tree, void (*del)(void *));

#endif