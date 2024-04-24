#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// -r: arquivo regular (S_IFREG)
// -d: diretório (S_IFDIR)
// -l: elo simbólico (S_IFLNK)
// -b: dispositivo estruturado (S_IFBLK)
// -c: dispositivo não-estruturado (S_IFCHR)

#define R_FILE 1
#define D_FILE 2
#define L_FILE 3
#define B_FILE 4
#define C_FILE 5

int cnt[6];

int walk_dir(const char *path, void (*func)(const char *)) {
  DIR *dirp;
  struct dirent *dp;
  char *p, *full_path;
  int len;
  /* abre o diretorio */
  if ((dirp = opendir(path)) == NULL)
    return (-1);
  len = strlen(path);
  /* aloca uma area na qual, garantidamente, o caminho cabera */
  if ((full_path = malloc(len + NAME_MAX + 2)) == NULL) {
    closedir(dirp);
    return (-1);
  }
  /* copia o prefixo e acrescenta a '/' ao final */
  memcpy(full_path, path, len);
  p = full_path + len;
  *p++ = '/'; /* deixa "p" no lugar certo! */
  while ((dp = readdir(dirp)) != NULL) {
    /* ignora as entradas "." e ".." */
    if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
      continue;
    strcpy(p, dp->d_name);
    /* "full_path" armazena o caminho */
    printf("Visitando caminho: %s\n", full_path);
    (*func)(full_path);
  }

  free(full_path);
  closedir(dirp);
  return (0);
} /* end walk_dir */

void visita_arquivo(const char *path) {
  struct stat stat_buffer;

  int result = lstat(path, &stat_buffer);

  if (result != 0) {
    printf("Não foi possível visitar o caminho %s\n", path);
    return;
  }

  printf("Mode: %08x\n", stat_buffer.st_mode);

  if (S_ISREG(stat_buffer.st_mode)) cnt[R_FILE]++;
  if (S_ISDIR(stat_buffer.st_mode)) cnt[D_FILE]++;
  if (S_ISLNK(stat_buffer.st_mode)) cnt[L_FILE]++;
  if (S_ISBLK(stat_buffer.st_mode)) cnt[B_FILE]++;
  if (S_ISCHR(stat_buffer.st_mode)) cnt[C_FILE]++;
}

void check_input(int input) {
  if(input != -1) {
    printf("Fornecer apenas um parâmetro como entrada!\n");
    exit(1);
  }
}

int read_input(int argc, char const *argv[]) {
  int opt, input_file_type = -1;

  while ((opt = getopt(argc, argv, "rdlbc")) != -1) {
    check_input(input_file_type);
    switch (opt) {
      case 'r':
        input_file_type = R_FILE;
        break;
      case 'd':
        input_file_type = D_FILE;
        break;
      case 'l':
        input_file_type = L_FILE;
        break;
      case 'b':
        input_file_type = B_FILE;
        break;
      case 'c':
        input_file_type = C_FILE;
        break;
      }
  }

  if(input_file_type == -1) {
    input_file_type = R_FILE;
  }

  return input_file_type;
}

void print_result(int input_type) {
  switch (input_type) {
    case R_FILE:
      printf("Arquivos regulares: %d\n", cnt[R_FILE]);
      break;
    case D_FILE:
      printf("Diretórios: %d\n", cnt[D_FILE]);
      break;
    case L_FILE:
      printf("Links: %d\n", cnt[L_FILE]);
      break;
    case B_FILE:
      printf("Blocos: %d\n", cnt[B_FILE]);
      break;
    case C_FILE:
      printf("Chars: %d\n", cnt[C_FILE]);
      break;
  }
}

int main(int argc, char const *argv[]) {
  int input_type;  
  char *path = ".";

  input_type = read_input(argc, argv);

  if (optind < argc) path = argv[optind];

  printf("Path: %s\n", path);
  walk_dir(path, visita_arquivo);
  print_result(input_type);

  return 0;
}
