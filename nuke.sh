#! /bin/sh -v
pkill multichaind
pkill setup
pkill connectToChain
pkill makeChain
echo -ne "${CHAIN_NAME} ${WALLET_ADRESS}" | nc 127.0.0.1 8081
echo -ne "${CHAIN_NAME} ${WALLET_ADRESS}" | nc 127.0.0.1 8082
echo -ne "${CHAIN_NAME} ${WALLET_ADRESS}" | nc 127.0.0.1 8083
echo -ne "${CHAIN_NAME} ${WALLET_ADRESS}" | nc 127.0.0.1 8084
pkill listener
rm -r node1
rm -r node2
rm -r node3
rm -r node4