#include "CKFaceServerKit.h"
vector<Rect> detectFaceInImage(Mat &image) {
	CascadeClassifier cascade_front;
	CascadeClassifier cascade_pro;
	CascadeClassifier cascade_eye;
	cascade_front.load("./cascade/haarcascade_frontalface_alt2.xml");
	cascade_pro.load("./cascade/haarcascade_frontalface_alt.xml");
	cascade_eye.load("./cascade/haarcascade_eye.xml");
	vector<Rect> faces1;
	vector<Rect> faces2;
	vector<Rect> eyes;
	vector<Rect> faces3;
	vector<Rect> faces;


	Mat gray_image;
	cvtColor(image, gray_image, CV_RGB2GRAY);

	cascade_front.detectMultiScale(gray_image, faces1, 1.1, 1, 0, Size(100, 100));
	cascade_pro.detectMultiScale(gray_image, faces2, 1.1, 1, 0, Size(100, 100));
	cascade_eye.detectMultiScale(gray_image, eyes, 1.1, 1, 0, Size(20, 20));


	if (faces2.size() > 0) {
		int rem[] = { 0 };
		for (int i = 0; i < faces2.size(); i++) {
			if (faces1.size() > 0) {
				for (int j = 0; j < faces1.size(); j++) {
					if (abs(faces2[i].x - faces1[j].x) < faces2[i].width*0.9 && abs(faces2[i].y - faces1[j].y) < faces2[i].height*0.9) {
						rem[i] = 1;
					}
				}
			}
		}
		for (int i = 0; i < faces2.size(); i++) {
			int cnt = 0;
			if (rem[i] == 1) {
				faces2.erase(faces2.begin() + i - cnt);
				cnt++;
			}
		}
	}


	if (eyes.size() >= 2) {
		for (int i = 0; i < eyes.size(); i++) {
			for (int j = 0; j < eyes.size(); j++) {
				if (eyes[i].x != eyes[j].x && abs(eyes[i].x - eyes[j].x) >= max(eyes[i].width, eyes[j].width) && abs(eyes[i].x - eyes[j].x) <= max(eyes[i].width, eyes[j].width)*2.5 && abs(eyes[i].y - eyes[j].y) <= min(eyes[i].height, eyes[j].height) && max(eyes[i].width, eyes[j].width) < min(eyes[i].width, eyes[j].width) * 2) {
					int X = min(eyes[i].x, eyes[j].x) - (min(eyes[i].width, eyes[j].width)*1.5);
					if (X < 0) {
						X = 0;
					}
					X = (int)X;
					int Y = min(eyes[i].y, eyes[j].y) - (min(eyes[i].height, eyes[j].height)*1.5);
					if (Y < 0) {
						Y = 0;
					}
					Y = (int)Y;
					int W = min(eyes[i].width, eyes[j].width) * 5;
					if (W > WIDTH - X) {
						W = WIDTH - X;
					}
					int H = min(eyes[i].height, eyes[j].height) * 5;
					if (H > HEIGHT - Y) {
						H = HEIGHT - Y;
					}
					int flag[3] = { 0, 0, 0 };

					//��͈͂����Ԃ��Ă���Ƃ���͂Ȃ����H�_���Ȏ��A1��flag�ɓ����
					if (faces1.size() > 0) {
						for (int k = 0; k < faces1.size(); k++) {
							if (flag[0] == 0 && abs(X - faces1[k].x) < W*0.9 && abs(Y - faces1[k].y) < H*0.9) {
								flag[0] = 1;
							}
						}
					}

					if (faces2.size() > 0) {
						for (int k = 0; k < faces2.size(); k++) {
							if (flag[1] == 0 && abs(X - faces2[k].x) < W*0.9 && abs(Y - faces2[k].y) < H*0.9) {
								flag[1] = 1;
							}
						}
					}

					if (faces3.size() == 0 && flag[0] == 0 && flag[1] == 0) {
						flag[2] = 0;
					}
					else if (faces3.size() > 0) {
						for (int k = 0; k < faces3.size(); k++) {
							if (flag[2] == 0 && abs(X - faces3[k].x) < W*0.9 && abs(Y - faces3[k].y) < H*0.9) {
								flag[2] = 1;
							}
						}
					}
					int si = faces3.size();

					if (flag[0] == 0 && flag[1] == 0 && flag[2] == 0){
						faces3.insert(faces3.end(),{X, Y, W, H}) ;
						//faces3[si].x = X;
						//faces3[si].y = Y;
						//faces3[si].width = W;
						//faces3[si].height = H;
					}
				}
			}
		}
	}

	faces.insert(faces.end(), faces1.begin(), faces1.end());
	faces.insert(faces.end(), faces2.begin(), faces2.end());
	faces.insert(faces.end(), faces3.begin(), faces3.end());

	return faces;
}

int main(int argc ,char *argv[]) {
	if(argc<2) return -1;
	string input_path;
	string output_path;
	int num_images=100;
	char input[255];
	char output[255];
	char mkcmd[255];
	char sendcmd[255];
	unsigned int    now = (unsigned int)time( 0 );
	srand(now);
	sprintf(input,"./face/inpic/%s/",argv[1]);
	input_path=string(input);
	input_path +="in_%03d.jpg";
	sprintf(output,"./face/outpic/%s/",argv[1]);
	output_path=string(output);
	output_path+=to_string(rand());
	output_path+="out_%03d.jpg";
	sprintf(mkcmd,"mkdir ./face/outpic/%s",argv[1]);
	system(mkcmd);
	sprintf(sendcmd,"scp -r ./face/outpic/%s Raspi:FaceMaker/outpic/",argv[1]);
	system(sendcmd);
	int y1, x1;
	int count = 0;
	Mat in;
	vector<Rect> face;
	VideoCapture cap(input_path);

	string out = output_path;

	VideoWriter writer(out, 0, 0.0, Size(200, 200));

	while (count<num_images) {
		cap >> in;
		face = detectFaceInImage(in);
		if (face.size() == 1) {
			Mat out(Size(face[0].width, face[0].height), CV_8UC3, Scalar(0, 0, 0));	
			y1 = face[0].height + face[0].y;
			x1 = face[0].width + face[0].x;
			for (int y = face[0].y; y < y1; ++y) {
				for (int x = face[0].x; x < x1; ++x) {
					for (int c = 0; c < 3; c++) {
						out.data[((y - face[0].y) * out.cols + (x - face[0].x)) * 3 + c] = in.data[(y* in.cols + x) * 3 + c];
					}
				}
			}
			resize(out, out, cv::Size(), 96.0 / out.cols, 96.0 / out.cols);
			writer << out;
		}
		count++;
	}
	sprintf(sendcmd,"scp -r ./face/outpic/%s Raspi:FaceMaker/outpic/",argv[1]);
	system(sendcmd);
return 0;
}
