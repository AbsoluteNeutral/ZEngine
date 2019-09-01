
#ifndef _XMLWRAP_H
#define _XMLWRAP_H

#include "tinyxml2.h"
#include <string>

typedef tinyxml2::XMLError		FXMLError;
typedef tinyxml2::XMLDocument	FXMLDocument;
typedef tinyxml2::XMLElement	FXMLElement;
typedef tinyxml2::XMLNode		FXMLNode;

ZG_API FXMLError XMLLoadFile(FXMLDocument& doc, const std::string& filename_);
ZG_API FXMLError XMLSaveFile(FXMLDocument& doc, const std::string& filename_);

ZG_API bool XMLLoadPath(FXMLDocument& doc, const std::string& pathname_);
ZG_API bool XMLLoadPath(FXMLDocument& doc, const char* pathname_);
ZG_API FXMLError XMLSavePath(FXMLDocument& doc, const std::string& pathname_);

ZG_API void XMLCheckResult(FXMLError a_eResult, const std::string& pathname_);

#define XMLLoadF(doc,filename) { FXMLError eR = XMLLoadFile(doc,filename); XMLCheckResult(eR,filename); }
#define XMLSaveF(doc,filename) { FXMLError eR = XMLSaveFile(doc,filename); XMLCheckResult(eR,filename); }
#define XMLLoadP(doc,pathname) { FXMLError eR = XMLLoadPath(doc,pathname); XMLCheckResult(eR,pathname); }
#define XMLSaveP(doc,pathname) { FXMLError eR = XMLSavePath(doc,pathname); XMLCheckResult(eR,pathname); }

//#define XMLErrorNullPtr(a)	 { ErrorIf(a == nullptr, " nullptr, Nodes/Element not found"); }

#endif //_XMLWRAP_H
