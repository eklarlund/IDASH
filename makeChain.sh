#! /bin/bash

#arguements: chainName folder RPCPort Port
if [ -f ${PWD}/../paths.config ]
then  
    source ${PWD}/../paths.config
    echo "found path 1 up"
elif [ -f ${PWD}/paths.config ]
then
    source ${PWD}/paths.config
    echo "found path"
else
  echo "no source found"
  exit 1
fi
# MULTICHAIN_BIN="/home/esben/Documents/Github/multichain/multichain/src"
# DATA_BIN="/home/esben/Documents/Data"
# SCRIPT_BIN=$PWD
NODE_BIN=${2}
NODE_IP=$(ip addr | grep 'state UP' -A2 | tail -n1 | awk '{print $2}' | cut -f1  -d'/')
CLI_COM="${MULTICHAIN_BIN}/multichain-cli"
RPCPORT=${3}
PORT=${4}
ADD_TO_LIST=${5}



create_stream() {
    echo "MAKING STREAM----------------------------------------------------------"
    $CLI_COM ${1} -datadir=$NODE_BIN create stream stream1 true
    $CLI_COM ${1} -datadir=$NODE_BIN subscribe stream1
}

echo "Params"
echo $1
echo $2
echo $3
echo $4


if [ -d "${2}/${1}" ] 
then 
  echo "chain already exists, exiting"
  exit 0
fi


"${MULTICHAIN_BIN}/multichain-util" create ${1} 10010 -datadir="${NODE_BIN}" -default-rpc-port=${RPCPORT} -default-network-port=${PORT}
"${MULTICHAIN_BIN}/multichaind" ${1} -datadir="${NODE_BIN}" -rpcport=${RPCPORT} -port=${PORT} -daemon
#Ensure that communication has occurred between daemon and chain, so stream can be created
WALLET_ADRESS=$(${CLI_COM} "${1}" -datadir=${NODE_BIN} getaddresses| grep "\[" -A1 | tail -n1)

echo 'Making stream------------------------------'

while [[ -z "${RESULT}" ]]
do
    ${CLI_COM} "${1}" -datadir=${NODE_BIN} send ${WALLET_ADRESS//\"} 0
    # echo "WALLET ADRESS $WALLET_ADRESS"
    create_stream "${1}"
    RESULT=$( ${CLI_COM} "${1}" -datadir=${NODE_BIN} liststreamitems stream1 | grep "\[" )
    echo "MAKECHAIN ${1} STREAM RESULT"
    echo \'$RESULT\'
    sleep 1
done

auxResponse=
echo $ADD_TO_LIST
if ! [ -z ${ADD_TO_LIST} ]
then
    NODE_NUMBER="${NODE_BIN: -1}"
    while [ -z $auxResponse ]
    do
        auxResponse=$( ${CLI_COM} "aux" -rpcport=4${NODE_NUMBER}11 -datadir=${NODE_BIN} liststreams | grep "stream1" )
        echo "auxResponse ${auxResponse}"
        ${CLI_COM} "aux" -rpcport=4${NODE_NUMBER}11 -datadir=${NODE_BIN} subscribe stream1
        echo     ${CLI_COM} "aux" -rpcport=4${NODE_NUMBER}11 -datadir=${NODE_BIN} publish stream1 "${1}@${NODE_IP}:${PORT}" 00
        ${CLI_COM} "aux" -rpcport=4${NODE_NUMBER}11 -datadir=${NODE_BIN} publish stream1 "${1}@${NODE_IP}:${PORT}" 00
    done
fi
$CLI_COM ${1} -datadir=$NODE_BIN subscribe stream1 # ensure that stream was subscribed
exit 0