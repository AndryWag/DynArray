#include <iostream>

#include "DynArray.h"


template<typename T>
DynArray<T>::DynArray()
{
	m_size = 0;
	m_capacity = 0;
	m_data = nullptr;
}

template<typename T>
inline DynArray<T>::DynArray(int size)
{
	m_size = size;
	m_capacity = size;
	m_data = new T[size];
	for(int idx = 0; idx < size; ++idx)
		m_data[idx] = 0;
}

template<typename T>
DynArray<T>::DynArray(const DynArray& other)			
{
	m_size = other.m_size;								
	m_capacity = other.m_capacity;
	m_data = new T[m_size];	
	std::copy(other.m_data, other.m_data + other.m_size, m_data);
}

template<typename T>
DynArray<T>::DynArray(const DynArray&& other)			
{
	m_size = other.m_size;
	m_data = new T[m_size];
	m_data = other.m_data;
	m_capacity = other.m_capacity;
	other.m_data = nullptr;
}

template<typename T>
DynArray<T>::DynArray(const iterator start, const iterator end)
{
	m_size = range(start, end);
	m_capacity = m_size;
	m_data = new T[m_size];
	std::copy(start, end, m_data);
}

template<typename T>
DynArray<T>::~DynArray()
{
	if (m_data != nullptr) 
		delete[] m_data;
}



template<typename T>
DynArray<T>& DynArray<T>::operator=(const DynArray& other)
{
	if (other.m_size > m_capacity) {
		resize(other.m_size);
	}
	for (int idx = 0; idx < other.m_size; ++idx)
		m_data[idx] = other.m_data[idx];
	
	return *this;
}

template<typename T>
DynArray<T>& DynArray<T>::operator=(const DynArray&& other)
{
	if (other.m_size > m_capacity) {
		resize(other.m_size);
	}
	for (int idx = 0; idx < other.m_size; ++idx)
		m_data[idx] = other.m_data[idx];

	return *this;
}

template<typename T>
DynArray<T>& DynArray<T>::operator=(std::initializer_list<T> ilist)
{
	int idx = 0;

	if (ilist.size() > m_capacity) {
		resize(ilist.size());
	}
	m_size = ilist.size();
	for (auto& elem : ilist) {
		m_data[idx] = elem;
		++idx;
	}
	return *this;
}

template<typename T>
constexpr void DynArray<T>::assign(size_type count, const T& value)
{
	iterator beg = begin();
	if (count > m_capacity)
		dataCapacityIncrease(count);
	for (int idx = 0; idx < count; ++idx)
		m_data[idx] = value;
	m_size = count;
}

template<typename T>
template<typename InputIt>
constexpr void DynArray<T>::assign(InputIt first, InputIt last)
{
	iterator beg = first;
	int count = 0;
	while (beg != last) {
		++count;
		++beg;
	}
	if (count > m_capacity)
		dataCapacityIncrease(count);
	beg = first;
	for (int idx = 0; idx < count; ++idx, ++beg)
		m_data[idx] = *beg;
	m_size = count;
}

template<typename T>
constexpr void DynArray<T>::assign(std::initializer_list<T> ilist)
{
	int count = ilist.size();
	if (count > m_capacity)
		dataCapacityIncrease(count);
	for (int idx = 0; idx < count; ++idx)
		m_data[idx] = *(ilist.begin() + idx);
	m_size = count;
}

template<typename T>
typename constexpr DynArray<T>::size_type DynArray<T>::size()
{
	return m_size;
}

template<typename T>
constexpr typename DynArray<T>::size_type DynArray<T>::capacity()
{
	return m_capacity;
}

template<typename T>
constexpr void DynArray<T>::shrink_to_fit()
{
	value_type* new_data = new value_type[m_size];
	m_capacity = m_size;
	for (int idx = 0; idx < m_size; ++idx) 
		new_data[idx] = m_data[idx];
	delete[] m_data;
	m_data = new_data;
}

template<typename T>
constexpr void DynArray<T>::clear()
{
	delete[] m_data;
	m_data = nullptr;
	m_size = 0;
	m_capacity = 0;
}

template<typename T>
constexpr typename DynArray<T>::iterator DynArray<T>::insert(const iterator pos, const T& value)
{
	if (pos == nullptr) {
		std::cout << "[ERROR]: method insert(pos has nullptr value)!" << std::endl;
		return nullptr;
	}
	if (pos > m_data + m_size) {
		std::cout << "[ERROR]: method insert(pos has out-of-range value)!" << std::endl;
		return nullptr;
	}
	iterator newPos = data_offset(pos, 1);
	*newPos = value;
	++m_size;
	return newPos;
}

template<typename T>
constexpr typename DynArray<T>::iterator DynArray<T>::insert(const iterator pos, T&& value)
{
	if (pos == nullptr) {
		std::cout << "[ERROR]: method insert(pos has nullptr value)!" << std::endl;
		return nullptr;
	}
	if (pos > m_data + m_size) {
		std::cout << "[ERROR]: method insert(pos has out-of-range value)!" << std::endl;
		return nullptr;
	}
	iterator newPos = data_offset(pos, 1);
	*newPos = value;
	++m_size;
	return newPos;
}

