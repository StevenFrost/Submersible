#ifndef IOBSERVER_H
#define IOBSERVER_H

#include "header.h"

class IObservable;

class IObserver {
public:
	virtual void update(IObservable *observerable) = 0;
};

#endif // !IOBSERVER_H