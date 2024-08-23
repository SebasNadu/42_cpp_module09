/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebasnadu <johnavar@student.42berlin.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 00:07:05 by sebasnadu         #+#    #+#             */
/*   Updated: 2024/08/23 13:29:30 by johnavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/colors.hpp"
#include "../include/PmergeMe.hpp"
#include <iostream>
#include <sstream>

PmergeMe::PmergeMe(unsigned int const *array, int size, containerType type) {
	if (size < 1) {
		throw std::invalid_argument("Empty array.");
	}
	if ((size & 1) != 0) {
		this->_straggler = array[size - 1];
		size -= 1;
	} else {
		this->_straggler = -1;
	}
	if (type == VECTOR) {
		this->_vec.assign(array, array + size);
		this->_sortedVec.reserve(size);
		this->_type = VECTOR;
	} else if (type == DEQUE) {
		this->_deq.assign(array, array + size);
		this->_type = DEQUE;
	} else if (type == LIST) {
		this->_lst.assign(array, array + size);
		this->_type = LIST;
	} else {
		throw std::invalid_argument("Invalid container type.");
	}
}

PmergeMe::~PmergeMe(void) {};

std::string PmergeMe::_getString(std::vector<unsigned int> &vec, bool withStraggler) {
  std::stringstream ss;
	if (vec.empty())
		return ("");
	std::vector<unsigned int>::iterator it = vec.begin();
	for (; it != vec.end(); ++it) {
		ss << "[" << *it << "]";
	}
	if (withStraggler && this->_straggler != -1)
		ss << "[" << this->_straggler << "]";
	return ss.str();
}

int	PmergeMe::getStraggler(void) const { return this->_straggler; }

std::string PmergeMe::_getString(std::vector< std::pair<unsigned int, unsigned int> > &vec, bool withStraggler) {
	std::stringstream ss;
	if (vec.empty())
		return ("");
	std::vector< std::pair<unsigned int, unsigned int> >::iterator it = vec.begin();
	for (; it != vec.end(); ++it) {
		ss << "[" << it->first << "," << it->second << "]";
	}
	if (withStraggler && this->_straggler != -1)
		ss << "[" << this->_straggler << "]";
	return ss.str();
}

std::string PmergeMe::_getString(std::deque<unsigned int> &deq, bool withStraggler) {
	std::stringstream ss;
	if (deq.empty())
		return ("");
	std::deque<unsigned int>::iterator it = deq.begin();
	for (; it != deq.end(); ++it) {
		ss << "[" << *it << "]";
	}
	if (withStraggler && this->_straggler != -1)
		ss << "[" << this->_straggler << "]";
	return ss.str();
}

std::string PmergeMe::_getString(std::deque< std::pair<unsigned int, unsigned int> > &deq, bool withStraggler) {
	std::stringstream ss;
	if (deq.empty())
		return ("");
	std::deque< std::pair<unsigned int, unsigned int> >::iterator it = deq.begin();
	for (; it != deq.end(); ++it) {
		ss << "[" << it->first << "," << it->second << "]";
	}
	if (withStraggler && this->_straggler != -1)
		ss << "[" << this->_straggler << "]";
	return ss.str();
}

std::string PmergeMe::_getString(std::list<unsigned int> &lst, bool withStraggler) {
	std::stringstream ss;
	if (lst.empty())
		return ("");
	std::list<unsigned int>::iterator it = lst.begin();
	for (; it != lst.end(); ++it) {
		ss << "[" << *it << "]";
	}
	if (withStraggler && this->_straggler != -1)
		ss << "[" << this->_straggler << "]";
	return ss.str();
}

std::string PmergeMe::_getString(std::list< std::pair<unsigned int, unsigned int> > &lst, bool withStraggler) {
	std::stringstream ss;
	if (lst.empty())
		return ("");
	std::list< std::pair<unsigned int, unsigned int> >::iterator it = lst.begin();
	for (; it != lst.end(); ++it) {
		ss << "[" << it->first << "," << it->second << "]";
	}
	if (withStraggler && this->_straggler != -1)
		ss << "[" << this->_straggler << "]";
	return ss.str();
}

int	PmergeMe::_jacobsthal(int n) {
	if (n == 0)
		return 0;
	if (n == 1)
		return 1;
	return this->_jacobsthal(n - 1) + 2 * this->_jacobsthal(n - 2);
}

void	PmergeMe::_mergeInsertionSort(std::vector<unsigned int> &vec) {
	if (this->_isSorted(vec))
		return ;
	std::vector< std::pair<unsigned int, unsigned int> > splitVec;
	this->_createPairs(vec, splitVec);
	this->_sortEachPair(splitVec);
	this->_mergeSortPairs(splitVec);
	this->_createSortedSequence(this->_sortedVec, splitVec);
	if (this->_straggler != -1) {
		this->_sortedVec.insert(std::lower_bound(this->_sortedVec.begin(),
				this->_sortedVec.end(), this->_straggler), this->_straggler);
		if (DEBUG)
			this->_print(this->_sortedVec, "Straggler inserted", GREEN, false);
	}
}

void	PmergeMe::_mergeInsertionSort(std::deque<unsigned int> &deq) {
	if (this->_isSorted(deq))
		return ;
	std::deque< std::pair<unsigned int, unsigned int> > splitDeq;
	this->_createPairs(deq, splitDeq);
	this->_sortEachPair(splitDeq);
	this->_mergeSortPairs(splitDeq);
	this->_createSortedSequence(this->_sortedDeq, splitDeq);
	if (this->_straggler != -1) {
		this->_sortedDeq.insert(std::lower_bound(this->_sortedDeq.begin(),
				this->_sortedDeq.end(), this->_straggler), this->_straggler);
		if (DEBUG)
			this->_print(this->_sortedDeq, "Sorted with straggler", GREEN, false);
	}
}

void	PmergeMe::_mergeInsertionSort(std::list<unsigned int> &lst) {
	if (this->_isSorted(lst))
		return ;
	std::list< std::pair<unsigned int, unsigned int> > splitLst;
	this->_createPairs(lst, splitLst);
	this->_sortEachPair(splitLst);
	this->_mergeSortPairs(splitLst);
	this->_createSortedSequence(this->_sortedLst, splitLst);
	if (this->_straggler != -1) {
		this->_sortedLst.insert(std::lower_bound(this->_sortedLst.begin(),
				this->_sortedLst.end(), this->_straggler), this->_straggler);
		if (DEBUG)
			this->_print(this->_sortedLst, "Sorted with straggler", GREEN, false);
	}
}

void	PmergeMe::sort(void) {
	if (this->_type == VECTOR) {
		if (this->_vec.empty()) {
			if (this->_straggler != -1) {
				this->_sortedVec.push_back(_straggler);
				return;
			}
			throw std::invalid_argument("Empty array.");
		}
		if (DEBUG)
			this->_print(this->_vec, "Before sort", WHITE);
		this->_mergeInsertionSort(this->_vec);
	} else if (this->_type == DEQUE) {
		if (this->_deq.empty()) {
			if (this->_straggler != -1) {
				this->_sortedDeq.push_back(_straggler);
				return;
			}
			throw std::invalid_argument("Empty array.");
		}
		if (DEBUG)
			this->_print(this->_deq, "Before sort", WHITE);
		this->_mergeInsertionSort(this->_deq);
	} else if (this->_type == LIST) {
		if (this->_lst.empty()) {
			if (this->_straggler != -1) {
				this->_sortedLst.push_back(_straggler);
				return;
			}
			throw std::invalid_argument("Empty array.");
		}
		if (DEBUG)
			this->_print(this->_lst, "Before sort", WHITE);
		this->_mergeInsertionSort(this->_lst);
	} else {
		throw std::invalid_argument("Invalid container type, fatal error.");
	}
}