template<typename T>
constexpr typename DynArray<T>::iterator DynArray<T>::insert(const iterator pos, size_type count, const T& value)
{
	if (pos == nullptr) {
		std::cout << "[ERROR]: method insert(pos has nullptr value)!" << std::endl;
		return nullptr;
	}
	if (pos > m_data + m_size) {
		std::cout << "[ERROR]: method insert(pos has out-of-range value)!" << std::endl;
		return nullptr;
	}
	iterator newPos = data_offset(pos, count);
	iterator retValue = newPos;
	unsigned int insCount = 0;
	while (insCount < count) {
		*newPos = value;
		++newPos;
		++insCount;
		++m_size;
	}

	return retValue;
}

template<typename T>
template<typename InputIt>
constexpr typename DynArray<T>::iterator DynArray<T>::insert(const iterator pos, InputIt first, InputIt last)
{
	if (pos == nullptr) {
		std::cout << "[ERROR]: method insert(pos has nullptr value)!" << std::endl;
		return nullptr;
	}
	if (pos > m_data + m_size) {
		std::cout << "[ERROR]: method insert(pos has out-of-range value)!" << std::endl;
		return nullptr;
	}
	iterator newPos = data_offset(pos, range(first, last));
	iterator retValue = newPos;
	while (first != last) {
		*newPos = *first;
		++newPos;
		++first;
		++m_size;
	}
	return retValue;

}

template<typename T>
constexpr typename DynArray<T>::iterator DynArray<T>::insert(const iterator pos, std::initializer_list<T> ilist)
{
	if (pos == nullptr) {
		std::cout << "[ERROR]: method insert(pos has nullptr value)!" << std::endl;
		return nullptr;
	}
	if (pos > m_data + m_size) {
		std::cout << "[ERROR]: method insert(pos has out-of-range value)!" << std::endl;
		return nullptr;
	}
	if (!ilist.size())
		return pos;
	int count = ilist.size();
	iterator newPos = data_offset(pos, count);
	iterator retValue = newPos;
	for (auto& elem : ilist) {
		*newPos = elem;
		++newPos;
		++m_size;
	}
	return retValue;
}

template<typename T>
constexpr typename DynArray<T>::iterator DynArray<T>::erase(const iterator pos)
{
	if (pos == nullptr) {
		std::cout << "[ERROR]: method erase(pos has nullptr value)!" << std::endl;
		return nullptr;
	}
	if (pos >= m_data + m_size - 1) {
		std::cout << "[ERROR]: method erase(pos has out-of-range value)!" << std::endl;
		return nullptr;
	}
	iterator end = m_data + m_size - 1; 
	iterator temp = pos;
	while (temp != end) {
		*temp = *(temp + 1);
		++temp;
	}
	end = nullptr;
	--m_size;
	return pos;
}


template<typename T>
constexpr typename DynArray<T>::iterator DynArray<T>::erase(const iterator first, const iterator last)
{
	if (first == nullptr || last == nullptr) {
		std::cout << "[ERROR]: method erase(first/last has nullptr value)!" << std::endl;
		return nullptr;

	}
	if (last >= m_data + m_size - 1) {
		std::cout << "[ERROR]: method erase(pos has out-of-range value)!" << std::endl;
		return nullptr;
	}
	iterator fst = first;
	iterator lst = last;
	iterator end = m_data + m_size - 1; 
	int offset = 0;
	m_size -= range(first, last);
	fst = first;
	while (last + offset <= end) {
		*fst = *(last + offset);
		++offset;
		++fst;
	}
	end = nullptr;
	return first;
}


template<typename T>
constexpr void DynArray<T>::push_back(const T& value)
{
	if (m_data == nullptr || m_capacity < m_size + 1)
		dataCapacityIncrease(m_size + 1);
	m_data[m_size] = value;
	++m_size;
}

template<typename T>
constexpr void DynArray<T>::push_back(T&& value)
{
	if (m_data == nullptr || m_capacity < m_size + 1)
		dataCapacityIncrease(m_size + 1);
	m_data[m_size] = value;
	++m_size;
}

template<typename T>
constexpr void DynArray<T>::pop_back()
{
	if (m_data == nullptr) {
		std::cout << "[ERROR]: method pop_back(DynArray is empty)!" << std::endl;
		return;
	}
	--m_size;
}

template<typename T>
constexpr void DynArray<T>::resize(size_type count)
{
	if (count < m_size) {	
		m_size = count;
		return;
	}
	if (m_capacity < count) {
		T* new_data = new T[count];
		m_capacity = count;
		for (int idx = 0; idx < m_size && idx < count; ++idx)
			new_data[idx] = m_data[idx];

		for (int idx = m_size; idx < count; ++idx)
			new_data[idx] = 0;
		delete[]m_data;
		m_data = new_data;
		m_size = count;
		return;
	}
	for (int idx = m_size; idx < count; ++idx)
		m_data[idx] = 0;
	m_size = count;
}

