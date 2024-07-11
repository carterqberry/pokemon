#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Set.h"
#include "HashMap.h"

using namespace std;

#ifdef _MSC_VER // memory leak check
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define VS_MEM_CHECK _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#else
#define VS_MEM_CHECK
#endif

string effectiveness(int v);  //function found below main

int main(int argc, char* argv[]) {
    VS_MEM_CHECK  //enable memory leak check

    // reading inputs / outputs
    if (argc < 3) {
        cerr << "Please provide name of input and output files";
        return 1;
    }
    ifstream in(argv[1]);  //input file
    if (!in) {
        cerr << "Unable to open " << argv[1] << " for input";
        return 2;
    } else {
        cout << argv[1] << " opened successfully.\n";
    }
    ofstream out(argv[2]);  //output file
    if (!out) {
        in.close();
        cerr << "Unable to open " << argv[2] << " for output";
        return 3;
    } else {
        cout << argv[2] << " opened successfully.\n";
    }

    HashMap<string, string> pokemons;
    HashMap<string, string> moves;
    HashMap<string, Set<string>> effectives;
    HashMap<string, Set<string>> ineffectives;

    string line;
    while(getline(in, line)){
        string command;
        try{
            if(line.size() == 0) {  //if line is empty skip
                continue;
            }
            istringstream iss(line);
            iss >> command;

            //input info commands:
            if(command == "Set:"){
                out << line << endl;
                Set<string> set;
                string value;
                while(iss >> value){
                    set.insert(value);
                }
                out << "  [" << set << "]" << endl;
            }
            else if(command == "Pokemon:"){  //insert type of the given pokemon
                out << line;
                string name, type;
                iss >> name >> type;
                pokemons[name] = type;
            }
            else if(command == "Move:"){  //insert the given move type
                out << line;
                string move, type;
                iss >> move >> type;
                moves[move] = type;
            }
            else if(command == "Effective:"){  //insert what type is effective against the given type
                out << line;
                Set<string> types;
                string type, value;
                iss >> type;
                while(iss >> value){
                    types.insert(value);
                }
                effectives[type] = types;
            }
            else if(command == "Ineffective:"){  //insert what type is ineffective against the given type
                out << line;
                Set<string> types;
                string type, value;
                iss >> type;
                while(iss >> value){
                    types.insert(value);
                }
                ineffectives[type] = types;
            }
            else if(command == "Battle:"){
                out << endl << line << endl;
                string pokemonA, pokemonB, moveA, moveB;
                iss >> pokemonA >> moveA >> pokemonB >> moveB;

                //determines the effectivity of the pokemons moves against each other:
                int damageAtoB = effectives[moves[moveA]].count(pokemons[pokemonB]) - ineffectives[moves[moveA]].count(pokemons[pokemonB]);
                int damageBtoA = effectives[moves[moveB]].count(pokemons[pokemonA]) - ineffectives[moves[moveB]].count(pokemons[pokemonA]);

                // output pokemon move and type
                out << "  " << pokemonA << " (" << moveA << ")" << " vs " << pokemonB << " (" << moveB << ")" << endl;
                out << "  " << pokemonA << " is a ";
                out << pokemons[pokemonA] << " type Pokemon." << endl;
                out << "  " << pokemonB << " is a ";
                out << pokemons[pokemonB] << " type Pokemon." << endl;

                //output the moves types
                out << "  " << moveA << " is a ";
                out << moves[moveA] << " type move." << endl;
                out << "  " << moveB << " is a ";
                out << moves[moveB] << " type move." << endl;

                //output the types that pokemon A move are effective against
                out << "  " << moveA << " is super effective against [";
                out << effectives[moves[moveA]] << "] type Pokemon." << endl;
                out << "  " << moveA << " is ineffective against [";
                out << ineffectives[moves[moveA]] << "] type Pokemon." << endl;

                // output the move effectiveness of pokemon A against pokemon B
                out << "  " << pokemonA << "'s " << moveA << " is ";
                out << effectiveness(damageAtoB) << " against " << pokemonB << endl;

                // output the types that Pokemon B move are effective against
                out << "  " << moveB << " is super effective against [";
                out << effectives[moves[moveB]] << "] type Pokemon." << endl;
                out << "  " << moveB << " is ineffective against [";
                out << ineffectives[moves[moveB]] << "] type Pokemon." << endl;

                // output the move effectiveness of Pokemon B  against Pokemon A
                out << "  " << pokemonB << "'s " << moveB << " is ";
                out << effectiveness(damageBtoA) << " against " << pokemonA << endl;

                // outputs the result of the battle:
                if((damageAtoB - damageBtoA) == 0){ // if battle is a tie
                    out << "  The battle between " << pokemonA;
                    out << " and " << pokemonB << " is a tie." << endl;
                }
                else if((damageAtoB - damageBtoA) > 0){ // if pokemon A wins
                    out << "  In the battle between " << pokemonA << " and ";
                    out << pokemonB << ", " << pokemonA << " wins!" << endl;
                }
                else if ((damageAtoB - damageBtoA) < 0){ // if pokemon B wins
                    out << "  In the battle between " << pokemonA << " and ";
                    out << pokemonB << ", " << pokemonB << " wins!" << endl;
                }
            }
            //output info commands
            else if(command == "Pokemon"){  //output all the pokemon and their types
                out << endl << line << pokemons;
            }
            else if(command == "Moves"){  //output all the moveds and their types
                out << endl << line << moves;
            }
            else if(command == "Effectivities") {  //output the types and what theyre effective against
                out << endl << line << effectives;
            }
            else if(command == "Ineffectivities"){  //output the types and what theyre ineffective against
                out << endl << line << ineffectives;
            }
            out << endl;
        }
        catch(string& error){
            out << error;
        }

    }
    return 0;
}

string effectiveness(int v){
    if(v == -1){
        return "ineffective";
    }
    else if(v == 0){
        return "effective";
    }
    else if(v == 1){
        return "super effective";
    }
    else{
        return "unknown effective";
    }
}
