/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   number_utils.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toloandr <toloandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 07:48:53 by toloandr          #+#    #+#             */
/*   Updated: 2026/02/17 07:48:54 by toloandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMBER_UTILS_HPP
# define NUMBER_UTILS_HPP

#include <string>
#include <cctype>
#include <sstream>
#include <cstdlib>

bool isPositiveInt(const std::string &s);
bool isIpV4(const std::string &s);

#endif