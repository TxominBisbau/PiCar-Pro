#include "manette.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
#include <cstring>
#include <iostream>
 
static int g_sock = -1;
static EtatManette g_dernierEtat;
 
bool Manette::init(int port) {
    g_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (g_sock < 0) {
        std::cerr << "[MANETTE] Erreur creation du socket\n";
        return false;
    }
 
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
 
    if (bind(g_sock, (sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "[MANETTE] Erreur bind sur le port " << port << "\n";
        return false;
    }

    int flags = fcntl(g_sock, F_GETFL, 0);
    fcntl(g_sock, F_SETFL, flags | O_NONBLOCK);
 
    std::cout << "[MANETTE] En ecoute sur le port " << port << "\n";
    return true;
}
 
void Manette::cleanup() {
    if (g_sock >= 0) close(g_sock);
}
 
EtatManette Manette::read() {
    char buf[256];
    ssize_t n = recv(g_sock, buf, sizeof(buf) - 1, 0);
 
    if (n > 0) {
        buf[n] = '\0';
 
        int xL, yL, xR, yR;
        unsigned int pov;
        unsigned long btn;
 
        int nbLu = sscanf(buf, "X_L=%d;Y_L=%d;X_R=%d;Y_R=%d;POV=%u;BTN=%lu",
                           &xL, &yL, &xR, &yR, &pov, &btn);
 
        if (nbLu == 6) {
            g_dernierEtat.xL = xL;
            g_dernierEtat.yL = yL;
            g_dernierEtat.xR = xR;
            g_dernierEtat.yR = yR;
            g_dernierEtat.pov = pov;
            g_dernierEtat.btn = btn;
        }
    }
 
    return g_dernierEtat;
}
 



