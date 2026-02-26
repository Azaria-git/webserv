/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toloandr <toloandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 07:49:12 by toloandr          #+#    #+#             */
/*   Updated: 2026/02/17 08:34:10 by toloandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config()
{
}

Config::Config(std::string const &fileName): data(""), confId(-1)
{
    data = getFileContent(fileName);
    fillConfigs();
}

Config::Config(const Config& other)
    : data(other.data),
      confId(other.confId),
      confs(other.confs),
      configs(other.configs)
{
}


Config::~Config()
{
}

Config& Config::operator=(const Config& other)
{
    if (this != &other)
    {
        data = other.data;
        confId = other.confId;
        confs = other.confs;
        configs = other.configs;
    }
    return *this;
}



std::string Config::getFileContent(std::string fileName)
{
    bool hasServer = false;
    std::ifstream file(fileName.c_str());
    std::string line;
    std::string fileContent = "";
    if (!file.is_open())
    {
        throw ConfigException("Configuration file: can't open file");
    }
    while (std::getline(file, line))
    {
        fileContent  += line + "\n";
        if (line == "server:")
        {
            hasServer = true;
            confId++;
            confs[confId] = "";
            continue;
        }
        if (confId >= 0)
        {
            confs[confId] += line + "\n";
        }
        // std::cout << line << std::endl;
    }
    file.close();
    if (!hasServer)
        throw ConfigException("'server:' keyword not found");
    return (fileContent);
}

int Config::validEntry(std::string &line)
{
    if (line.find(':') != std::string::npos)
    {
        if (line == "root:" || line == "client_max_body_size:" ||  line == "host:" || line == "port:" || line == "index:"  || line == "default_error_page:" || line == "methods:" || line == "uploads:" || line == "directory_listing:")
            return (1);
        else
            return (0);
    }
    return (1);
}


void Config::fillConfigs()
{
    int mandatory = 0;
    bool rootDefined = false;
    bool keyDefined = false;
    std::map<std::string, int> key_count;
    for (int idx = 0; idx <= confId; idx++)
    {
        configs[idx].methods = 0;
    }
    for (int idx = 0; idx <= confId; idx++)
    {
        std::istringstream iss(confs[idx]);
        std::string line;
        std::string currentKey;

        std::string error;
        std::string rootPath = "websites/"; 

        while (std::getline(iss, line))
        {
            if (line.size() == 0)
                continue;
            if (!validEntry(line))
                throw ConfigException("Configuration file: invalid entry");
            if (line == "root:" || line == "client_max_body_size:" || line == "host:" || line == "port:" || line == "index:" || line == "default_error_page:" || line == "methods:" || line == "uploads:" || line == "directory_listing:")
            {
                key_count[line]++;
                if (key_count[line] > 1)
                    throw ConfigException("Configuration file: duplicate keys");
                if (keyDefined)
                    throw ConfigException("Configuration file: key with no value");
                currentKey = line;
                keyDefined = true;
                continue;
            }
            if (currentKey == "root:")
            {
                rootPath += line;
                if (!validDirectory(rootPath, error))
                    throw ConfigException("root: folder not found");
                configs[idx].root = rootPath;
                mandatory |= 1;
                rootDefined = true;
                keyDefined = false;
            }
            else if (currentKey == "client_max_body_size:")
            {
                if (!isPositiveInt(line))
                    throw ConfigException("client_max_body_size: value > 0 and Integer");
                double value = std::strtod(line.c_str(), NULL);
                if (value > 10000)
                    throw ConfigException("client_max_body_size: value < 10000");
                configs[idx].client_max_body_size = value;
                mandatory |= 2;
                keyDefined = false;
            }
            else if (currentKey == "host:")
            {
                if (!isIpV4(line))
                    throw ConfigException("host: IPv4");
                configs[idx].host = line;
                mandatory |= 4;
                keyDefined = false;
            }
            else if (currentKey == "port:")
            {
                if (!isPositiveInt(line))
                    throw ConfigException("port: should be a positive integer");
                double value = std::strtod(line.c_str(), NULL);
                if (value > 65535)
                    throw ConfigException("port: value > 65535");
                configs[idx].port = value;
                mandatory |= 8;
                keyDefined = false;
            }
            else if (currentKey == "index:")
            {
                configs[idx].index = line;
                mandatory |= 16;
                keyDefined = false;
            }
            else if (currentKey == "default_error_page:")
            {
                std::string error_page = configs[idx].root + "/" + line;
                if (!validFile(error_page, error))
                    throw ConfigException("default_error_page: " + error);
                if (!rootDefined)
                    throw ConfigException("default_error_page: root not defined");
                configs[idx].default_error_page = error_page;
                mandatory |= 32;
                keyDefined = false;
            }
            else if (currentKey == "methods:")
            {
                if (line != "GET" && line != "POST" && line != "DELETE")
                    throw ConfigException("methods: GET , POST or DELETE");
                if (line == "GET")
                    configs[idx].methods |= GET;
                if (line == "POST")
                    configs[idx].methods |= POST;
                if (line == "DELETE")
                    configs[idx].methods |= DELETE;
                mandatory |= 64;
                keyDefined = false;
                
            }
            else if (currentKey == "uploads:")
            {
                if (!validDirectory(line, error))
                    throw ConfigException("uploads: " + error);
                if (!rootDefined)
                    throw ConfigException("uploads: root not defined");
                configs[idx].uploads = configs[idx].root + "/" + line;
                mandatory |= 128;
                keyDefined = false;
            }
            else if (currentKey == "directory_listing:")
            {
                if (line == "ON")
                    configs[idx].directory_listing = 1;
                else if (line == "OFF")
                    configs[idx].directory_listing = 0;
                else
                    throw ConfigException("directory_listing: ON or OFF");
                mandatory |= 256;
                keyDefined = false;
            }
        }
        // if (mandatory != (1+2+4+8+16+32+64+128+256))
        if (mandatory != 511)
            throw ConfigException("Configuration file: missing field(s)");
        mandatory = 0;
        rootDefined = false;
        key_count.clear();

    }
}

int Config::getConfId()
{
    return confId;
}

std::map<int, UnitConf_t> Config::getConfigs()
{
    return configs;
}

Config::ConfigException::ConfigException(const std::string& msg) : message(msg)
{}

const char* Config::ConfigException::what() const throw()
{
    return (message.c_str());
}

Config::ConfigException::~ConfigException() throw()
{}