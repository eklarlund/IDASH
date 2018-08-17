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
fi# MULTICHAIN_BIN="/home/esben/Documents/Github/multichain/multichain/src"
# DATA_BIN="/home/esben/Documents/Data"
# SCRIPT_BIN=$PWD
NODE_IP=$(ip addr | grep 'state UP' -A2 | tail -n1 | awk '{print $2}' | cut -f1  -d'/')
# NODE_IP=127.0.0.1 # $(ip addr | grep 'state UP' -A2 | tail -n1 | awk '{print $2}' | cut -f1  -d'/')
CLI_COM="${MULTICHAIN_BIN}/multichain-cli"
NODE_NUMBER=0


create_stream() {
    echo "MAKING STREAM----------------------------------------------------------"
    $CLI_COM ${1} -datadir=$NODE_BIN create stream ${2} true
}


IP[0]=${1}
IP[1]=${2}
IP[2]=${3}
IP[3]=${4}

IFS=$'\n' sorted=($(sort <<<"${IP[*]}"))
printf "IP: %s\n" "${sorted[@]}"
# if [ ! -d "node1" ] 
# then 
#   mkdir "node1"
# fi
# if [ ! -d "node2" ] 
# then 
#   mkdir "node2"
# fi
# if [ ! -d "node3" ] 
# then 
#   mkdir "node3"
# fi
# if [ ! -d "node4" ] 
# then 
#   mkdir "node4"
# fi

# determine node number
NODE_BIN=$PWD
if ! [ -z ${5} ]
then
    NODE_NUMBER=${5}
elif [ ${NODE_IP} = ${sorted[0]} ]
then 
    NODE_NUMBER=1

elif [ ${NODE_IP} = ${sorted[1]} ]
then
    NODE_NUMBER=2

elif [ ${NODE_IP} = ${sorted[2]} ]
then
    NODE_NUMBER=3

elif [ ${NODE_IP} = ${sorted[3]} ]
then
    NODE_NUMBER=4

else
    echo "ERROR WITH NODE NUMBER----------------------------------------------------------"
    exit
fi
echo assigned node num: ${NODE_NUMBER}

if [ ! -d "node${NODE_NUMBER}" ] 
then 
  mkdir "node${NODE_NUMBER}"
fi


