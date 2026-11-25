# Basic Authentication 

Basic Authentication is the simplest protocol available for performing 
authentication over HTTP. 
It involves sending a **Base64–encoded username and password** within 
a request header to the server. 

## Protocol

* An unauthorized client tries to access a secure service.

* The server would reply with an HTTP response of: 

    ```
    HTTP/1.1 401 Unauthorized 
    WWW-Authenticate: Basic realm="ApplicationRealm"
    ```

    The **401** response tells the client that it is **not authorized** 
    to access the URI it tried to invoke on. 

    The WWW-Authenticate header specifies which authentication protocol 
    the client should use. 
    **Basic** means Basic Authentication should be used. 

    The **realm** attribute identifies a collection of secured resources 
    on a website. 

* To perform authentication, the client must send a request with the 
    Authorization header set to a Base 64–encoded string of our 
    username and a colon character, followed by the password:
    
    ```
    Authorization: Basic c3R1ZGVudDpzdHVkZW50
    ```
    
    The **client needs to send this Authorization header with each request** 
    it makes to the server.

    Don’t forget to use an **encrypted connection** like HTTPS!


## References

* Prabath Siriwardena. **Advanced API Security**. Apress 2019

* Prabath Siriwardena, Nuwan Dias. **Microservices Security in Action**. MANNING 2020

_Egon Teiniker, 2025, GPL v3.0_
