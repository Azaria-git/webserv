/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toloandr <toloandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 07:48:47 by toloandr          #+#    #+#             */
/*   Updated: 2026/02/17 08:24:19 by toloandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PATH_UTILS_HPP
# define PATH_UTILS_HPP

#include <sys/stat.h>
#include <string>
#include <unistd.h>

bool validDirectory(const std::string& path, std::string& errorMsg);
bool validFile(const std::string& path, std::string& errorMsg);

#endif