// Input: data directory, <param>=<value>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
//#include <cstdlib>
//#include <stdlib.h>
#include <ios>
#include <map>
#include <iostream>
#include <array>
#include <algorithm>

using namespace std;

string currentPath = "/home/esben/Documents/simulator";
string multichainPath = "/home/esben/Documents/Github/multichain/multichain/src";

int sortNum = -1;
string findChain(const string chainName, const int RPCPort, const string datadir, const string nodeNum)
{

    int auxRPCPort = 4011 + 100 * stoi(nodeNum);
    string connectCommand = "\"" + multichainPath + "/multichain-cli\" aux" +
                            " -datadir=\"" + datadir + "\" -rpcport=\"" +
                            to_string(auxRPCPort) + "\" subscribe stream1";
    system(connectCommand.c_str()); //ensure that node is subscribed to stream
    string command = "\"" + multichainPath + "/multichain-cli\" aux" +
                     " -datadir=\"" + datadir +
                     "\" -rpcport=\"" + to_string(auxRPCPort) + "\" liststreamitems stream1 false 9999999" + " | grep " + chainName + " > dat.txt";
    system(command.c_str()); //find if chain exists

    string curr;
    string chain = "Not Found";
    ifstream datFile;
    datFile.open("dat.txt");
    if (datFile.is_open())
    {
        while (!(datFile.eof()))
        {
            getline(datFile, curr);
            cout << curr << endl;
            if (curr != "")
            {
                curr.erase(std::remove(curr.begin(), curr.end(), '\t'), curr.end());
                curr.erase(std::remove(curr.begin(), curr.end(), ' '), curr.end());
                curr.erase(std::remove(curr.begin(), curr.end(), '\"'), curr.end());
                curr.erase(std::remove(curr.begin(), curr.end(), ','), curr.end());
                chain = curr;
                cout << "CHAIN: " << chainName << " found\n";
                break;
            }
        }
        datFile.close();
    }

    if (chain.find(':') != std::string::npos)
    {
        vector<string> tokens;
        stringstream check1(chain);
        string intermediate;
        while (getline(check1, intermediate, ':'))
        {
            tokens.push_back(intermediate);
        }
        chain = tokens[1];
        for(int i = 2; i < tokens.size(); i++){
            chain = chain + ":" + tokens[i];
        }
    }
    return chain;
}

int connectToChain(const string chainName, const string nodeNum, const int RPCPort, const int NETPort, const int COMPort, const string datadir)
{ //returns 0 if sucesful, 1 if not
    int auxRPCPort = 4011 + 100 * stoi(nodeNum);
    string connectCommand = currentPath + "/connectToChain.sh " + " " + datadir + " " + chainName + " " + to_string(RPCPort) + " " + to_string(NETPort) + " " + to_string(COMPort) + " " + "10";
    cout << connectCommand << endl;
    system(connectCommand.c_str());

    string checkCommand = multichainPath + "/multichain-cli aux -datadir=" + datadir + " -rpcport=" +
                          to_string(auxRPCPort) + " getinfo> dat.txt";
    cout << checkCommand << endl;
    system(checkCommand.c_str()); // check if connected

    string output;
    bool foundThing = false;
    ifstream datFile;
    datFile.open("dat.txt");
    if (datFile.is_open())
    {
        while (!datFile.eof())
        {
            datFile >> output;
            output.erase(std::remove(output.begin(), output.end(), '\t'), output.end());
            output.erase(std::remove(output.begin(), output.end(), ' '), output.end());
            if (output != "")
            { //if there is anything in dat.txt, then the chain has been connected to sucessfully
                datFile.close();
                cout << "CONNECTION SUCESS\n";
                return 0;
            }
        }
        datFile.close();
    }
    cout << "CONNECTION FAIL\n";

    return 1;
}
int readChain(const string chainName, const int RPCPort, const string datadir, const int numberOfBlocks)
{ //TODO randomize head of file to prevent searching old data
    //reads chain into temporary file
    string subscribeCommand = multichainPath + "/multichain-cli " + chainName + " -datadir=" + datadir +
                              " -rpcport=" + to_string(RPCPort) + " subscribe stream1";
    system(subscribeCommand.c_str());

    string listCommand = multichainPath + "/multichain-cli " + chainName + " -datadir=" + datadir +
                         " -rpcport=" + to_string(RPCPort) + " liststreamitems stream1 false " + to_string(numberOfBlocks) +
                         "  | grep \"data\" | cut -f2  -d':' | tr -d ',' | tr -d '\"' | tr -d 'b'> dat.txt";
    system(listCommand.c_str());
    cout << listCommand << endl;
    return 0;
}

