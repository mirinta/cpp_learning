#pragma once

#include <string>
#include <unordered_set>

#include "Observer.h"

class Person
{
public:
    enum class StateChange { ForenameChanged, SurnameChanged, AddressChanged };
    using PersonObserver = Observer<Person, Person::StateChange>;

    bool attach(PersonObserver*);
    bool detach(PersonObserver*);
    void notify(StateChange);

    const std::string& forename() const { return _forename; }
    const std::string& surname() const { return _surname; }
    const std::string& address() const { return _address; }

    void setForename(const std::string&);
    void setSurname(const std::string&);
    void setAddress(const std::string&);

private:
    std::unordered_set<PersonObserver*> _observers;
    std::string _forename;
    std::string _surname;
    std::string _address;
};