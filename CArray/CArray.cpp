#include <iostream>
#include <random>  
#include <ctime>  
#include <functional>

//////////////////////////////////////////////////////////////////////////
#include "CArray.h"

//////////////////////////////////////////////////////////////////////////

template<typename TData>
void print_(
	const CArray<TData>& _arr
)
{
	std::cout << "[";
	for ( std::size_t i = 0; i < _arr.size(); ++i )
		std::cout << _arr[i] << ( i != _arr.size() - 1 ? ", " : "" );
	std::cout << "]" << std::endl;
}

template<typename TData>
void printStage_(
	const std::string& _str,
	const CArray<TData>& _arr,
	std::function<void()> _f
)
{
	std::cout << _str << std::endl;
	_f();
	print_( _arr );
	std::cout << std::endl;
}

void testDigits_()
{
	constexpr std::size_t MAX_DIGIT_ARR_SIZE = 20;
	CArray<int> array_;
	array_.resize( MAX_DIGIT_ARR_SIZE );
	std::mt19937 gen( 0 );
	std::uniform_int_distribution<> uid( 0, 100 );

	//////////////////////////////////////////////////////////////////////////
	printStage_( "1.1", array_, [&]() {
		std::generate( array_.data(), array_.data() + array_.size(), [&uid, &gen]() {
			return uid( gen );
		} );
	} );
	//////////////////////////////////////////////////////////////////////////
	printStage_( "1.2", array_, [&]() {
		std::sort( array_.data(), array_.data() + array_.size() );
	} );

	//////////////////////////////////////////////////////////////////////////
	printStage_( "1.3", array_, [&]() {
		for ( std::size_t i = 0, add = 1; i < array_.size(); ++i )
		{
			if ( ( i + add ) % 2 == 0 )
			{
				add = add == 1 ? 0 : 1;
				array_.erase( i-- );
			}
		}
	} );

	//////////////////////////////////////////////////////////////////////////
	printStage_( "1.4", array_, [&]() {
		for ( std::size_t i = 0; i < MAX_DIGIT_ARR_SIZE / 2; ++i )
			array_.insert( uid( gen ) % array_.size(), uid( gen ) );
	} );

	//////////////////////////////////////////////////////////////////////////
	printStage_( "1.5", array_, [&]() {
		array_.clear();
	} );
}

void testStrings_()
{
	constexpr std::size_t MAX_STRING_ARR_SIZE = 15;
	CArray<std::string> array_;
	array_.resize( MAX_STRING_ARR_SIZE );
	std::mt19937 gen( 0 );
	std::uniform_int_distribution<> randomChar( 'a', 'z' );
	std::uniform_int_distribution<> randomSize( 4, 8 );
	std::uniform_int_distribution<> randomPos( 0, MAX_STRING_ARR_SIZE );

	auto generateRandomString = [&randomSize, &randomChar, &gen]() {
		std::string str;
		str.resize( randomSize( gen ) );
		std::generate( str.begin(), str.end(), [&randomChar, &gen]() {
			return static_cast<char>( randomChar( gen ) );
		} );
		return str;
	};

	//////////////////////////////////////////////////////////////////////////
	printStage_( "1.1", array_, [&]() {
		std::generate( array_.data(), array_.data() + array_.size(), generateRandomString );
	} );

	//////////////////////////////////////////////////////////////////////////
	printStage_( "1.2", array_, [&]() {
		std::sort( array_.data(), array_.data() + array_.size() );
	} );

	//////////////////////////////////////////////////////////////////////////
	printStage_( "1.3", array_, [&]() {
		for ( std::size_t i = 0; i < array_.size(); ++i )
			if ( array_[i].find_first_of( "abcde" ) != std::string::npos )
				array_.erase( i-- );
	} );

	//////////////////////////////////////////////////////////////////////////
	printStage_( "1.4", array_, [&]() {
		for ( std::size_t i = 0; i < 3; ++i )
			array_.insert( randomPos( gen ) % array_.size(), generateRandomString() );
	} );
}

int main()
{
	testDigits_();
	testStrings_();
	return 0;
}