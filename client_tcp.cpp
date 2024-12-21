#include <cstring>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <ostream>
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
 * Code du client
 */

#define SIZE 100
#define PORT 9600

int main(int argc, char *argv[]) {
    if (argc < 2) {
      printError({{"Arguments invalides : ", DEFAULT},
                  {"./client_tcp 127.0.0.1", BLUE}});
      return -1;
    }

  /*
   * Variables du client
   *
   * Déclarer ici les variables suivantes :
   * - stockfd le descripteur de socket
   * - structure d'adresse du serveur
   * - pointeur vers la structure descriptive de machine (hostent)
   * - zone de mémoire destinée à accueillir la chaîne
   *   entrée au clavier
   * - taille de la chaîne à envoyer
   *
   */

   int sockfd;
   struct sockaddr_in server_adress; // structure d'adresse serveur
   struct hostent *host_details;     // pointeur structure descriptive de machine
   char buffer[SIZE];                // zone mémoire pour accueillir la chaîne
   size_t buffer_length = 0;         // taille chaîne à envoyer

  /*
   * Code du client
   *
   * - Ouvrir le socket du client
   * - Récupérer l'adresse IP du serveur à partir de son nom
   *   donné en ligne de commande
   * - Remplir la structure d'adresse du serveur
   * - Lire une ligne de l'entrée standard
   * - Envoyer la chaîne lue au serveur
   */

   // Ouverture socket client
  sockfd = socket(PF_INET, SOCK_STREAM, 0);

  if (sockfd == -1) {
    printError("Échec lors de la création du socket.");
    return -1;
  }

  // Récupérer l'adresse IP par le nom d'hôte fourni
  host_details = gethostbyname(argv[1]);

  if (host_details == NULL) {
    printError("Échec lors de la recherche du serveur.");
    close(sockfd);
    return -1;
  }

  // Remplir structure d'adresse
  memset(&server_adress, 0, sizeof(server_adress));
  server_adress.sin_family = AF_INET;
  server_adress.sin_port = htons(PORT);
  memcpy(&server_adress.sin_addr.s_addr, host_details->h_addr,
         host_details->h_length);

  if (connect(sockfd, (struct sockaddr *)&server_adress, sizeof(server_adress)) == -1) {
      printError("Connexion au serveur refusée.");
      close(sockfd);
      return -1;
  }

  // Lecture sur entrée standard
  printColoredParts({{"Saisir le contenu du fichier à envoyer au serveur :", BLUE}});

  buffer_length = read(STDIN_FILENO, buffer, sizeof(buffer));

  if (buffer_length == 0) {
    printError("Message vide.");
    close(sockfd);
    return 0;
  }

  if (write(sockfd, buffer, buffer_length) == -1)  {
      printError("Échec de l'envoi du message vers le serveur.");
      close(sockfd);
      return -1;
  }

  printColoredParts({{"Message envoyé avec succès.", GREEN}});

  if (close(sockfd) == -1) {
    printError("Échec lors de la fermeture du socket.");
    return -1;
  }

  printColoredParts({{"Socket fermé avec succès.", GREEN}});

  return 0;
}
