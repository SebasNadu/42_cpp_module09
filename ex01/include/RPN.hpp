/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebasnadu <johnavar@student.42berlin.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 12:36:22 by sebasnadu         #+#    #+#             */
/*   Updated: 2024/08/16 16:48:31 by sebasnadu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <stack>

#ifndef DEBUG
# define DEBUG 0
#endif

class RPN {
public:
	RPN(std::string const &expression);
	RPN(RPN const &src);
	~RPN(void);
	RPN	&operator=(RPN const &rhs);

	int 	calculate(void);
private:
	RPN(void);

	void	_checkExpression(std::string const &expression);
	void	_handleToken(std::string const &token);
	bool	_isOperator(std::string const &token);
	bool	_isOperand(std::string const &token);
	void	_handleOperator(std::string const &token);
  void	_handleOperand(std::string const &token);

	std::stack<int> _stack;
	std::string			_expression;
	static std::string const _validOperands;
	static std::string const _validOperators;
	static std::string const _validSpace;
};
