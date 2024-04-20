/* COMPSCI 424 Program 3
   Name: Sam Dawidowich
   
   p3main.cpp: contains the main function for this program.

   This is a template. Feel free to edit any of these files, even
   these pre-written comments or my provided code. You can add any
   classes, methods, and data structures that you need to solve the
   problem and display your solution in the correct format.
*/



// You will probably need to add more #include directives.
// Remember: use <angle brackets> for standard C++ headers/libraries
// and "double quotes" for headers in the same directory as this file.
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <sstream>
#include <vector>
#include <mutex>

#include "Resource.h"
#include "Process.h"
#include "Command.h"
#include "Random.h"

/*
   If you want your variables and data structures for the banker's
   algorithm to have global scope, declare them here. This may make
   the rest of your program easier to write. 
   
   Most software engineers say global variables are a Bad Idea (and 
   they're usually correct!), but systems programmers do it all the
   time, so I'm allowing it here.
*/

bool isBlocked(std::vector<Resource> resourceList, Process process) {
    for (int i = 0; i < resourceList.size(); i++) {
        if (process.getPotentialRequests(i) > resourceList[i].getAvailable()) {
            return true;
        }
    }

    return false;
}

bool isReducible(std::vector<Resource> resourceList, std::vector<Process> processList) {
    while (processList.size() > 0) {
        int startSize = processList.size();

        // Remove an unblocked process
        for (int i = 0; i < processList.size(); i++) {
            if (!isBlocked(resourceList, processList[i])) {
                for (int j = 0; j < resourceList.size(); j++) {
                    int currAvailable = resourceList[j].getAvailable();
                    int processAlloc = processList[i].getCurrentAlloc()[j];
                    resourceList[j].setAvailable(currAvailable + processAlloc);
                }

                processList.erase(processList.begin() + i);
                break;
            }
        }

        // If size didn't change, then there were no unblocked processes to remove, so allocation graph is not reducible.
        if (processList.size() == startSize) {
            return false;
        }
    }

    return true;
}

void displayProcesses(std::vector<Resource>& resourceList, std::vector<Process>& processList) {
    // Display all processes' resource max claims and allocations
    for (int i = 0; i < processList.size(); i++) {
        std::vector<int> process = processList[i].getMaxClaims();
        for (int j = 0; j < resourceList.size(); j++) {
            std::cout << process[j] << " ";
        }
        std::cout << std::endl;
    }
    for (int i = 0; i < processList.size(); i++) {
        std::vector<int> process = processList[i].getCurrentAlloc();
        for (int j = 0; j < resourceList.size(); j++) {
            std::cout << process[j] << " ";
        }
        std::cout << std::endl;
    }
}

void displayResources(std::vector<Resource>& resourceList) {
    // Display total resources
    for (int j = 0; j < resourceList.size(); j++) {
        int total = resourceList[j].getTotal();
        std::cout << total << std::endl;
    }
}

void executeCommand(std::vector<Resource>& resourceList, std::vector<Process>& processList, Command& command) {
    int available = resourceList[command.getResource()].getAvailable();
    int currAlloc = processList[command.getProcess()].getCurrentAlloc()[command.getResource()];
    int max = processList[command.getProcess()].getMaxClaims()[command.getResource()];

    if (command.getType() == CommandType::REQUEST) {
        std::vector<Resource> tempRL = resourceList;
        std::vector<Process> tempPL = processList;

        if (command.getAmount() > available) {
            std::cout << "Process " << command.getProcess() << " requested " << command.getAmount() << " units of resource " << command.getResource() << ". Result: Denied" << std::endl;
            std::cout << "Reason: Only " << available << " units of resource " << command.getResource() << " are available" << std::endl;
            return;
        }
        else if ((currAlloc + command.getAmount()) > max) {
            std::cout << "Process " << command.getProcess() << " requested " << command.getAmount() << " units of resource " << command.getResource() << ". Result: Denied" << std::endl;
            std::cout << "Reason: Process " << command.getProcess() << " Current Allocation (" << currAlloc << ") + Requested (" << command.getAmount() << ") > Max (" << max << ")" << std::endl;
            return;
        }

        tempRL[command.getResource()].setAvailable(available - command.getAmount());
        tempPL[command.getProcess()].setCurrentAlloc(command.getResource(), currAlloc + command.getAmount());

        if (isReducible(tempRL, tempPL)) {
            resourceList = tempRL;
            processList = tempPL;
            std::cout << "Process " << command.getProcess() << " requested " << command.getAmount() << " units of resource " << command.getResource() << ". Result: Granted" << std::endl;
            return;
        }

        std::cout << "Process " << command.getProcess() << " requested " << command.getAmount() << " units of resource " << command.getResource() << ". Result: Denied" << std::endl;
        std::cout << "Reason: Resource allocation graph was not reducible." << std::endl;
        return;
    }
    else {
        if (command.getAmount() < 0) {
            std::cout << "Process " << command.getProcess() << " unable to release " << command.getAmount() << " units of resource " << command.getResource() << "." << std::endl;
            std::cout << "Reason: Cannot release a negative number of units." << std::endl;
            return;
        }
        else if (command.getAmount() > currAlloc) {
            std::cout << "Process " << command.getProcess() << " unable to release " << command.getAmount() << " units of resource " << command.getResource() << "." << std::endl;
            std::cout << "Reason: Process " << command.getProcess() << " only has " << currAlloc << " units currently allocated." << std::endl;
            return;
        }

        resourceList[command.getResource()].setAvailable(available + command.getAmount());
        processList[command.getProcess()].setCurrentAlloc(command.getResource(), currAlloc - command.getAmount());
        std::cout << "Process " << command.getProcess() << " released " << command.getAmount() << " units of resource " << command.getResource() << "." << std::endl;
        return;
    }
}

