#include"CKFaceKit.h"
#include <unistd.h>
void cap_image(Mat &in,string path, int width, int height,int num_images,VideoCapture cap) {
	int count=0;
	//VideoCapture cap(0);
	VideoWriter writer(path, 0, 0.0, Size(width, height));
	while(count<=num_images){	
		cap >> in;
		namedWindow("FaceMaker_Capturing",0);
		setWindowProperty("FaceMaker_Capturing",CV_WND_PROP_FULLSCREEN,CV_WINDOW_FULLSCREEN);
		imshow("FaceMaker_Capturing",in);
		writer << in;
		count++;
		waitKey(30);
		}
	destroyWindow("FaceMaker_Capturing");
	//cap.release();
}

int main(){
    int PICT=0;
    int flag=1;
    char Name[NAME_SIZE];
    char mkcmd[SYSCMD];//クライアント側の./face/にNameのディレクトリ作成
    char rmcmd[SYSCMD];//クライアント側の./face/inpicディレクトリを初期化
    char sendcmd[SYSCMD];//画像をサーバに送る
    char svrm[SYSCMD];//サーバ側の./face/Nameを削除
    char out_path[255];
    int ws=2;//遅延時間
    string output_path;
    Mat frame,input_image;
    VideoCapture cap(0);
    
    if(!cap.isOpened()){  // 成功したかどうかをチェック
        printf("カメラの起動中...\n");
				return -1;
	}else{
	     printf("カメラの起動を確認\n");

	 }
    printf("FaceMakerへようこそ\n");
    sprintf(rmcmd,"rm -rf ./pic/*");
    system(rmcmd);
    printf("撮影枚数:");
    scanf("%d",&PICT);
    while(flag){
        printf("名前:");
        scanf("%s",Name);
        printf("Hello %s!\n",Name);
        sleep(ws);
        sprintf(out_path,"./pic/%s",Name);
        sprintf(mkcmd,"mkdir %s",out_path);
        strcat(out_path,"/in_%03d.jpg");
        output_path=string(out_path);
        system(mkcmd);
        cap_image(frame,output_path, WIDTH,HEIGHT,PICT,cap);//撮影
        printf("撮影を続行しますか？(Y=1/n=0)\n");
        scanf("%d",&flag);
    }
    printf("＊＊Convert＊＊\n");
    printf("サーバ側の「face/」ディレクトリを初期化しています...\n");
    sprintf(svrm,"ssh Neon rm -rf FaceMakerServer/face/inpic/*");
	system(svrm);
    sprintf(sendcmd,"scp -r pic/* Neon:FaceMakerServer/face/inpic/");
    printf("サーバに送信しています...\n");
    system(sendcmd);
    return 0;
}