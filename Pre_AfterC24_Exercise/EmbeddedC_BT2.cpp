#include <iostream>
#include <stdint.h>
using namespace std;

int main ()
{
   int int_arr[1];
   uint8_t uint8_arr[1];
   uint16_t uint16_arr[1];
   uint32_t uint32_arr[1];
   uint64_t uint64_arr[1];

   int* int_arr_address = &int_arr[0];
   uint8_t * uint8_arr_address = &uint8_arr[0];
   uint16_t * uint16_arr_address = &uint16_arr[0];
   uint32_t * uint32_arr_address = &uint32_arr[0];
   uint64_t * uint64_arr_address = &uint64_arr[0];

   cout << "Dua vao dia chi duoc in ra, dung luong cua 1 phan tu trong mang int la " << sizeof(* int_arr_address) << " byte." << endl;
   cout << "Dua vao dia chi duoc in ra, dung luong cua 1 phan tu trong mang uint8 la " << sizeof(* uint8_arr_address) << " byte." << endl;
   cout << "Dua vao dia chi duoc in ra, dung luong cua 1 phan tu trong mang uint16 la " << sizeof(* uint16_arr_address) << " byte." << endl;
   cout << "Dua vao dia chi duoc in ra, dung luong cua 1 phan tu trong mang uint32 la " << sizeof(* uint32_arr_address) << " byte." << endl;
   cout << "Dua vao dia chi duoc in ra, dung luong cua 1 phan tu trong mang uint64 la " << sizeof(* uint64_arr_address) << " byte." << endl;

   return 0;
}
