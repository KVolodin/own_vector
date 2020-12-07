#pragma once
//////////////////////////////////////////////////////////////////////////
#include <algorithm>

//////////////////////////////////////////////////////////////////////////

template<typename TData>
class CArray
{
	std::unique_ptr<TData[]> data_;
	std::size_t  size_;
	std::size_t  capacity_;

public:
	CArray()
		: size_( 0 )
		, capacity_( 0 )
	{
	}
	explicit CArray(
		std::size_t _size, TData&& _value
	) : CArray()
	{
		resize( _size );
		std::fill( data_.get(), data_.get() + _size, std::forward<TData>( _value ) );
	}

	CArray(
		const CArray& _other
	)
		: data_( std::make_unique<TData[]>( _other.capacity_ ) )
		, capacity_( _other.capacity_ )
		, size_( _other.size_ )
	{
		std::copy( _other.data_.get(), _other.data_.get() + _other.size_, data_.get() );
	}

	CArray(
		CArray&& _other
	) noexcept
		: data_( std::move( _other.data_ ) )
	{
		std::swap( capacity_, _other.capacity_ );
		std::swap( size_, _other.size_ );
	}

	CArray& operator =(
		CArray&& _other
		) noexcept
	{
		if ( &_other == this )
			return *this;

		data_ = std::move( _other.data_ );
		capacity_ = _other.capacity_;
		size_ = _other.size_;

		_other.capacity_ = 0;
		_other.size_ = 0;

		return *this;
	}

	~CArray()
	{
	}

	CArray& operator = (
		const CArray& _other
		)
	{
		if ( this == &_other )
			return *this;

		data_.reset( new TData[_other.capacity_] );
		capacity_ = _other.capacity_;
		size_ = _other.size_;
		std::copy( data(), _other.data() + _other.size_, data() );

		return *this;
	}

	void reserve(
		std::size_t _capacity
	)
	{
		if ( capacity_ >= _capacity )
			return;

		auto tmp = std::make_unique<TData[]>( _capacity );
		std::copy( data(), data() + size_, tmp.get() );
		data_ = std::move( tmp );
		capacity_ = _capacity;
	}
	void resize(
		std::size_t _size
	)
	{
		if ( _size > capacity_ )
			reserve( _size == 0 ? 1 : _size * 3 / 2 );
		size_ = _size;
	}

	void push_back(
		TData&& _value
	)
	{
		insert( size_, std::forward<TData>( _value ) );
	}
	template<typename ... Ts>
	void emplace_back(
		Ts&& ...params
	)
	{
		insert( size_, TData( std::forward<Ts>( params )... ) );
	}

	void insert(
		std::size_t _index,
		TData&& _value
	)
	{
		if ( _index > size_ )
			throw std::out_of_range( "" );

		resize( size_ + 1 );
		if ( _index + 1 != size_ )
			std::rotate( data() + _index, data() + size_ - 1, data() + size_ );

		data_[_index] = std::forward<TData>( _value );
	}
	void erase(
		std::size_t _index
	)
	{
		if ( _index >= size_ )
			throw std::out_of_range( "" );
		if ( _index != size_ - 1 )
			std::rotate( data() + _index, data() + _index + 1, data() + size_ );
		resize( size_ - 1 );
	}

	const std::size_t size() const
	{
		return size_;
	}

	void clear()
	{
		resize( 0 );
	}

	TData& operator [](
		std::size_t _index
		)
	{
		return data_[_index];
	}

	const TData& operator [](
		std::size_t _index
		) const
	{
		return data_[_index];
	}

	TData& at(
		std::size_t _index
	)
	{
		if ( _index >= size_ )
			throw std::out_of_range( "" );
		return data_[_index];
	}

	const TData& at(
		std::size_t _index
	) const
	{
		if ( _index >= size_ )
			throw std::out_of_range( "" );
		return data_[_index];
	}

	TData* data()
	{
		return data_.get();
	}

	const TData* data() const
	{
		return data_.get();
	}
};