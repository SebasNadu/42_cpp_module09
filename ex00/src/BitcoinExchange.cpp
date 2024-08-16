/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebasnadu <johnavar@student.42berlin.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 14:35:09 by sebasnadu         #+#    #+#             */
/*   Updated: 2024/08/16 11:36:08 by sebasnadu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/BitcoinExchange.hpp"
#include "../include/colors.hpp"

#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <iostream>
#include <limits>
#include <ctime>

void BitcoinExchange::_checkDatabaseHeader(std::string const &line) {
	if (line != "date,exchange_rate")
		throw std::runtime_error("Invalid header on the input file => " + line);
}

void BitcoinExchange::_checkInputHeader(std::string const &line) {
	std::string word;

	std::istringstream stream(line);
	for (int i = 0; i < 3; i++) {
		stream >> word;
		switch (i) {
			case 0:
				if (word != "date")
					throw std::runtime_error("Invalid header on the Database input file => " + word);
				break;
			case 1:
				if (word != "|")
					throw std::runtime_error("Invalid header on the Database input file => " + word);
				break;
			case 2:
				if (word != "value")
					throw std::runtime_error("Invalid header on the Database input file => " + word);
				break;
		}
	}
	if (stream >> word)
		throw std::runtime_error(
			"Invalid header on the Database input file, extra unexpected text found => " + word);
}

void BitcoinExchange::_checkFile(std::string const &filename) {
	struct stat buffer;
	if (stat(filename.c_str(), &buffer) != 0) {
		throw std::runtime_error(filename + " does not exist.");
	}
	if ((buffer.st_mode & S_IFREG) == 0) {
		throw std::runtime_error(filename + " is a directory.");
	}
	if ((buffer.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) != 0) {
			throw std::runtime_error(filename + " is executable.");
	}
}

bool	BitcoinExchange::_setDate(std::string const &s_date, time_t &date) {
	if (s_date.size() != 10)
		return false;

	int year, month, day;
	char dash1, dash2;

	std::istringstream stream(s_date);
	stream >> year >> dash1 >> month >> dash2 >> day;
	if (stream.fail() || dash1 != '-' || dash2 != '-') {
		return false;
	}
	std::tm tm = {};
	tm.tm_year = year - 1900;
	tm.tm_mon = month - 1;
	tm.tm_mday = day;
	tm.tm_isdst = -1;
	date = std::mktime(&tm);
	if (date == -1) {
		return false;
	}
	return true;
}

bool BitcoinExchange::_setRate(std::string const &s_rate, double &rate) {
	if (s_rate.empty())
		return false;
	std::istringstream stream(s_rate);
	stream >> rate;
	if (stream.fail()) {
		return false;
	}
	if (rate < std::numeric_limits<int>::min() || rate > std::numeric_limits<int>::max()) {
		return false;
	}
	return true;
}

bool BitcoinExchange::_setValue(std::string const &s_value, double &value) {
	if (s_value.empty())
		return false;
	std::istringstream stream(s_value);
	stream >> value;
	if (stream.fail()) {
		return false;
	}
	if (value < 0) {
		std::cerr << RED "Error: not a positive number." RESET << std::endl;
		return false;
	} else if (value > 1000)  {
		std::cerr << RED "Error: too large a number." RESET << std::endl;
		return false;
	}
	return true;
}

BitcoinExchange::BitcoinExchange(std::string const &filename) {
	_checkFile(filename);
	std::ifstream file(filename.c_str(), std::ifstream::in);
	if (file.fail()) {
		throw std::runtime_error("Could not open file.");
	}
	std::string line;
	std::getline(file, line);
	this->_checkDatabaseHeader(line);
	std::string s_date;
	std::string s_rate;
	time_t date;
	double rate;
	while (std::getline(file, line)) {
		std::string::size_type pos = line.find(',');
		if (std::string::npos == pos) {
			throw std::runtime_error("Invalid format on the Database => " + line);
		}
		s_date = line.substr(0, pos);
		s_rate = line.substr(pos + 1);
		if (!this->_setDate(s_date, date)) {
			throw std::runtime_error("Invalid date on the Database => " + line);
		}
		if (!this->_setRate(s_rate, rate)) {
			throw std::runtime_error("Invalid rate on the Database => " + line);
		}
		std::pair<std::map<time_t, double>::iterator, bool> result
			= this->_btcDB.insert(std::make_pair(date, rate));
		if (!result.second) {
			throw std::runtime_error("Duplicate entry on the Database => " + line);
		}
	}
}

BitcoinExchange::BitcoinExchange (BitcoinExchange const &src) {
	*this = src;
}

BitcoinExchange &BitcoinExchange::operator=(BitcoinExchange const &rhs) {
	if (this != &rhs) {
		this->_btcDB = rhs._btcDB;
	}
	return *this;
}

BitcoinExchange::BitcoinExchange(void) {};

BitcoinExchange::~BitcoinExchange(void) {};

std::map<time_t, double> const &BitcoinExchange::getBtcDB(void) const {
	return this->_btcDB;
}

std::ostream &operator<<(std::ostream &out, BitcoinExchange const &btc) {
	char buffer[11];
	std::map<time_t, double>::const_iterator it = btc.getBtcDB().begin();
	while (it != btc.getBtcDB().end()) {
		std::tm *tm = std::localtime(&it->first);
		std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", tm);
		out << YELLOW << buffer << RESET " | " GREEN << it->second << RESET << std::endl;
		it++;
	}
	return out;
}

bool BitcoinExchange::_setExchangeValue(time_t date, double &value, double &exchangeValue) {
	std::map<time_t, double>::const_iterator it = this->_btcDB.lower_bound(date);
	if (it == this->_btcDB.begin() && it->first != date) {
		std::cerr << RED "Error: No exchange rate found for this date => " << date << RESET << std::endl;
		return false;
	}
	if (it == this->_btcDB.end() || it->first != date) {
		it--;
	}

	exchangeValue = it->second * value;
	return true;
}

void	BitcoinExchange::processFile(std::string const &filename) {
	_checkFile(filename);
	std::ifstream file(filename.c_str(), std::ifstream::in);
	if (file.fail()) {
		throw std::runtime_error("Could not open file.");
	}
	std::string line;
	std::getline(file, line);
	this->_checkInputHeader(line);
	std::string s_date;
	std::string s_value;
	time_t	date;
	double	value;
	double	exchangeValue;
	while (std::getline(file, line)) {
		std::string::size_type pos = line.find('|');
		if (std::string::npos == pos) {
			std::cerr << RED "Error: bad input => " << line << RESET << std::endl;
			continue;
		}
		s_date = line.substr(0, pos - 1);
		s_value = line.substr(pos + 2);
		if (!this->_setDate(s_date, date)) {
				std::cerr << RED "Error: bad input => " << date << RESET << std::endl;
				continue;
		}
		if (!this->_setValue(s_value, value)) {
				continue;
		}
		if (!this->_setExchangeValue(date, value, exchangeValue))
			continue;
		std::cout << YELLOW << s_date << RESET " => " BLUE << value
			<< RESET " = " GREEN << exchangeValue << RESET << std::endl;
	}
}
