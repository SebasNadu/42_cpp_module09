/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebasnadu <johnavar@student.42berlin.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 13:50:13 by sebasnadu         #+#    #+#             */
/*   Updated: 2024/08/16 11:40:21 by sebasnadu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "../include/colors.hpp"
#include "../include/BitcoinExchange.hpp"

void	validateArguments(int argc) {
	if (argc != 2){
		throw std::runtime_error("Error: could not open file");
	}
}

int	main(int argc, char **argv) {
	try {
		validateArguments(argc);
		BitcoinExchange btc("./data.csv");
		btc.processFile(argv[1]);
	} catch (std::exception &e) {
		std::cerr << RED << e.what() << RESET << std::endl;
		return 1;
	}
	return 0;
}
