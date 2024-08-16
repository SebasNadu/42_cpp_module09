/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebasnadu <johnavar@student.42berlin.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 12:02:58 by sebasnadu         #+#    #+#             */
/*   Updated: 2024/08/16 16:33:24 by sebasnadu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/colors.hpp"
#include "../include/RPN.hpp"

#include <iostream>

void	validateArguments(int argc) {
	if (argc != 2){
		throw std::runtime_error("wrong number of arguments.\n\t\tUsage: ./RPN \"expression\"");
	}
}

int	main(int argc, char **argv) {
	try {
		validateArguments(argc);
		RPN rpn(argv[1]);
		std::cout << GREEN << rpn.calculate() << RESET << std::endl;
	} catch (std::exception &e) {
		std::cerr << RED "Error: " << e.what() << RESET << std::endl;
		return 1;
	}
	return 0;
}
