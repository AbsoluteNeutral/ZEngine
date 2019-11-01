
#ifndef _HASHMESH_H
#define _HASHMESH_H

#include <string>

ZG_API size_t GenerateHash(const std::string& string_);
ZG_API size_t GenerateHash3(const std::string& string_);
ZG_API size_t GenerateHash(std::string&& string_);
ZG_API size_t GenerateHash3(std::string&& string_);

ZG_API size_t GetHashFromString(const std::string& string_);
ZG_API const std::string& GetStringFromHash(size_t hash_);

ZG_API void MergeHashTables();

#endif //_HASHMESH_H
