
#include "stdafx.h"
#include "XMLWrap.h"
#include "Logging.h"

FXMLError XMLLoadFile(FXMLDocument& doc, const std::string& filename_) {
	return doc.LoadFile(std::string{ XML_DEFAULT_PATHNAME + filename_ }.c_str());
}

FXMLError XMLSaveFile(FXMLDocument& doc, const std::string& filename_) {
	return doc.SaveFile(std::string{ XML_DEFAULT_PATHNAME + filename_ }.c_str());
}

bool XMLLoadPath(FXMLDocument& doc, const std::string& pathname_) {
	FXMLError eR = doc.LoadFile(pathname_.c_str());
	if (eR != tinyxml2::XML_SUCCESS) {
		Warn(std::string{ pathname_ + " Error not a xml file" }.c_str());
		return false;
	}
	return true;
}

bool XMLLoadPath(FXMLDocument& doc, const char* pathname_) {
	FXMLError eR = doc.LoadFile(pathname_);
	if (eR != tinyxml2::XML_SUCCESS) {
		Warn(std::string{ std::string{ pathname_ } +" Error not a xml file" }.c_str());
		return false;
	}
	return true;
}

FXMLError XMLSavePath(FXMLDocument& doc, const std::string& pathname_) {
	return doc.SaveFile(std::string{ pathname_ }.c_str());
}

void XMLCheckResult(FXMLError eResult, const std::string& filename_){
	switch (eResult) {
	case tinyxml2::XML_SUCCESS:
		Logg("No Error %s", filename_.c_str());
		break;
	case tinyxml2::XML_NO_ATTRIBUTE:
		ErrorMsg(std::string{ filename_ + "XML_NO_ATTRIBUTE" }.c_str());
		break;
	case tinyxml2::XML_WRONG_ATTRIBUTE_TYPE:
		ErrorMsg(std::string{ filename_ + "XML_WRONG_ATTRIBUTE_TYPE" }.c_str());
		break;
	case tinyxml2::XML_ERROR_FILE_NOT_FOUND:
		ErrorMsg(std::string{ filename_ + "XML_ERROR_FILE_NOT_FOUND" }.c_str());
		break;
	case tinyxml2::XML_ERROR_FILE_COULD_NOT_BE_OPENED:
		ErrorMsg(std::string{ filename_ + "XML_ERROR_FILE_COULD_NOT_BE_OPENED" }.c_str());
		break;
	case tinyxml2::XML_ERROR_FILE_READ_ERROR:
		ErrorMsg(std::string{ filename_ + "XML_ERROR_FILE_READ_ERROR" }.c_str());
		break;
	case tinyxml2::XML_ERROR_PARSING_ELEMENT:
		ErrorMsg(std::string{ filename_ + "XML_ERROR_PARSING_ELEMENT" }.c_str());
		break;
	case tinyxml2::XML_ERROR_PARSING_ATTRIBUTE:
		ErrorMsg(std::string{ filename_ + "XML_ERROR_PARSING_ATTRIBUTE" }.c_str());
		break;
	case tinyxml2::XML_ERROR_PARSING_TEXT:
		ErrorMsg(std::string{ filename_ + "XML_ERROR_PARSING_TEXT" }.c_str());
		break;
	case tinyxml2::XML_ERROR_PARSING_CDATA:
		ErrorMsg(std::string{ filename_ + "XML_ERROR_PARSING_CDATA" }.c_str());
		break;
	case tinyxml2::XML_ERROR_PARSING_COMMENT:
		ErrorMsg(std::string{ filename_ + "XML_ERROR_PARSING_COMMENT" }.c_str());
		break;
	case tinyxml2::XML_ERROR_PARSING_DECLARATION:
		ErrorMsg(std::string{ filename_ + "XML_ERROR_PARSING_DECLARATION" }.c_str());
		break;
	case tinyxml2::XML_ERROR_PARSING_UNKNOWN:
		ErrorMsg(std::string{ filename_ + "XML_ERROR_PARSING_UNKNOWN" }.c_str());
		break;
	case tinyxml2::XML_ERROR_EMPTY_DOCUMENT:
		ErrorMsg(std::string{ filename_ + "XML_ERROR_EMPTY_DOCUMENT" }.c_str());
		break;
	case tinyxml2::XML_ERROR_MISMATCHED_ELEMENT:
		ErrorMsg(std::string{ filename_ + "XML_ERROR_MISMATCHED_ELEMENT" }.c_str());
		break;
	case tinyxml2::XML_ERROR_PARSING:
		ErrorMsg(std::string{ filename_ + "XML_ERROR_PARSING" }.c_str());
		break;
	case tinyxml2::XML_CAN_NOT_CONVERT_TEXT:
		ErrorMsg(std::string{ filename_ + "XML_CAN_NOT_CONVERT_TEXT" }.c_str());
		break;
	case tinyxml2::XML_NO_TEXT_NODE:
		ErrorMsg(std::string{ filename_ + "XML_NO_TEXT_NODE" }.c_str());
		break;
	default:
		ErrorMsg(std::string{ filename_ + "UNKNOW ERROR" }.c_str());
		break;
	}
}
