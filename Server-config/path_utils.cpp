/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toloandr <toloandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 07:48:50 by toloandr          #+#    #+#             */
/*   Updated: 2026/02/17 08:23:54 by toloandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "path_utils.hpp"

bool validDirectory(const std::string& path, std::string& errorMsg)
{
    if (path.empty())
    {
        errorMsg = "Path cannot be empty";
        return (false);
    }
    struct stat st;
    if (stat(path.c_str(), &st) != 0 || !S_ISDIR(st.st_mode))
    {
        errorMsg = "Path does not exist or is not a directory: " + path;
        return (false);
    }
    if (access(path.c_str(), W_OK) != 0)
    {
        errorMsg = "Directory is not writable: " + path;
        return (false);
    }
    return (true);
}


bool validFile(const std::string& path, std::string& errorMsg)
{
    if (path.empty())
    {
        errorMsg = "File path cannot be empty";
        return (false);
    }
    struct stat st;
    if (stat(path.c_str(), &st) != 0)
    {
        errorMsg = "File does not exist: " + path;
        return (false);
    }
    if (!S_ISREG(st.st_mode))
    {
        errorMsg = "Path is not a regular file: " + path;
        return (false);
    }
    if (access(path.c_str(), R_OK) != 0)
    {
        errorMsg = "File is not readable: " + path;
        return (false);
    }
    return (true);
}