/*
 * Object.h
 *
 *  Created on: 03.03.2015
 *      Author: dmitry
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include "main.h"

// Класс для хранения общей информации об объекте.
class iObject{
protected:
	std::string msName;	// Имя объекта.
	bool mbIsCreated;	// Построен ли объект.

public:

	// Конструктор объекта
	iObject(std::string asName) {
		msName = asName;
		mbIsCreated = false;}
	virtual ~iObject() {}	// Виртуальный деструктор объекта.

	// Заполнение и уничтожение.
	virtual void Create(){mbIsCreated = true;}
	virtual void Destroy(){mbIsCreated = false;}

	// Построен ли объект.
	bool IsCreated(){return mbIsCreated;}
};



#endif /* OBJECT_H_ */
