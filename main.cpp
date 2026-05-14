#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Produit {
    int id;             
    char nom[50];       
    float prix;         
    int quantite;       
};

struct Client {
    int id;             
    char nom[50];       
    char telephone[20]; 
};

struct Produit inventaire[100]; 
struct Client annuaire[100];    
int nbProduits = 0;             
int nbClients = 0;              


void chargerProduits() {
    FILE *f = fopen("produit.txt", "r");
    if (f != NULL) {
        while (fscanf(f, "%d %s %f %d", &inventaire[nbProduits].id, inventaire[nbProduits].nom, 
                      &inventaire[nbProduits].prix, &inventaire[nbProduits].quantite) == 4) {
            nbProduits++;
        }
        fclose(f);
    }
}


void sauvegarderProduits() {
    FILE *f = fopen("produit.txt", "w");
    if (f != NULL) {
        for (int i = 0; i < nbProduits; i++) {
            fprintf(f, "%d %s %.2f %d\n", inventaire[i].id, inventaire[i].nom, 
                    inventaire[i].prix, inventaire[i].quantite);
        }
        fclose(f);
    }
}


void chargerClients() {
    FILE *f = fopen("client.txt", "r");
    if (f != NULL) {
        while (fscanf(f, "%d %s %s", &annuaire[nbClients].id, annuaire[nbClients].nom, 
                      annuaire[nbClients].telephone) == 3) {
            nbClients++;
        }
        fclose(f);
    }
}


void ajouterProduit() {
    printf("Entrez l'ID du produit : ");
    scanf("%d", &inventaire[nbProduits].id);
    printf("Nom du produit : ");
    scanf("%s", inventaire[nbProduits].nom);
    printf("Prix unitaire : ");
    scanf("%f", &inventaire[nbProduits].prix);
    printf("Quantite initiale : ");
    scanf("%d", &inventaire[nbProduits].quantite);
    
    nbProduits++;
    sauvegarderProduits(); 
    printf("Produit ajoute !\n");
}

void afficherProduits() {
    printf("\n--- LISTE DES PRODUITS ---\n");
    for (int i = 0; i < nbProduits; i++) {
        printf("ID: %d | Nom: %s | Prix: %.2f | Stock: %d\n", 
               inventaire[i].id, inventaire[i].nom, inventaire[i].prix, inventaire[i].quantite);
    }
}


void ajouterClient() {
    printf("Entrez l'ID du client : ");
    scanf("%d", &annuaire[nbClients].id);
    printf("Nom du client : ");
    scanf("%s", annuaire[nbClients].nom);
    printf("Telephone : ");
    scanf("%s", annuaire[nbClients].telephone);

    
    FILE *f = fopen("client.txt", "a");
    if (f != NULL) {
        fprintf(f, "%d %s %s\n", annuaire[nbClients].id, annuaire[nbClients].nom, annuaire[nbClients].telephone);
        fclose(f);
    }
    nbClients++;
    printf("Client ajoute !\n");
}

void afficherClients() {
    printf("\n--- LISTE DES CLIENTS ---\n");
    for (int i = 0; i < nbClients; i++) {
        printf("ID: %d | Nom: %s | Tel: %s\n", 
               annuaire[i].id, annuaire[i].nom, annuaire[i].telephone);
    }
}


void effectuerVente() {
    int idC, idP, qte, indexP = -1;
    
    printf("Saisir ID client : "); scanf("%d", &idC);
    printf("Saisir ID produit : "); scanf("%d", &idP);

    for (int i = 0; i < nbProduits; i++) {
        if (inventaire[i].id == idP) {
            indexP = i;
            break;
        }
    }

    if (indexP == -1) {
        printf("Erreur : Produit introuvable.\n");
        return;
    }

    printf("Quantite demandee : "); scanf("%d", &qte);

    
    if (inventaire[indexP].quantite < qte) {
        printf("Stock insuffisant ! Approvisionnement de 50 unites...\n");
        inventaire[indexP].quantite += 50; 
    }

    inventaire[indexP].quantite -= qte;
    float total = qte * inventaire[indexP].prix;

    
    FILE *fv = fopen("Ventes.txt", "a");
    if (fv != NULL) {
        fprintf(fv, "Client:%d Produit:%s Qte:%d Total:%.2f\n", 
                idC, inventaire[indexP].nom, qte, total);
        fclose(fv);
    }

    sauvegarderProduits(); 
    printf("Vente reussie ! Montant total : %.2f\n", total);
}

void afficherHistorique() {
    FILE *f = fopen("Ventes.txt", "r");
    char ligne[255];
    if (f == NULL) {
        printf("Aucun historique disponible.\n");
        return;
    }
    printf("\n--- HISTORIQUE DES VENTES ---\n");
    while (fgets(ligne, 255, f) != NULL) {
        printf("%s", ligne);
    }
    fclose(f);
}

int main() {
    chargerProduits();
    chargerClients();

    int choix;
    do {
        printf("\n--- MENU DE GESTION BOUTIQUE DE ROLLAND ---\n");
        printf("1. Ajouter un produit\n2. Afficher les produits\n3. Ajouter un client\n");
        printf("4. Afficher les clients\n5. Effectuer une vente\n6. Voir historique\n7. Quitter\n");
        printf("Choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1: ajouterProduit(); break;
            case 2: afficherProduits(); break;
            case 3: ajouterClient(); break;
            case 4: afficherClients(); break;
            case 5: effectuerVente(); break;
            case 6: afficherHistorique(); break;
            case 7: printf("Sortie du programme...\n"); break;
            default: printf("Choix invalide !\n");
        }
    } while (choix != 7);

    return 0;
}
