/* ************************************************************************** */
/*	                                                                        */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 08:58:10 by craimond          #+#    #+#             */
/*   Updated: 2024/03/05 16:53:02 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

void		ft_putstr_fd(const char *const s, const uint16_t fd);
char		*ft_strtok(char *const str, const char sep);
char		*ft_strtrim(const char *const s1, const char *const set, const uint8_t alloc_type);
uint16_t	ft_strlen(const char *const s);
char		*ft_strdup(const char *const s, const uint8_t alloc_type);
char		*ft_strjoin(const char *const s1, const char *const s2, const uint8_t alloc_type);
char		*ft_strnstr(const char *const big, const char *const little, const uint16_t len);
char		*ft_strcpy(char *const dst, const char *const src);
uint16_t	ft_strlcpy(char *const dst, const char *const src, const uint16_t size);
char		*ft_strcat(char *const dst, const char *const src);
int16_t		ft_strcmp(const char *const s1, const char *const s2);
int16_t		ft_strncmp(const char *const s1, const char *const s2, const uint16_t n);
int16_t		ft_strcmp_lower(const char *const s1, const char *const s2);
char		*ft_strchr(const char *const s, const char c);
char		ft_tolower(const char c);

void		*ft_calloc(const size_t count, const size_t size);
uint16_t	ft_matrixsize(const char **const matrix);
bool		is_digit(const char c);
int32_t		ft_atoi(const char *str);
char		*ft_itoa(const int32_t n);
void		set_errnos(void);
bool		*get_errnos(void);
bool		is_bad_errno(const int errno_value);
bool		is_quote(const char c);
bool		is_shell_space(const char c);
bool		is_empty_str(const char *str);
bool		is_digit(const char c);

t_list		*lstnew(const void *const content, const uint8_t alloc_type);
void		lstadd_front(t_list **const lst, t_list *const new);
t_list		*lstlast(const t_list *lst);
void		lstadd_back(t_list **const lst, t_list *const new);
void		lstreverse(t_list **const lst);
void		lstclear(t_list **lst, void (*del)(void *));
void		lstremoveone(t_list **const head, t_list *const to_remove, void (*del)(void *));
void		lstremoveif(t_list **const lst, bool (*f)(const void *const), void (*del)(void *));

t_tree		*treenew(const void *const content, const uint8_t alloc_type);
t_tree		*treeadd_below(t_tree *const node, const t_tree *const new);
t_tree		*treefirst(const t_tree *root);
void		treeclear(t_tree **const tree, void (*del)(void *const));

#endif