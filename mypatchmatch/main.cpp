#include "head.h"
#include "util.h"

typedef vector<vector<vector<int>>> Vector3i;
typedef vector<vector<int>> Vector2i;

double(*sim)(Mat&, Mat&) = util::sim_abs_diff;

/*
 * ��ʼ��
 */
void initialize(Vector3i& f, int n_rows, int n_cols, int patch_size)
{
	f.resize(n_rows);

	// ����ռ�
	for (int i = 0; i < n_rows; i++)
	{
		f[i].resize(n_cols);
		for (int j = 0; j < n_cols; j++)
		{
			f[i][j].resize(2);
		}
	}

	// �����ʼ��
	for (int i = 0; i < n_rows; i++)
	{
		for (int j = 0; j < n_cols; j++)
		{
			f[i][j][0] = int(util::random_range(0, n_rows - patch_size));
			f[i][j][1] = int(util::random_range(0, n_cols - patch_size));

			if (f[i][j][0] > n_rows - 1 || f[i][j][1] > n_cols - 1 || f[i][j][0] < 0 || f[i][j][1] < 0)
			{
				cout << "error" << endl;
				exit(0);
			}

			if (i == 0 || i == n_rows - 1 || j == 0 || j == n_cols - 1)
			{
				f[i][j][0] == 0;
				f[i][j][1] == 0;
			}
		}
	}

}

// �ع�ͼ��
void reconstruct(Vector3i& f, Mat& dst, Mat& ref, int patch_size)
{
	int n_rows = dst.rows, n_cols = dst.cols;
	// �������п�
	for (int i = 0; i < n_rows; i++)
	{
		for (int j = 0; j < n_cols; j++)
		{
			int r = f[i][j][0], c = f[i][j][1];
			dst.at<Vec3b>(i, j) = ref.at<Vec3b>(r, c);
		}
	}

	// �̶��߽��  ��֪��Ϊʲô  �� �� �� ����Ҫ���� �� �� ����Ҫ
	int r, c;
	// 1. up
	//r = 0;
	//for (c = 0; c < n_cols - patch_size; c++)
	//{
	//	Mat up_patch = ref(Range(f[r][c][0], f[r][c][0] + patch_size), Range(f[r][c][1], f[r][c][1] + patch_size));
	//	for (int i = 0; i < patch_size; i++)
	//	{
	//		for (int j = 0; j < patch_size; j++)
	//		{
	//			assert(i >= 0 && i + r >= 0 && j >= 0 && j + c >= 0 &&
	//				i < n_rows && i + r < n_rows && j < n_cols && j + c < n_cols);
	//			dst.at<Vec3b>(r + i, c + j) = up_patch.at<Vec3b>(i, j);
	//		}
	//	}
	//}

	// 2. down
	r = n_rows - patch_size - 1;
	for (c = 0; c < n_cols - patch_size - 1; c++)
	{
		Mat up_patch = ref(Range(f[r][c][0], f[r][c][0] + patch_size), Range(f[r][c][1], f[r][c][1] + patch_size));
		for (int i = 0; i < patch_size; i++)
		{
			for (int j = 0; j < patch_size; j++)
			{
				assert(i >= 0 && i + r >= 0 && j >= 0 && j + c >= 0 &&
					i < n_rows && i + r < n_rows && j < n_cols && j + c < n_cols);
				dst.at<Vec3b>(r + i, c + j) = up_patch.at<Vec3b>(i, j);
			}
		}
	}

	// 3. left
	//c = 0;
	//for (r = 0; r < n_rows - patch_size; r++)
	//{
	//	Mat up_patch = ref(Range(f[r][c][0], f[r][c][0] + patch_size), Range(f[r][c][1], f[r][c][1] + patch_size));
	//	for (int i = 0; i < patch_size; i++)
	//	{
	//		for (int j = 0; j < patch_size; j++)
	//		{
	//			assert(i >= 0 && i + r >= 0 && j >= 0 && j + c >= 0 &&
	//				i < n_rows && i + r < n_rows && j < n_cols && j + c < n_cols);
	//			dst.at<Vec3b>(r + i, c + j) = up_patch.at<Vec3b>(i, j);
	//		}
	//	}
	//}

	// 4. right
	c = n_cols - patch_size - 1;
	for (r = 0; r < n_rows - patch_size - 1; r++)
	{
		Mat up_patch = ref(Range(f[r][c][0], f[r][c][0] + patch_size), Range(f[r][c][1], f[r][c][1] + patch_size));
		for (int i = 0; i < patch_size; i++)
		{
			for (int j = 0; j < patch_size; j++)
			{
				assert(i >= 0 && i + r >= 0 && j >= 0 && j + c >= 0 &&
					i < n_rows && i + r < n_rows && j < n_cols && j + c < n_cols);
				dst.at<Vec3b>(r + i, c + j) = up_patch.at<Vec3b>(i, j);
			}
		}
	}
}


