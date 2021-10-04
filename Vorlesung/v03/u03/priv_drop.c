#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <grp.h>
#include <pwd.h>
#include <string.h>
#include <dirent.h>

// The following parameters may need adjustment
// the user to drop privileges to
#define TARGET_USER "vagrant"


void on_error(int err, char * msg) {  
  // more convenient error output
  // Tests whether 'err' is -1 and prints error and msg if so
  if (err == -1) {                
    perror(msg);                  
    exit(-1);
  }
}


struct passwd * prep_privilege_drop() {
  // Prepares the privilege drop
  // As we cannot look up users anymore after the chroot(), we
  // need to do this before. On the other hand, the privilege
  // drop itself needs to be done after the chroot as chroot needs to
  // be done as root. Hence we split the task.
  // Does not return on error. Gives pointer to a passwd struct on success
  gid_t gid;
  struct passwd * user_s;
  int res;
  
  // make sure we are root
  if (getuid() != 0) {
    errno = EPERM;
    on_error(-1, "This needs to be called as root or as SUID root");
  }
  
  // look up gid and uid from names
  user_s = getpwnam(TARGET_USER);
  if (user_s == NULL) {
    on_error(-1, "getpwnam() failed");
  }  
  gid = user_s->pw_gid;

  // Call initgroups() to remove any additional groups root may be in.
  // This prevents unauhorized accesses via those extra groups.
  res = initgroups(TARGET_USER, gid);  
  on_error(res, "initgroups() failed");
  
  return user_s;
}


int do_privilege_drop(struct passwd * user_s) {
  // Does the privilege drop.
  // For simplicity, target user is hardcoded and group is its default group
  // Return value is 0 on success, does not return on error
  int res;
  
  // drop privilege, groups first
  res = setgid(user_s->pw_gid);
  on_error(res, "setgid() failed");
  res = setuid(user_s->pw_uid);
  on_error(res, "setuif() failed");

  // be paranoid: Verify we cannot become root anymore:
  if (setuid(0) != -1) {
    errno = EPERM;   // just something that sounds righ from POSIX.1
    on_error(-1, "Privilege drop verification failed");
  }    
  return 0;
}

void print_current_userinfo() {
  struct passwd *p;
  uid_t  uid;

  if ((p = getpwuid(uid = geteuid())) == NULL)
    perror("getpwuid() error");
  else {
    puts("getpwuid() returned the following info for your userid:");
    printf("  pw_name  : %s\n",       p->pw_name);
    printf("  pw_uid   : %d\n", (int) p->pw_uid);
    printf("  pw_gid   : %d\n", (int) p->pw_gid);
    printf("  pw_dir   : %s\n",       p->pw_dir);
    printf("  pw_shell : %s\n",       p->pw_shell);
  }
}


int main() {  
  struct passwd * user_s;

  // Prepare privilege drop
  user_s = prep_privilege_drop();

  // Do privilege drop
  // Note: This must always be done before the first external input.
  do_privilege_drop(user_s);

  return 0;

}