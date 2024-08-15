/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebasnadu <johnavar@student.42berlin.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 13:50:13 by sebasnadu         #+#    #+#             */
/*   Updated: 2024/08/15 12:45:40 by sebasnadu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "../include/colors.hpp"
#include "../include/BitcoinExchange.hpp"

void	validateArguments(int argc) {
	if (argc != 2){
		throw std::runtime_error(
			"Wrong number of arguments\nUsage: ./btc path/second/database");
	}
}
int	main(int argc, char **argv) {
	try {
		validateArguments(argc);
		BitcoinExchange btc("./data.csv");
		std::cout << btc;
	} catch (std::exception &e) {
		std::cerr << RED << e.what() << RESET << std::endl;
		return 1;
	}
	return 0;
}
