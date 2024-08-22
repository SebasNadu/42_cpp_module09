/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebasnadu <johnavar@student.42berlin.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 21:32:33 by sebasnadu         #+#    #+#             */
/*   Updated: 2024/08/22 17:58:40 by sebasnadu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "colors.hpp"

#include <vector>
#include <deque>
#include <list>
#include <iostream>
#include <iomanip>
#include <sstream>

#ifndef DEBUG
# define DEBUG 0
#endif

class PmergeMe {
public:
	typedef enum { VECTOR, DEQUE, LIST } containerType;

	PmergeMe(unsigned int const *array, int size, containerType type);
	~PmergeMe(void);

	void	sort(void);
	int		getStraggler(void) const;

	template <typename C>
	C const &getSortedContainer(void) const { 
		if (this->_type == VECTOR) return (this->_sortedVec);
		if (this->_type == DEQUE) return (this->_sortedDeq);
		if (this->_type == LIST) return (this->_sortedLst);
	}

	template <typename C>
	C const &getUnsortedContainer(void) const {
		if (this->_type == VECTOR) return (this->_vec);
		if (this->_type == DEQUE) return (this->_deq);
		if (this->_type == LIST) return (this->_lst);
	}

	template <>
	std::vector<unsigned int> const &getSortedContainer<std::vector<unsigned int> >() const {
		return _sortedVec;
	}

	template <>
	std::deque<unsigned int> const &getSortedContainer<std::deque<unsigned int> >() const {
		return _sortedDeq;
	}

	template <>
	std::list<unsigned int> const &getSortedContainer<std::list<unsigned int> >() const {
		return _sortedLst;
	}

	template <>
	std::vector<unsigned int> const &getUnsortedContainer<std::vector<unsigned int> >() const {
		return _vec;
	}

	template <>
	std::deque<unsigned int> const &getUnsortedContainer<std::deque<unsigned int> >() const {
		return _deq;
	}

	template <>
	std::list<unsigned int> const &getUnsortedContainer<std::list<unsigned int> >() const {
		return _lst;
	}

private:
	PmergeMe(void);
	PmergeMe(PmergeMe const &src);
	PmergeMe &operator=(PmergeMe const &rhs);

	template <typename C>
	void	_print(C &container, std::string const &msg,
			std::string const &color, bool withStraggler = true) {
		std::stringstream ss;
		int has_straggler = (this->_straggler != -1 && withStraggler);

		ss << "-> " << msg << " vector (size " << container.size() + has_straggler << "): ";

		std::cout << color << std::setw(45) << std::left << ss.str()
			<< this->_getString(container, withStraggler) << RESET << std::endl;
	}

	std::string _getString(std::vector<unsigned int> &vec, bool withStraggler);
	std::string _getString(std::vector< std::pair<unsigned int, unsigned int> > &vec, bool withStraggler);
	std::string _getString(std::deque<unsigned int> &deq, bool withStraggler);
	std::string _getString(std::deque< std::pair<unsigned int, unsigned int> > &deq, bool withStraggler);
	std::string _getString(std::list<unsigned int> &lst, bool withStraggler);
	std::string _getString(std::list< std::pair<unsigned int, unsigned int> > &lst, bool withStraggler);

	void	_mergeInsertionSort(std::vector<unsigned int> &vec);
	void	_mergeInsertionSort(std::deque<unsigned int> &deq);
	void	_mergeInsertionSort(std::list<unsigned int> &lst);

	template <typename C, typename CByPairs>
	void	_createPairs(C &container, CByPairs &splitContainer) {
		typename C::iterator it = container.begin();
		typename C::iterator ite = container.end();
		int first = -1;
		for (; it != ite; ++it) {
			if (first == -1) {
				first = *it;
			} else {
				splitContainer.push_back(std::make_pair(first, *it));
				first = -1;
			}
		}
		if (DEBUG)
			this->_print(splitContainer, "Pairs created", PURPLE);
	}

	template <typename C>
	bool _isSorted(C const &container) {
    typename C::const_iterator it = container.begin();
    typename C::const_iterator ite = container.end();

    if (it == ite)
        return true;
    typename C::value_type prev = *it;
    ++it;
    for (; it != ite; ++it) {
        if (*it < prev)
            return false;
        prev = *it;
    }
    return true;
	}

	template <typename C>
	void	_sortEachPair(C &splitContainer) {
		typename C::iterator it = splitContainer.begin();
		for ( ; it != splitContainer.end(); ++it) {
			if (it->first > it->second) {
				std::swap(it->first, it->second);
			}
		}
		if (DEBUG)
			this->_print(splitContainer, "Each Pair sorted", PURPLE, false);
	}

	template <typename C>
	C _merge(C &left, C &right) {
		C merged;
		
		typename C::iterator itLeft = left.begin();
		typename C::iterator itRight = right.begin();
		
		while (itLeft != left.end() && itRight != right.end()) {
				if (itLeft->second < itRight->second) {
						merged.push_back(*itLeft);
						++itLeft;
				} else {
						merged.push_back(*itRight);
						++itRight;
				}
		}
		
		merged.insert(merged.end(), itLeft, left.end());
		merged.insert(merged.end(), itRight, right.end());

		return merged;
	}

	template <>
	std::vector<std::pair<unsigned int, unsigned int> >	_merge(
		std::vector<std::pair<unsigned int, unsigned int> > &left,
		std::vector<std::pair<unsigned int, unsigned int> > &right) {

		std::vector<std::pair<unsigned int, unsigned int> > merged;

		merged.reserve(left.size() + right.size());
		std::vector<std::pair<unsigned int, unsigned int> >::iterator itLeft = left.begin();
		std::vector<std::pair<unsigned int, unsigned int> >::iterator itRight = right.begin();
		while (itLeft != left.end() && itRight != right.end()) {
			if (itLeft->second < itRight->second) {
				merged.push_back(*itLeft);
				++itLeft;
			} else {
				merged.push_back(*itRight);
				++itRight;
			}
		}

		merged.insert(merged.end(), itLeft, left.end());
		merged.insert(merged.end(), itRight, right.end());

		return merged;
	}

	template <typename C>
	void	_insertionSortByPair(C &splitContainer) {
		if (splitContainer.size() <= 1) return;

		typename C::iterator middle = splitContainer.begin();
		std::advance(middle, splitContainer.size() / 2);

		C	left(splitContainer.begin(), middle);
		C	right(middle, splitContainer.end());

		this->_insertionSortByPair(left);
		this->_insertionSortByPair(right);
		splitContainer = this->_merge(left, right);
	}

	template <>
	void	_insertionSortByPair(std::list<std::pair<unsigned int, unsigned int> > &splitContainer) {
		if (splitContainer.size() <= 1) return;

		std::list<std::pair<unsigned int, unsigned int> >::iterator middle = splitContainer.begin();
		std::advance(middle, splitContainer.size() / 2);

		std::list<std::pair<unsigned int, unsigned int> >	left(splitContainer.begin(), middle);
		std::list<std::pair<unsigned int, unsigned int> >	right(middle, splitContainer.end());

		this->_insertionSortByPair(left);
		this->_insertionSortByPair(right);
		splitContainer = this->_merge(left, right);
	}

	int	_jacobsthal(int n);

	template <typename C>
	void	_buildJacobSequence(int size, C &jacobSequence) {
		int jacobIndex = 3;
		int jacobValue = this->_jacobsthal(jacobIndex);
		while (jacobValue < size - 1) {
			jacobSequence.push_back(jacobValue);
			jacobValue = this->_jacobsthal(++jacobIndex);
		}
		if (DEBUG)
			this->_print(jacobSequence, "Jacob Sequence", CYAN, false);
	}

	template <typename C>
	void	_createIndexSequence(C &pending, C &indexSequence) {
		bool					lastWasJacobNumber = false;
		unsigned int	pendingSize = pending.size();
		C 						jacobSequence;

		indexSequence.push_back(1);
		if (pendingSize == 1) return;

		this->_buildJacobSequence(pendingSize, jacobSequence);
		unsigned int i = 1;
		while (i <= pendingSize) {
			if (!jacobSequence.empty() && !lastWasJacobNumber) {
				indexSequence.push_back(jacobSequence.front());
				jacobSequence.pop_front();
				lastWasJacobNumber = true;
				continue ;
			}
			typename C::iterator indexIt = indexSequence.begin();
			for (; indexIt != indexSequence.end(); ++indexIt) {
				if (i == *indexIt)
					++i;
			}
			indexSequence.push_back(i);
			lastWasJacobNumber = false;
			++i;
		}
		if (DEBUG)
			this->_print(indexSequence, "Index Sequence", CYAN, false);
	}

	template <>
	void	_createIndexSequence(
		std::vector<unsigned int> &pending, std::vector<unsigned int> &indexSequence) {
		bool											lastWasJacobNumber = false;
		unsigned int							pendingSize = pending.size();
		std::vector<unsigned int>	jacobSequence;

		indexSequence.push_back(1);
		if (pendingSize == 1) return ;

		this->_buildJacobSequence(pendingSize, jacobSequence);
		unsigned int i = 1;
		while (i <= pendingSize) {
			if (!jacobSequence.empty() && !lastWasJacobNumber) {
				indexSequence.push_back(jacobSequence[0]);
				jacobSequence.erase(jacobSequence.begin());
				lastWasJacobNumber = true;
				continue ;
			}
			std::vector<unsigned int>::iterator indexIt = indexSequence.begin();
			for (; indexIt != indexSequence.end(); ++indexIt) {
				if (i == *indexIt)
					++i;
			}
			indexSequence.push_back(i);
			lastWasJacobNumber = false;
			++i;
		}
		if (DEBUG)
			this->_print(indexSequence, "Index Sequence", CYAN, false);
	}

	template <typename C>
	void	_insertAtBisection(C &sortedContainer, unsigned int value) {
		typename C::iterator it = sortedContainer.begin();
		typename C::iterator ite = sortedContainer.end();
		typename C::iterator mid = it + (ite - it) / 2;

		while (it != ite) {
			if (value < *mid) {
				ite = mid;
				mid = it + (ite - it) / 2;
			} else {
				it = mid + 1;
				mid = it + (ite - it) / 2;
			}
		}

		sortedContainer.insert(mid, value);
	}

	template <>
	void	_insertAtBisection(std::list<unsigned int> &sortedContainer, unsigned int value) {
		std::list<unsigned int>::iterator it = sortedContainer.begin();
		std::list<unsigned int>::iterator ite = sortedContainer.end();
		std::list<unsigned int>::iterator mid = it;
		std::advance(mid, std::distance(it, ite) / 2);

		while (it != ite) {
			if (value < *mid) {
				ite = mid;
				mid = it;
				std::advance(mid, std::distance(it, ite) / 2);
			} else {
				it = mid;
				++it;
				mid = it;
				std::advance(mid, std::distance(it, ite) / 2);
			}
		}
		sortedContainer.insert(mid, value);
	}

	template <typename C, typename CByPairs>
	void	_createSortedSequence(C &sortedContainer, CByPairs &splitContainer) {
		C pending;
		C indexSequence;
		typename CByPairs::iterator it = splitContainer.begin();

		for (; it != splitContainer.end(); ++it) {
			sortedContainer.push_back(it->second);
			pending.push_back(it->first);
		}
		if (DEBUG) {
			this->_print(sortedContainer, "Sorted", GREEN, false);
			this->_print(pending, "Pending", WHITE, false);
		}
		this->_createIndexSequence(pending, indexSequence);

		if (DEBUG)
			std::cout << CYAN << std::setw(45) << std::left << "-> Inserting order:";

		typename C::iterator	indexIt = indexSequence.begin();
		for (; indexIt != indexSequence.end(); ++indexIt) {
			unsigned int value = pending[*indexIt - 1];

			if (DEBUG)
				std::cout << '[' << value << ']';
			this->_insertAtBisection(sortedContainer, value);
		}
		std::cout << RESET << std::endl;
		if (DEBUG)
			this->_print(sortedContainer, "Sorted Sequence", GREEN, false);
	};

	template <>
	void	_createSortedSequence(std::list<unsigned int> &sortedContainer,
			std::list< std::pair<unsigned int, unsigned int> > &splitContainer) {
		std::list<unsigned int> pending;
		std::list<unsigned int> indexSequence;
		std::list< std::pair<unsigned int, unsigned int> >::iterator it
			= splitContainer.begin();

		for (; it != splitContainer.end(); ++it) {
			sortedContainer.push_back(it->second);
			pending.push_back(it->first);
		}
		if (DEBUG) {
			this->_print(sortedContainer, "Sorted", GREEN, false);
			this->_print(pending, "Pending", WHITE, false);
		}
		this->_createIndexSequence(pending, indexSequence);

		if (DEBUG)
			std::cout << CYAN << std::setw(45) << std::left << "-> Inserting order:";

		std::list<unsigned int>::iterator	indexIt = indexSequence.begin();
		for (; indexIt != indexSequence.end(); ++indexIt) {
			std::list<unsigned int>::iterator pendingIt = pending.begin();
			std::advance(pendingIt, *indexIt - 1);
			unsigned int value = *pendingIt;

			if (DEBUG)
				std::cout << '[' << value << ']';
			this->_insertAtBisection(sortedContainer, value);
		}
		std::cout << RESET << std::endl;
		if (DEBUG)
			this->_print(sortedContainer, "Sorted Sequence", GREEN, false);
	};

	std::vector<unsigned int> _vec;
	std::vector<unsigned int> _sortedVec;
	std::deque<unsigned int> 	_deq;
	std::deque<unsigned int> 	_sortedDeq;
	std::list<unsigned int> 	_lst;
	std::list<unsigned int> 	_sortedLst;
	containerType 						_type;
	int												_straggler;
};
