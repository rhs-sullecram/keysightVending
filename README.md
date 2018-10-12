# keysightVending
Vending machine for key sight

The following code simulates a vending machine. Users input a inventory and transactions list in json format and the program will return an output.json with the results.

Asumptions:

-The JSON sample format(name, funds) etc will be the same for each subsequent JSON.
-Files are loaded in the exact same order.
-File locations are absolute and relative.
-File locations won't succeed 100 characters.
-Numbers in their respective file will be consistent.
-I'm not allowed to use third party JSON readers for C++ so I had to decode it myself.
-Although the examples have Doritos, the vending machine could have multiple other types of food.
-Nothing above $1, i.e. $5 and up will be entered.
