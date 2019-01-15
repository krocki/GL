/*
* @Author: kmrocki
* @Date:   2016-11-07 17:56:01
* @Last Modified by:   kmrocki
* @Last Modified time: 2016-11-08 22:33:30
*
* Author: Kamil Rocki <kmrocki@us.ibm.com>
*
* Copyright (c) 2016, IBM Corporation. All rights reserved.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
*
* Matrix<T>
*
*/

#ifndef __MATRIX_
#define __MATRIX__

#include <containers/datatype.h>
#include <random>
#include <iostream>
#include <iomanip>
#include <string.h>
#include <typeinfo>
#include <functional>
#include <assert.h>

#include <cblas.h>

/* TODO: column-major or row-major */
/* 0-indexing vs 1-indexing */
/* i - which row, j - which column */

/* now: */
/* column-major indexing 0-based*/

template <typename T>
class Matrix {

public:

	T* _data_ = nullptr;

	size_t _rows = 0;
	size_t _cols = 0;
	size_t _size = 0;
	size_t bytes = 0;

	size_t bytes_allocated = 0;

	void alloc ( size_t rows, size_t cols ) {

		bytes_allocated = rows * cols * sizeof ( T );

		_data_ = ( T* ) malloc ( bytes_allocated );

		memset ( _data_, '\0', bytes_allocated );

	}

	void dealloc() {

		if ( bytes_allocated > 0 ) {

			if ( _data_ != nullptr ) {

				free ( _data_ );

				bytes_allocated = 0;
				_data_ = nullptr;

			}

		}

	}

	/* * * * * READ-WRITE ACCESS METHODS * * * * */

	T* data() { return _data_; }

	virtual ~Matrix() {

		dealloc();

	}

	void sync_device() { }

	void sync_host() { }

	/* main constr */
	Matrix ( const size_t rows, const size_t cols ) {

		resize ( rows, cols );

	}

	inline T& operator() ( const size_t i ) {

		assert ( i < _size );
		return data() [ i ];
	}


	inline T& operator() ( const size_t i, const size_t j ) {

		assert ( i < _rows );
		assert ( j < _cols );
		/* colmajor-0 */
		return data() [(j) * rows() + (i)];

	}

	void setZero() {

		memset ( data(), '\0', bytes_allocated );

	}

	void resize ( const size_t new_rows, const size_t new_cols ) {

		size_t other_bytes = new_rows * new_cols * sizeof ( T );

		if ( other_bytes > bytes_allocated ) {

			/* realloc */
			dealloc();
			alloc ( new_rows, new_cols );

		}

		_rows = new_rows;
		_cols = new_cols;
		_size = new_rows * new_cols;
		bytes = other_bytes;

	}

	void set ( const Matrix& src ) {

		resize ( src.rows(), src.cols() );
		memcpy ( _data_, src.data(), src.bytes );
	}

	Matrix& operator= ( const Matrix& other ) {

		set ( other );
		return *this;

	};

	/* TODO: move semantics */

	/* copy constr from Matrix */
	Matrix ( const Matrix& other ) { operator= ( other ); }

	/* TODO: lambda mutable, constexpr */
	/* http://en.cppreference.com/w/cpp/language/lambda */
	/* variable number of arguments */
	template<typename F, typename...X>
	void forall ( const F& lambda, X ...x ) {

		for ( size_t i = 0; i < size(); i++ )
			this->data() [i] = lambda (x(i)... );

	}

	/* * * * * READ-ONLY ACCESS METHODS * * * * */

	/* default constr */
	Matrix() { /* we don't know the dimensions, so do nothing */ }

	const inline size_t rows() const { return _rows; }
	const inline size_t cols() const { return _cols; }
	const inline size_t size() const { return _size; }

	const T* data() const { return _data_;	}
	const inline T& operator() ( const size_t i ) const { return data() [ i ]; }
	const inline T& operator() ( const size_t i, const size_t j ) const {

		/* colmajor-0 */
		return data() [ (j) * rows() + (i) ]; /* read-only access */
	}

	friend std::ostream& operator<< ( std::ostream& os, const Matrix<T>& obj ) {

		// Display dimensions.
		std::cout << "RAW: " << obj.size() << " elements " << std::endl;

		for ( size_t i = 0; i < obj.size(); i++ )
			os << obj.data() [ i ] << " " ;

		std::cout << std::endl;

		os << "[" << obj.size() << " = " << obj.rows() << " x " << obj.cols() << std::endl;

		// Display elements.
		if ( typeid ( T ) == typeid ( int ) ) {
			for ( size_t i = 0; i < obj.size(); i++ )
				os << ( char ) obj.data() [ i ];

		}

		else {

			for ( size_t i = 0; i < obj.rows(); i++ ) {

				for ( size_t j = 0; j < obj.cols(); j++ ) {
					os << std::setw ( 8 ) << std::setprecision ( 4 ) << obj ( i, j ) << "";

					if ( j % ( obj.cols() / 4 ) == ( obj.cols() / 4 - 1 ) ) os << "    ";

				}

				std::cout << std::endl;
			}


		}

		os << "]" << std::endl;

		return os;
	}

	dtype* operator[] ( size_t offset ) {

		return _data_ + offset;

	}

	template<class Archive>
	void serialize ( Archive& archive ) {

		archive ( _rows, _cols );

		for ( int i = 0; i < _rows; i++ )
			for ( int j = 0; j < _cols; j++ )
				archive ( this->operator() ( i, j ) );

	}
};

#endif /* MATRIX */
