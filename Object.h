/*
 * Object.h
 *
 *  Created on: 03.03.2015
 *      Author: dmitry

 	Copyright 2016 Dmitry Kilchanov <dmitrykilchanov@gmail.com>

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
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
