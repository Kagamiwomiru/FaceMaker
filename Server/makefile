# コンパイラを指定
CC :=g++
# インクルードファイル等
CXXFLAGS := -std=c++11 ` pkg-config opencv --cflags` `pkg-config opencv --libs` 

converter : main.o 
	$(CC)  main.o -o converter   $(CXXFLAGS)

main.o : CKFaceServerKit.h



clean:
	$(RM) *.o
	$(RM) convert
