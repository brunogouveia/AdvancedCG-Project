/*
	Copyright (C) 2012 Alexander Overvoorde

	Permission is hereby granted, free of charge, to any person obtaining a copy of
	this software and associated documentation files (the "Software"), to deal in
	the Software without restriction, including without limitation the rights to
	use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
	the Software, and to permit persons to whom the Software is furnished to do so,
	subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
	FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
	COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
	IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
	CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE
*/

#pragma once

#ifndef OOGL_BYTEBUFFER_HPP
#define OOGL_BYTEBUFFER_HPP

#pragma once

#ifndef OOGL_PLATFORM_HPP
#define OOGL_PLATFORM_HPP

/*
	Platform identification
*/

#if defined( _WIN32 )
	#define OOGL_PLATFORM_WINDOWS
	#include <Windows.h>
	#include <WindowsX.h>
	#include <GL/GL.h>
#elif defined( __linux__ )
	#define OOGL_PLATFORM_LINUX
	#include <X11/Xlib.h>
	#include <GL/gl.h>
	#include <GL/glx.h>
	#include <sys/time.h>
#elif defined( __APPLE__ )
	#define OOGL_PLATFORM_OSX
#endif

/*
	Types
*/

namespace GL
{
	typedef unsigned char uchar;
	typedef unsigned short ushort;
	typedef unsigned int uint;
	typedef unsigned long ulong;

	namespace Type
	{
		enum type_t
		{
			Byte = GL_BYTE,
			UnsignedByte = GL_UNSIGNED_BYTE,
			Short = GL_SHORT,
			UnsignedShort = GL_UNSIGNED_SHORT,
			Int = GL_INT,
			UnsignedInt = GL_UNSIGNED_INT,
			Float = GL_FLOAT,
			Double = GL_DOUBLE
		};
	}

	typedef GLint Attribute;
	typedef GLint Uniform;
}

#endif
#include <vector>
#include <climits>

namespace GL
{
	/*
		Byte reader
	*/
	class ByteReader
	{
	public:
		ByteReader( uint length, bool littleEndian ) : buffer( new uchar[length] ), length( length ), ptr( 0 ), littleEndian( littleEndian ) {}
		~ByteReader() { delete [] buffer; }

		void Reuse( uint length )
		{
			delete [] buffer;
			buffer = new uchar[length];
			this->length = length;
			ptr = 0;
		}

		uchar* Data() { return buffer; }
		uint Length() { return length; }

		void Advance( uint count ) { ptr += count; }
		void Move( uint location ) { ptr = location; }

		bool Compare( uint location, uint length, const uchar* data )
		{
			if ( location + length > this->length ) return false;
			
			for ( uint i = 0; i < length; i++ )
				if ( buffer[ location + i ] != data[i] ) return false;
			
			return true;
		}

		uchar PeekByte( uint offset = 0 ) { return buffer[ptr+offset]; }

		uchar ReadUbyte()
		{
			uchar val = buffer[ptr];
			ptr++;
			return val;
		}

		ushort ReadUshort()
		{
			ushort val;
			if ( littleEndian ) val = buffer[ptr+0] + ( buffer[ptr+1] << 8 );
			else val = buffer[ptr+1] + ( buffer[ptr+0] << 8 );
			ptr += 2;
			return val;
		}

		uint ReadUint()
		{
			uint val;
			if ( littleEndian ) val = buffer[ptr+0] + ( buffer[ptr+1] << 8 ) + ( buffer[ptr+2] << 16 ) + ( buffer[ptr+3] << 24 );
			else val = buffer[ptr+3] + ( buffer[ptr+2] << 8 ) + ( buffer[ptr+1] << 16 ) + ( buffer[ptr+0] << 24 );
			ptr += 4;
			return val;
		}

		int ReadInt()
		{
			uint val = ReadUint();
			if ( val <= INT_MAX )
				return val;
			else
				return INT_MIN + val - INT_MAX - 1;
		}

		void Read( uchar* dest, uint length )
		{
			for ( uint i = 0; i < length; i++ )
				dest[i] = buffer[ptr+i];
			ptr += length;
		}

	private:
		uchar* buffer;
		uint length;
		uint ptr;
		bool littleEndian;

		ByteReader( const ByteReader& );
		const ByteReader& operator=( const ByteReader& );
	};

	/*
		Byte writer
	*/
	class ByteWriter
	{
	public:
		ByteWriter( bool littleEndian ) : littleEndian( littleEndian ) {};

		uchar* Data() { return &buffer[0]; }
		uint Length() { return buffer.size(); }

		void Pad( uint count )
		{
			for ( uint i = 0; i < count; i++ )
				buffer.push_back( 0 );
		}

		void WriteUbyte( uchar val )
		{
			buffer.push_back( val );
		}
	
		void WriteUshort( ushort val )
		{
			if ( littleEndian ) {
				buffer.push_back( val & 0x00FF );
				buffer.push_back( ( val & 0xFF00 ) >> 8 );
			} else {
				buffer.push_back( ( val & 0xFF00 ) >> 8 );
				buffer.push_back( val & 0x00FF );
			}
		}

		void WriteUint( uint val )
		{
			if ( littleEndian ) {
				buffer.push_back( (uchar)( val & 0x000000FF ) );
				buffer.push_back( (uchar)( ( val & 0x0000FF00 ) >> 8 ) );
				buffer.push_back( (uchar)( ( val & 0x00FF0000 ) >> 16 ) );
				buffer.push_back( (uchar)( ( val & 0xFF000000 ) >> 24 ) );
			} else {
				buffer.push_back( (uchar)( ( val & 0xFF000000 ) >> 24 ) );
				buffer.push_back( (uchar)( ( val & 0x00FF0000 ) >> 16 ) );
				buffer.push_back( (uchar)( ( val & 0x0000FF00 ) >> 8 ) );
				buffer.push_back( (uchar)( val & 0x000000FF ) );
			}
		}

		void WriteString( const char* str )
		{
			while ( *str != 0 )
				buffer.push_back( *(str++) );
			buffer.push_back( 0 );
		}

	private:
		std::vector<uchar> buffer;
		bool littleEndian;

		ByteWriter( const ByteWriter& );
		const ByteWriter& operator=( const ByteWriter& );
	};
}

#endif