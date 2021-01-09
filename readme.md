* client.cpp --> Client-side c++ code
* server.cpp --> Server-side c++ code
* clientTOserver.txt --> Whenever client wants to send data to server it will put data in this file
* serverTOclient.txt --> Whenever server wants to send data to client it will put data in this file
* mean_Of_EachPixel.txt --> Storing mean of each pixel of all Images.
* test.txt --> Storing test_images
* New_test.txt --> Storing images that are stored in server database.

How to do authentication process:

* Execute client.cpp using below command line
	g++ -o client client.cpp -lpari -fpermissive
	
* Execute server.cpp using below command line
	g++ -o server server.cpp -lpari -fpermissive
	
* Finally, Run client code using below command line
	./client p           (Where p can be any number from 1 to 80)  
