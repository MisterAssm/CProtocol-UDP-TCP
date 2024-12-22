# CProtocol-UDP-TCP

**Objectif du TP** : Développer une communication rudimentaire entre deux processus, en adoptant une architecture de type client/serveur. La communication entre le client et le serveur se fera en utilisant l'interface de communication des sockets.

**Contraintes** : La structure de la requête est une simple chaîne de 20 caractères maximum

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

**Note** : Sur le sujet du TP, le serveur et le client doivent être lancé avec le port d'écoute, or dans les annexes, les ports sont déjà présent. Nous avons donc choisis de ne pas les mettre en argument.

## Contributeurs

* [Assim Zemouchi](mailto:assim.zemouchi@etu-upsaclay.fr?subject=%5BGithub%5D%20CProtocol-UDP-TCP%20%3A)
