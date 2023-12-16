const float a = 3;

int sum(int cnt, int a[]) {
  int i = 0;
  int total = 0;
  while (i < cnt) {
    total = total + a[i];
  }
  return total;
}

int main() {
  int array[7] = {1, 2, 3, 4, 5, 6, 7};
  int a = array[0] * array[1];
  return sum(7, array);
}
