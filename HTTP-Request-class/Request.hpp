/*
   File: Request.hpp
   By: Azaria
   Created: 2026/03/02 14:28:56
*/


#ifndef REQUEST_HPP
#define REQUEST_HPP

#define BUFFER_MAX 4000
#define CONTENT_LENGTH_MAX 1000000
#define CRLF "\r\n"

#include <vector>
#include <algorithm>
#include <sstream>
#include <map>
# include "../ClientSocket.hpp"

class Request
{
	public :
        Request(std::string &value, int receivedBytes);
        ~Request();

		Request(ClientSocket& client);

        std::string getMethod() const;
        std::string getPathName() const;
        std::string getHTTPversion() const;
        bool        getHasQuery() const;
        int         getPort() const;
        int         getContentLength() const;
        bool        getHasContentLength() const;
        std::string getContentType() const;
		std::string getBody( void ) const;


		class NotImplementedMethod: public std::exception
		{
            const char *what () const throw();
        };

        class InvalidMethod: public std::exception
		{
            const char *what () const throw();
        };

        class BadRequest: public std::exception
		{
            const char *what () const throw();
        };

        class URITooLong: public std::exception
		{
            const char *what () const throw();
        };

        class NotFound: public std::exception
		{
            const char *what () const throw();
        };

        class LengthRequired: public std::exception
		{
            const char *what () const throw();
        };

        class PayloadTooLarge: public std::exception
		{
            const char *what () const throw();
        };

		class UnsupportedMediaType: public std::exception
		{
            const char *what () const throw();
		};

	private :
        std::string		_buffer;
        int				_receivedByte;
		int				_i;
        std::string		_method;
        std::string		_pathname;
        std::string		_file;
        std::string		_tmp;
        std::string		_httpVersion;
        bool			_hasPourcentEncode;
        bool			_hasQuery;
        bool			_hasContentLength;
        bool			_hasContentType;
        size_t			_contentLength;
        int				_port;
		std::string		_body;
		bool			_hasBody;
        std::vector<std::string> 			_contentType;
        std::map<std::string, std::string>	_headers;

        bool    checkHeaderLineEncode(char c) const;
        bool    checkPathEncode(char c) const;
        bool    checkPathPourcentEncoded(char c) const; 
        bool    checkQueryPourcentEncoded(char c) const;
        bool    checkQueryEncode(char c) const;
        bool    checkHeaderEncode(char c) const;

		bool	isHexa(char c) const;
        int     HexaToInt(char c) const;
        char    PourcentHexaToChar(char first, char second) const;

		std::string &toLowerCase(std::string &src) const;

        void    setPathname();
        void    setMethod();
        void    setHost();
        void    setContentLength();
        void    setContentType();
		void	setBody( void );
        void    setHttpVersion();

        void    readProtocol();
        char    readByte(); 
        void    readHeader();
        void    readQuery();
        char    readPourcentHexa();

		void	normalizePathname();
        void    DecodePourcentEncode();
		bool	requiredHeaderPresent (void) const;
};

#endif /* REQUEST_HPP */
