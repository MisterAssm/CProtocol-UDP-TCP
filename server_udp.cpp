#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#define BOLD "1"

#define DEFAULT "0"
#define RED "91"
#define GREEN "92"
#define BLUE "94"

#define BOLD_RED BOLD ";" RED

// Structure pour associer texte et couleur
struct ColoredText {
  std::string text;
  std::string colorCode;
};

void printColoredParts(const std::vector<ColoredText> &parts) {
  for (const auto &part : parts) {
    std::cout << "\033[" << part.colorCode << "m" << part.text << "\033[0m";
  }
  std::cout << std::endl; // Nouvelle ligne après l'affichage
}

void printError(const std::vector<ColoredText> &parts) {
  std::vector<ColoredText> errorMessage = {{"Erreur: ", BOLD_RED}};
  errorMessage.insert(errorMessage.end(), parts.begin(), parts.end());
  printColoredParts(errorMessage);
}

void printError(const std::string &errorMessage) {
  printError({{errorMessage, DEFAULT}});
}

/*
 * Code du serveur
 */

/* Port local du serveur */
#define PORT 9600
#define IP_ADRESS "127.0.0.1"
#define MAX_RECEPTION_SIZE 20 // 20 caractères maximum

int main(int argc, char *argv[]) {

  printColoredParts({{"Information sur le serveur", BLUE}});
  printColoredParts({{"- Adresse IP : ", BLUE}, {IP_ADRESS, GREEN}});
  printColoredParts({{"- Port : ", BLUE}, {std::to_string(PORT), GREEN}});
  printColoredParts({{"- Protocole : ", BLUE}, {"UDP", RED}});

  /*
   * Variables du serveur
   *
   * Déclarer ici les variables suivantes :
   * - sockfd le descripteur de socket
   * - structure d'adresse locale du serveur
   * - structure d'adresse du client
   * - taille de l'adresse du client
   *
   */

  int sockfd;
  struct sockaddr_in server_adress, client_adress; // structure d'adresse serveur / client

  // type int non compatible pour l'appel de recvfrom
  socklen_t client_adress_length = sizeof(client_adress); // Taille de l'@ du client

  /*
   * Code du serveur
   *
   * - Ouvrir le socket du serveur
   * - Remplir la structure d'adresse locale du serveur : (fait au dessus)
   *   - la famille d'adresse
   *   - l'adresse IP
   *   - le port
   *
   * - Spécifier l'adresse locale du socket du serveur
   */

  // Ouverture socket réseau
  // Protocole IP, socket orienté datagramme, 0 car SOCK_DGRAM correspond à UDP
  sockfd = socket(PF_INET, SOCK_DGRAM, 0);

  if (sockfd == -1) {
    printError("Échec lors de la création du socket.");
    return -1;
  }

  printColoredParts({{"Socket créé avec succès.", GREEN}});

  // Remplir structure d'adresse locale
  server_adress.sin_family = AF_INET;
  server_adress.sin_port = htons(PORT); // conversion int vers type compatible modèle IP
  server_adress.sin_addr.s_addr = inet_addr(IP_ADRESS); // conversion string vers type compatible modèle IP

  // on cast server_adress en sockaddr
  if (bind(sockfd, reinterpret_cast<struct sockaddr *>(&server_adress),
           sizeof(server_adress)) == -1) {
    printError("Échec lors du bind.");
    close(sockfd);
    return -1;
  }

  while (1) {
    printColoredParts({{"\nEn attente de récéption de données...", BLUE}});

    // MAX_RECEPTION_SIZE + 1 : pour réserver l'espace d'un caractère nul
    char buffer[MAX_RECEPTION_SIZE + 1];
    ssize_t bytes_received =
        recvfrom(sockfd, buffer, MAX_RECEPTION_SIZE, 0,
                 reinterpret_cast<struct sockaddr *>(&client_adress),
                 &client_adress_length);

    if (bytes_received == -1) {
      printError("Échec lors de la réception des données.");
      close(sockfd);
      return -1;
    }

    buffer[bytes_received] = '\0'; // caractère nul (fin de chaîne)

    char client_ip[INET_ADDRSTRLEN]; // adresse IP en format texte

    std::string client_info = inet_ntop(AF_INET, &(client_adress.sin_addr), client_ip, INET_ADDRSTRLEN); // Convertit l'adresse ip binaire en texte
    client_info += ":" + std::to_string(ntohs(client_adress.sin_port)); // Convertit le port

    // Affichage des données
    std::string message = "Données reçues de " + client_info + " (" + std::to_string(bytes_received) + " octets) :";
    printColoredParts({{message, GREEN}});

    ssize_t bytes_sent = write(STDOUT_FILENO, buffer, bytes_received);

    if (bytes_sent == -1) {
      printError("Échec lors de l'affichage des données.");
      close(sockfd);
      return -1;
    }

    std::cout << std::endl;

    // Réinitialisation du buffer (problèmes d'affichage)
    std::memset(buffer, '\0', MAX_RECEPTION_SIZE);
  }

  if (close(sockfd) == -1) {
    printError("Échec lors de la fermeture du socket.");
    return -1;
  }

  printColoredParts({{"Socket fermé avec succès.", GREEN}});
  return 0;
}