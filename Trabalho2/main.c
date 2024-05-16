#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

const char log_path[] = "zombies.log";

int run = 1;

void term(int signum) {
  printf("Desligando!!\n");
  run = 0;
}

void create_file() {
  int exist = 1;
  if (!(fopen(log_path, "r"))) {
    exist = 0;
  }

  if(exist) return;

  FILE* log = fopen(log_path, "w+");
  fprintf(log, "PID\tPPID\tNome do Programa\n");
  fprintf(log, "=================================\n");
  fclose(log);
}

void get_zombie_proccess() {
  system("ps -eo pid,ppid,state,comm | awk '$3==\"Z\"' > /tmp/zombie");

  char pid[50], ppid[50], trash1[50], trash2[50], command[50], buffer[50];
  FILE *zombieFile;

  if (!(zombieFile = fopen("/tmp/zombie", "r"))) {
    printf("Erro! Impossivel abrir o arquivo!\n");
    exit(1);
  }

  FILE* log = fopen(log_path, "a+");
  while (!feof(zombieFile)) {
    fscanf(zombieFile, "%s %s %s %s %s", pid, ppid, trash1, command, trash2);
    fprintf(log, "%s\t%s\t%s\n", pid, ppid, command);
  }
  fprintf(log, "=================================\n");
  fclose(log);
}


int main(int argc, char const *argv[]) {
  for(int signal_id = 0; signal_id < 31; signal_id++) {
    if(signal_id == SIGKILL) continue;
    signal(signal_id, SIG_IGN);
  }

  signal(SIGTERM, term);

  int sleep_time;
  if ((sleep_time = (argc == 1) ? 5 : atoi(argv[1])) <= 0) {
    printf("Informe um inteiro positivo e maior que zero como argumento!\n");
    return (1);
  }

  create_file();
  while(run) {
    get_zombie_proccess();
    sleep(sleep_time);
  }

  return 0;
}

