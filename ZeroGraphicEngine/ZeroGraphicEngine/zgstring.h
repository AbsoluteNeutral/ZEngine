#ifndef _ZG_STRING_H
#define _ZG_STRING_H

namespace zg 
{
	//__________________________________________________ Function
	size_t strlen	(const char* string_);
	void strcpy		(char* dest_, const char* source_);
	void strncpy	(char* dest_, const char* source_, size_t num_);

	//__________________________________________________ Class
	class ZG_API string
	{
	public:
		
		string	() noexcept;
		string	(const char* string_) noexcept;
		string	(const string& rhs_) noexcept;
		string	(string&& rhs_) noexcept;
		~string	();

		string& operator=	(const string& rhs_);
		string& operator=	(string&& rhs_);

		//__________________________________________________ Modifiers:
		string& operator+=	(const char* rhs_);
		string& operator+=	(const string& rhs_);
		void pushback		(const char* string_);
		void pushback		(const string& string_);
		//__________________________________________________ Capacity
		size_t		size() const;
		size_t		length() const;

		//__________________________________________________ Element access
		char&		at		(size_t pos_);
		const char& at		(size_t pos_) const;
		char&		back	();
		const char& back	() const;
		char&		front	();
		const char& front	() const;
		
		const char* cstr	() const;
		const char* c_str	() const;
		
	private:
		char* buffer;
		char* end;
	};

} // namespace zg



#endif //_UNITSTEST
