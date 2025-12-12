# API Security 

API security protects REST services from unauthorized access, misuse, 
and attacks by ensuring only trusted users and systems can 
interact with them. 

* Introduction
    - [OWASP API-Security Top10](introduction/OWASP-API-Security-Top10.md)

* Transport Layer Security  
    - Example: [flask-article-service-tls](tls/article-service-tls/)
    - Example: [article-service-client-tls](tls/article-service-client-tls/)

* Authentication
    - [Basic Authentication](authentication/introduction/README.md)
    - Example: [flask-article-service-auth](authentication/article-service-auth/)
    - Example: [article-service-client-auth](authentication/article-service-client-auth/)
    - Exercise: [flask-book-service](authentication/book-service-exercise/)
        ([Model Solution](authentication/book-service/))

* Authorization
    - [Role Based Authorization](authorization/introduction/README.md)
    - Method Level Authorization
        - Example: [flask-articla-service-roles](authorization/method-level/article-service-roles/)
    - Object Level Authorization
        - Example: [flask-article-service-uuid](authorization/object-level/article-service-uuid/)    

_Egon Teiniker, 2025, GPL v3.0_