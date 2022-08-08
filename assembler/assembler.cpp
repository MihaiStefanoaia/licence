#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <string>
#include <regex>
#include <list>
#include <vector>
#include <unordered_map>
#include <set>

int main(int argc, char** argv){
    bool finishExecution = false;
    std::regex validInput(".*\\.hra");
    std::regex validOutput(".*\\.bin");
    std::string inputFilename("none");
    std::string outputFilename("a.bin");
    std::list<std::string> errors;
    
    //handling cli input
    for(int i = 0; i < argc; i++){
        if(std::regex_match(argv[i], std::regex("--([a-z0-9]+-)*[a-z0-9]*"))){   //verbose
            if(strstr(argv[i],"--output")){

                if(std::regex_match(argv[i+1],validOutput)){
                    outputFilename = argv[i+1];
                    i++;
                } else {
                    std::string err("fatal error: ");
                    err += "invalid filename: ";
                    err += argv[i];
                    err += ": filename must end in \".bin\"";
                    errors.push_back(err);
                }
            } else {
                std::string err("error: ");
                err += "invalid argument: '";
                err += argv[i];
                err += "': ignored";
                errors.push_back(err);
            }
        } else if(std::regex_match(argv[i], std::regex("-[a-z]*"))){            //flags
            bool outputSet = false;
            for(int j = 1; j < strlen(argv[i]); j++){
                if(argv[i][j] == 'o'){

                    if(std::regex_match(argv[i+1],validOutput)){
                        outputFilename = argv[i+1];
                        outputSet = true;
                    } else {
                        std::string err("fatal error: ");
                        err += "invalid filename: ";
                        err += argv[i];
                        err += ": filename must end in \".bin\"";
                        errors.push_back(err);
                    }
                } else {
                    std::string err("error: ");
                    err += "invalid flag: '";
                    err += argv[i][j];
                    err += "': ignored";
                    errors.push_back(err);
                }
            }
            if(outputSet)
                i++;
        } else {                                                                //input filename
            inputFilename = argv[i];
        }
    }

    if(!std::regex_match(inputFilename, validInput)){
        finishExecution = true;
        std::string err("fatal error: ");
        err += "no valid input filename found: ";
        err += inputFilename;
        errors.push_back(err);
    }

    for(auto err : errors){
        std::cout<<err<<'\n';
    }

    if(finishExecution)
        return 0;

    errors.clear();

    //arguments are good, start assembly
    std::string codeLine;

    std::ifstream inputFile(inputFilename);
    std::ofstream outputFile(outputFilename);
    std::stringstream outputBuffer;

    std::unordered_map<short int, std::string> unfilled_jumps;
    std::unordered_map<std::string, short int> labels;
    std::set<std::string> labelKeys;

    //tokens
    std::vector<std::string> opcodes        {"nop","mov","spc","jfg","inc","dec","not","rot","and","or","adc","sbb","mul","div","mov16","add16"};
    std::vector<std::string> registers8     {"rga","rgb","rgc","rgd","rmh","rml","rsh","rsl","rbh","rbl","rph","rpl"};
    std::vector<std::string> registers16    {"rmx","rsx","rbx","rcx"};
    std::vector<std::string> memoryAccess   {"@rmx","@rsx","@rbx","@rcx"};
    
    short int location = 0;
    int line = 0;


    while(std::getline(inputFile, codeLine,'\n') && !finishExecution){
        std::cout<< codeLine<<'\n';

        unsigned char instructionVector[3] = {0};
        int instructionSize;
        std::vector<std::string> components;
        std::stringstream tmp(codeLine);
        std::string str;

        while(std::getline(tmp,str,' ')){
            components.push_back(str);
        }

        if(std::regex_match(components[0],std::regex("_.*"))){  //if it is label
            if(labelKeys.find(components[0]) != labelKeys.end()){
                labels[components[0]] = location;
                labelKeys.insert(components[0]);
            } else {
                std::string err("error: ");
                err += "label \"";
                err += components[0];
                err += "\" is used multiple times";
                errors.push_back(err);
            }
        } else {                                                //opcode
            for(int op = 0; op < opcodes.size(); op++){
                if(components[0] == opcodes[op]){   //nop
                    if(op == 0){
                        instructionSize = 1;
                        instructionVector[0] = 0;

                    } else if(op == 1 || (op >= 8 && op <= 11)){ //mov,jfg,and,or,adc,sbb
                        if(components.size() != 3){
                            std::string err("error: inappropriate amount of arguments for");
                            err += opcodes[op];
                            err += "instruction at line ";
                            err += line;
                            errors.push_back(err);
                            finishExecution = true;
                        }


                        int memoryAccesses = 0;
                        instructionSize = 2;
                        instructionVector[0] |= op;

                        bool found = false; //evaluate left operand

                        for(int i = 0; i < registers8.size(); i++){
                            if(components[1] == registers8[i]){
                                instructionVector[0] |= 1 << 4;
                                instructionVector[1] |= i+4;
                                found = true;
                            }
                        }
                        if(!found){
                            for(int i = 0; i < memoryAccess.size(); i++){
                                if(components[1] == memoryAccess[i]){
                                    instructionVector[0] |= 2 << 4;
                                    instructionVector[1] |= i;
                                    memoryAccesses++;
                                    found = true;
                                }
                            }
                        }
                        if(!found){
                            std::string err("error: invalid left operand at line ");
                            err += line;
                            errors.push_back(err);
                            finishExecution = true;
                        }

                        found = false; //evaluate right operand

                        for(int i = 0; i < registers8.size(); i++){
                            if(components[2] == registers8[i]){
                                instructionVector[0] |= 1 << 6;
                                instructionVector[1] |= i+4 << 4;
                                found = true;
                            }
                        }
                        if(!found){
                            for(int i = 0; i < memoryAccess.size(); i++){
                                if(components[2] == memoryAccess[i]){
                                    instructionVector[0] |= 2 << 6;
                                    instructionVector[1] |= i << 4;
                                    memoryAccesses++;
                                    found = true;
                                }
                            }
                        }
                        if(!found){
                            if(std::regex_match(components[2],std::regex("(-?[1-9][0-9]*)|0"))){ //check for immediate
                                int imm = atoi(components[2].c_str());
                                std::cout<<imm<<'\n';
                                if(imm > 255 | imm < -128){
                                    std::string err("warning: immediate is out of bounds (");
                                    err += imm;
                                    err += " is not in [0,255] or [-128,127]) at line "; 
                                    err += line;
                                    errors.push_back(err);
                                }
                                instructionSize = 3;
                                instructionVector[0] |= 3 << 6;
                                instructionVector[2]  = imm % 256;
                                found = true;
                            }
                        }
                        if(!found){
                            std::string err("error: invalid right operand at line ");
                            err += line;
                            errors.push_back(err);
                            finishExecution = true;
                        }
                        if(memoryAccesses == 2){
                            std::string err("error: too many memory accesses at line ");
                            err += line;
                            errors.push_back(err);
                            finishExecution = true;
                        }
                    } else if (op >= 4 || op <= 6)  //inc, dec, not
                    {
                        if(components.size() != 3){
                            std::string err("error: inappropriate amount of arguments for");
                            err += opcodes[op];
                            err += "instruction at line ";
                            err += line;
                            errors.push_back(err);
                            finishExecution = true;
                        }


                        int memoryAccesses = 0;
                        instructionSize = 2;
                        instructionVector[0] |= op;

                        bool found = false; //evaluate left operand

                        for(int i = 0; i < registers8.size(); i++){
                            if(components[1] == registers8[i]){
                                instructionVector[0] |= 1 << 4;
                                instructionVector[1] |= i+4;
                                found = true;
                            }
                        }
                        if(!found){
                            for(int i = 0; i < memoryAccess.size(); i++){
                                if(components[1] == memoryAccess[i]){
                                    instructionVector[0] |= 2 << 4;
                                    instructionVector[1] |= i;
                                    memoryAccesses++;
                                    found = true;
                                }
                            }
                        }
                        if(!found){
                            std::string err("error: invalid left operand at line ");
                            err += line;
                            errors.push_back(err);
                            finishExecution = true;
                        }
                    }
                    
                }
            }
        }
        if(!finishExecution){
            outputBuffer.write(reinterpret_cast<const char*>(instructionVector),instructionSize);
            location += instructionSize;
        }
        line++;
    }
    std::string output = outputBuffer.str();
    outputFile << output;
    outputFile.close();

    for(auto err : errors){
        std::cout<<err<<'\n';
    }

    if(finishExecution)
        return 0;

    return 0;
}