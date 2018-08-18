
/* MCHN START */

/* MCHN END */
//#include <boost/filesystem/fstream.hpp>
//g++ -DHAVE_CONFIG_H -I${S} -I${S}../src/config  -I${S} -I${S}/obj -pthread -I${S}/usr/include -I${S}/leveldb/include -I${S}/leveldb/helpers/memenv   -I${S}secp256k1/include  -DBOOST_SPIRIT_THREADSAFE -DHAVE_BUILD_INFO -D__STDC_FORMAT_MACROS  -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=2  -g -O2 -Wall -Wextra -Wformat -Wformat-security -Wno-unused-parameter  -Wstack-protector -fstack-protector-all -fPIE -fvisibility=hidden -MT ${S}multichain/multichain_cli-multichain-cli.o -MD -MP -MF ${S}multichain/.deps/multichain_cli-multichain-cli.Tpo -std=c++11 -c -o multichain_climod.o  'reader.cpp'
// g++ -g -O2 -Wall -Wextra -Wformat -Wformat-security -Wno-unused-parameter -Wstack-protector -fstack-protector-all -fPIE -fvisibility=hidden -Wl,--exclude-libs -Wl,ALL -pthread -Wl,-z -Wl,relro -Wl,-z -Wl,now -pie -o multichain-cli_mod multichain_climod.o  ${S}libbitcoin_cli.a ${S}libbitcoin_util.a ${S}multichain/libbitcoin_multichain.a ${S}crypto/libbitcoin_crypto.a ${S}leveldb/libleveldb.a -L${S}usr/lib/x86_64-linux-gnu -lboost_system -lboost_filesystem -lboost_program_options -lboost_thread -lboost_chrono -lssl -lcrypto -lanl -pthread
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <stdlib.h>
#include <ios>
#include <map>
#include <iostream>
#include <array>
#include <algorithm>
using namespace std;
//input format: chain name, log file, chain number

string currentPath = "/home/esben/Documents/simulator";
string multichainPath = "/home/esben/Documents/Github/multichain/multichain/src";

