//
// Created by Sullecram Pleasant on 10/10/18.
//

#ifndef KEYSIGHT1_VENDING_H
#define KEYSIGHT1_VENDING_H

#include <iostream>
#include <string>
#include <list>

class Product{
public:
    //Constructors
    Product () {}
    Product (double p, int i, int q) : price(p), ID(i), quantity(q) {}

    //Methods
    void setQuan(int q){quantity = q;}
    void setPrice(double p){price = p;}
    //void setName(char* n){name = n;}
    void setID(int i){ID = i;}
    double getPrice(){return price;}
    //char* getName(){return name;}
    int getID(){return ID;}
    int getQuan(){return quantity;}

    char name[50];

private:
    //Variable components
    double price;
    int ID;
    int quantity = 0;
};

class TranNode{
public:
    TranNode () {}
    TranNode (int f, int i) : funds(f), ID(i) {}

    //Methods
    void setFunds(int f){funds = f;}
    void setID(int i){ID = i;}
    int getFunds(){return funds;}
    char* getName(){return name;}
    int getID(){return ID;}

    char name[50];
private:
    //Variable components
    int funds;
    int ID;
};

class Result{
public:
    Result () {}
    Result (bool d, int c) : delivered(d), change(c) {}

    void setDelivered(bool d){delivered = d;}
    void setChange(int c){change = c;}
    bool getDelivered(){return delivered;}
    int getChange(){return change;}

private:
    bool delivered = false;
    int change;
};


#endif //KEYSIGHT1_VENDING_H
