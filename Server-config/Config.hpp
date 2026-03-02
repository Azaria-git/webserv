/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toloandr <toloandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 07:49:06 by toloandr          #+#    #+#             */
/*   Updated: 2026/03/01 01:22:29 by toloandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include "path_utils.hpp"
#include "number_utils.hpp"
#include <cstdlib>
#include <exception>
#include <string>
#include <cctype>


enum HttpMethod
{
    GET    = 1,  // 001
    POST   = 2,  // 010
    DELETE = 4   // 100
};

typedef struct UnitConf
{
    std::string root;
    int client_max_body_size; 
    int port;
    std::string host;
    std::string index;
    std::string default_error_page;
    int methods;
    std::string uploads;
    bool directory_listing;
} UnitConf_t;

class Config
{
private:
    std::string data;
    int confId;
    std::map<int, std::string> confs;
    std::map<int, UnitConf_t> configs;
public:
    Config();
    Config(std::string const &fileName);
    Config(const Config& other);
    ~Config();

    Config& operator=(const Config& other);

    std::string getFileContent(std::string fileName);
    int validEntry(std::string &line);
    void fillConfigs();
    int getConfId();
    std::map<int, UnitConf_t> getConfigs();

    class ConfigException : public std::exception
    {
    private:
        std::string message;

    public:
        ConfigException(const std::string& msg);
        virtual const char* what() const throw();
        virtual ~ConfigException() throw();       
    };
    void checkPorts();
    void checkDirectoryListing();
    int checkLine(std::string line);
};

#endif
