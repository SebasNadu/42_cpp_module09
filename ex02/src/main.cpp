/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebasnadu <johnavar@student.42berlin.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 21:27:14 by sebasnadu         #+#    #+#             */
/*   Updated: 2024/08/22 17:18:42 by sebasnadu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/colors.hpp"
#include "../include/PmergeMe.hpp"

#include <iostream>
#include <sstream>
#include <set>

void	validateArguments(int argc, char **argv, unsigned int *&array) {
	if (argc < 2){
		throw std::runtime_error("wrong number of arguments.\n\t\tUsage: ./PmergeMe <list of unique positive integers>");
	}
	std::set<unsigned int> set;
	int num;
	array = new unsigned int[argc - 1];
	for (int i = 1; i < argc; ++i) {
		std::stringstream ss(argv[i]);
		if (!(ss >> num) || num < 0) {
			throw std::runtime_error("invalid argument.\n\t\tUsage: ./PmergeMe <list of unique positive integers>");
		}
		if (set.find(num) != set.end()) {
			throw std::runtime_error("duplicated argument.\n\t\tUsage: ./PmergeMe <list of unique positive integers>");
		}
		set.insert(num);
		array[i - 1] = num;
	}
}

template <typename C>
std::string getString(C &container) {
	std::stringstream ss;
	if (container.empty())
		return ("");
	typename C::const_iterator it = container.begin();
	for (; it != container.end(); ++it) {
		ss << '[' << *it << ']';
	}
	return ss.str();
}

template <typename C>
void	printContainer(C &container, std::string const &msg,
		std::string const &color) {
	std::stringstream ss;

	ss << "-> " << msg << ": ";

	std::cout << color << std::setw(12) << std::left << ss.str()
		<< getString(container) << RESET << std::endl;
}

template <typename C>
void	checkSort(PmergeMe &pmm) {
	C		const &unsortedContainer = pmm.getUnsortedContainer<C>();
	int const	straggler = pmm.getStraggler();
  C const 	&sortedContainer = pmm.getSortedContainer<C>();
	std::vector<unsigned int> checkContainer;
	checkContainer.assign(unsortedContainer.begin(), unsortedContainer.end());
	
	if (straggler != -1)
		checkContainer.push_back(straggler);
	if (DEBUG)
		std::cout << '\n';
	std::cout << WHITE BOLD "____Result____\n" RESET << std::endl;
	printContainer(checkContainer, "Before", PURPLE);
	std::sort(checkContainer.begin(), checkContainer.end());
	typename C::const_iterator it = sortedContainer.begin();
	std::vector<unsigned int>::const_iterator itCheck = checkContainer.begin();
	for (; it != sortedContainer.end(); ++it, ++itCheck) {
		if (*it != *itCheck) {
			printContainer(sortedContainer, "After", RED);
			printContainer(checkContainer, "Expected", CYAN);
			std::cout << RED BOLD "** KO: wrong sorted." RESET << std::endl;
			return ;
		}
	}
	printContainer(sortedContainer, "After", CYAN);
	std::cout << GREEN BOLD "** OK: Sorted correctly." RESET << std::endl;
}

std::clock_t	testVector(unsigned int **array, int size) {
	if (DEBUG)
		std::cout
			<< YELLOW BOLD "\n--------Merge-insertion sort with std::vector-------\n" RESET << std::endl;
	std::clock_t	start = std::clock();
	PmergeMe	pmm(*array, size, PmergeMe::VECTOR);
	pmm.sort();
	std::clock_t	end = std::clock() - start;
	checkSort<std::vector<unsigned int> >(pmm);
	return (end);
}

std::clock_t	testDeque(unsigned int **array, int size) {
	if (DEBUG)
		std::cout << YELLOW BOLD "\n--------Merge-insertion sort with std::deque-------\n" RESET << std::endl;
	std::clock_t	start = std::clock();
	PmergeMe	pmm(*array, size, PmergeMe::DEQUE);
	pmm.sort();
	std::clock_t	end = std::clock() - start;
	checkSort<std::deque<unsigned int> >(pmm);
	return (end);
}

std::clock_t	testList(unsigned int **array, int size) {
	if (DEBUG)
		std::cout << YELLOW BOLD "\n--------Merge-insertion sort with std::list-------\n" RESET << std::endl;
	std::clock_t	start = std::clock();
	PmergeMe	pmm(*array, size, PmergeMe::LIST);
	pmm.sort();
	std::clock_t	end = std::clock() - start;
	checkSort<std::list<unsigned int> >(pmm);
	return (end);
}

void	printTime(PmergeMe::containerType type, std::clock_t time, int size) {
	std::string container;
	double 			clockPerMs = static_cast<double>(CLOCKS_PER_SEC) / 1000;
	double			timeInMs = time / clockPerMs;

	if (type == PmergeMe::VECTOR)
		container = "std::vector";
	else if (type == PmergeMe::DEQUE)
		container = "std::deque";
	else
		container = "std::list";
	std::cout << WHITE "-- Time to process a range of " << CYAN "( "
		<< std::setw(4) << std::right << size << " )" WHITE
		<< " elements with " PURPLE << container << WHITE " : " GREEN
		<< timeInMs << WHITE " ms" RESET << std::endl;
}

int	main(int argc, char **argv) {
	try {
		unsigned int *array = NULL;
		validateArguments(argc, argv, array);
		std::clock_t	vectorTime = testVector(&array, argc - 1);
		printTime(PmergeMe::VECTOR, vectorTime, argc - 1);
		std::clock_t	dequeTime = testDeque(&array, argc - 1);
		printTime(PmergeMe::DEQUE, dequeTime, argc - 1);
		std::clock_t	listTime = testList(&array, argc - 1);
		printTime(PmergeMe::LIST, listTime, argc - 1);
		delete[] array;
	} catch (std::exception &e) {
		std::cerr << RED "Error: " << e.what() << RESET << std::endl;
		return 1;
	}
	return 0;
}
