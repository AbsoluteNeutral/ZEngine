#include "stdafx.h"
#include "zgvector.h"

namespace zg
{
	//__________________________________________________ Capacity

	//__________________________________________________ Class
	template<typename T>
	vector<T>::vector() noexcept
		:buffer(nullptr), end(nullptr)
	{}

	template<typename T>
	vector<T>::vector(const char* string_) noexcept
		:buffer(nullptr), end(nullptr)
	{
		size_t sz	= zg::strlen(string_);
		buffer		= static_cast<char*>(malloc(sizeof(char) * (sz + 1)));
		zg::strcpy(buffer, string_);
		end			= buffer + sz;
	}

	template<typename T>
	vector<T>::vector(const vector<T>& rhs_)noexcept
		:buffer(nullptr), end(nullptr)
	{
		size_t sz = zg::strlen(rhs_.buffer);
		buffer = static_cast<char*>(malloc(sizeof(char) * (sz + 1)));
		zg::strcpy(buffer, rhs_.buffer);
		end = buffer + sz;
	}

	template<typename T>
	vector<T>::vector(vector<T>&& rhs_) noexcept
		:buffer(rhs_.buffer), end(rhs_.end)
	{
		rhs_.end = rhs_.buffer = nullptr;
	}

	template<typename T>
	vector<T>::~vector()
	{
		free(buffer);
	}

	template<typename T>
	vector<T>& vector<T>::operator=(const vector<T>& rhs_)
	{
		size_t sz = zg::strlen(rhs_.buffer);
		char* newBuff = static_cast<char*>(malloc(sizeof(char) * (sz + 1)));
		zg::strcpy(newBuff, rhs_.buffer);

		free(buffer);
		buffer = newBuff;
		end = buffer + sz;
		return *this;
	}
	template<typename T>
	vector<T>& vector<T>::operator=(vector<T>&& rhs_)
	{
		buffer = rhs_.buffer;
		end = rhs_.end;
		rhs_.buffer = nullptr;
		return *this;
	}

	template<typename T>
	vector<T>& vector<T>::operator+=(const char* rhs_)
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
	template<typename T>
	vector<T>& vector<T>::operator+=(const vector<T>& rhs_) {
		return operator+=(rhs_.cstr());
	}
	template<typename T>
	 void vector<T>::pushback(const char* string_) {
		operator+=(string_);
	}
	template<typename T>
	 void vector<T>::pushback(const vector<T>& string_) {
		operator+=(string_.cstr());
	}

	//__________________________________________________ Capacity
	 template<typename T>
	size_t vector<T>::size() const {
		return end - buffer;
	}
	template<typename T>
	size_t vector<T>::length() const {
		return end - buffer;
	}

	//__________________________________________________ Element access
	template<typename T>
	char& vector<T>::at(size_t pos_) {
		return buffer[pos_];
	}
	template<typename T>
	const char& vector<T>::at(size_t pos_) const {
		return buffer[pos_];
	}
	template<typename T>
	char&		vector<T>::back() {
		return *(end - 1);
	}
	template<typename T>
	const char& vector<T>::back() const {
		return *(end - 1);
	}
	template<typename T>
	char& vector<T>::front() {
		return *buffer;
	}
	template<typename T>
	const char& vector<T>::front() const {
		return *buffer;
	}

	
	template<typename T>
	const char* vector<T>::cstr() const {
		return buffer;
	}
	template<typename T>
	const char* vector<T>::c_str() const {
		return buffer;
	}

} // namespace zg
