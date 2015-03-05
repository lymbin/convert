/*
 * Object.h
 *
 *  Created on: 03.03.2015
 *      Author: dmitry
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include "main.h"

class iObject{
protected:
	std::string msName;
	bool mbIsCreated;

public:

	iObject(std::string asName) {
		msName = asName;
		mbIsCreated = false;}
	virtual ~iObject() {}

	// Заполнение и уничтожение
	virtual void Create(){mbIsCreated = true;}
	virtual void Destroy(){mbIsCreated = false;}

	bool IsCreated(){return mbIsCreated;}
};



#endif /* OBJECT_H_ */
