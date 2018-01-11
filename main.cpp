#include"CKFaceKit.h"
#include <unistd.h>
void cap_image(Mat &in,string path, int width, int height,int num_images) {
	int count=0;
    VideoCapture cap(0);
    while(!cap.isOpened()){
        VideoCapture cap(0);
        printf("カメラの起動中...\n");
    } 
    
	VideoWriter writer(path, 0, 0.0, Size(width, height));
    
	while(count<=num_images){	
		cap >> in;
		namedWindow("FaceMaker_Capturing",0);
		setWindowProperty("FaceMaker_Capturing",CV_WND_PROP_FULLSCREEN,CV_WINDOW_FULLSCREEN);
		imshow("FaceMaker_Capturing",in);
		writer << in;
		count++;
		waitKey(30);
        usleep(500000);
		}
	destroyWindow("FaceMaker_Capturing");
	cap.release();
}

int main(){
    int PICT=0;
    char Name[NAME_SIZE];
    char mkcmd[SYSCMD];//クライアント側の./face/にNameのディレクトリ作成
    char sendcmd[SYSCMD];//画像をサーバに送る
    char svrm[SYSCMD];//サーバ側の./face/Nameを削除
    char execmd[SYSCMD];//コンバート処理
    char out_path[255];
    int ws=2;//遅延時間
    string output_path;
    Mat frame,input_image;

    printf("FaceMakerへようこそ\n");
    printf("サーバ側の「face/」ディレクトリを初期化しています...\n");
    sprintf(svrm,"ssh Neptune 'rm -rf FaceMakerServer/face/inpic/*;rm -rf FaceMakerServer/face/outpic/*'");
	system(svrm);

    printf("撮影枚数:");
    scanf("%d",&PICT);

    printf("名前:");
    scanf("%s",Name);
    printf("Hello %s!\n",Name);
    sleep(ws);
    sprintf(out_path,"./pic/%s",Name);
    sprintf(mkcmd,"mkdir %s",out_path);
    strcat(out_path,"/in_%03d.jpg");
    output_path=string(out_path);
    system(mkcmd);
    cap_image(frame,output_path, WIDTH,HEIGHT,PICT);//撮影
    sprintf(sendcmd,"scp -r pic/%s Neptune:FaceMakerServer/face/inpic/",Name);
    printf("＊＊Convert＊＊\n");
    printf("サーバに送信しています...\n");
    system(sendcmd);
    printf("コンバート中...\n");
    sprintf(execmd,"ssh Neptune 'cd FaceMakerServer/;./converter %s'",Name);
    system(execmd);    

    return 0;
}