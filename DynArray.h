#pragma once

template <typename T>
class DynArray
{
public:
	// TYPEDEFS
	typedef T value_type;
	typedef std::size_t size_type;
	typedef value_type* iterator;
	typedef const iterator const_iterator;

	// NON-BLOCK REALIZATION
	// const unsigned int BLOCK_SIZE = 16;
	


	DynArray();
	DynArray(int size);
	DynArray(const DynArray& other);
	DynArray(const DynArray&& other);
	DynArray(const iterator start, const iterator end);
	~DynArray();

	// MEMBER FUNCTIONS
	DynArray<T>& operator=(const DynArray& other);
	DynArray<T>& operator=(const DynArray&& other);
	DynArray<T>& operator=(std::initializer_list<T> ilist);
	constexpr void assign(size_type count, const T& value);
	template<typename InputIt>
	constexpr void assign(InputIt first, InputIt last);
	constexpr void assign(std::initializer_list<T> ilist);

	// ELEMENT ACCESS
	T& at(size_type pos);
	T& operator[](size_type pos);
	T& front();
	T& back();

	// ITERATORS
	iterator begin();
	const_iterator cbegin();
	iterator end();
	const_iterator cend();
	iterator rbegin();
	const_iterator crbegin();
	iterator rend();
	const_iterator crend();


	// CAPACITY
	bool empty();
	constexpr size_type size();
	constexpr size_type capacity();
	constexpr void shrink_to_fit();

	// MODIFIERES
	constexpr void clear();
	constexpr iterator insert(const iterator pos, const T& value);
	constexpr iterator insert(const iterator pos, T&& value);
	constexpr iterator insert(const iterator pos, size_type count, const T& value);
	template<typename InputIt>
	constexpr iterator insert(const iterator pos, InputIt first, InputIt last);
	constexpr iterator insert(const iterator pos, std::initializer_list<T> ilist);


	constexpr iterator erase(const iterator pos);
	constexpr iterator erase(const iterator first, const iterator last);
	constexpr void push_back(const T& value);
	constexpr void push_back(T&& value);
	constexpr void pop_back();
	constexpr void resize(size_type count);
	constexpr void resize(size_type count, const T& value);

	// NON-MEMBER FUNCTIONS
	bool operator==(const DynArray<T>& rhs);
	bool operator!=(const DynArray<T>& rhs);


private:
	unsigned int m_size;
	unsigned int m_capacity;
	T* m_data;

	constexpr iterator data_offset(const iterator pos, const size_type offset);
	constexpr iterator dataCapacityIncrease(size_type new_size, const iterator pos = nullptr);
	constexpr unsigned int range(const iterator first, const iterator last);
};


