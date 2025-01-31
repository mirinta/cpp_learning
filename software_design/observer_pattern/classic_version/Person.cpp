#include "Person.h"

bool Person::attach(PersonObserver* observer)
{
    if (!observer)
        return false;

    return _observers.insert(observer).second;
}

bool Person::detach(PersonObserver* observer)
{
    if (!observer || !_observers.count(observer))
        return false;

    _observers.erase(observer);
    return true;
}

void Person::notify(StateChange change)
{
    for (auto& observer : _observers) {
        observer->update(*this, change);
    }
}

void Person::setForename(const std::string& name)
{
    if (_forename != name) {
        _forename = name;
        notify(StateChange::ForenameChanged);
    }
}

void Person::setSurname(const std::string& name)
{
    if (_surname != name) {
        _surname = name;
        notify(StateChange::SurnameChanged);
    }
}

void Person::setAddress(const std::string& address)
{
    if (_address != address) {
        _address = address;
        notify(StateChange::AddressChanged);
    }
}