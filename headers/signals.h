/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:04:13 by craimond          #+#    #+#             */
/*   Updated: 2024/03/10 22:33:21 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# define S_INTERACTIVE  0
# define S_HEREDOC      1
# define S_COMMAND      2

void	set_signals(const uint8_t mode);
void	safe_exit(const int32_t signo);

#endif
