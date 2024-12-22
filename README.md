# CProtocol-UDP-TCP

**Objectif du TP** : Développer une communication rudimentaire entre deux processus, en adoptant une architecture de type client/serveur. La communication entre le client et le serveur se fera en utilisant l'interface de communication des sockets.

## Compilation :
```bash
# UDP Client / Server
g++ -o out/server_udp server_udp.cpp
g++ -o out/client_udp client_udp.cpp

# TCP Client / Server
g++ -o out/server_tcp server_tcp.cpp
g++ -o out/client_tcp client_tcp.cpp
```

## Utilisation :
```bash
$ out/server_udp
$ out/client_udp <address>
```
```bash
$ out/server_tcp
$ out/client_tcp <address>
```
