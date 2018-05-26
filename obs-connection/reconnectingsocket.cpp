//
// Created by Sony on 25.05.2018.
//

#include "reconnectingsocket.h"

QString doSmt() {
	static int f = 1;
	f++;
    return QString::number(f);
}

int TestFoo::counter = 1;

int TestFoo::boo()
{
	TestFoo::counter *= 2;
	return TestFoo::counter;
}
