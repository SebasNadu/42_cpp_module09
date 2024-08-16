/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebasnadu <johnavar@student.42berlin.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 13:54:45 by sebasnadu         #+#    #+#             */
/*   Updated: 2024/08/16 16:49:15 by sebasnadu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/RPN.hpp"
#include "../include/colors.hpp"

#include <iostream>

std::string const RPN::_validOperands = "0123456789";
std::string const RPN::_validOperators = "+-*/";
std::string const RPN::_validSpace = " \t";

RPN::RPN(std::string const &expression) {
	this->_checkExpression(expression);
	this->_expression = expression;
}

RPN::RPN(RPN const &src) {
	*this = src;
}

RPN::~RPN(void) {};

RPN	&RPN::operator=(RPN const &rhs) {
	if (this != &rhs) {
		this->_stack = rhs._stack;
	}
	return *this;
}

void	RPN::_checkExpression(std::string const &expression) {
	static std::string const validInput = this->_validOperands
		+ this->_validOperators + this->_validSpace;

	size_t pos = expression.find_first_not_of(validInput);
	if (pos != std::string::npos)
		throw std::out_of_range(
			"Invalid Expression: The expression should only contain: " + validInput);
	pos = expression.find_first_of(this->_validOperands);
	if (pos == std::string::npos)
		throw std::out_of_range("Invalid Expression: The expression should contain"
			" at least one of: " + this->_validOperands);
	pos = expression.find_first_of(this->_validOperators);
	if (pos == std::string::npos)
		throw std::out_of_range("Invalid Expression: The expression should contain"
			" at least one of: " + this->_validOperators);
}

bool	RPN::_isOperator(std::string const &token) {
	size_t pos = token.find_first_of(this->_validOperators, 0);
	if (pos == std::string::npos)
		return false;
	return true;
}

bool	RPN::_isOperand(std::string const &token) {
	size_t pos = token.find_first_of(this->_validOperands, 0);
	if (pos == std::string::npos)
		return false;
	return true;
}

void	RPN::_handleOperator(std::string const &token) {
	if (this->_stack.size() < 2)
		throw std::runtime_error("Not enough operands before an operation.");
	int	right = this->_stack.top();
	this->_stack.pop();
	int	left = this->_stack.top();
	this->_stack.pop();
	int	result(0);
	switch (token[0]) {
		case '+':
			result = left + right;
			break;
		case '-':
			result = left - right;
			break;
		case '*':
			result = left * right;
			break;
		case '/':
			if (right == 0)
				throw std::runtime_error("Division by zero.");
			result = left / right;
			break;
	}
	if (DEBUG)
		std::cout << BLUE << left << ' ' << token << ' ' << right << " = "
				<< result << RESET << std::endl;
	this->_stack.push(result);
}

void	RPN::_handleOperand(std::string const &token) {
	this->_stack.push(std::atoi(token.c_str()));
}

void	RPN::_handleToken(std::string const &token) {
	if (this->_isOperator(token))
		this->_handleOperator(token);
	else if (this->_isOperand(token))
		this->_handleOperand(token);
}

int	RPN::calculate(void) {
	if (this->_expression.empty())
		throw std::runtime_error("Empty expression.");
	std::string token;
	for (std::string::const_iterator it = this->_expression.begin();
    it != this->_expression.end(); it++) {
		if (*it == ' ' || *it == '\t')
			continue;
		token = std::string(1, *it);
		this->_handleToken(token);
	}
	if (this->_stack.size() != 1)
		throw std::runtime_error("Invalid expression.");
	return this->_stack.top();
}
