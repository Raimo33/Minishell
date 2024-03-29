/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 17:21:31 by egualand          #+#    #+#             */
/*   Updated: 2024/03/05 16:41:49 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVP_H
# define ENVP_H

void	envp_init(const char **const envp);
void	ft_setenv(const char *const name, const char *const value, const bool replace);
char	*ft_getenv(const char *const name);
void	ft_unsetenv(const char *const name);

t_tree	*envp_tree_add(t_tree *const root, const char *const str);
t_tree	*envp_tree_remove(t_tree *const root, const char *const name, const uint16_t name_len);
t_tree	*envp_tree_find(const t_tree *const root, const char *const name, const uint16_t name_len);
void	envp_print_export(const t_tree *const root);

void	envp_matrix_add(const char *str);
void	envp_matrix_remove(const char *const env_name, const uint16_t name_len);
void	envp_print_env(void);

#endif