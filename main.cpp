#include"CKFaceKit.h"
#include <unistd.h>
void cap_image(Mat &in,string path, int width, int height,int num_images,VideoCapture cap) {
	int count=0;
    int msgf=0;
    int fontType = FONT_HERSHEY_SIMPLEX;
	VideoWriter writer(path, 0, 0.0, Size(width, height));
    vector<string> msgList={"【正面】","【少し左】","【少し右】","【右上】","【左上】","【左下】","【右下】","【下】"};
    string msg="を向いて'r'を押してください。";

	while(count<=num_images){	
		cap >> in;
		namedWindow("FaceMaker_Capturing", CV_WINDOW_AUTOSIZE);
        if(!msgf){
            string buf=msgList[count]+msg;
            printf("%s\n",buf.c_str());
            msgf=1;
        }
		//setWindowProperty("FaceMaker_Capturing",CV_WND_PROP_FULLSCREEN,CV_WINDOW_FULLSCREEN);
		imshow("FaceMaker_Capturing",in);
        if('r'==waitKey(30)){
            writer << in;
            count++;
            msgf=0;
        }
	}
	destroyWindow("FaceMaker_Capturing");
	cap.release();
}

int main(){
    int PICT=7;
    char Name[NAME_SIZE];
    char mkcmd[SYSCMD];//クライアント側の./face/にNameのディレクトリ作成
    char sendcmd[SYSCMD];//画像をサーバに送る
    char svrm[SYSCMD];//サーバ側の./face/Nameを削除
    char execmd[SYSCMD];//コンバート処理
    char out_path[255];
    int ws=2;//遅延時間
    string output_path;
    Mat frame,input_image;
    VideoCapture cap(0);
    if(!cap.isOpened())//カメラデバイスが正常にオープンしたか確認．
    {
        //読み込みに失敗したときの処理
        printf("カメラの起動中\n");
        return -1;
    }
    printf("FaceMakerへようこそ\n");
    printf("サーバ側の「face/」ディレクトリを初期化しています...\n");
    sprintf(svrm,"ssh Neptune 'rm -rf FaceMakerServer/face/inpic/*;rm -rf FaceMakerServer/face/outpic/*'");
	system(svrm);

    // printf("撮影枚数:");
    // scanf("%d",&PICT);

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
    sprintf(sendcmd,"scp -r pic/%s Neptune:FaceMakerServer/face/inpic/",Name);
    printf("＊＊Convert＊＊\n");
    printf("サーバに送信しています...\n");
    system(sendcmd);
    printf("コンバート中...\n");
    sprintf(execmd,"ssh Neptune 'cd FaceMakerServer/;./converter %s'",Name);
    system(execmd);    

    return 0;
}