
Rörelse sensor --(rå data)--> ESP32.client ----> Laptop/program.client --(TCP?)--> Laptop/program.server ----> ESP32.server ----> buzzer 

TCP hjälp kod för Windows:

Client: https://learn.microsoft.com/en-us/windows/win32/winsock/complete-client-code

Server: https://learn.microsoft.com/en-us/windows/win32/winsock/complete-server-code

*Server programmet ska börja före client

UDP filerna är backup om TCP inte fungerar
