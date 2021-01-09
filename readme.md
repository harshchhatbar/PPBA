1. client.cpp --> Client-side c++ code

2. server.cpp --> Server-side c++ code

3. clientTOserver.txt --> Whenever client wants to send data to server it will put data in this file

4. serverTOclient.txt --> Whenever server wants to send data to client it will put data in this file

5. mean_Of_EachPixel.txt --> Storing mean of each pixel of all Images.

6. test.txt --> Storing test_images

7. New_test.txt --> Storing images that are stored in server database.

How to do authentication process:

1) Execute client.cpp using below command line
--> g++ -o client client.cpp -lpari -fpermissive
	
2) Execute server.cpp using below command line
--> g++ -o server server.cpp -lpari -fpermissive
	
3) Finally, Run client code using below command line
--> ./client p           (Where p can be any number from 1 to 80)  
