#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <iomanip>
#include <string>
//#include <json/value.h>
#include "vending.h"
#include "stdio.h"

char inLocation[100]; //= "/Users/Sullecram/CLionProjects/keysight1/files/test2.json";
char transLocation[100];// = "/Users/Sullecram/CLionProjects/keysight1/files/test3.json";

//Creates output file
void pOutput(std::list<Result> plist){
    long size = plist.size();
    int change = 0;
    std::list<Result>::iterator it1;
    it1=plist.begin();

    FILE * fFile;
    fFile = fopen("output.json","w");

    fprintf(fFile,"[\n");
    for(it1=plist.begin(); it1!=plist.end(); ++it1) {
        fprintf(fFile, "  {\n");
        fprintf(fFile, "    \"product_delivered\": %s,\n", it1->getDelivered() ? "true" : "false");
        change = it1->getChange();
        if (change == 0)
            fprintf(fFile, "    \"change\": []\n");
        else {
            fprintf(fFile, "    \"change\": [\n");
            while (change != 0) {
                if (change >= 100) {
                    fprintf(fFile, "      100");
                    change = change - 100;
                    if (change > 0)
                        fprintf(fFile, ",\n");
                    else
                        fprintf(fFile, "\n");
                } else if (change >= 25) {
                    fprintf(fFile, "      25");
                    change = change - 25;
                    if (change > 0)
                        fprintf(fFile, ",\n");
                    else
                        fprintf(fFile, "\n");
                } else if (change >= 10) {
                    fprintf(fFile, "      10");
                    change = change - 10;
                    if (change > 0)
                        fprintf(fFile, ",\n");
                    else
                        fprintf(fFile, "\n");
                } else {
                    fprintf(fFile, "     5");
                    change = change - 5;
                    if (change > 0)
                        fprintf(fFile, ",\n");
                    else
                        fprintf(fFile, "\n");
                }
            }
            fprintf(fFile, "    ]\n");
        }
            if (size > 1) {
                fprintf(fFile, "  },\n");
                --size;
            } else
                fprintf(fFile, "  }\n");
    }
    fprintf(fFile, "]");
    fclose(fFile);
}

//Executes the transactions
void calcResults(std::list<Product>  productL, std::list<TranNode> transL, std::list<Result> * resultL){
    std::list<Product>::iterator it1;
    std::list<TranNode>::iterator it2;
    std::list<Result>::iterator it3;
    //Iterates through each entry, "decoding" the jsons and comparing them
    for (it2=transL.begin(); it2!=transL.end(); ++it2) { //transactions
        for(it1=productL.begin(); it1!=productL.end(); ++it1){
            int CP = (int)(it1->getPrice() * 100);
            //std::cout << "Diff: " << (it2->getFunds()-CP) << std::endl;
            //std::cout << "Quan: " << (it1->getQuan()) << std::endl;
            if( (strcmp(it2->name,it1->name)==0) && (it1->getQuan()!=0) && ((it2->getFunds()-CP)>=0))
            {
                //std::cout << "um" << std::endl;
                it1->setQuan(it1->getQuan()-1);
                Result * node = new Result();
                node->setDelivered(true);
                node->setChange(it2->getFunds()-CP);
                resultL->push_back(*node);
                break;
            }
            else
            {
                Result * node = new Result();
                node->setDelivered(false);
                node->setChange(it2->getFunds());
                resultL->push_back(*node);
            }

        }
    }

}

