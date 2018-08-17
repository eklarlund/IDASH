
Virtual machine Setup

http://cdimage.ubuntu.com/ubuntu-gnome/releases/14.04/release/ubuntu-gnome-14.04-desktop-amd64.iso  
-install vmware 12 player  
-download a 64 bit version of ubuntu 14.04, I got mine from https://distrowatch.com/  
-open a terminal and enter “sudo apt update” followed by “sudo apt upgrade”  
-Follow instructions from https://kb.vmware.com/s/article/1022525 to install vmware tools  
-download multichain 1.04 from https://www.multichain.com/download-install/, and follow the instructions to install  

Setting up the chains  

-Add initializer.sh to path and run it, it should add all other shellscripts to path and compile both c++ programs  
-Open paths.config, change the MULTICHAIN_BIN to the path leading to your multichain .exe’s  
-To set everything up, run setup.sh. As inputs, it takes the IP addresses of the 4 nodes, including itself, and the node number (whatever the node field is in its log) as a 5th input.  
	-run as “./setup.sh  <1st node IP> 	<2nd node IP>
       <3rd node IP> 	<4th node IP> 		<node number>”  
Upload and Searching  

-To upload, run upload.cpp, with the log file and node number as inputs  
	-run as “./upload  <log file>  <node number>”  
	-example “./upload 	../Data/training1.txt 	1”  
-To search, run searchLog.cpp, with the directory of the chain (which should be “node<node number>”, in the directory setup.sh was ran in) followed by the search parameters in the format “<field>=<value>” (for example user=10)  
	-run as “./searchlog  <directory> 	<field1>=<value1> 	<field2>=<value2>, etc”  
-example “./searchlog 	node1 	ID=4 	resource=REQ_RESOURCE”  