int main(int argc, char* argv)
{
	srand((unsigned)time(NULL));

	// ��ȡͼƬ
	string str_dst = "F:\\00\\00\\PatchMatch-1-master\\image_files\\brick_pavement.jpg";	// Ŀ��ͼ�� ����ͼ��
	string str_ref = "F:\\00\\00\\PatchMatch-1-master\\image_files\\brick_pavement_with_hole.png";	// �ο�ͼ�� �޸�ͼ��

	Mat img_dst = imread(str_dst);
	Mat img_ref = imread(str_ref);

	if (img_dst.empty())
	{
		cout << "img_dst open faild!" << endl;
		exit(0);
	}

	if (img_ref.empty())
	{
		cout << "img_ref open faild!" << endl;
		exit(0);
	}

	namedWindow("img_ref");
	imshow("img_ref", img_ref);

	namedWindow("img_dst");
	imshow("img_dst", img_dst);

	if (img_ref.cols != img_dst.cols || img_ref.rows != img_dst.rows)
	{
		cout << "The target picture is not the same size as the reference picture " << endl;
		exit(0);
	}

	int n_cols = img_dst.cols;
	int n_rows = img_dst.rows;
	int patchsize = 3;

	Vector3i vec_coordinates;	// �洢dst��ÿ����ref�ж�Ӧ�Ŀ�����

	cout << "1.  Initializing......" << endl;
	// 1.�����ʼ��
	initialize(vec_coordinates, n_rows, n_cols, patchsize);
	cout << "1.  Initialization success!" << endl;


	// 2.����

	cout << "2.  Iterating......" << endl;

	int row_start, row_end, col_start, col_end, step;

	Vector2i v;	// �ڴ��������������洢���ƶ�
	v.resize(n_rows);

	for (int i = 0; i < n_rows; i++)
	{
		v[i].resize(n_cols);
	}

	// �ȼ���֮ǰ�����ʼ����Ӧ��֮������ƶ�
	for (int i = 0; i < n_rows - patchsize; i++)
	{
		for (int j = 0; j < n_cols - patchsize; j++)
		{
			assert(i >= 0 && i + patchsize >= 0 && j >= 0 && j + patchsize >= 0 &&
				i < n_rows && i + patchsize < n_rows && j < n_cols && j + patchsize < n_cols);
			assert(vec_coordinates[i][j][0] >= 0 && vec_coordinates[i][j][0] + patchsize >= 0 && vec_coordinates[i][j][1] >= 0 && vec_coordinates[i][j][1] + patchsize >= 0 &&
				vec_coordinates[i][j][0] < n_rows && vec_coordinates[i][j][0] + patchsize < n_rows && vec_coordinates[i][j][1] < n_cols && vec_coordinates[i][j][1] + patchsize < n_cols);
			Mat dst_patch = img_dst(Range(i, i + patchsize), Range(j, j + patchsize));
			Mat ref_patch = img_ref(Range(vec_coordinates[i][j][0], vec_coordinates[i][j][0] + patchsize), Range(vec_coordinates[i][j][1], vec_coordinates[i][j][1] + patchsize));
			v[i][j] = sim(dst_patch, ref_patch);
		}
	}

	bool reverse = false;	// ż��������ɨ�裨x+1, y�� ��x, y+1��  ������˳��ɨ�� ��x-1, y����x, y-1��

	int n_iterations = 5; // ��������
	

	for (int t = 0; t < n_iterations; t++)
	{

		//Mat progress(img_dst.rows, img_dst.cols, img_dst.type());
		//reconstruct(vec_coordinates, progress, img_ref, patchsize);
		//imshow("progress", progress);
		// waitKey(0);

		// 2.1 ���� Propagation
		cout << "iteration " << t + 1 << endl;

		if (reverse)	// ����
		{
			row_start = n_rows - patchsize - 2;
			row_end = -1;
			col_start = n_cols - patchsize - 2;
			col_end = -1;
			step = -1;
		}
		else            // ˳��
		{
			row_start = 1;
			row_end = n_rows - patchsize;
			col_start = 1;
			col_end = n_cols - patchsize;
			step = 1;
		}

		// ���������ж� Lambda���ʽ  [capture](parameters){body}
		auto checkvalid = [patchsize, n_rows, n_cols](int r, int c)
		{
			if (r < 0)
			{
				return false;
			}
			if (c < 0)
			{
				return false;
			}
			if (r + patchsize >= n_rows)
			{
				return false;
			}
			if (c + patchsize >= n_cols)
			{
				return false;
			}
			return true;
		};

		for (int i = row_start; i != row_end; i += step)
		{
			for (int j = col_start; j != col_end; j += step)
			{
				float sm[3];

				// ��ȡĿ��ͼƬ�еĿ�
				Mat patch = img_dst(Range(i, i + patchsize), Range(j, j + patchsize));

				sm[0] = v[i][j];	// ��ʼ���ƶ�

				/* ȡ��r�����c����ġ��ڽӡ�match(r-1, c)��match(r, c-1) */
				/* checkvalid�������ڼ���µ�ƫ�����Ƿ�Ϸ� */
				if (checkvalid(vec_coordinates[i - step][j][0], vec_coordinates[i - step][j][1]))
				{
					Mat xpatch = img_ref(Range(vec_coordinates[i - step][j][0], vec_coordinates[i - step][j][0]+patchsize), Range(vec_coordinates[i - step][j][1], vec_coordinates[i - step][j][1]+patchsize));
					sm[1] = sim(patch, xpatch);
				}
				else
				{
					sm[1] = -1e6f;
				}

				if (checkvalid(vec_coordinates[i][j - step][0], vec_coordinates[i][j - step][1]))
				{
					Mat ypatch = img_ref(Range(vec_coordinates[i][j - step][0], vec_coordinates[i][j - step][0] + patchsize), Range(vec_coordinates[i][j - step][1], vec_coordinates[i][j - step][1] + patchsize));
					sm[2] = sim(patch, ypatch);
				}
				else
				{
					sm[2] = -1e6f;
				}

				int k = util::argmax(sm, 3);
				v[i][j] = sm[k];

				switch (k)
				{
				case 0: break;
				case 1:vec_coordinates[i][j][0] = vec_coordinates[i - step][j][0]; vec_coordinates[i][j][1] = vec_coordinates[i - step][j][1]; break;
				case 2:vec_coordinates[i][j][0] = vec_coordinates[i][j - step][0]; vec_coordinates[i][j][1] = vec_coordinates[i][j - step][1]; break;
				default: break;
				}
			}
		}
		

		reverse != reverse;

		// 2.2 ������� random search

		for (int i = row_start; i != row_end; i += step)
		{
			for (int j = col_start; j != col_end; j += step)
			{
				int r_ws = n_rows, c_ws = n_cols;	// ���ڴ�С
				float alpha = 0.5f, exp = 0.5f;		// alpha�ǵ�ǰ�������뾶���ӣ�exp�������ٶ�
													// ���ڳߴ� =  win(r_ws*alpha, c_ws*alpha)

				while (r_ws*alpha > 1 && c_ws*alpha > 1)
				{
					/* ȷ���Ϸ��������ռ䣬�����±�Խ�� */
					// r_ws * alpha  �뾶
					int rmin = util::max(0, int(vec_coordinates[i][j][0] - alpha * r_ws));
					int rmax = util::min(n_rows - patchsize, int(vec_coordinates[i][j][0] + alpha * r_ws));
					int cmin = util::max(0, int(vec_coordinates[i][j][1] - alpha * c_ws));
					int cmax = util::min(n_cols - patchsize, int(vec_coordinates[i][j][1] + alpha * c_ws));
					

					int r_random = int(util::random_range(rmin, rmax));
					int c_random = int(util::random_range(cmin, cmax));

					Mat patch = img_dst(Range(i, i+patchsize), Range(j, j+patchsize));
					Mat random_match = img_ref(Range(r_random, r_random + patchsize), Range(c_random, c_random + patchsize));

					int similarity = sim(patch, random_match);

					if (similarity > v[i][j])
					{
						v[i][j] = similarity;
						vec_coordinates[i][j][0] = r_random;
						vec_coordinates[i][j][1] = c_random;
					}
					alpha *= exp;	// ���ڴ�С����
				}

			}
		}

	}
	reconstruct(vec_coordinates, img_dst, img_ref, patchsize);
	imshow("result", img_dst);

	cout << "2.  Successful iteration!" << endl;


	waitKey(0);
	return 0;
}