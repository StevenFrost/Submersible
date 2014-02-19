#ifndef IOBSERVABLE_H
#define IOBSERVABLE_H

#include <vector>
#include <algorithm>
#include "header.h"
#include "IObserver.h"

class IObservable {
public:
	virtual void attach(IObserver *observer) {
		observers.push_back(observer);
	}

	virtual void detatch(IObserver *observer) {
		observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
	}

	virtual void notify() {
		for (IObserver *observer : observers) {
			observer->update(this);
		}
	}
protected:
	std::vector<IObserver *> observers;
};

#endif // !IOBSERVABLE_H