#include <iostream>
#include <vector>
#include <map>
using namespace std;


vector<long> arr_n;
vector<long> arr_m;
map<long, vector<int> > map_result;

void quick_search(int left,int right,vector<long> arr)
{
	int mid = left + (right - left) / 2;
	int equal_l = 1;
	while (mid-1 >= left &&  arr_n[mid] == arr_n[mid - 1] || mid + equal_l <= right && arr_n[mid + equal_l - 1] == arr_n[mid + equal_l])
	{

		if (mid - 1 >= left && arr_n[mid] == arr_n[mid - 1])
		{
			mid--;
			equal_l++;
		}
		if (mid + equal_l <= right && arr_n[mid + equal_l - 1] == arr_n[mid + equal_l])
			equal_l++;
	}
	vector<long> arr_left;
	vector<long> arr_right;
	for (int i = 0; i < arr.size(); i++)
	{
		if (arr[i]==arr_n[mid])
		{
			vector<int> temp;
			temp.push_back(mid);
			temp.push_back(mid + equal_l - 1);
			map_result[arr[i]] = temp;
		}
		if (arr[i] > arr_n[mid])
			arr_right.push_back(arr[i]);
		if (arr[i] < arr_n[mid])
			arr_left.push_back(arr[i]);
	}
	if (mid - 1 >= left && arr_left.size() != 0)
		quick_search(left, mid - 1, arr_left);
	if (mid + equal_l <= right && arr_right.size() != 0)
		quick_search(mid + 1, right, arr_right);
	return;
}


int main()
{
	int n, m;
	cin >> n >> m;
	for (int i = 0; i < n; i++)
	{
		int tmp;
		cin >> tmp;
		arr_n.push_back(tmp);
	}
	for (int i = 0; i < m; i++)
	{
		long tmp;
		cin >> tmp;
		arr_m.push_back(tmp);
	}
	quick_search(0, arr_n.size()-1,arr_m);
	for (int i = 0; i < arr_m.size(); i++)
	{
		if (map_result.find(arr_m[i])==map_result.end())
		{
			cout << -1 << " " << -1 << endl;
		}
		else
		{
			cout << map_result[arr_m[i]][0] << " ";
			cout << map_result[arr_m[i]][1] << endl;
		}
	}
	return 0;
}