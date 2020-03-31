#include <iostream>
#include <vector>
#include "Person.cpp"

#define SPREADRATE 1
#define SPREADRATE_TOL 0.25

#define DEATHRATE_HPT 2.0
//DEATHRATE_HPT should be a double
#define TREATMENT_QUALITY 0.5
//TREATMENT_QUALITY should be less than 1

using namespace std;


class Cluster
{
    vector<Person> member;
    int infected;
    int recovered;
    int death;
    int deathToll;

public:
    Cluster();
    Cluster(int num);
    void setPopulation(int num);
    void travelTo(Cluster &c);
    void update();
    void init(int id);
    int getPopulation();
    int getInfected();
    int getRecovered();
    int getDeathToday();
    int getDeathToll();
    vector<Person> & getMember();
    Person getMemberAt(int id);
};

Cluster::Cluster()
{
    infected=0;
    recovered=0;
    death=0;
    deathToll=0;
}

Cluster::Cluster(int num)
{
    infected=0;
    recovered=0;
    death=0;
    deathToll=0;
    Person temp;
    for(int i=0;i<num;i++)
        member.push_back(temp);
}

void Cluster::setPopulation(int num)
{
    Person temp;
    for(int i=0;i<num;i++)
        member.push_back(temp);
}

void Cluster::travelTo(Cluster &c)
{
    if(member.empty()) {std::cout<<std::endl<<"the cluster is empty"; exit(0);}
    int id=randint(0,member.size()-1);
    c.member.push_back(member[id]);
    member.erase(member.begin()+id);
}

int Cluster::getPopulation()
{
    return member.size();
}

int Cluster::getInfected()
{
    return infected;
}

int Cluster::getRecovered()
{
    return recovered;
}

int Cluster::getDeathToday()
{
    return death;
}

int Cluster::getDeathToll()
{
    return deathToll;
}

void Cluster::update()
{
    vector<int> mourge, sick;
    infected=0;
    recovered=0;
    death=0;
    for(int i=0;i<member.size();i++)
    {
        member[i].increaseDays();
        if(member[i].getHealth()==INFECTED) { ++infected; sick.push_back(i);}
        else if(member[i].getHealth()==RECOVERED) {++recovered;}
        else if(member[i].getHealth()==DEAD) {++death; mourge.push_back(i);}
        
    }

    for(int i=0;i<sick.size();i++)
    {
        int r=abs(round(randnormal(SPREADRATE,SPREADRATE_TOL)));

        for(int j=0;j<r;j++)
        {
            int temp;
            do
            {
                temp=randint(0,member.size()-1);
            }while(temp==sick[i]);
            member[sick[i]].infect(member[temp]);
        }
    }
    for(int i=0;i<mourge.size();i++) member.erase(member.begin()+mourge[i]-i);
    deathToll+=mourge.size();
}

void Cluster::init(int id)
{
    std::cout<<"Infecting "<<id<<"-th member of the cluster"<<std::endl;
    member[id].infect();
    ++infected;
}

vector<Person> & Cluster::getMember()
{
    return member;
}

Person Cluster::getMemberAt(int id)
{
    return member[id];
}


class Hospital
{
    vector<Person> member;
    vector<Cluster *> location;
    int capacity;

    void release(int id)
    {
        location[id]->getMember().push_back(member[id]);
        member.erase(member.begin()+id);
        location.erase(location.begin()+id);
    }

public:
    Hospital(int cap)
    {
        capacity=cap;
    }

    void addMember(Cluster &c, int id)
    {
        if(member.size()>=capacity) {std::cout<<"Hospital is full!"<<std::endl; return;}
        Person temp=c.getMemberAt(id);
        if(temp.getFate()==DEAD)
        {
            if(100.0*(1-DEATHRATE_HPT/DEATHRATE)*100>=randint(1,10000)) temp.setFate(RECOVERED);
        }

        if(temp.getFate==RECOVERED)
        {
            temp.setRecoveryPeriod(temp.getRecoveryPeriod()-(temp.getRecoveryPeriod()-temp.getDays())*TREATMENT_QUALITY);
        }
        else if(temp.getFate()==RECOVERED)
        {
            temp.setRecoveryPeriod(temp.getRecoveryPeriod()*2);
        }

        member.push_back(temp);
        location.push_back(&c);
        c.getMember().erase(c.getMember().begin()+id);
    }

    void update()
    {
        vector<int> rel;
        for(int i=0;i<member.size();i++)
        {
            member[i].increaseDays();
            if(member[i].getHealth()==DEAD||member[i].getHealth()==RECOVERED) rel.push_back(i); 
        }


        for(int i=0;i<rel.size();i++)
        {
            if(member[rel[i]].getHealth==RECOVERED)
            {
                location[rel[i]]->getMember().push_back(member[rel[i]]);
            }

            location.erase(location.begin()+rel[i]-i);
            member.erase(member.begin()+rel[i]-i);
        }
    }

};
