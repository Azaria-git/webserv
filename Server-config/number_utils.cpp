/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   number_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toloandr <toloandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 07:48:56 by toloandr          #+#    #+#             */
/*   Updated: 2026/02/17 07:48:57 by toloandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "number_utils.hpp"

bool isPositiveInt(const std::string &s)
{
    size_t i = 0;

    if (s.empty())
        return (false);
    if (i == s.size())
        return (false);
    for (; i < s.size(); i++)
        if (!std::isdigit(static_cast<unsigned char>(s[i])))
            return (false);
    std::string numStr = s;
    if (numStr.length() > 10)
        return (false);
    if (numStr.length() == 10)
    {
        if (numStr > "2147483647")
            return (false);
    }
    return (true);
}

bool isIpV4(const std::string &s)
{
    int dots = 0;
    for (std::string::const_iterator it = s.begin(); it != s.end(); it++)
    {
        if (*it == '.')
            dots ++;
    }
    int c = 0;
    std::istringstream iss(s);
    std::string word;
    double d;

    while (std::getline(iss, word, '.'))
    {
        if (!isPositiveInt(word))
            return (false);
        d = std::strtod(word.c_str(), NULL);
        if (d > 255)
            return (false);
        c++;
    }
    if (c != 4 || dots != 3)
        return (false);
    return (true);
}