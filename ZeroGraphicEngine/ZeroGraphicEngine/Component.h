
#ifndef _COMPONENT_H
#define _COMPONENT_H

#include "GameObject.h"
#include "XMLWrap.h"

class ZG_API Component
{
public:
    Component					() noexcept;
	Component					(const Component& rhs) noexcept;
	virtual ~Component			();
	virtual Component& operator=(const Component& rhs);
	
	virtual bool	IsEnable	() const;
	virtual void	SetEnable	(bool enable_);
	GameObject*		Owner		();

	virtual void	Serial		(FXMLDocument* doc, FXMLElement* root);
	virtual void	Unserial	(FXMLElement* pElem);

    GameObject* owner;
	bool enable;
};

#endif //_COMPONENT_H