int updateMap(const string streamName, std::map<std::string, int> &activityMap, const string chainNum)
{ 
    string newItem;
    int RPCPort = 100 * stoi(chainNum) + 4011;
    // if (chainName == "log1" || chainName == "log2" || chainName == "log3" || chainName == "log4")
    // {
    //     RPCPort += 4000 + RPCPort;
    // }
    // else if (chainName == "activity")
    // {
    //     RPCPort += 4011;
    // }
    // else if (chainName == "resource")
    // {
    //     RPCPort += 4021;
    // }
    // else
    // {
    //     cout << "ERROR, INVALID CHAIN\n\n";
    //     return 1;
    // }
    ostringstream stream;
    //liststreamitems stream1 false 9999
    cout << "STARTING UPDATE MAP\n";
    //activityMap.clear(); TODO REBUILD MAP EACH UPDATE
    int count = 0;
    // vector<string> strParams;
    // strParams.push_back("stream1");
    // //strParams.push_back("false");
    // //strParams.push_back("99999");
    // for (unsigned int a = 0; a < strParams.size(); a++)
    // {
    //     cout << "Param: " << strParams[a] << endl;
    // }
    // Array params = ProcessArgs(strParams);

    //IMPORTED CODE

    //END IMPORTED CODE

    string connectCommand = "\"" + multichainPath + "/multichain-cli\" aux" +
                            " -datadir=\"$PWD/node" + chainNum + "\" -rpcport=\"" +
                            to_string(RPCPort) + "\" subscribe " + streamName;
    std::system(connectCommand.c_str()); //ensure that node is subscribed to stream

    string command = "\"" + multichainPath + "/multichain-cli\" " +
                     "aux" + " -datadir=\"$PWD/node" + chainNum +
                     "\" -rpcport=\"" + to_string(RPCPort) + "\" liststreamitems " + streamName + " > dat.txt";

    cout << "REPLY==========================\n\n\n"
         << command << endl;
    std::system(command.c_str()); 
    ifstream datFile;
    datFile.open("dat.txt");
    string output;
    bool foundThing = false;
    if (datFile.is_open())
    {
        while (!datFile.eof())
        {
            output.erase(std::remove(output.begin(), output.end(), '\n'), output.end());
            output.erase(std::remove(output.begin(), output.end(), '\t'), output.end());
            output.erase(std::remove(output.begin(), output.end(), ' '), output.end());
            output.erase(std::remove(output.begin(), output.end(), ','), output.end());

            datFile >> output;
            cout << output << endl;
            if (!foundThing)
            {
                if (output.substr(1, 4) == "keys" || output.substr(1, 3) == "key")
                {
                    cout << "FOUND " << output << "\n\n";
                    foundThing = true;
                    if (output.find(':') != std::string::npos)
                    {
                        vector<string> tokens;
                        stringstream check1(output);
                        string intermediate;
                        while (getline(check1, intermediate, ':'))
                        {
                            cout << intermediate << endl;
                            tokens.push_back(intermediate);
                        }
                        tokens[1].erase(std::remove(tokens[1].begin(), tokens[1].end(), '['), tokens[1].end());
                        tokens[1].erase(std::remove(tokens[1].begin(), tokens[1].end(), '\"'), tokens[1].end());
                        if (tokens[1] != "")
                        {
                            newItem = tokens[1];
                            if (newItem.at(newItem.length() - 1) == '\"')
                                newItem = newItem.substr(0, newItem.length() - 1);
                            else
                                cout << "NEWITEM END: " << newItem.at(newItem.length() - 1) << endl;
                            cout << "OUTPUT IS: " << newItem << endl;
                            if (activityMap.find(newItem) == activityMap.end())
                            {
                                //add item to map
                                cout << "ADDING: " << newItem << " | " << activityMap.size() << endl;
                                activityMap[newItem] = activityMap.size();
                            }
                            foundThing = false;
                        }
                    }
                    //check if key is on same line as the word "key"
                }
            }

            else if (output.substr(0, 1) == "\"" && foundThing)
            {
                newItem = output; //.substr(1, output.size() - 2); //remove quotes
                newItem.erase(std::remove(newItem.begin(), newItem.end(), '\"'), newItem.end());
                newItem.erase(std::remove(newItem.begin(), newItem.end(), ','), newItem.end());

                cout << "OUTPUT IS: " << newItem << endl;
                if (activityMap.find(newItem) == activityMap.end())
                {
                    //add item to map
                    cout << "ADDING: " << newItem << " | " << activityMap.size() << endl;
                    activityMap[newItem] = activityMap.size();
                }
                foundThing = false;
            }
        }
    }
    cout << "DONE WITH UPDATING=======================================\n=======================================\n=======================================\n=======================================\n";
    datFile.close();

    // size_t found = strPrint.find("keys"); // Search output for first "keys"

    // while (found != std::string::npos)
    // {
    //     cout << "NPOS: " << std::string::npos << endl
    //          << endl;
    //     string strTrunc = strPrint.substr(found); //find substring
    //     strTrunc = strTrunc.substr(7, strTrunc.find(']', 0));
    //     cout << "KEY:       ||" << strTrunc << "||  --  ||" << found << endl;
    //     activityMap.insert(std::make_pair("strTrunc", count));
    //     found = strPrint.find("keys", found + 1, 3); // find next instance of "keys"
    //     count++;
    // }
    return 0;
}
int getStringNum(const string streamName, const string activity, std::map<std::string, int> &activityMap, const string chainNum)
{
    cout << "ACTIVITY MAP SIZE: " << activityMap.size() << endl;

    for (map<string, int>::iterator it = activityMap.begin(); it != activityMap.end(); it++)
    {
        cout << it->first << "|" << it->second << endl;
    }

    int RPCPort = 100 * stoi(chainNum) + 4011;
    // if (chainName == "log")
    // {
    //     RPCPort += 4000 + stoi(chainNum);
    // }
    // else if (chainName == "activity")
    // {
    //     RPCPort += 4011;
    // }
    // else if (chainName == "resource")
    // {
    //     RPCPort += 4021;
    // }
    // else
    // {
    //     cout << "ERROR, INVALID CHAIN\n\n";
    //     return -1;
    // }
    int activityNum;
    if (activityMap.find(activity) == activityMap.end())
    { //did not find activity
        int updatedMap = updateMap(streamName, activityMap, chainNum);
        if (activityMap.find(activity) != activityMap.end())
        { //check to see if map has been updated
            int activityNum = activityMap[activity];
            return activityNum;
        }
        std::cout << "activity " + activity << " not found" << std::endl;
        //if no activity string is found

        string command = "\"" + multichainPath + "/multichain-cli\" aux -datadir=\"$PWD/node" +
                         chainNum + "\" -rpcport=\"" + to_string(RPCPort) + "\" publish " + streamName +
                         " " + activity + " 00";

        cout << "REPLY==========================\n\n"
             << command << endl;
        std::system(command.c_str());
        updatedMap = updateMap(streamName, activityMap, chainNum);
        if (updatedMap == 0)
        {
            if (activityMap.find(activity) != activityMap.end())
            {
                activityNum = activityMap[activity];
                cout << "ACTIVITY " << activity << " HAS NUMBER: " << activityNum << endl;
                return activityNum;
            }
        }

        activityNum = getStringNum(streamName, activity, activityMap, chainNum);
    }
    else
    { //found activity
        int activityNum = activityMap[activity];
    }

    return activityNum;
}
int REFIDnumToBin(int auxNum)
{
    return 10 + int(pow(8, auxNum));
}

