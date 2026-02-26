/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toloandr <toloandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 07:49:03 by toloandr          #+#    #+#             */
/*   Updated: 2026/02/17 07:49:04 by toloandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

#include <iostream>

int main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << "usage: ./webserv [configuration file]" << std::endl;
		return (1);
	}
	try
	{
		std::string filename = av[1];
		Config conf(filename);
		std::map<int, UnitConf_t> configs;
		configs = conf.getConfigs();
		std::cout << configs[0].root << std::endl;
		std::cout << configs[0].port << std::endl;
		std::cout << configs[0].host << std::endl;
		std::cout << configs[0].index << std::endl;
		std::cout << configs[0].default_error_page << std::endl;
		std::cout << configs[0].methods << std::endl;
		std::cout << configs[0].uploads << std::endl;
		std::cout << configs[0].client_max_body_size << std::endl;
		std::cout << configs[0].directory_listing << std::endl;
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}


	return (0);
}