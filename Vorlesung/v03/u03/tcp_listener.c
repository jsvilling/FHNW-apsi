// Binds to a port, accepts the first connection and 
// sends one input line onwards via out_file.
// Waits then for a response via in_file and,
// when that arrives echos that to the user and closes 
// the connection.

// Client: Use, e.g. telnet 127.0.0.1 8500
//              or   telnet localhost 8500
//         on the same machine.
//
// Note:   To exit a telnet-session give <crtl>-] 
//         and then the command "quit"

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

void on_error(int err, char * msg) {  // more convenient error output
  if (err == -1) {                    // Tests whether 'err' is -1 and
    perror(msg);                      // prints error and msg if so.
    exit(-1);
  }
}

int main () {  
  int srv_sock, inst_sock;
  struct sockaddr_in  srv_adr, inst_adr;
  int inst_adr_size;
  char s[100], t[100];
  int res;
  int outf, inf;
  ssize_t in_len, out_len, wr_len, rd_len;

  // prepare communication file names
  char * out_file = "input_data";
  char * out_file_tmp = "input_data.tmp";
  char * in_file = "result_data";
  char * in_file_tmp = "result_data.tmp";
  
  // prepare server address structure
  srv_adr.sin_family = AF_INET;
  srv_adr.sin_port = htons(8500);

  // the following would restrict the connections to allow only connections from the local host on port 8500
  //srv_adr.sin_addr.s_addr = inet_addr("127.0.0.1");                                   
  
  // create server listening socket 
  srv_sock = socket(AF_INET, SOCK_STREAM, 0);
  on_error(srv_sock, "\nSocket creation:\n"); 

  // bind server listening socket to address 
  res = bind(srv_sock, (struct sockaddr *) &srv_adr, 
	     sizeof(srv_adr));
  on_error(res, "\n socket binding:\n");
  
  // set listening socket queue length 
  // lengt = 1 means only one at a time can connect
  // and additional ones get an error
  res = listen(srv_sock, 1);
  on_error(res, "\nQueue setting:\n");

  while (1) {
    // accept requests, 
    inst_adr_size = sizeof(inst_adr);
    inst_sock = accept(srv_sock, (struct sockaddr *) &inst_adr,
                       &inst_adr_size);
    on_error(inst_sock, "\n socket instance creation:\n");

    // now interact with the client on the instance socket.
    // we read exactly the first transfer 
    // with a telnet client, this will be the first line
    // We will read at most 100 chars.
    in_len = read(inst_sock, &s, 100); // waits for input and reads it into s
    on_error(in_len, "error accepting connection");
    if (in_len == 0) {                 // 0 char: client closed connection
      // clean up and wait for next connection
      close(inst_sock);
      continue; 
    }
    // write data to out_file_tmp
    outf = open(out_file_tmp, O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
    on_error(outf, "error creating output file");
    wr_len = write(outf, s, in_len);
    on_error(wr_len, "error creating output file");
    if (wr_len != in_len) {
      fprintf(stderr, "short write to disk");
      exit(-1);
    }  
    res = close(outf);
    on_error(res, "error creating output file");
    // rename
    res = rename(out_file_tmp, out_file);  
    on_error(res, "error renaming file");    
    
    // wait for result
    while (1) {
      inf = open(in_file, O_RDONLY);
      if (inf == -1 && errno == ENOENT) {
        // file does not exist
        sleep(1);
        continue;
      }
      on_error(inf, "error opening input file");
      break;
    }
    // read result
    rd_len = read(inf, t, 100);
    on_error(rd_len, "error reading file");
    res = close(inf);
    on_error(res, "error closing file after read");
    // remove file
    res = unlink(in_file);
    on_error(res, "error unlinking input file");
    
    // echo it to socket
    write(inst_sock, &t, rd_len);
    close(inst_sock);               // clean up
  }  
  close(srv_sock);                // clean up
}
  
