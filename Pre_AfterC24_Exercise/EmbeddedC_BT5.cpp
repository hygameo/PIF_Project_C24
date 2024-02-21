#include <cstdio>
#include <stdint.h>

struct infor
{
    char name[32];
    char mssv[8];
    uint8_t course_c;
};

void infor_input(struct infor* stu_infor)
{
    printf("Nhap [ten][ho] cua nguoi code (toi da 32 ky tu): ");
    scanf("%31s", stu_infor->name);

    printf("Nhap mssv cua nguoi code (8 ky tu): ");
    scanf("%7s", stu_infor->mssv);

    printf("Nhap so thu tu khoa C da tham gia: ");
    scanf("%hhu", &stu_infor->course_c);
}

void infor_print(const infor* stu_infor) 
{
    printf("name: %s\n", stu_infor->name);
    printf("mssv: %s\n", stu_infor->mssv);
    printf("course_c: %d\n", stu_infor->course_c);
}

int main() {
    infor student_info;
    infor_input(&student_info);
    infor_print(&student_info);
    return 0;
}