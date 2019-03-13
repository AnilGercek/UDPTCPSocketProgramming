Anıl Gerçek - 1942093
Emre Barış Çoşkun - 1949338

To run the program:
1. In the broker node: 
        $ g++ broker.cpp -o broker -std=c++11
        $ ./broker
2. In the router 1 node: 
        $ g++ router1.cpp -o router1 -std=c++11
        $ ./router1
3. In the router 2 node: 
        $ g++ router2.cpp -o router2 -std=c++11
        $ ./router2
4. In the destination node:
        $ g++ destination.cpp -o destination -std=c++11
        $ ./destination
5. In the source node: 
        $ g++ source.cpp -o source -std=c++11
        $ ./source <message>       

This program calculates Rounding Trip Time (RTT) by sending message to destination and send back from destination to source.
Then, program uses this formula to calculate end to end delay:
        end-to-end delay = RTT/2

Experiment 1:
    First, delays are configured for this experiment. Here is the terminal commands to config for each node.
    Broker node:
        sudo tc qdisc add dev eth1 root netem delay 1ms 5ms
        sudo tc qdisc add dev eth2 root netem delay 1ms 5ms
    Router1 node:
        sudo tc qdisc add dev eth1 root netem delay 1ms 5ms
        sudo tc qdisc add dev eth2 root netem delay 1ms 5ms
    Router2 node:
        sudo tc qdisc add dev eth1 root netem delay 1ms 5ms
        sudo tc qdisc add dev eth2 root netem delay 1ms 5ms
    Destination node:
        sudo tc qdisc add dev eth1 root netem delay 1ms 5ms
        sudo tc qdisc add dev eth2 root netem delay 1ms 5ms
    After configs are done. We sent messages to destination node. This program outputs time passed between connections. 
    By using these outputs we gathered necessary data to make calculations and plot the graph.

Experiment 2:
    First, delays are configured for this experiment. Here is the terminal commands to config for each node.
    Broker node:
        sudo tc qdisc change dev eth1 root netem delay 20ms 5ms
        sudo tc qdisc change dev eth2 root netem delay 20ms 5ms
    Router1 node:
        sudo tc qdisc change dev eth1 root netem delay 20ms 5ms
        sudo tc qdisc change dev eth2 root netem delay 20ms 5ms
    Router2 node:
        sudo tc qdisc change dev eth1 root netem delay 20ms 5ms
        sudo tc qdisc change dev eth2 root netem delay 20ms 5ms
    Destination node:
        sudo tc qdisc change dev eth1 root netem delay 20ms 5ms
        sudo tc qdisc change dev eth2 root netem delay 20ms 5ms
    After configs are done. We sent messages to destination node. This program outputs time passed between connections. 
    By using these outputs we gathered necessary data to make calculations and plot the graph.

Experiment 3:
    First, delays are configured for this experiment. Here is the terminal commands to config for each node.
    Broker node:
        sudo tc qdisc change dev eth1 root netem delay 60ms 5ms
        sudo tc qdisc change dev eth2 root netem delay 60ms 5ms
    Router1 node:
        sudo tc qdisc change dev eth1 root netem delay 60ms 5ms
        sudo tc qdisc change dev eth2 root netem delay 60ms 5ms
    Router2 node:
        sudo tc qdisc change dev eth1 root netem delay 60ms 5ms
        sudo tc qdisc change dev eth2 root netem delay 60ms 5ms
    Destination node:
        sudo tc qdisc change dev eth1 root netem delay 60ms 5ms
        sudo tc qdisc change dev eth2 root netem delay 60ms 5ms
    After configs are done. We sent messages to destination node. This program outputs time passed between connections. 
    By using these outputs we gathered necessary data to make calculations and plot the graph.