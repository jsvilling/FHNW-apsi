// Looks for new input file, reads it deletes it
// Performs service on it: To upper case
// Note: The only valid input is 3 lowercase letters!
// Writes result to output file


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


void on_error(int err, char * msg) {  // more convenient error output
  if (err == -1) {                    // Tests whether 'err' is -1 and
    perror(msg);                      // prints error and msg if so.
    exit(-1);
  }
}

int main () {
  char s[100];
  int i;
  int res;
  int inf, outf;
  ssize_t in_len, out_len, wr_len, rd_len;

  
  // prepare communication file names
  char * in_file = "input_data";
  char * in_file_tmp = "input_data.tmp";
  char * out_file = "result_data";
  char * out_file_tmp = "result_data.tmp";
  

  // main loop
  while (1) {
    // wait for data
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
    rd_len = read(inf, s, 100);
    on_error(rd_len, "error reading file");
    res = close(inf);
    on_error(res, "error closing file after read");
    res = unlink(in_file);
    on_error(res, "error unlinking input file");
    
    // convert to uppercase
    for (i = 0; i < 3; i++) {
      s[i] = s[i] - 32;
    }  
    
    // send result
    outf = open(out_file_tmp, O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
    on_error(outf, "error creating output file");
    wr_len = write(outf, s, rd_len);
    on_error(wr_len, "error creating output file");
    if (wr_len != rd_len) {
      fprintf(stderr, "short write to disk");
      exit(-1);
    }
    res = close(outf);
    on_error(res, "error creating output file");
    // rename
    res = rename(out_file_tmp, out_file);
    on_error(res, "error renaming file");
  }
}  
  
  

