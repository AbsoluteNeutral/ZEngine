/*****************************************************************************/
/*!
\file			Component.cpp
\brief			Base Class
*/
/*****************************************************************************/
#include "stdafx.h"
#include "Component.h"

Component::Component() noexcept
	:enable(true)
	, owner(nullptr)
{}
Component::Component(const Component& rhs) noexcept
	: enable(rhs.enable)
	, owner{ nullptr }
{}
Component::~Component() {
	owner = nullptr;
}
Component& Component::operator=(const Component& rhs){
	enable = rhs.enable;
	owner = nullptr;
	return *this;
}

bool Component::IsEnable() const {
	return enable;
}
void Component::SetEnable(bool enable_) {
	enable = enable_;
}
GameObject* Component::Owner() {
	return owner;
}

void Component::Serial(FXMLDocument* doc, FXMLElement* pElem) {
	FXMLElement* pSubElem = doc->NewElement("enable");
	pSubElem->SetAttribute("num", enable);
	pElem->InsertEndChild(pSubElem);
}
void Component::Unserial(FXMLElement* pElem) {
	FXMLElement* pSubElem = pElem->FirstChildElement("enable");
	enable = pSubElem->BoolAttribute("num");
}
