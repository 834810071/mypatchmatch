#pragma once
#include "head.h"

namespace util
{
	int random_range(float min, float max)
	{
		if (min > max)
		{
			int tmp = min;
			min = max;
			max = tmp;
		}

		float f = (rand() % 65535) * 1.0f / 65535.0f;	// 65537 
		return f * (max - min) + min;
	}


	double sim_abs_diff(Mat& a, Mat& b)
	{
		Mat tmp;
		absdiff(a, b, tmp);
		//cout << tmp << endl;
		//cout << sum(tmp) << endl;	// 各个通道单独求和
		//cout << sum(sum(tmp)) << endl;
		return 1e4 - sum(sum(tmp))[0];
	}

	int argmax(float arr[], int n)
	{
		int max_i = 0;
		for (int i = 1; i < n; i++)
		{
			if (arr[i] > arr[max_i])
			{
				max_i = i;
			}
		}
		return max_i;
	}

	int max(int a, int b)
	{
		return a > b ? a : b;
	}

	int min(int a, int b)
	{
		return a < b ? a : b;
	}

}