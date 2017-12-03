#include <unistd.h>
#include <fcntl.h>

unsigned char hexdigit[] = {0x3F, 0x06, 0x5B, 0x4F,
                            0x66, 0x6D, 0x7D, 0x07, 
                            0x7F, 0x6F, 0x77, 0x7C,
			                      0x39, 0x5E, 0x79, 0x71};

typedef struct Pio {
  int pio;
  int dado;
} pio;

int main() {
  int i;
  pio j, k;
  k.pio = 1;
  j.pio = 5;
  int dev = open("/dev/de2i150_altera", O_RDWR);

  for (i=0; i>-1; i++) {
    read(dev, &j, 4);
    k.dado = hexdigit[j.dado & 0xF]
      | (hexdigit[(j.dado >>  4) & 0xF] << 8)
      | (hexdigit[(j.dado >>  8) & 0xF] << 16)
      | (hexdigit[(j.dado >> 12) & 0xF] << 24);
    k = ~k;
    write(dev, &k, 4);
  }

  close(dev);
  return 0;
}

0000 0000 0000 0000 0000 0011      0001 0010
0000 0000 0000 0000 0000 0000 0000 1111