if [ ${NODE_NUMBER} = 1 ]
then
    echo "FIRST----------------------------------------------------------"
    NODE_NUMBER=1
    NODE_BIN="${PWD}/node1"
    cd ${NODE_BIN}

    # ${SCRIPT_BIN}/makeChain.sh activity node1 4111 5111
    # ${SCRIPT_BIN}/makeChain.sh resource node1 4121 5121
    ${SCRIPT_BIN}/makeChain.sh aux ${PWD} 4111 5111
    OTHER_NODES=(2 3 4)
    RESULT1="ERROR"
    RESULT2="ERROR"
    while ! [[ -z "${RESULT1}" ]]
    do
        ${CLI_COM} "aux" -datadir=${NODE_BIN} send ${WALLET_ADRESS//\"} 0
        # echo "WALLET ADRESS $WALLET_ADRESS"
        RESULT1=$( create_stream aux activity | grep Error )
        echo "MAKECHAIN STREAM RESULT1"
        echo \'$RESULT1\'
        sleep 1
    done
    while ! [[ -z "${RESULT2}" ]]
    do
        ${CLI_COM} "aux" -datadir=${NODE_BIN} send ${WALLET_ADRESS//\"} 0
        # echo "WALLET ADRESS $WALLET_ADRESS"
        RESULT2=$( create_stream aux resource | grep Error )
        echo "MAKECHAIN STREAM RESULT2"
        echo \'$RESULT2\'
        sleep 1
    done

elif [ ${NODE_NUMBER} = 2 ]
then
    echo "SECOND----------------------------------------------------------"
    NODE_NUMBER=2
    NODE_BIN="${PWD}/node2"
    OTHER_NODES=(1 3 4)
    cd ${NODE_BIN}

elif [ ${NODE_NUMBER} = 3 ]
then
    echo "THIRD----------------------------------------------------------"
    NODE_NUMBER=3
    NODE_BIN="${PWD}/node3"
    OTHER_NODES=(1 2 4)
    cd ${NODE_BIN}

elif [ ${NODE_NUMBER} = 4 ]
then
    echo "FOURTH----------------------------------------------------------"
    NODE_NUMBER=4
    NODE_BIN="${PWD}/node4"
    OTHER_NODES=(1 2 3)
    cd ${NODE_BIN}

else
    echo "ERROR WITH NODE NUMBER----------------------------------------------------------"
    exit
fi

echo "NODE_BIN= $NODE_BIN NODE_NUMBER=$NODE_NUMBER"
echo "PWD=$PWD"
# OTHER_NODES=(1 2 3 4)
# OTHER_NODES=( "${OTHER_NODES[@]/${NODE_NUMBER}}" )
echo ${SCRIPT_BIN}/makeChain.sh log${NODE_NUMBER} ${PWD} 4${NODE_NUMBER}0${NODE_NUMBER} 5${NODE_NUMBER}0${NODE_NUMBER}
${SCRIPT_BIN}/makeChain.sh log${NODE_NUMBER} ${PWD} 4${NODE_NUMBER}0${NODE_NUMBER} 5${NODE_NUMBER}0${NODE_NUMBER}




${SCRIPT_BIN}/listener.sh 808${NODE_NUMBER} &
echo "STARTING CONNECTION TO OTHER NODES: ${OTHER_NODES[@]}"
if ! [ ${NODE_NUMBER} -eq 1 ]
then
    ${SCRIPT_BIN}/connectToChain.sh ${PWD} aux@${sorted[0]}:5111 \
                                    4${NODE_NUMBER}11 5${NODE_NUMBER}11 8081
fi

for i in ${OTHER_NODES[@]}; do
    # ${SCRIPT_BIN}/connectToChain.sh ${PWD} log${i}@127.0.0.1:5${i}0${i} \
    #                                4${NODE_NUMBER}0${i} 5${NODE_NUMBER}0${i} 808${i}
    ${SCRIPT_BIN}/connectToChain.sh ${PWD} log${i}@${sorted[i-1]}:5${i}0${i} \
                                   4${NODE_NUMBER}0${i} 5${NODE_NUMBER}0${i} 808${i}
    # while ! ${CONNECTED}; do
    #     echo "CONNECTING..."
    #     # ${SCRIPT_BIN}/connectToChain.sh ${PWD} log${i}@10.0.0.22:5${i}0${i} \
    #     #                                 4${NODE_NUMBER}0${i} 5${NODE_NUMBER}0${i} 808${i}
    #     echo "MIDDLE OF LOOP ${i}"
    #     # multichaind -daemon does not work when stdout is redirected to a file descriptor, as in pipeline;
    #     # redirect must be to a file, apparently
    #     RESULT_FILE=$(mktemp /tmp/result.XXXXXXX)
    #     "${MULTICHAIN_BIN}/multichaind" log${i} -datadir="${PWD}" -rpcport="4${NODE_NUMBER}0${i}" -port="5${NODE_NUMBER}0${i}" -daemon > ${RESULT_FILE}
    #     RESULT=$( cat ${RESULT_FILE} | grep "Node ready." )
    #     rm $RESULT_FILE
    #     echo "RESULT OF: ------------------------------------------------------------------------------"
    #     echo \'${RESULT}\'
    #     if [ "${RESULT}" = "Node ready." ]
    #     then 
    #         echo "DONE CONNECTING-----------------------------------------------------"
    #         CONNECTED=true
    #     else
    #         echo "NOT CONNECTED-----------"
    #         sleep 5
    #     fi
    # done
done
echo "NODE ${NODE_NUMBER} COMPLETE=============================================================="