template<typename T>
constexpr void DynArray<T>::resize(size_type count, const T& value)
{
	if (count < m_size) {
		m_size = count;
		return;
	}
	if (m_capacity < count) {
		T* new_data = new T[count];
		m_capacity = count;
		for (int idx = 0; idx < m_size && idx < count; ++idx)
			new_data[idx] = m_data[idx];

		for (int idx = m_size; idx < count; ++idx)
			new_data[idx] = value;
		delete[]m_data;
		m_data = new_data;
		m_size = count;
		return;
	}
	for (int idx = m_size; idx < count; ++idx)
		m_data[idx] = value;
	m_size = count;
}

template<typename T>
bool DynArray<T>::operator==(const DynArray<T>& rhs)
{
	if (m_size != rhs.m_size)
		return false;
	
	for (int idx = 0; idx < m_size; ++idx) {
		if (m_data[idx] != rhs.m_data[idx])
			return false;
	}
	return true;
}

template<typename T>
bool DynArray<T>::operator!=(const DynArray<T>& rhs)
{
	if (m_size != rhs.m_size)
		return true;

	for (int idx = 0; idx < m_size; ++idx) {
		if (m_data[idx] != rhs.m_data[idx])
			return true;
	}
	return false;
}

template<typename T>
constexpr typename DynArray<T>::iterator DynArray<T>::data_offset(const iterator pos, const size_type offset)
{
	iterator newPos = pos;
	if (pos == nullptr) {
		std::cout << "[ERROR]: func data_offset(pos has nullptr value)!" << std::endl;
		return nullptr;
	}
	if (pos > m_data + m_size) {
		std::cout << "[ERROR]: func data_offset(pos has out-of-range value)!" << std::endl;
		return nullptr;
	}
	if (m_size + offset > m_capacity) {
		newPos = dataCapacityIncrease(m_size + offset, pos);
	}
	iterator lst = m_data + m_size - 1;
	iterator newLst = m_data + m_size + offset - 1;
	while (lst >= newPos) {
		*newLst = *lst;
		--newLst;
		--lst;
	}
	return newPos;
}

template<typename T>
constexpr typename DynArray<T>::iterator DynArray<T>::dataCapacityIncrease(size_type new_size, const iterator pos)
{
	unsigned int inc_value = 1;// BLOCK_SIZE; (NON-BLOCK VERSION)
	iterator retIt = nullptr;
	if (inc_value < new_size)
		inc_value = new_size;
	value_type* new_data = new value_type[inc_value];
	m_capacity = inc_value;
	for (int idx = 0; idx < m_size; ++idx) {
		new_data[idx] = m_data[idx];
		if (m_data + idx == pos)
			retIt = new_data + idx;
	}
	for (int idx = m_size; idx < new_size; ++idx)
		new_data[idx] = 0;
	delete[]m_data;
	m_data = new_data;
	if (retIt == nullptr)
		retIt = m_data + m_size;
	return retIt;
}

template<typename T>
constexpr unsigned int DynArray<T>::range(const iterator first, const iterator last)
{
	int rng = 0;
	iterator fst = first;
	while (fst != last) {
		++fst;
		++rng;
	}
	return rng;
}

template<typename T>
bool DynArray<T>::empty()
{
	if (m_size == 0)
		return true;
	return false;
}



template<typename T>
T& DynArray<T>::operator[](size_type pos)
{
	if (pos < 0) {
		std::cout << "[ERROR]: operator[](pos has negative value)." << std::endl;
	}
	return m_data[pos];
}

template<typename T>
T& DynArray<T>::at(size_type pos)
{
	if (pos > m_size) {
		std::cout << "[ERROR]: method at(pos has bigger value then DynArray size)." << std::endl;
	}
	return *(m_data + pos);
}

template<typename T>
T& DynArray<T>::front()
{
	return *m_data;
}

template<typename T>
T& DynArray<T>::back()
{
	return *(m_data + m_size - 1);
}

template<typename T>
typename DynArray<T>::iterator DynArray<T>::begin()
{
	return m_data;
}

template<typename T>
typename DynArray<T>::const_iterator DynArray<T>::cbegin()
{
	return m_data;
}

template<typename T>
typename DynArray<T>::iterator DynArray<T>::end()
{
	return m_data + m_size;
}

template<typename T>
typename DynArray<T>::const_iterator DynArray<T>::cend()
{
	return m_data + m_size;
}

template<typename T>
typename DynArray<T>::iterator DynArray<T>::rbegin()
{
	return m_data + m_size - 1;
}

template<typename T>
typename DynArray<T>::const_iterator DynArray<T>::crbegin()
{
	return m_data + m_size - 1;
}

template<typename T>
typename DynArray<T>::iterator DynArray<T>::rend()
{
	return m_data - 1;
}

template<typename T>
typename DynArray<T>::const_iterator DynArray<T>::crend()
{
	return m_data - 1;
}