void runManual(std::vector<Resource>& resourceList, std::vector<Process>& processList) {
    std::vector<Command> commandList;

    std::cout << "Enter request/release commands. Enter 'end' when finished: " << std::endl;
    std::string line;
    std::getline(std::cin, line);

    while (line != "end") {
        std::stringstream ss(line);

        std::string trash;
        
        std::string type;
        int amount;
        int resource;
        int process;

        // request/release (type) I (amount) of (trash) J (resource) for (trash) K (process)
        ss >> type >> amount >> trash >> resource >> trash >> process;

        if (type == "request") {
            commandList.push_back(Command(CommandType::REQUEST, amount, resource, process));
        }
        else if (type == "release") {
            commandList.push_back(Command(CommandType::RELEASE, amount, resource, process));
        }
        else {
            std::cout << "Invalid command." << std::endl;
        }

        std::getline(std::cin, line);
    }

    for (int i = 0; i < commandList.size(); i++) {
        executeCommand(resourceList, processList, commandList[i]);
        std::cout << std::endl;
    }
}

Command generateRequest(Random& rand, std::vector<Resource>& resourceList, Process& process, int processID, CommandType type) {
    int resource = rand.randomInt(0, resourceList.size() - 1);
    int amount = rand.randomInt(0, process.getMaxClaims()[resource]);

    return Command(type, amount, resource, processID);
}

void runAutoCommands(std::mutex* m, std::vector<Resource>* resourceList, std::vector<Process>* processList, int processID, std::vector<Command>* requestCommands, std::vector<Command>* releaseCommands) {
    for (int i = 0; i < 3; i++) {
        m->lock();
        executeCommand((*resourceList), (*processList), (*requestCommands)[i]);
        std::cout << std::endl;
        m->unlock();

        
        m->lock();
        executeCommand((*resourceList), (*processList), (*releaseCommands)[i]);
        std::cout << std::endl;
        m->unlock();
    }
}

void runAuto(std::vector<Resource>& resourceList, std::vector<Process>& processList) {
    std::vector<std::vector<Command>> requestCommands;
    std::vector<std::vector<Command>> releaseCommands;

    Random rand;
    std::mutex* m = new std::mutex;

    for (int i = 0; i < processList.size(); i++) {
        requestCommands.push_back(std::vector<Command>());
        releaseCommands.push_back(std::vector<Command>());

        // Generate Request Commands
        for (int j = 0; j < 3; j++) {
            requestCommands[i].push_back(generateRequest(rand, resourceList, processList[i], i, CommandType::REQUEST));
        }
        
        // Generate Release Commands
        for (int j = 0; j < 3; j++) {
            releaseCommands[i].push_back(generateRequest(rand, resourceList, processList[i], i, CommandType::RELEASE));
        }
    }

    std::vector<std::thread> threads;
    
    // Create a thread for each process
    for (int i = 0; i < processList.size(); i++) {
        threads.push_back(std::thread(runAutoCommands, m, &resourceList, &processList, i, &(requestCommands[i]), &(releaseCommands[i])));
    }
    
    // Join threads
    for (int i = 0; i < processList.size(); i++) {
        threads[i].join();
    }
}

