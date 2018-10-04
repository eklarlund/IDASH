# IDASH Privacy and Security workshop 2018: Competition task 1

For this competition, we were tasked with storing acess logs to a server using [Multichain](https://www.multichain.com/). In the competetion, 4 nodes, each represented by a different Virtual Machine, upload their access files simultaneously. Afterwards, each node must be able to search activity from all 4 nodes. We provide the detailed task in the end of this note (copied from the [Workshop's website](http://www.humangenomeprivacy.org/2018/competition-tasks.html)). 
To reduce the search time in our solution, we decided to create multiple chains for each node, rather than storing all of our data on a single chain. This results in shorter chains and faster searches.

## Usage


### Virtual machine Setup

This is how we set up the virtual machines used for testing. The judges used a similar process.	
>-Install vmware 12 player  
>-Download a 64 bit version of ubuntu 14.04, such as [this](http://cdimage.ubuntu.com/ubuntu-gnome/releases/14.04/release/ubuntu-gnome-14.04-desktop-amd64.iso).	
>-Open a terminal and enter “sudo apt update” followed by “sudo apt upgrade”  
>-Follow instructions from [here](https://kb.vmware.com/s/article/1022525) to install vmware tools.  
>-download multichain 1.04 from [here](https://www.multichain.com/download-install/), and follow the instructions to install.  

### Setting up the chains  

Instructions on how to set up all of the chains needed to upload and search logs.	
>-Add initializer.sh to path and run it, it should add all other shellscripts to path and compile both c++ programs  
>-Open paths.config, change the MULTICHAIN_BIN to the path leading to your multichain .exe’s  
>-To set everything up, run setup.sh. As inputs, it takes the IP addresses of the 4 nodes, including itself, and the node number (whatever the node field is in its log) as a 5th input.  
	-run as “./setup.sh &nbsp;&nbsp; \<1st node IP> &nbsp;&nbsp; \<2nd node IP>  &nbsp;&nbsp; 
       \<3rd node IP>  &nbsp;&nbsp; \<4th node IP>  &nbsp;&nbsp; \<node number>”  
	
### Upload and Searching  

Instructions for uploading and searching logs. The format for upload files can be found in the competition description.	
>-To upload, run upload.cpp, with the log file and node number as inputs. The result is printed to the screen, and written to the file results.txt.  
	-run as “./upload &nbsp;&nbsp; \<log file> &nbsp;&nbsp; \<node number>”  
	-example “./upload  &nbsp;&nbsp; ../Data/training1.txt  &nbsp;&nbsp; 1”  
>-To search, run searchLog.cpp, with the directory of the chain (which should be “node<node number>”, in the directory setup.sh was ran in) followed by the search parameters in the format “<field>=<value>” (for example user=10)  
	-run as “./searchlog &nbsp;&nbsp; \<directory> &nbsp;&nbsp; \<field1>=\<value1> &nbsp;&nbsp; \<field2>=\<value2>”, etc.  
>-example “./searchlog &nbsp;&nbsp; node1 &nbsp;&nbsp; ID=4 &nbsp;&nbsp; resource=REQ_RESOURCE”  


## Original task copied from the Workshop's website

### Track 1: Blockchain-based immutable logging and querying for cross-site genomic dataset access audit trial

The goal of this track is to develop blockchain-based ledgering solutions to log and query the user activities of accessing genomic datasets (e.g., GTEx) across multiple sites.

### Experimental setting:
Given a genomic data access log file, design a time/space efficient data structure and mechanisms to store and retrieve the logs based on MultiChain version 1.0.4 (https://www.multichain.com/download-install/).

### Challenge:
Each line in the data access log must be saved individually as one transaction (i.e., participants cannot save entire file in just one transaction), and all log data and intermediate data (such as index or cache) must be saved on-chain (no off-chain data storage allowed). It is up to you to determine how you choose to represent and store each line in transactions. It does not need to be a plain text copy of the log entry. Also, the query implementation should allow a user to search using any field of one log line (i.e., node, id, user, resource, activity, timestamp, and a “reference id” referring to the id of the original resource request), any “AND” combination (e.g., node AND id AND user AND resource), and any timestamp range (e.g., from 1522000002418 to 1522000011441) using a command-line interface. Also, the user should be able to sort the returning results in ascending/descending order with any field (e.g., timestamp). There will be 4 nodes in the blockchain network, and 4 log files to be stored. Users should be able to query the data from any of the 4 sites. Participants can implement any algorithm to store, retrieve and present the log data correctly and efficiently.

### Requirement:
Submission requires open source code.

### Evaluation Criteria:
The logging/querying system needs to demonstrate good performance (i.e., accurate query results) by using a testing dataset, which is different from the one provided online. We will evaluate the speed, storage/memory cost, and scalability of each solution. We will use the binary version of MultiChain 1.0.4 on 64-bit Ubuntu 14.04 with the default parameters as the test bed for fairness. No modification of the underlying MultiChain source code is allowed. The participants must consent to release any code or binaries submitted to the competition under the GNU General Public License v3.0 Open Source license. The submitted executable binaries should be non-interactive (i.e., depend only on parameters with no input required while it works), and should contain a readme file to specify the parameters. We will test all submissions using 4 virtual machines, each with 2-Core CPU, 8GB RAMs and 100GB storage.
