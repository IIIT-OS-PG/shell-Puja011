#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <pwd.h>
#include <glob.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <iostream> 
#include<fstream>
#define COMMAND_BUFSIZE 1024

using namespace std;


int pipeval=0;


void HistoryShell(char* line)
{
FILE *fp;
fp = fopen("ShellHistory.txt","a");
fprintf(fp,"%s\n",&line[0]);
fclose(fp);

}



void InitialiseBashrc()
{


//char *buffer = (char*)(malloc(sizeof(char)));
long int n1;
int fsrc;
char *s1 =																																																																																																																												 getenv("HOME");
char *s2 = getenv("LOG");
char *s3 = getenv("USER");
char *s4 = getenv("PATH");
FILE *fptr;
fptr = fopen("MyBash.txt", "w+");
if(fptr == NULL)
   {
      printf("Error!");
      exit(1);
   }

fprintf(fptr,"%s\n", s1);
fprintf(fptr,"%s\n", s2);
fprintf(fptr,"%s\n", s3);
fprintf(fptr,"%s\n", s4);
   fclose(fptr);


}




void ExecuteNp(char** arr)
{

pid_t p1,p2;
p1 = fork();
int p;

if(p1<0)
{
printf("error");
}

if(p1==0)
execvp(arr[0], arr);

else 
wait(&p);


}


struct shell_info {
    char cur_user[64];
    char cur_host[64];
    char cur_dir[1024];
    char pw_dir[1024];
    char ps1[64];
};

struct shell_info *shell;
int set_ps1 = 0;



char** parsenonpipe(char* line)
{
//cout<<"\nin parse space\n";
char** arr=(char**)malloc(sizeof(char));
char* pch;
pch = strtok(line," \n");
int i =0;
while(pch!=NULL)
{
arr[i++] = pch;
//printf("%s\n",arr[i-1]);
//cout<<"\n"<<arr[i];
pch = strtok(NULL," \n");
}


return arr;

}



void shellinit() {
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
//    signal(SIGTSTP, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
   InitialiseBashrc();


    pid_t pid = getpid();
    setpgid(pid, pid);
    tcsetpgrp(0, pid);

    shell = (struct shell_info*) malloc(sizeof(struct shell_info));
    getlogin_r(shell->cur_user, sizeof(shell->cur_user));

    struct passwd *pw = getpwuid(getuid());
    strcpy(shell->pw_dir, pw->pw_dir);
    gethostname(shell->cur_host, 20);

    getcwd(shell->cur_dir, sizeof(shell->cur_dir));
}


void type_promt() {
  	printf("%s@%s:%s\n", shell->cur_user, shell->cur_host, shell->cur_dir);
	gid_t gid = getegid();
	if(set_ps1){
		printf("%s", shell->ps1);
		return;	
	}
	if(gid == 0){
		printf("root(#)");
	}
	else{
		printf("user($)");	
	}
}


char* shellread_line()
{
int bufsize = COMMAND_BUFSIZE;
    int position = 0;
   int countredir=0;
    char *buffer = (char*)malloc(sizeof(char) * bufsize);
//printf("\n after buffer");
    int p;

     if (!buffer) {
        fprintf(stderr, "shell: allocation error\n");
        exit(1);
    }

while(1)
{

p = getchar();
//printf("\n in getchar\n");

if (p == EOF || p == '\n') {
            buffer[position] = '\0';
            return buffer;
        } 

if(p == '~'){
			buffer[position++] = '/';
			buffer[position++] = 'h';
			buffer[position++] = 'o';
			buffer[position++] = 'm';
			buffer[position] = 'e';						
		}


if(p=='|')
pipeval++;


if(p =='>')
{
countredir=1;

}

if (position >= bufsize) {
            bufsize += COMMAND_BUFSIZE;
            buffer = (char*)realloc(buffer, bufsize);
            if (!buffer) {
                fprintf(stderr, "shell: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

else{
			buffer[position] = p;
		}
position++;




}

if(countredir==1)
{
FILE *fptr;
int buffersize = COMMAND_BUFSIZE;
   int countsig = 0,x=0;
char** take1,take2;
int fd;
    char** redir = (char**)malloc(sizeof(char) * buffersize);
   //char** redir2 = (char*)malloc(sizeof(char) * buffersize);
char* pch;

pch = strtok(buffer," ");

while(pch!=NULL)
{
redir[x++] = pch;
if(redir[x]==">")
{
redir[x] = NULL;
}
pch = strtok(buffer," ");
}

fd = open(redir[position-1],O_CREAT|O_WRONLY|O_TRUNC, 0700);
if(fork()==0)
{
dup2(fd,1);

execvp(redir[0],redir);
}

}


}




void shellloop() {
    char *line;
char** takeval;
  char command[100];
    int status = 1;
  InitialiseBashrc();
char c;
FILE *fptr;// for reading input from terminal

    while (1) {
        type_promt();
         line = shellread_line();
        HistoryShell(line);
        fstream f;

takeval = parsenonpipe(line);

if(strcmp((line),"history")==0)
{
fptr = fopen("ShellHistory.txt","r");
if(fptr==NULL)
{
printf("cannot open the file");
}
 c = fgetc(fptr);
while(c != EOF)
{
printf("%c",c);
c = fgetc(fptr);

}

fclose(fptr);



}

if(strcmp((line),"$PATH")==0)
{
string str;
 f.open("MyBash.txt");

int count1=1;
while(count1<=4)
{
getline(f, str); 
if(count1==4)
cout<<str<<"\n";
count1++;
}


}

if(strcmp((line),"$HOME")==0)
{
string str;
 f.open("MyBash.txt");

int count2=1;
while(count2<=4)
{
getline(f, str); 
if(count2==1)
cout<<str<<"\n";
count2++;
}


}


if(strcmp((line),"$LOG")==0)
{
string str;
 f.open("MyBash.txt");

int count4=1;
while(count4<=4)
{
getline(f, str); 
if(count4==2)
cout<<str<<"\n";
count4++;
}


}





if(strcmp((line),"$USER")==0)
{
string str;
 f.open("MyBash.txt");

int count3=1;
while(count3<=4)
{
getline(f, str); 
if(count3==3)
cout<<str<<"\n";
count3++;
}


}
ExecuteNp(takeval);


      



 }
       
}





int main(int argc, char **argv) {
    shellinit();
    shellloop();

    return EXIT_SUCCESS;
}