void getTrans(std::list<TranNode> * inlist){
    int ID = 1; //Unused idea
    int count = 0;
    double funds = 0.0;
    char hold;
    char name[100];
    //std::cout << "Getting transactions" << std::endl; <- For debugging
    std::ifstream invFile;
    invFile.open(transLocation, std::ios::in);
    if (!invFile) {
        std::cerr << "Can't open input file " << transLocation << std::endl;
        exit(1);
    }
    char output;
    char outputL[100];

    if (invFile.is_open()) {
        while (!invFile.eof()) {
            invFile >> output;
            if(output!='{' && output!='[' && output!=']' && output!=':' && output!=',' && output!='\'' )
            {
                //Search through the JSON while ignoring certain characters to get the important data
                while(output!='{' &&  output!='}' && output!='[' && output!=']' && output!=':' && output!=',' && output!='\'')
                {
                    if(output==' ')
                        continue;
                    outputL[count] = output;
                    count++;
                    invFile >> output;
                }
                if(hold=='n')
                {
                    strcpy(name, outputL);
                }
                //For transactions
                if(hold == 'f') { //all this to get the price up to 2 decimals
                    while(true){
                        if(output==',' || output==']') {
                            switch (count) {
                                case 1:
                                    funds = funds + (outputL[0] - '0');
                                    count = 0;
                                    memset(outputL, 0, 100);
                                    //invFile >> output;
                                    break;
                                case 2:
                                    funds = funds + (outputL[1] - '0') + ((outputL[0] - '0') * 10.00);
                                    count = 0;
                                    memset(outputL, 0, 100);
                                    //invFile >> output;
                                    break;
                                case 3:
                                    funds = funds + (outputL[2] - '0') + ((outputL[1] - '0') * 10.00) +
                                            ((outputL[0] - '0') * 100.00);
                                    count = 0;
                                    memset(outputL, 0, 100);
                                    //invFile >> output;
                                    break;
                            }
                        }
                        if(output==']')
                            break;
                        if(output==','){
                            invFile >> output;
                            continue;
                        }
                        outputL[count] = output;
                        count++;
                        invFile >> output;
                    }
                    //break;
                }
                hold = outputL[1];
                count = 0;
                memset(outputL, 0, 100 );
                //break;
            }
            //Converts the JSON to my transaction node and into a list
            if(output=='}'){
                TranNode * node = new TranNode();
                node->setID(ID);
                node->setFunds(funds);
                strcpy(node->name,name);
                inlist->push_back(*node);
                memset(name, 0, 100);
                ID = 0;
                funds = 0.0;
            }
            //std::cout<<output<<std::endl;
        }
    }
    invFile.close();
}

void receiveInv(std::list<Product> * inlist , Product* node){
    int ID = 1; //Unused idea
    int count = 0;
    int quant = 0;
    double price = 0.0;
    char hold; //holds a special char to denote what I'm reading in
    char food[100];
    std::ifstream invFile;
    invFile.open(inLocation, std::ios::in);
    if (!invFile) {
        std::cerr << "Can't open input file " << inLocation << std::endl;
        exit(1);
    }
    bool again = true; //just to get the name
    char output;
    char outputL[100];

    if (invFile.is_open()) {
        while (!invFile.eof()) {
            invFile >> output;
            if(output!='{' && output!='}' && output!='[' && output!=']' && output!=':' && output!=',' && output!='\'' )
            {
                //Search through and get the important data from inventory.json
                while(output!='{' && output!='}' && output!='[' && output!=']' && output!=':' && output!=',' && output!='\'')
                {
                    outputL[count] = output;
                    count++;
                    invFile >> output;
                }
                if(output==':' && again==true)
                {
                    strcpy(food, outputL);
                    again = false;
                }
                //check to see what it is
                if(hold == 'q')
                    quant = quant + (outputL[0]-'0');
                if(hold == 'p') { //all this to get the price up to 2 decimals
                    switch (count) {
                        case 1:
                            price = (outputL[0] - '0');
                            break;
                        case 3:
                            price = (outputL[0] - '0') + ((outputL[2] - '0') / 10.0);
                            break;
                        case 4:
                            price = (outputL[0] - '0') + ((outputL[2] - '0') / 10.0) + ((outputL[3] - '0') / 100.00);
                            break;
                    }
                    break;
                }
                hold = outputL[1];
                count = 0;
                memset(outputL, 0, 100 );
                //break;
            }
        }
    }
    invFile.close();
    //insert information
    strcpy(node->name, food);
    node->setPrice(price);
    node->setID(ID);
    node->setQuan(quant);
    inlist->push_front(*node);
}
int main() {
    std::cin >> inLocation;
    std::cin >> transLocation;

    Product * proNode = new Product();
    std::list<Product> prodList;
    std::list<TranNode> tranlist;
    std::list<Result> resultList;
    //Read in inventory
    receiveInv(&prodList, proNode);
    //Read in transactions
    getTrans(&tranlist);
    //Compare the lists
    calcResults(prodList, tranlist, &resultList);
    //Produce output.json
    pOutput(resultList);

    return 0;
}