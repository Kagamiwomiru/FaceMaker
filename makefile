# コンパイラを指定
CC :=g++
# インクルードファイル等
CFLAGS :=`pkg-config opencv --cflags` `pkg-config opencv --libs`

FaceMaker: main.o 
	$(CC) -o FaceMaker main.o     $(CFLAGS)

main.o : main.cpp



clean:
	$(RM) *.o
	$(RM) CKFace
