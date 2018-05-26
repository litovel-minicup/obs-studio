//
// Created by Sony on 25.05.2018.
//

#ifndef OBS_STUDIO_RECONNECTINGSOCKET_H
#define OBS_STUDIO_RECONNECTINGSOCKET_H


#include <QString>

#ifdef _MSC_VER
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

DLLEXPORT QString doSmt();

class DLLEXPORT TestFoo
{
    private:
	    static int counter;

    public:
	    int boo();
};


#endif //OBS_STUDIO_RECONNECTINGSOCKET_H
