#pragma once

#include "Person.h"
#include "Observer.h"

class NameObserver : public Observer<Person, Person::StateChange>
{
public:
    explicit NameObserver() = default;

    void update(const Person&, Person::StateChange property) const override
    {
        if (property == Person::ForenameChanged || property == Person::SurnameChanged) {
            // response to changed name
        }
    }
};

class AddressObserver : public Observer<Person, Person::StateChange>
{
public:
    explicit AddressObserver() = default;

    void update(const Person&, Person::StateChange property) const override
    {
        if (property == Person::AddressChanged) {
            // respond to changed address
        }
    }
};
