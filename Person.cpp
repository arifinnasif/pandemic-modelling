#include <iostream>
#include "rand.cpp"

#define DEATHRATE 10
#define RECOVERY 34
#define RECOVERY_TOL 3

typedef enum State{HEALTHY,INFECTED,RECOVERED,DEAD} State;

class Person
{
    int days;
    int recoveryPeriod;
    State health;



public:
    Person();
    void increaseDays();
    int increaseDays2();
    void infect();
    void infect(Person &p);
    void recover();
    void kill();
    void setRecoveryPeriod(int d);
    int getRecoveryPeriod();
    int getDays() {return days;}
    State getHealth();
};

Person::Person()
{
    days=0;
    health=HEALTHY;
}

int Person::increaseDays2()
{
    ++days;
    return days>=recoveryPeriod;
}

void Person::increaseDays()
{
    if(health==INFECTED)
    {
        
        if(increaseDays2())
        {
            if(DEATHRATE*100 >= randint(1,10000)) kill();
            else recover();
        }
    }
}

void Person::infect()
{
    if(health==DEAD||health==RECOVERED) {std::cout<<std::endl<<"can't infect"; exit(0);}
    health=INFECTED;
    recoveryPeriod=round(randnormal(RECOVERY*1.0,RECOVERY_TOL*1.0));
}

void Person::infect(Person &p)
{
    if(health!=INFECTED) {std::cout<<std::endl<<"can't infect"; exit(0);}
    if(p.health==HEALTHY)
    {
        p.health=INFECTED;
        p.recoveryPeriod=round(randnormal(RECOVERY*1.0,RECOVERY_TOL*1.0));
    }
}

void Person::kill()
{
    days=0;
    recoveryPeriod=0;
    health=DEAD;
}

void Person::recover()
{
    days=0;
    recoveryPeriod=0;
    health=RECOVERED;
}

State Person::getHealth()
{
    return health;
}

void Person::setRecoveryPeriod(int d)
{
    recoveryPeriod=d;
}

int Person::getRecoveryPeriod()
{
    return recoveryPeriod;
}