/*
  Arguments:
    argc: the number of command-line arguments, which should be >= 2
    argv[0]: the name of the executable file
    argv[1]: a C-string, either "manual" or "auto"
    argv[2]: a C-string containing the name of the setup file (and the
             path to the file, if it's not in the current directory)
*/
int main (int argc, char *argv[]) {
    // To help you get started, the major steps for the main program
    // are shown here as comments. Feel free to add more comments to
    // help you understand your code, or for any reason. Also feel free
    // to edit this comment to be more helpful.

    // Code to test command-line processing. You can keep, modify, or
    // remove this code. It's not required.
    if (argc < 2) {
        std::cerr << argc << " command-line arguments provided, 2 expected; exiting." << std::endl;
        if (argc == 1) std::cerr << "argv[1] == " << argv[1] << std::endl;
        return 1; // non-zero return value indicates abnormal termination
    }

    std::cout << "Selected mode: " << argv[1] << std::endl;
    std::cout << "Setup file location: " << argv[2] << std::endl;

    // 1. Open the setup file using the path in argv[2]
    std::ifstream setup_file;
    std::string line;

    int num_resources;
    int num_processes;

    std::vector<Resource> resourceList;
    std::vector<Process> processList;

    setup_file.open(argv[2], std::ios::in);
    if (setup_file.is_open()) {
        // 2. Get the number of resources and processes from the setup
        // file, and use this info to create the Banker's Algorithm
        // data structures
        setup_file >> num_resources;
        std::cout << num_resources << " resources" << std::endl;

        std::getline(setup_file, line); // skips the rest of the "resources" line
        
        setup_file >> num_processes;
        std::cout << num_processes << " processes" << std::endl;
        
        std::getline(setup_file, line); // skips the rest of the "processes" line

        // Create the Banker's Algorithm data structures, in any
        // way you like as long as they have the correct size
        // (unfortunately, you might not be able to use sscanf for this...)

        // 3. Use the rest of the setup file to initialize the data structures

        // Get Resources
        std::getline(setup_file, line); // skips "Available" label line

        for (int i = 0; i < num_resources; i++) {
            int available;
            setup_file >> available;
            resourceList.push_back(Resource(available));
        }

        std::getline(setup_file, line); // skips rest of the "Available" line
        std::getline(setup_file, line); // skips "Max" label line

        for (int i = 0; i < num_processes; i++) {
            processList.push_back(Process());

            for (int j = 0; j < num_resources; j++) {
                int max;
                setup_file >> max;
                processList[i].addResource(max, 0, 0);
            }
        }

        std::getline(setup_file, line); // skips rest of the "Max" line
        std::getline(setup_file, line); // skips "Allocation" label line

        for (int i = 0; i < num_processes; i++) {
            for (int j = 0; j < num_resources; j++) {
                int allocation;
                setup_file >> allocation;
                processList[i].setCurrentAlloc(j, allocation);
            }
        }

        // Done reading the file, so close it
        setup_file.close();
    } // end: if setup_file.is_open()

    // 4. Check initial conditions to ensure that the system is
    // beginning in a safe state: see "Check initial conditions"
    // in the Program 3 instructions

    // Check if more is allocated than the max
    for (int i = 0; i < num_processes; i++) {
        std::vector<int> maxClaims = processList[i].getMaxClaims();
        std::vector<int> currentAlloc = processList[i].getCurrentAlloc();
        for (int j = 0; j < num_resources; j++) {
            // Check if more is allocated than the max
            if (currentAlloc[j] > maxClaims[j]) {
                std::cout << "Invalid initial conditions: Process " << i << " has more allocated than the max claims for resource " << j << "." << std::endl;
                exit(-1);
            }

            // Sum total allocated of each resource
            resourceList[j].setTotal(resourceList[j].getTotal() + currentAlloc[j]);
        }
    }

    if (!isReducible(resourceList, processList)) {
        std::cout << "Invalid initial conditions: The resource allocation graph is not completely reducible." << std::endl;
        exit(-2);
    }

    // 5. Go into either manual or automatic mode, depending on
    // the value of args[0]; you could implement these two modes
    // as separate methods within this class, as separate classes
    // with their own main methods, or as additional code within
    // this main method.

    // Get mode

    std::string mode = argv[1];
    
    if (mode == "manual") {
        runManual(resourceList, processList);
    }
    else if (mode == "auto" || mode == "automatic") {
        runAuto(resourceList, processList);
    }
    else {
        std::cout << "Invalid mode. Valid modes: manual or auto/automatic." << std::endl;
        exit(-3);
    }

    
    return 0; // terminate normally
}