Banque avec guichets multiples :
* Description : Une banque avec plusieurs guichets sert des clients (threads). Les clients effectuent des dépôts, des retraits ou consultent leurs soldes. Les guichets doivent coordonner l’accès à un compte partagé.

* Problème :
    * Éviter que deux threads modifient le même compte simultanément.
    * Gérer les priorités entre les clients.
* Concepts impliqués :
    * Mutex pour protéger les opérations sur les comptes.
    * Lecteurs-rédacteurs pour autoriser les lectures simultanées mais pas les écritures.
    
* Bonus : Simule une fraude ou une surcharge des guichets.