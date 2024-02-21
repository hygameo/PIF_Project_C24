#include <iostream>
#include <iomanip>
using namespace std;

int main ()
{
    int n;

    do
    {
        cout << "Nhap so phan tu n = ";

        cin >> n;
    }
    while (n <= 5);

    cout << "Khoi tao mang int arr[" << n << "]" << endl;

    int arr[n];
    float total = 0;

    for (int i = 0; i < n; ++i)
    {
        cout << "arr[" << i << "] = ";
        cin >> arr[i];
        total += arr[i];
    }

    cout << "arr[] = {";

    for (int i = 0; i < n; ++i)
        cout << " " << arr[i];
    
    cout << " }" << endl;

    float avg = total/n;

    cout << "gia tri trung binh la " << fixed << setprecision(3) << avg << endl;
    return 0;
}