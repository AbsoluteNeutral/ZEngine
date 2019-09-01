#include "stdafx.h"
#include "zgstring.h"

#include <string>

namespace zg
{
	//__________________________________________________ Capacity
	size_t strlen(const char* string_)
	{
		const char* begin = string_;
		while (*begin != '\0')
			++begin;
		return begin - string_;
	}

	void strcpy(char* dest_, const char* source_)
	{
		const char* begin = source_;
		char* cpy = dest_;
		while (*begin != '\0')
		{
			*cpy = *begin;
			++begin;
			++cpy;
		}

		*cpy = *begin;
	}

	void strncpy(char* dest_, const char* source_, size_t num_)
	{
		const char* begin = source_;
		char* cpy = dest_;
		while (num_--)
		{
			*cpy = *begin;
			++begin;
			++cpy;
		}
	}

	//__________________________________________________ Class
	string::string() noexcept
		:buffer(nullptr), end(nullptr)
	{}

	string::string(const char* string_) noexcept
		:buffer(nullptr), end(nullptr)
	{
		size_t sz	= zg::strlen(string_);
		buffer		= static_cast<char*>(malloc(sizeof(char) * (sz + 1)));
		zg::strcpy(buffer, string_);
		end			= buffer + sz;
	}

	string::string(const string& rhs_)noexcept
		:buffer(nullptr), end(nullptr)
	{
		size_t sz = zg::strlen(rhs_.buffer);
		buffer = static_cast<char*>(malloc(sizeof(char) * (sz + 1)));
		zg::strcpy(buffer, rhs_.buffer);
		end = buffer + sz;
	}

	string::string(string&& rhs_) noexcept
		:buffer(rhs_.buffer), end(rhs_.end)
	{
		rhs_.end = rhs_.buffer = nullptr;
	}

	string::~string()
	{
		free(buffer);
	}

	string& string::operator=(const string& rhs_) 
	{
		size_t sz = zg::strlen(rhs_.buffer);
		char* newBuff = static_cast<char*>(malloc(sizeof(char) * (sz + 1)));
		zg::strcpy(newBuff, rhs_.buffer);

		free(buffer);
		buffer = newBuff;
		end = buffer + sz;
		return *this;
	}
	string& string::operator=(string&& rhs_) 
	{
		buffer = rhs_.buffer;
		end = rhs_.end;
		rhs_.buffer = nullptr;
		return *this;
	}

	string& string::operator+=(const char* rhs_)
	{
		size_t mylen  = zg::strlen(buffer);
		size_t rhslen = zg::strlen(rhs_);

		char* newBuff = static_cast<char*>(malloc(sizeof(char) * (mylen + rhslen + 1)));
		zg::strncpy(newBuff, buffer, mylen);
		zg::strcpy(newBuff + mylen, rhs_);

		free(buffer);
		buffer = newBuff;
		end = buffer + mylen + rhslen;
		return *this;
	}
	string& string::operator+=(const string& rhs_) {
		return operator+=(rhs_.cstr());
	}
	void string::pushback(const char* string_) {
		operator+=(string_);
	}
	void string::pushback(const string& string_) {
		operator+=(string_.cstr());
	}

	//__________________________________________________ Capacity
	size_t string::size() const {
		return end - buffer;
	}
	size_t string::length() const {
		return end - buffer;
	}

	//__________________________________________________ Element access
	char& string::at(size_t pos_) {
		return buffer[pos_];
	}
	const char& string::at(size_t pos_) const {
		return buffer[pos_];
	}
	char&		string::back() {
		return *(end - 1);
	}
	const char& string::back() const {
		return *(end - 1);
	}
	char& string::front() {
		return *buffer;
	}
	const char& string::front() const {
		return *buffer;
	}

	

	const char* string::cstr() const {
		return buffer;
	}
	const char* string::c_str() const {
		return buffer;
	}

} // namespace zg
