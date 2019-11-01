

#include "stdafx.h"
#include "HashString.h"
#include "Logging.h"

#include <unordered_map>

static std::unordered_map<size_t, std::string> GLOBAL_HASH_STRING_TABLE;
static std::unordered_map<size_t, std::string> GLOBAL_HASH_STRING_TABLE2;
static std::unordered_map<size_t, std::string> GLOBAL_HASH_STRING_TABLE3;

size_t GenerateHash(const std::string& string_) {
	static std::hash<std::string> hasher;
	size_t hashedString = hasher(string_);						// Compute the hash for the string
	GLOBAL_HASH_STRING_TABLE.emplace(hashedString, string_);
	return hashedString;
}

size_t GenerateHash(std::string&& string_) {
	static std::hash<std::string> hasher;
	size_t hashedString = hasher(string_);						// Compute the hash for the string
	GLOBAL_HASH_STRING_TABLE.emplace(hashedString, string_);
	return hashedString;
}

size_t GenerateHash2(std::string&& string_) {
  static std::hash<std::string> hasher;
  size_t hashedString = hasher(string_);						// Compute the hash for the string
  GLOBAL_HASH_STRING_TABLE2.emplace(hashedString, string_);
  return hashedString;
}

size_t GenerateHash2(const std::string& string_) {
  static std::hash<std::string> hasher;
  size_t hashedString = hasher(string_);						// Compute the hash for the string
  GLOBAL_HASH_STRING_TABLE2.emplace(hashedString, string_);
  return hashedString;
}

size_t GenerateHash3(std::string&& string_) {
  static std::hash<std::string> hasher;
  size_t hashedString = hasher(string_);						// Compute the hash for the string
  GLOBAL_HASH_STRING_TABLE2.emplace(hashedString, string_);
  return hashedString;
}

size_t GenerateHash3(const std::string& string_) {
  static std::hash<std::string> hasher;
  size_t hashedString = hasher(string_);						// Compute the hash for the string
  GLOBAL_HASH_STRING_TABLE3.emplace(hashedString, string_);
  return hashedString;
}

size_t GetHashFromString(const std::string& string_) {
	static std::hash<std::string> hasher;
	size_t hashedString = hasher(string_);	// Compute the hash for the string

	// Search lookup table for the hashed string
	auto it = GLOBAL_HASH_STRING_TABLE.find(hashedString);

	//If the hashedString is not registered
	if (it == GLOBAL_HASH_STRING_TABLE.end())
		GenerateHash(string_);
	//	ErrorMsg(std::string{ "Texture String name not found" + string_ }.c_str());
	return hashedString;
}

size_t GetHashFromString2(const std::string& string_) {
  static std::hash<std::string> hasher;
  size_t hashedString = hasher(string_);	// Compute the hash for the string

  // Search lookup table for the hashed string
  auto it = GLOBAL_HASH_STRING_TABLE2.find(hashedString);

  //If the hashedString is not registered
  if (it == GLOBAL_HASH_STRING_TABLE2.end())
    GenerateHash2(string_);
  //	ErrorMsg(std::string{ "Texture String name not found" + string_ }.c_str());
  return hashedString;
}

const std::string& GetStringFromHash(size_t hash_) {
#if defined(_DEBUG) || defined(_EDITOR_ON)
	auto it = GLOBAL_HASH_STRING_TABLE.find(hash_);
	// If it is not found, return the empty string
	if (it == GLOBAL_HASH_STRING_TABLE.end())
		ErrorMsg(std::string{ "Hash number not found" }.c_str());
	
	return it->second;
#else
	return GLOBAL_HASH_STRING_TABLE[hash_];
#endif
}

const std::string& GetStringFromHash2(size_t hash_) {
#if defined(_DEBUG) || defined(_EDITOR_ON)
  auto it = GLOBAL_HASH_STRING_TABLE2.find(hash_);
  // If it is not found, return the empty string
  if (it == GLOBAL_HASH_STRING_TABLE2.end())
    ErrorMsg(std::string{ "Hash number not found" }.c_str());

  return it->second;
#else
  return GLOBAL_HASH_STRING_TABLE[hash_];
#endif
}

//void MergeHashTables()
//{
//  GLOBAL_HASH_STRING_TABLE.insert(GLOBAL_HASH_STRING_TABLE2.begin(), GLOBAL_HASH_STRING_TABLE2.end());
//  GLOBAL_HASH_STRING_TABLE.insert(GLOBAL_HASH_STRING_TABLE3.begin(), GLOBAL_HASH_STRING_TABLE3.end());
//
//  GLOBAL_HASH_STRING_TABLE2.clear();
//  GLOBAL_HASH_STRING_TABLE2.rehash(0);
//  GLOBAL_HASH_STRING_TABLE3.clear();
//  GLOBAL_HASH_STRING_TABLE3.rehash(0);
//}
