Given a = 0x22122009
      b = 0xA10420F3

a. Biểu diễn a trong hệ thập phân, nhị phân
    - Hệ thập phân (Decimal): 
        = 9 * 16^0 + 
          0 * 16^1 + 
          0 * 16^2 + 
          2 * 16^3 +
          2 * 16^4 +
          1 * 16^5 +
          2 * 16^6 +
          2 * 16^7
        = (571613193)10
    - Hệ nhị phân
        - 9: 1001
          0: 0000
          0: 0000
          2: 0010
          2: 0010
          1: 0001
          2: 0010
          2: 0010
        = (0010 0010 0001 0010 0010 0000 0000 1001)2
b. Thực hiện phép toán: not a, a and b, a or b, a xor b
    - Convert a, b to binary:
        a = 0010 0010 0001 0010 0010 0000 0000 1001
        b = 1010 0001 0000 0100 0010 0000 1111 0011
    - Not a: 
        = 1101 1101 1110 1101 1101 1111 1111 0110
        = 0xDDEDDFF6
    - a and b:
        = 0010 0000 0000 0000 0010 0000 0000 0001
        = 0x20002001
    - a or b:
        = 1010 0011 0001 0110 0010 0000 1111 1011
        = 0xA31620FB
    - a xor b:
        = 1000 0011 0001 0110 0000 0000 1111 1010
        = 0x831600FA
c. Kích thước của số a và b là bao nhiêu byte?
    - a và b có kích thước là 8*4 = 32 bit = 32/8 = 4 byte
d. Nếu b đang được biểu diễn dưới dạng số bù 1 thì giá trị trong hệ thập phân của b là bao nhiêu?
    - chuyển đổi b dạng hex sang binary và đảo các bit
    => 0101 1110 1111 1011 1101 1111 0000 1100
    = 0x5DF7BC0C
e. Giải lại câu d với dạng số bù 2
    - ta có dạng số bù 1 của b là 0101 1110 1111 1011 1101 1111 0000 1100
    - ta +1 để ra dạng số bù 2
    => 0101 1110 1111 1011 1101 1111 0000 1101
    = 0x5DF7BC0D
f. Hãy đoán ý nghĩa của số a
    - 22122009 => Ngày 22 tháng 12 năm 2009 là ngày Ngày thành lập quân đội nhân dân Việt Nam 
