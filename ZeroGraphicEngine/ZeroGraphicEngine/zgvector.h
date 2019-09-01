#ifndef _ZG_VECTOR_H
#define _ZG_VECTOR_H

namespace zg 
{
	//__________________________________________________ Function

	//__________________________________________________ Class
	template<typename T>
	class ZG_API vector
	{
	public:
		
		vector	() noexcept;
		vector	(const char* string_) noexcept;
		vector	(const vector& rhs_) noexcept;
		vector	(vector&& rhs_) noexcept;
		~vector	();

		vector& operator=	(const vector& rhs_);
		vector& operator=	(vector&& rhs_);

		//__________________________________________________ Modifiers:
		vector& operator+=	(const char* rhs_);
		vector& operator+=	(const vector& rhs_);
		void pushback		(const char* string_);
		void pushback		(const vector& string_);
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
