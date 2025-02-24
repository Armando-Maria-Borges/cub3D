/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fseek.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnzila <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 09:42:34 by lnzila            #+#    #+#             */
/*   Updated: 2025/02/24 10:08:02 by lnzila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cubo3d.h"

int	ft_fseek(FILE *stream, long offset, int whence)
{
	long	new_pos;

	if (!stream)
		return (-1);
	if (whence == SEEK_SET)
		new_pos = offset;
	else if (whence == SEE_CUR)
		new_pos = ftell(stream) + offset;
	else if (whence == SEEK_END)
}