bool matchParams(const vector<int> existingParams, const vector<string> searchParams, const vector<string> tokens)
{
    for (int i = 0; i < existingParams.size(); i++)
    {
        if (searchParams[existingParams[i]] != tokens[existingParams[i]])
        {
            cout << "mismatch: ";
            cout << searchParams[existingParams[i]] << "  " << tokens[existingParams[i]] << endl;
            return false;
        }
    }
    return true;
}

int updateMap(const string streamName, std::map<std::string, int> &activityMap, const string chainNum)
{ //TODO finish
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
    std::system(command.c_str()); //TODO
    ifstream datFile;
    datFile.open("dat.txt");
    string output;
    bool foundThing = false;
    if (datFile.is_open())
    {
        while (!datFile.eof())
        {
            datFile >> output;
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

vector<string> longSearch(const vector<string> searchParams)
{ //search dat.txt for matching entries line by line
    ifstream datFile;
    datFile.open("dat.txt");
    string curr;
    vector<string> result;
    vector<int> existingParams;
    for (int i = 0; i < searchParams.size(); i++)
    { //find existing params
        if (searchParams[i] != "")
        {
            existingParams.push_back(i);
        }
    }

    if (datFile.is_open())
    {
        while (!(datFile.eof()))
        {
            getline(datFile, curr);
            cout << "line curr " << endl;
            curr.erase(std::remove(curr.begin(), curr.end(), '\t'), curr.end());
            curr.erase(std::remove(curr.begin(), curr.end(), '\"'), curr.end());
            if (curr == "")
                break;
            vector<string> tokens;
            stringstream check1(curr);
            string intermediate;
            while (getline(check1, intermediate, 'a'))
            {
                if (intermediate == "")
                    continue;
                cout << intermediate << endl;
                tokens.push_back(intermediate);
            }
            if (matchParams(existingParams, searchParams, tokens))
            {
                cout << "Found thing: " << curr << endl;
                result.push_back(curr);
                //logs = logs + curr + "\n";
            }
        }
        datFile.close();
    }
    return result;
}
int interpolateChangePerLog(const long val1, const long val2, const long distance, const long target)
{
    cout << "starting interpolate\n";
    cout << val1 << "    " << val2 << "      " << distance << endl;
    double changePerLog = abs((double)(val1 - val2) / (double)distance);
    cout << "change per log " << changePerLog << endl;
    return (int)((val2 - target) / changePerLog);
}

vector<string> searchNodeID(const long targetID, const int estNum, const string chainName, const int RPCPort, const string datadir)
{ //recursively search chain for specific ID
    cout << "searching with " << estNum << "blocks\n";
    readChain(chainName, RPCPort, datadir, estNum);
    int count = 0;
    vector<string> result;
    string curr;
    ifstream datFile;
    string currID;
    string firstID = "";
    datFile.open("dat.txt");
    if (datFile.is_open())
    {
        getline(datFile, curr);
        stringstream check1(curr);
        string intermediate;
        while (getline(check1, intermediate, 'a'))
        {
            if (intermediate == "")
                continue;
            getline(check1, intermediate, 'a');
            getline(check1, intermediate, 'a');
            currID = intermediate;
            cout << "currID" << currID << endl;
            break;
        }
        firstID = currID;
        if (stol(currID) == targetID)
        {
            result.push_back(curr);
            return result;
        }
        cout << "1st ID: " << firstID << endl;

        while (!(datFile.eof()))
        {
            count++;
            getline(datFile, curr);
            cout << curr << endl;
            curr.erase(std::remove(curr.begin(), curr.end(), '\t'), curr.end());
            if (curr == "")
                break;
            stringstream check1(curr);
            string intermediate;
            while (getline(check1, intermediate, 'a'))
            {
                if (intermediate == "")
                    continue;
                getline(check1, intermediate, 'a');
                getline(check1, intermediate, 'a');
                currID = intermediate;
                break;
            }
            if (targetID == stol(currID))
            {
                cout << "found target ID " << targetID << endl;
                result.push_back(curr);
                return result;
            }
            else if (targetID < stol(currID))
            {
                cout << "target: " << targetID << " curr " << currID << endl;
                break;
            }
        }
        datFile.close();
    }
    cout << firstID << endl
         << currID << endl
         << estNum << endl
         << targetID << endl;
    if (stol(firstID) < targetID)
    { //returns nothing if the program has found blocks older than the target, but not the target
        cout << "nothing found\n";
        return result;
    }
    int newEstBlocks = 100 + interpolateChangePerLog(stol(currID), stol(firstID), count, targetID);
    if (newEstBlocks <= estNum)
    { //returns nothing if whole chain is searched
        cout << "nothing more to find\n";
        return result;
    }
    return searchNodeID(targetID, newEstBlocks, chainName, RPCPort, datadir);
}

vector<string> searchNodeTime(const vector<long> targetTime, const int estNum, const string chainName, const int RPCPort, const string datadir)
{ //recursively search chain for specific ID
    cout << "searching with " << estNum << "blocks\n";
    long target1 = targetTime[0];
    long target2 = target1;
    if (targetTime.size() > 1)
    {
        target2 = targetTime[1];
    }
    readChain(chainName, RPCPort, datadir, estNum);
    int count = 0;
    vector<string> result;
    string curr;
    ifstream datFile;
    string currTime;
    string firstTime = "";
    datFile.open("dat.txt");
    if (datFile.is_open())
    {
        getline(datFile, curr);
        curr.erase(std::remove(curr.begin(), curr.end(), '\"'), curr.end());
        stringstream check1(curr);
        string intermediate;
        while (getline(check1, intermediate, 'a'))
        {
            if (intermediate == "")
                continue;
            else
            {
                currTime = intermediate;
                break;
            }
        }
        if (stol(currTime) == target1 || (targetTime.size() > 1 && target1 < stol(currTime)))
        {
            //result = result + curr + "\n";
            result.push_back(curr);
        }
        firstTime = currTime;
        cout << "1st time: " << firstTime << endl;

        while (!(datFile.eof()))
        {
            count++;
            getline(datFile, curr);
            curr.erase(std::remove(curr.begin(), curr.end(), '\"'), curr.end());
            if (curr == "")
                break;
            stringstream check1(curr);
            string intermediate;
            while (getline(check1, intermediate, 'a'))
            {
                if (intermediate == "")
                    continue;
                else
                {
                    currTime = intermediate;
                    break;
                }
            }
            if (target1 == stol(currTime) || (target1 <= stol(currTime) && target2 >= stol(currTime)))
            {
                cout << "found target " << target1 << endl;
                result.push_back(curr);
                //result = result + curr + "\n";
            }
            else if (target2 < stol(currTime))
            {
                cout << "target: " << target1 << " curr " << currTime << endl;
                break;
            }
        }
        datFile.close();
    }
    cout << "first:" << firstTime << endl
         << "curr:" << currTime << endl
         << estNum << endl
         << target1 << endl;
    if (result.size() != 0 && stol(firstTime) != target1)
    {
        return result; //if result has been found, and is not the 1st time found (to ensure all duplicates are accounted for)
                       // then result is returned
    }
    if (stol(firstTime) < target1)
    { //returns nothing if the program has found blocks older than the target, but not the target
        cout << "first time:" << firstTime << "no matching time found\n";
        return result;
    }
    int newEstBlocks = 100 + interpolateChangePerLog(stol(currTime), stol(firstTime), count, target1);
    if (newEstBlocks <= estNum)
    { //returns nothing if whole chain is searched
        cout << "nothing more to find\n";
        return result;
    }
    return searchNodeTime(targetTime, 5 + newEstBlocks, chainName, RPCPort, datadir);
    //newEstBlocks is incremented to ensure that duplicate times are all captured in the search
}

int REFIDnumToBin(int auxNum)
{
    return 10 + int(pow(8, auxNum));
}

string dataToText(const string data, map<std::string, int> activityMap, map<std::string, int> resourceMap)
{
    cout << "converting to readable format\n";
    //int RPCPort = 4000 + 100 * stoi(nodeNum) + stoi(chainNum);
    vector<string> tokens;
    // stringstream class check1
    stringstream check1(data);
    string intermediate;
    // Tokenizing w.r.t. space ' '
    while (getline(check1, intermediate, 'a'))
    {
        tokens.push_back(intermediate);
    }
    string text;
    text = tokens[0] + "\t" + tokens[1] + "\t" + tokens[2] + "\t" + tokens[3] + "\t" + tokens[4];
    for (map<string, int>::iterator it = activityMap.begin(); it != activityMap.end(); it++)
    {
        string activity = it->first;
        string activityNum = to_string(it->second);
        if (activityNum == tokens[5])
        {
            text = text + "\t" + activity;
            break;
        }
    }
    for (map<string, int>::iterator it = resourceMap.begin(); it != resourceMap.end(); it++)
    {
        string resource = it->first;
        string resourceNum = to_string(it->second);
        if (resourceNum == tokens[6])
        {
            text = text + "\t" + resource;
            break;
        }
    }
    return text;
}

string getNodeNum(string datadir)
{ //returns nodeNumber as a strings
    //TODO
    for (int i = datadir.length() - 1; i > 0; i--)
    {
        int n = datadir.at(i) - '0';
        if (n > 0 && n < 10)
        {
            return to_string(n);
        }
    }
    return "ERROR";
}

vector<string> searchChainNode(const vector<string> searchParams, const string datadir, const string chainNum,
                               const string nodeNum, map<std::string, int> activityMap, map<std::string, int> resourceMap)
{ // searchChain, but for specific node's chain
    // nodeNum = computer's chainNum
    // chainNum = number of the chain being searched

    vector<int> existingParams;
    int numParams = 0; // number of parameters, excluding node;
    for (int i = 0; i < searchParams.size(); i++)
    { //find existing params
        if (i != 1 && searchParams[i] != "")
        {
            numParams++;
            existingParams.push_back(i);
        }
    }
    int searchedParams = 0; // number of params taht have already been searched
    int RPCPort = 4000 + 100 * stoi(nodeNum) + stoi(chainNum);
    int NETPort = RPCPort + 1000;
    int COMPort;
    string chainName = "";
    int check;
    if (searchParams[5] != "")
    { //least chains
        if (chainNum == nodeNum)
        {
            RPCPort = 12000 + 100 * stoi(chainNum) + stoi(searchParams[5]);
            NETPort = 13000 + 100 * stoi(chainNum) + stoi(searchParams[5]);
            COMPort = 8080 + stoi(chainNum);
        }
        else
        {
            RPCPort = 12400 + 100 * stoi(nodeNum) + stoi(searchParams[5]);
            NETPort = 13400 + 100 * stoi(nodeNum) + stoi(searchParams[5]);
            COMPort = 8080 + stoi(chainNum);
        }
        COMPort = 8080 + stoi(chainNum);

        chainName = "log" + chainNum + "activity" + searchParams[5];

        // connectToChain(chainName, RPCPort, NETPort, COMPort, datadir);
        // check = readChain(chainName, RPCPort, datadir, 9999999);
    }
    if (searchParams[6] != "")
    {
        if (chainNum == nodeNum)
        {
            RPCPort = 6000 + 100 * stoi(chainNum) + stoi(searchParams[6]);
            NETPort = 7000 + 100 * stoi(chainNum) + stoi(searchParams[6]);
            COMPort = 8080 + stoi(chainNum);
        }
        else
        {
            RPCPort = 6400 + 100 * stoi(nodeNum) + stoi(searchParams[6]);
            NETPort = 7400 + 100 * stoi(nodeNum) + stoi(searchParams[6]);
            COMPort = 8080 + stoi(chainNum);
        }

        chainName = "log" + chainNum + "resource" + searchParams[6];

        // connectToChain(chainName, RPCPort, NETPort, COMPort, datadir);
        // check = readChain(chainName, RPCPort, datadir, 9999999);
    }
    if (searchParams[4] != "")
    {
        if (chainNum == nodeNum)
        {
            RPCPort = 10000 + 100 * stoi(chainNum) + stoi(searchParams[4]);
            NETPort = 11000 + 100 * stoi(chainNum) + stoi(searchParams[4]);
            COMPort = 8080 + stoi(chainNum);
        }
        else
        {
            RPCPort = 10400 + 100 * stoi(nodeNum) + stoi(searchParams[4]);
            NETPort = 11400 + 100 * stoi(nodeNum) + stoi(searchParams[4]);
            COMPort = 8080 + stoi(chainNum);
        }
        chainName = "log" + chainNum + "user" + searchParams[4];
    }
    if (searchParams[3] != "")
    {
        int auxNum = 0;
        while (stoi(searchParams[3]) > REFIDnumToBin(auxNum))
        {
            auxNum++;
        }
        if (chainNum == nodeNum)
        {
            RPCPort = 8000 + 100 * stoi(chainNum) + auxNum;
            NETPort = 9000 + 100 * stoi(chainNum) + auxNum;
            COMPort = 8080 + stoi(chainNum);
        }
        else
        {
            RPCPort = 8400 + 100 * stoi(nodeNum) + auxNum;
            NETPort = 9400 + 100 * stoi(nodeNum) + auxNum;
            COMPort = 8080 + stoi(chainNum);
        }
        RPCPort = 8000 + 100 * stoi(chainNum) + auxNum;
        NETPort = 9000 + 100 * stoi(chainNum) + auxNum;
        chainName = "log" + chainNum + "REFID" + to_string(auxNum);
        cout << chainName << endl;
    }
    string chain;
    if (chainName != "")
    {
        chain = findChain(chainName, RPCPort, datadir, nodeNum);
        if (chain != "Not Found")
        {
            searchedParams = 1;
            cout << "RPCPORT " << RPCPort << endl;
            int connectedToChain = connectToChain(chain, nodeNum, RPCPort, NETPort, COMPort, datadir);
            if (connectedToChain != 0)
            { //chain exists, but could not be found, presumably because corresponding node is offline
                chainName = "log" + chainNum;
            }
        }
        else
        { //TODO change back
            chainName = "log" + chainNum;
            RPCPort = 4000 + 100 * stoi(nodeNum) + stoi(chainNum);
            NETPort = RPCPort + 1000;
            //          return "no entries found";
        }
    }
    else
    { //default to main log chain, if no shorter chain is available for given search criteria
        chainName = "log" + chainNum;
    }
    if (searchParams[2] != "" || searchParams[0] != "")
    {
        vector<string> result;
        if (searchParams[2] != "")
        {
            //TODO seach by ID
            searchedParams++;
            result = searchNodeID(stol(searchParams[2]), 100, chainName, RPCPort, datadir);
            if (result.size() == 0)
                return result; //return if nothing found
        }
        else if (searchParams[0] != "")
        {
            searchedParams++;
            cout << "searching timestamp\n";
            vector<long> times;

            //find out if timestamp is a range or not
            if (searchParams[0].find('-') != std::string::npos)
            {
                cout << "TIME RANGE\n"
                     << "'" << searchParams[0].substr(0, searchParams[0].find("-", 0)) << "'\n'";

                times.push_back(stol(searchParams[0].substr(0, searchParams[0].find("-", 0))));
                cout << "TIME 1: " << times[0] << endl;
                times.push_back(stol(searchParams[0].substr(searchParams[0].find("-", 0) + 1, searchParams[0].size() - 1)));
                cout << "TIME 2: " << times[1] << endl;
            }
            else
            {
                times.push_back(stol(searchParams[0]));
            }

            result = searchNodeTime(times, 100, chainName, RPCPort, datadir);
            if (result.size() == 0)
                return result; //return if nothing found
        }
        //match other parameters
        cout << numParams << " total and " << searchedParams << "complete\n";
        vector<string> refinedResult;
        // vector<string> lines;
        // stringstream check0(result);
        string line;
        if (numParams > searchedParams)
        {
            cout << "Aditional search parameters remaining\n";
            //result with elements that dont match other parameters removed
            for (int i = 0; i < result.size(); i++)
            {
                line = result[i];
                line.erase(std::remove(line.begin(), line.end(), '\"'), line.end());
                vector<string> tokens;
                stringstream check1(line);
                string intermediate;
                while (getline(check1, intermediate, 'a'))
                {
                    if (intermediate == "")
                        continue;
                    tokens.push_back(intermediate);
                    cout << "intermediate " << intermediate << endl;
                }
                if (matchParams(existingParams, searchParams, tokens))
                {
                    refinedResult.push_back(dataToText(result[i], activityMap, resourceMap));
                    // refinedResult = refinedResult + dataToText(line, activityMap, resourceMap) + "\n";
                }
            }
            return refinedResult;
        }
        else
        {
            cout << "done searching\n";
        }
        for (int i = 0; i < result.size(); i++)
        {
            line = result[i];
            refinedResult.push_back(dataToText(line, activityMap, resourceMap));
        }
        // while (getline(check0, line, '\n'))
        // {
        //     vector<string> tokens;
        //     stringstream check1(line);
        //     string intermediate;
        //     while (getline(check1, intermediate, 'a'))
        //     {
        //         if (intermediate == "")
        //             continue;
        //         tokens.push_back(intermediate);
        //         cout << "intermediate " << intermediate << endl;
        //     }
        //     refinedResult = refinedResult + dataToText(line, activityMap, resourceMap) + "\n";
        // }
        return refinedResult;
    }
    else
    {
        check = readChain(chainName, RPCPort, datadir, 9999999);
        vector<string> result = longSearch(searchParams);

        vector<string> refinedResult;
        for (int i = 0; i < result.size(); i++)
        {
            refinedResult.push_back(dataToText(result[i], activityMap, resourceMap));
        }

        return refinedResult;
    }
}

string getParameterFromLine(string line)
{
    int wordStartPosition = 0;
    for (int i = 0; i < sortNum; i++)
        wordStartPosition = line.find_first_of('\t', wordStartPosition + 1);
    int wordEndPosition = line.find_first_of('\t', wordStartPosition + 1);
    std::string result = line.substr(wordStartPosition + 1, line.length() - wordEndPosition - 1);
    //std::cout << result << endl;
    return result;
}

bool compareParams(const string a, const string b)
{
    //find 1st parameter
    if(sortNum < 5){
        long paramA = stol(getParameterFromLine(a));
        long paramB = stol(getParameterFromLine(b));
        return paramA < paramB;
    }
    else{
        string paramA = getParameterFromLine(a);
        string paramB = getParameterFromLine(b);
        return (paramA.compare(paramB) < 0);
    }

}

vector<string> searchChain(vector<string> searchParams, const string datadir,
                           const string nodeNum, map<std::string, int> activityMap, map<std::string, int> resourceMap)
{ //complete search function
    vector<string> result;
    if (searchParams[1] == "")
    {
        for (int i = 1; i < 5; i++)
        {
            string chainNum = to_string(i);
            searchParams[1] = chainNum;
            vector<string> searchResult = searchChainNode(searchParams, datadir, chainNum, nodeNum, activityMap, resourceMap);
            result.insert(result.end(), searchResult.begin(), searchResult.end());
            //result = result + searchChainNode(searchParams, datadir, chainNum, nodeNum, activityMap, resourceMap);
        }
    }
    else
    {
        result = searchChainNode(searchParams, datadir, searchParams[1], nodeNum, activityMap, resourceMap);
    }
    return result;
}

bool is_number(const std::string &s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it))
        ++it;
    return !s.empty() && it == s.end();
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
                break;
            }
        }
        confFile.close();
    }

    std::map<std::string, int> activityMap;
    std::map<std::string, int> resourceMap;
    string current_exec_name = argv[0]; // Name of the current exec program
    vector<std::string> all_args;
    string chainName;
    if (argc > 1)
    {
        all_args.assign(argv + 1, argv + argc);
    }

    if (all_args.size() != 3)
    {
        cout << "Invalid input, enter <chain name> <log file> <node number>\n";
    }
    cout << all_args.size() << " args found\n";
    for (int i = 0; i < all_args.size(); i++)
    {
        cout << "arg: " << all_args[i] << endl;
    }
    string datadir = all_args[0];
    //    int sortNum = -1;    // number of parameter to sort, 0 = timestamp, 1 = Node, 2 = ID, 3 REFID, 4 = User, 5 = activity, 6 = resource
    string sortString;
    string nodeNum = getNodeNum(datadir);
    updateMap("activity", activityMap, nodeNum);
    updateMap("resource", resourceMap, nodeNum);
    vector<string> searchParams(7, ""); //0 = timestamp, 1 = Node, 2 = ID, 3 REFID, 4 = User, 5 = activity, 6 = resource
    for (int i = 1; i < all_args.size(); i++)
    {
        string line = all_args[i];
        vector<string> tokens;
        // stringstream class check1
        stringstream check1(line);
        string intermediate;
        // Tokenizing w.r.t. space ' '
        while (getline(check1, intermediate, '='))
        {
            tokens.push_back(intermediate);
            cout << intermediate << endl;
        }
        for (auto &c : tokens[0])
            c = toupper(c);
        if (tokens[0] == "TIMESTAMP")
        {
            searchParams[0] = tokens[1];
        }
        else if (tokens[0] == "NODE")
        {
            searchParams[1] = tokens[1];
        }
        else if (tokens[0] == "ID")
        {
            searchParams[2] = tokens[1];
        }
        else if (tokens[0] == "REFID")
        {
            searchParams[3] = tokens[1];
        }
        else if (tokens[0] == "USER")
        {
            searchParams[4] = tokens[1];
        }
        else if (tokens[0] == "ACTIVITY")
        {
            if (is_number(tokens[1]))
                searchParams[5] = tokens[1];
            else if (activityMap.find(tokens[1]) != activityMap.end())
                searchParams[5] = to_string(activityMap[tokens[1]]);
            else
                cout << "Activity " << tokens[1] << " not found\n";
        }
        else if (tokens[0] == "RESOURCE")
        {
            if (is_number(tokens[1]))
                searchParams[6] = tokens[1];
            else if (activityMap.find(tokens[1]) != activityMap.end())
                searchParams[6] = to_string(activityMap[tokens[1]]);
            else
                cout << "Resource " << tokens[1] << " not found\n";
        }
        else if (tokens[0] == "SORT")
        { //0 = timestamp, 1 = Node, 2 = ID, 3 REFID, 4 = User, 5 = activity, 6 = resource
            if (tokens[1].find('-') != std::string::npos)
            {
                sortString = tokens[1].substr(0, tokens[1].find("-", 0));
                for (auto &c : sortString)
                    c = toupper(c);
                if (sortString == "TIMESTAMP")
                {
                    cout << "TIMESTAMP FOUND FOR SORT\n";
                    sortNum = 0;
                }
                else if (sortString == "NODE")
                {
                    sortNum = 1;
                }
                else if (sortString == "ID")
                {
                    sortNum = 2;
                }
                else if (sortString == "REFID")
                {
                    sortNum = 3;
                }
                else if (sortString == "USER")
                {
                    sortNum = 4;
                }
                else if (sortString == "ACTIVITY")
                {
                    sortNum = 5;
                }
                else if (sortString == "RESOURCE")
                {
                    sortNum = 6;
                }
                cout << "SORT 1: " << sortNum << endl;
                sortString = tokens[1].substr(tokens[1].find("-", 0) + 1, tokens[1].size() - 1);
                for (auto &c : sortString)
                    c = toupper(c);
                cout << "SORT 2: " << sortString << endl;
            }
        }
    }
    // readChain("log" + nodeNum, stoi("4" + nodeNum + "0" + nodeNum), datadir, 999999);
    // string logs = longSearch(searchParams);
    // cout << logs << endl;
    // string mim = searchTimeStamp(1522000019451, "log" + nodeNum, nodeNum, nodeNum, datadir);
    cout << "nodenum" << nodeNum << endl;
    vector<string> result = searchChain(searchParams, datadir, nodeNum, activityMap, resourceMap);

    if (sortString == "ASC")
    {
        std::sort(result.begin(), result.end(),
                  [](const string a, const string b) {

                      return compareParams(a, b);
                  });
    }
    else if (sortString == "DEC")
    {
        std::sort(result.begin(), result.end(),
                  [](const string a, const string b) {
                      return ! compareParams(a, b);
                  });
    }
    cout << "sortParam: " << sortNum << sortString << endl;
    string resultString = "";
    for (int i = 0; i < result.size(); i++)
    {
        resultString = resultString + result[i] + "\n";
    }
    cout << "RESULT\n"
         << resultString << endl;
    system("rm dat.txt");
    string writeCommand = "echo \"" + resultString + "\" > result.txt";
    system(writeCommand.c_str());
}