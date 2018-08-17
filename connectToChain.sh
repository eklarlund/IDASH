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
fi
# MULTICHAIN_BIN="/home/esben/Documents/Github/multichain/multichain/src"
# DATA_BIN="/home/esben/Documents/Data"
# SCRIPT_BIN=$PWD
NODE_BIN=$PWD
PORT=${5}
COUNT=${6}
CLI_COM="${MULTICHAIN_BIN}/multichain-cli"

echo "Host IP:  ${HOST_IP}"
#INPUT: DIRECTORY, CHAINNAME@HOST_PORT, RPCPORT, PORT, PORT for sending info, Optional limit on number of tries to connect

connect_com() {
  echo "Connect command----------------------------------------------------------------------------------"
  echo   "${MULTICHAIN_BIN}/multichaind" ${2} -datadir="${1}" -rpcport="${3}" -port="${4}" -daemon
  "${MULTICHAIN_BIN}/multichaind" ${2} -datadir="${1}" -rpcport="${3}" -port="${4}" -daemon
}
  echo   "${MULTICHAIN_BIN}/multichaind" ${2} -datadir="${1}" -rpcport="${3}" -port="${4}" -daemon

CHAIN_NAME=$(echo ${2} | cut -d'@' -f 1)
HOST_IP=$(echo ${2} | cut -d '@' -f 2 | cut -d ':' -f 1)


echo "ps -ef | grep multichaind ${CHAIN_NAME} | grep "${3}""
ALREADYCONNECTED=$(ps -ef | grep "multichaind ${CHAIN_NAME}" | grep "${3}")
echo \'${ALREADYCONNECTED}\'
if ! [[ -z ${ALREADYCONNECTED} ]] && ! [[ -z ${COUNT} ]]
then
  echo "Already connected"
  exit 0
fi

# while [[ -z "$WALLET_ADRESS" ]]
# do
#   WALLET_ADRESS=$( connect_com ${1} ${2} ${3} ${4} | grep "multichain-cli.*receive" | cut -d ' ' -f 4 )
#   sleep 1 
# done

CONNECTED=false 
echo "${1} CONNECTING TO CHAIN ${2}"
while ! ${CONNECTED}; do
  echo "CONNECTING..."
  WALLET_ADRESS=$( connect_com ${1} ${2} ${3} ${4} | grep "multichain-cli.*receive" | cut -d ' ' -f 4 )
  if  [[ -z "$WALLET_ADRESS" ]]
  then
    echo "CONNECTTOCHAIN FAILED TO CONNECT"
  else
    echo "CONNECTTOCHAIN WALLET ADRESS: ${WALLET_ADRESS}"
    echo "CONNECTTOCHAIN CHAIN NAME: ${CHAIN_NAME} IP: ${HOST_IP}"
    echo "${CHAIN_NAME} ${WALLET_ADRESS} | nc ${HOST_IP} ${PORT}"
    echo -ne "${CHAIN_NAME} ${WALLET_ADRESS}" | nc ${HOST_IP} ${PORT}
  fi
  # ${SCRIPT_BIN}/connectToChain.sh ${PWD} log${i}@10.0.0.22:5${i}0${i} \
  #                                 4${NODE_NUMBER}0${i} 5${NODE_NUMBER}0${i} 808${i}
  echo "MIDDLE OF LOOP CONNECTING TO ${2}"
  # multichaind -daemon does not work when stdout is redirected to a file descriptor, as in pipeline;
  # redirect must be to a file, apparently
  RESULT_FILE=$(mktemp /tmp/result.XXXXXXX)
    "${MULTICHAIN_BIN}/multichaind" ${2} -datadir="${1}" -rpcport="${3}" -port="${4}" -daemon > ${RESULT_FILE}
  RESULT=$( cat ${RESULT_FILE} | grep "Node ready." )
  rm $RESULT_FILE
  echo "RESULT OF: ------------------------------------------------------------------------------"
  echo \'${RESULT}\'
  if [ "${RESULT}" = "Node ready." ]
  then 
    echo "DONE CONNECTING-----------------------------------------------------"
    CONNECTED=true
  else
    echo "NOT CONNECTED-----------"
    sleep 5
  fi

  if ! [[ -z ${COUNT} ]]
  then 
    COUNT=$((${COUNT}-1))
    echo ${COUNT}
    if [ ${COUNT} = 0 ]
    then
      exit 0
    fi
  fi
  
done
# "${MULTICHAIN_BIN}/multichaind" ${2} -datadir="${1}" -rpcport="${3}" -port="${4}" -daemon &

SUBSCRIBE_ERROR="stream1"
while ! [[ -z ${SUBSCRIBE_ERROR} ]]
do
  SUBSCRIBE_ERROR=$($CLI_COM ${2} -datadir="${1}" -rpcport="${3}" -port="${4}" subscribe stream1 | grep error)
  echo ${SUBSCRIBE_ERROR}
  sleep 1
done

