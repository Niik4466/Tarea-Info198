#include <cstdio>
#include <string>

int main(int argc, char** argv){
  if (argc != 5)
    return 1;

  int id_core = atoi(argv[1]);
  char op = argv[2][0];
  float n1 = std::atof(argv[3]);
  float n2 = std::atof(argv[4]);

  switch (op) {
    case '/':
      if (n2 == 0)
        printf("Err");
      else
        printf("%f", n1/n2);
      break;
    case '*':
      printf("%f", n1*n2);
      break;
    case '+':
      printf("%f", n1+n2);
      break;
    case '-':
      printf("%f", n1-n2);
      break;
    default:
      return 1;
  }

  return 0;
}
