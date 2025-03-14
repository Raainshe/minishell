/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_holder.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakoni <rmakoni@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 16:49:53 by ksinn             #+#    #+#             */
/*   Updated: 2025/03/14 17:19:34 by rmakoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"

t_list	**gc_holder(void)
{
	static t_list ptr[GC_ARR_SIZE] = {0};

	return (&ptr);
}