
#! /bin/bash
if [ -f ${PWD}/../paths.config ]
then  
    source ${PWD}/../paths.config
elif [ -f ${PWD}/paths.config ]
then
    source ${PWD}/paths.config
else
  echo "no source found"
  exit 1
fi # MULTICHAIN_BIN="/home/esben/Documents/Github/multichain/multichain/src"
# DATA_BIN="/home/esben/Documents/Data"
# SCRIPT_BIN=$PWD

NODE_BIN=$PWD
PORT=${1}
while true
do 
    RESPONSE=$(nc -l -p ${PORT})
    echo $RESPONSE
    CHAINNAME=$(echo "$RESPONSE" | cut -d' ' -f 1)
    WALLET_ADRESS=$(echo "$RESPONSE" | cut -d' ' -f 2)
    echo \'${CHAINNAME}\'
    echo \'${WALLET_ADRESS}\'

    echo "${MULTICHAIN_BIN}/multichain-cli" $CHAINNAME -datadir="$PWD" grant $WALLET_ADRESS connect
    "${MULTICHAIN_BIN}/multichain-cli" $CHAINNAME -datadir="$PWD" grant $WALLET_ADRESS connect
    "${MULTICHAIN_BIN}/multichain-cli" $CHAINNAME -datadir="$PWD" grant $WALLET_ADRESS send
    "${MULTICHAIN_BIN}/multichain-cli" $CHAINNAME -datadir="$PWD" grant $WALLET_ADRESS receive
    "${MULTICHAIN_BIN}/multichain-cli" $CHAINNAME -datadir="$PWD" grant $WALLET_ADRESS admin
    "${MULTICHAIN_BIN}/multichain-cli" $CHAINNAME -datadir="$PWD" grant $WALLET_ADRESS issue
    "${MULTICHAIN_BIN}/multichain-cli" $CHAINNAME -datadir="$PWD" grant $WALLET_ADRESS create
    "${MULTICHAIN_BIN}/multichain-cli" $CHAINNAME -datadir="$PWD" grant $WALLET_ADRESS activate
    "${MULTICHAIN_BIN}/multichain-cli" $CHAINNAME -datadir="$PWD" grant $WALLET_ADRESS mine
        "${MULTICHAIN_BIN}/multichain-cli" $CHAINNAME -datadir="$PWD" grant $WALLET_ADRESS connect

done
echo "ERROR, LISTENER CLOSED----------------------------------------------------------------"


# echo -ne "mem" | nc 127.0.0.1 8080

# exec 3<>/dev/tcp/"$1"/"$2"
# echo -ne "$3" >&3

#! 192.168.133.131

#   "${MULTICHAIN_BIN}/multichaind" log1@192.168.133.131:4400 -datadir="$PWD/node2" -rpcport="4402" -port="4420" -daemon
