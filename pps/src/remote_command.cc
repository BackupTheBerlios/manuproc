
#include <unistd.h>
#include <string>
#include <vector>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <Artikel/ArtikelBezeichnung.h>


int fdes_c2s[]={-1,-1};
int fdes_s2c[]={-1,-1};

extern char **env;


void RC_close()
{
 if(fdes_c2s[1] || fdes_s2c[0])
   {
    write(fdes_c2s[1],"STOP",4); 
    write(fdes_c2s[1],"\x0a",1); 

    close(fdes_c2s[1]);
    close(fdes_s2c[0]);
   }

 fdes_c2s[0]=fdes_c2s[1]=-1;
 fdes_s2c[0]=fdes_s2c[1]=-1;
}


int RC_OffenMenge(cH_ArtikelBezeichnung b, ManuProC::Datum &d)
{
 int forkedpid=0;

 if(fdes_s2c[0]==fdes_s2c[1] &&
	fdes_c2s[0]==fdes_c2s[1] &&
	fdes_c2s[0]==fdes_s2c[0] &&
	fdes_c2s[0]==-1) 
	// pipes must be created first
 {
  if(pipe(fdes_c2s) <0 || pipe(fdes_s2c)<0)
    {fdes_c2s[0]=fdes_c2s[1]=-1;
     fdes_s2c[0]=fdes_s2c[1]=-1;
     return -1;
    }

 if((forkedpid=fork()) < 0)
    {fdes_c2s[0]=fdes_c2s[1]=-1;
     fdes_s2c[0]=fdes_s2c[1]=-1;
     return -1;
    }   

  if(forkedpid>0) // client (parent)
    {
     close(fdes_c2s[0]);
     close(fdes_s2c[1]);

    }
  else		// server (child)
    {
     close(fdes_c2s[1]);
     close(fdes_s2c[0]);
    
     char *args[]={"sh",
		"-c",
		" /usr/bin/ssh -2 -C -i ~/.ssh/query_rsa "
		" mabella@petig-baender.dyndns.org "
		" ./bin/offen_fuer_artikel ",
		NULL};

     dup2(fdes_c2s[0],0);
     dup2(fdes_s2c[1],1);

    execve("/bin/sh",args,env);

	int err=errno;
   	std::cerr << "execve failed (errno:" << err << "):\n"
   	 	<< args[0] << "\t"
   	 	<< args[1] << "\t"    	 	
   	 	<< args[2] << "\t"
   	 	<< args[3] << "\t"     	 	
   	 	<< args[4] << "\n";	
// 	 	<< args[5] << "\n";	

     exit(1);
    }

 }

 fd_set fds;
 fd_set expt_fds;
 struct timeval tv;
 int ret;

 FD_ZERO(&fds);
 FD_ZERO(&expt_fds);
 FD_SET(fdes_c2s[1],&fds);
 FD_SET(fdes_c2s[1],&expt_fds);
 tv.tv_sec=15;
 tv.tv_usec=0;
 
 ret=select(fdes_c2s[1]+1,NULL,&fds,&expt_fds,&tv);

 if(ret==-1) {perror("select() error"); return -1;}
 
 if(ret==0)
	{perror("ssh: write would block"); return -1;}  

 if(FD_ISSET(fdes_c2s[1],&expt_fds))
	{perror("ssh: exception on pipe"); return -1;}  

 if(signal(SIGPIPE,SIG_IGN)==SIG_ERR)
   {perror("ssh: signal error"); return -1;}  

 std::string komp;
 ret=write(fdes_c2s[1],"QUERY",sizeof "QUERY");
 if(ret==-1)
    {
     close(fdes_c2s[1]);
     close(fdes_s2c[0]);
     fdes_c2s[0]=fdes_c2s[1]=-1;
     fdes_s2c[0]=fdes_s2c[1]=-1;
     wait(NULL);
     return -1;
    }
   
 write(fdes_c2s[1],"\x0a",1);
 komp=b->Komponente(0);
 write(fdes_c2s[1],komp.c_str(),komp.size()); 
 write(fdes_c2s[1],"\x0a",1); 
 komp=b->Komponente(1);
 write(fdes_c2s[1],komp.c_str(),komp.size()); 
 write(fdes_c2s[1],"\x0a",1); 
 komp=b->Komponente(2);
 write(fdes_c2s[1],komp.c_str(),komp.size()); 
 write(fdes_c2s[1],"\x0a",1); 
 komp=b->Komponente(3);
 write(fdes_c2s[1],komp.c_str(),komp.size()); 
 write(fdes_c2s[1],"\x0a",1); 

 const int buflen=20;
 char buf[buflen];
 int len;

 FD_ZERO(&fds);
 FD_ZERO(&expt_fds);
 FD_SET(fdes_s2c[0],&fds);
 FD_SET(fdes_s2c[0],&expt_fds);
 tv.tv_sec=20;
 tv.tv_usec=0;
 
 ret=select(fdes_s2c[0]+1,&fds,NULL,&expt_fds,&tv);

 if(ret==-1) {perror("select() error "__FILELINE__); return -1;}
 
 if(ret==0)
	{perror("ssh: read would block"); return 0;}  

 if(FD_ISSET(fdes_s2c[0],&expt_fds))
	{perror("ssh: exception on pipe"); return -1;}  

 len=read(fdes_s2c[0],buf,buflen);
 if(len<buflen)
   buf[len]=0;
 else buf[buflen-1]=0;  

 int menge(atoi(buf));

 if(menge==-1) return menge;

 char *datum=strstr(buf,"|");
 if(datum)
   d.from_postgres(datum+1);
 
 return menge;
 
}