int publishAuxBlock(const int type, const int val, const string data, const string chainNum)
{ // Publish auxilirary block for searching
    int RPCPort;
    int NETPort;
    int auxNum = 0;

    string auxChainName;
    switch (type)
    {
    case 0:
        //id
        auxChainName = "log" + chainNum + "id";
        break;
    case 1:
        //REFid
        while (val > REFIDnumToBin(auxNum))
        {
            auxNum++;
        }
        RPCPort = 8000 + 100 * stoi(chainNum) + auxNum;
        NETPort = 9000 + 100 * stoi(chainNum) + auxNum;
        auxChainName = "log" + chainNum + "REFID" + to_string(auxNum);
        break;
    case 2:
        //user
        RPCPort = 10000 + 100 * stoi(chainNum) + val;
        NETPort = 11000 + 100 * stoi(chainNum) + val;
        auxChainName = "log" + chainNum + "user" + to_string(val);
        break;
    case 3:
        //activity
        RPCPort = 12000 + 100 * stoi(chainNum) + val;
        NETPort = 13000 + 100 * stoi(chainNum) + val;
        auxChainName = "log" + chainNum + "activity" + to_string(val);
        break;
    case 4:
        //resource
        auxChainName = "log" + chainNum + "resource" + to_string(val);
        RPCPort = 6000 + 100 * stoi(chainNum) + val;
        NETPort = 7000 + 100 * stoi(chainNum) + val;
        break;
    default:
        cerr << "ERROR: UNKOWN AUX BLOCK\n";
        break;
    }
    cout << "AUX CHAIN " << auxChainName << endl;
    string makeChain = "\"" + currentPath + "/makeChain.sh\" " + auxChainName +
                       " node" + chainNum +
                       " " + to_string(RPCPort) +
                       " " + to_string(NETPort) +
                       " y";
    cout << "MAKECHAIN\n"
         << makeChain << endl;
    std::system(makeChain.c_str());

    //make sure that the node is subscribed to the right chain
    string subscribe = "\"" + multichainPath + "/multichain-cli\" " + auxChainName + " -datadir=\"$PWD/node" +
                       chainNum + "\" -rpcport=\"" + to_string(RPCPort) + "\" subscribe stream1";
    std::system(subscribe.c_str());

    string command = "\"" + multichainPath + "/multichain-cli\" " + auxChainName + " -datadir=\"$PWD/node" +
                     chainNum + "\" -rpcport=\"" + to_string(RPCPort) + "\" publish stream1 \"" + "0" + "\" " + data;
    cout << command << endl;
    std::system(command.c_str());
}
int main(int argc, char *argv[])
{
    string confLine;
    ifstream confFile;
    confFile.open("paths.config");
    if (confFile.is_open())
    {
        while (!(confFile.eof()))
        {
            getline(confFile, confLine);
            vector<string> tokens;
            // stringstream class check1
            stringstream check1(confLine);
            string intermediate;
            // Tokenizing w.r.t. space ' '
            while (getline(check1, intermediate, '='))
            {
                tokens.push_back(intermediate);
                cout << intermediate << endl;
            }
            if (tokens[0] == "MULTICHAIN_BIN")
            {
                multichainPath = tokens[1];
            }
            else if (tokens[0] == "SCRIPT_BIN")
            {
                currentPath = tokens[1];
                cout << "done updating\n";
                break;
            }
        }
        cout << "closing\n";
        confFile.close();
        cout << "closed\n";
    }

    std::map<std::string, int> activityMap;
    std::map<std::string, int> resourceMap;
    string current_exec_name = argv[0]; // Name of the current exec program
    vector<std::string> all_args;
    if (argc > 1)
    {
        all_args.assign(argv + 1, argv + argc);
    }
    if (all_args.size() != 2)
    {
        cout << "Invalid input, enter <log file> <node number>\n";
        return 1;
    }
    cout << all_args.size() << " args found\n";
    for (int i = 0; i < all_args.size(); i++)
    {
        cout << "arg: " << all_args[i] << endl;
    }
    //string chainName = all_args[0];
    string logFile = all_args[0];
    string nodeNumber = all_args[1];

    //connect

    string strRPCPort = "4" + nodeNumber + "0" + nodeNumber;
    string datadir = currentPath + "/node" + nodeNumber;

    cout << "Starting write now------------------------------------------------------------\n\n\n";

    ifstream infile(logFile);
    cout << all_args[0] << " found file\n";
    string curr;
    string key;
    string data = "";
    int count = 0;
    int index = 0;
    cout << "Connected?---------------------------------------------------------------------\n";

    while (infile >> curr)
    { //note that while data in multichain is a hexidecimal, it is being stored as integers deliminated by a 'a'
        int REFID;
        int userNum;
        int activityNum;
        int resourceNum;
        cout << curr << endl;
        switch (count)
        {
        case 0: //field 1, timestamp
            data = /*"\"" + */ curr;
            break;
        case 1: //field 2, node
            cout << "node: " + curr;
            data = data + "a" + curr;
            break;
        case 2: //field 3, id
            data = data + "a" + curr;
            break;
        case 3: //field 4, refid
            REFID = stoi(curr);
            cout << " refid: " + curr;
            data = data + "a" + curr;
            break;
        case 4: //field 5, user
            userNum = std::stoi(curr);
            data = data + "a" + curr;
            cout << " user: " << curr;
            break;
        case 5:
            activityNum = getStringNum("activity", curr, activityMap, nodeNumber);
            activityNum = activityMap[curr];
            data = data + "a" + to_string(activityMap[curr]);
            cout << "ACTIVITY: " << curr << " NUM: " << activityNum << endl;
            cout << endl
                 << data << endl;
            break;
        case 6: //field 6, Resource

            resourceNum = getStringNum("resource", curr, resourceMap, nodeNumber);
            resourceNum = resourceMap[curr];
            data = data + "a" + to_string(resourceMap[curr]) /* + "\""*/;
            cout << "RESCOURCE: " << curr << " NUM: " << resourceNum << endl;
            cout << endl
                 << data << endl;
        }
        count++;
        if (count > 6)
        {
            if (data.length() % 2 == 1)
            {
                data = data + "b";
            }
            //int mim = updateMap("activity", activityMap, nodeNumber);

            string publishCommand = "\"" + multichainPath + "/multichain-cli\" log" + nodeNumber + " -datadir=\"$PWD/node" +
                                    nodeNumber + "\" -rpcport=\"" + strRPCPort + "\" publish stream1 \"" + key + "\" " + data;
            cout << "publish command: " << publishCommand << endl;
            system(publishCommand.c_str());
            publishAuxBlock(1, REFID, data, nodeNumber);
            publishAuxBlock(2, userNum, data, nodeNumber);
            //publishAuxBlock(3, activityNum, data, nodeNumber);
            publishAuxBlock(4, resourceNum, data, nodeNumber);

            count = 0;
            index++;
            cout << "index " << index << endl;
            key = "";
            data = "";
        }
    }
    system("rm dat.txt");
    cout << "ACTIVITY MAP SIZE: " << activityMap.size() << endl;

    for (map<string, int>::iterator it = activityMap.begin(); it != activityMap.end(); it++)
    {
        cout << it->first << "|" << it->second << endl;
    }

    cout << "RESOURCE MAP SIZE: " << resourceMap.size() << endl;

    for (map<string, int>::iterator it = resourceMap.begin(); it != resourceMap.end(); it++)
    {
        cout << it->first << "|" << it->second << endl;
    }
}
