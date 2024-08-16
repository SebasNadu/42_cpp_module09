/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebasnadu <johnavar@student.42berlin.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 13:50:57 by sebasnadu         #+#    #+#             */
/*   Updated: 2024/08/16 11:23:44 by sebasnadu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <map>
#include <time.h>

class BitcoinExchange {
public:
	BitcoinExchange(std::string const &filename);
	BitcoinExchange(BitcoinExchange const &src);
	BitcoinExchange	&operator=(BitcoinExchange const &rhs);
	~BitcoinExchange(void);

	std::map<time_t, double> const	&getBtcDB(void) const;
	void	processFile(std::string const &filename);
private:
	BitcoinExchange(void);

	std::map<time_t, double>	_btcDB;

	void	_checkFile(std::string const &filename);
	void	_checkDatabaseHeader(std::string const &line);
	void	_checkInputHeader(std::string const &line);
	bool	_setDate(std::string const &s_date, time_t &date);
	bool	_setRate(std::string const &s_rate, double &rate);
	bool	_setValue(std::string const &s_value, double &value);
	bool	_setExchangeValue(time_t date, double &value, double &exchangeValue);
};

std::ostream &operator<<(std::ostream &out, BitcoinExchange const &btc);
