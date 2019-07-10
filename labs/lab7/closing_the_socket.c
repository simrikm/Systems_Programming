/**Here i am reading the meddage from the cient if the client types logout it will end the sessionthis code should be placed after you read the message from the client in the server**/
if(strcmp(buf, "logout")==0){
	getpeername(sock_fd,(struct sockaddr*)&their_addr,(socklen_t*)&sin_size);
	printf("%s disconnected",inet_ntoa(their_addr.sin_addr);
	close(sock_fd);
	proc[i]=0;

}
