
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int nThreshold1 = 170;
int nThreshold2 = 240;
int tmin = 100, tmax = 220;
int amax = 520;
Mat src;
Mat src_gray, binary1, binary2, edge, edgepro;
void on_trackbar(int, void*)
{

}
// ���ɹ켣��
void creat_trackbar()
{
	
	namedWindow("��ֵ",1);
	resizeWindow("��ֵ", 640, 320);
	createTrackbar("��ֵͼ 1", "��ֵ", &nThreshold1, 254, on_trackbar);
	createTrackbar("��ֵͼ 2", "��ֵ", &nThreshold2, 254, on_trackbar);
	createTrackbar("canny��ֵ1", "��ֵ", &tmin, 254, on_trackbar);
	createTrackbar("canny��ֵ2", "��ֵ", &tmax, 254, on_trackbar);
	createTrackbar("������", "��ֵ", &amax, 1500, on_trackbar);
}

int main()
{
	VideoCapture capture(0);
	creat_trackbar();
	on_trackbar(0,0);
	while (1)
    {
		Mat frame;
		capture >> frame;
		src = frame;
		Mat result = src.clone();

		//�ҶȻ�
		cvtColor(src, src_gray, COLOR_BGR2GRAY);
		//�״ζ�ֵ�����˳���Ƶ�޹��źţ�Ĭ����ֵΪ170
		threshold(src_gray, binary1, nThreshold1, 255, THRESH_TOZERO);
		//�ٴζ�ֵ����ѹ�Ƹ߹⣬ֻ�����м����Ĭ����ֵΪ240
		threshold(binary1, binary2, nThreshold2, 255, THRESH_TRUNC);
		imshow("binary", binary2);
		//��ֵ�˲�����
		medianBlur(binary2, edge, 3);
		imshow("edge1", edge);
		//��˹ģ������
		GaussianBlur(edge, edge, cv::Size(3, 3), 0);
		imshow("edge2", edge);
		//����canny���ӣ���ȡ��Ե��Ĭ�ϸߵ���ֵ�ֱ�Ϊ220��100
		Canny(edge, edge, tmin, tmax, 3);

		//���м򵥵ı�Ե���ͣ���ֹ�ظ�ʶ��ͬһ��Ե
		dilate(edge, edgepro, cv::Mat());
		imshow("edgepro", edgepro);

		//��������
		vector<vector<Point>> contours;
		vector<Point> point;
		vector<Vec4i> hireachy;
		findContours(edgepro, contours, hireachy, RETR_TREE, CHAIN_APPROX_NONE, Point());
		int num = 0;
		int a = 0;
		int b = 0;
		int c = 0;
		int d = 0;
		int e = 0;
		//���Ƴ���������
		for (int t = 1; t < contours.size(); t++)
		{
			int x = arcLength(contours[t], true);
			int area = contourArea(contours[t], true);
			int epsilon = 0.01 * arcLength(contours[t], true);
			approxPolyDP(contours[t], point, epsilon, true);  
			// ���ܳ�������������ƣ���Ч��������
			if (x > 80)
			{
				//�������trackbar���и��ģ�Ĭ����С���Ϊ520
				if (area > amax)
				{
					drawContours(result, contours, t, Scalar(128, 0, 128), 3, 8, Mat(), 0, Point());
					num++;
					//�ñ����ж϶�Ӧͼ��
					if (point.size() == 3)
					{
						a++;
					}
					else if (point.size() == 4)
					{
						b++;
					}
					else if (point.size() == 5)
					{
						c++;
					}
					else if (point.size() == 10)
					{
						d++;
					}
					else if (point.size() >= 15)
					{
						e++;
					}
				}
			}
		}
		//�ҳ�����Ԫ�أ�ʶ��Ϊ��Ӧ��ͼ��
		int n[5] = { a,b,c,d,e };
		int x = *max_element(n, n + 5);
		string A;
		stringstream S1;
		S1 << num;
		S1 >> A;
		if (x == 0)
		{
			string t0 = "none!";
			putText(result, t0, Point(50, 60), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);
		}
		else if (x == a)
		{
			string t1 = "triangle:" + A;
			putText(result, t1, Point(50, 60), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);
		}
		else if (x == b)
		{
			string t2 = "rectangle:" + A;
			putText(result, t2, Point(50, 60), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);
		}
		else if (x == c)
		{
			string t3 = "pentagon:" + A;
			putText(result, t3, Point(50, 60), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);
		}
		else if (x == d)
		{
			string t4 = "star:" + A;
			putText(result, t4, Point(50, 60), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);
		}
		else if (x == e)
		{
			string t5 = "circle:" + A;
			putText(result, t5, Point(50, 60), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);
		}
		imshow("result", result);
		waitKey(50);
	}

	return 0;
}