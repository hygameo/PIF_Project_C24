#include <iostream>
using namespace std;

int main ()
{
    int n;

    do
    {
        cout << "Nhap so phan tu n = ";

        cin >> n;
    }
    while ((n <= 0) || (n > 16));

    cout << "Khoi tao mang int arr[" << n << "]" << endl;

    int arr[n];

    for (int i = 0; i < n; ++i)
    {
        cout << "arr[" << i << "] = ";
        cin >> arr[i];
    }

    cout << "arr[] = {";

    for (int i = 0; i < n; ++i)
        cout << " " << arr[i];
    
    cout << " }" << endl;

    cout << "dia chi cua tung phan tu:" << endl;

    for(int i = 0; i < n; ++i)
        cout << "&arr[" << i << "] = " << &arr[i] << endl;

    return 0;
}