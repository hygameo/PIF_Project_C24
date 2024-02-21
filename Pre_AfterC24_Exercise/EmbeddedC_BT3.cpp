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
    while (n <= 5);

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

    int max = arr[0], min = arr[0];

    for(int i = 0; i < n; ++i)
    {
        if(max < arr[i])
            max = arr[i];
        if(min > arr[i])
            min = arr[i];
    }

    cout << "phan tu lon nhat trong mang la " << max << endl << "phan tu be nhat trong mang la " << min << endl;
    return 0;
}