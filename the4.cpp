#include "the4.h"

// do not add extra libraries here


#define DEBUG 
#define DEBUG_STDERR(x) do { std::cerr << (x) << endl; } while(0)
#define DEBUG_ARRAY(a, n) do { for (int i = 0; i < n; i++) std::cerr << a[i] << " "; std::cerr << endl; } while(0)

// for example usage of DEBUG macros check test.cpp


int counterForRec = 0;
int counterForMemo = 0; 
int counnterForTab = 0;

int min(int num1, int num2){
  if(num1 >= num2){
    return num2;
  }
  return num1; 
}

int recur(int*& a1, int*& a2 ,int n, int*& t1, int*& t2, int e1, int e2, int x1, int x2, int index, int lineNum){
  counterForRec++;
  if(index == n - 1){
    if(lineNum == 1){
      return min( a1[index] + x1, t1[index-1] + a2[index] + x2);
    }
    else{
      return min( t2[index-1] + a1[index] + x1, + a2[index] + x2);
    }
  }
  if(index == 0){
    return(min(e1+a1[0]+recur( a1, a2, n, t1, t2, e1, e2, x1, x2, index+1, 1), e2+a2[0]+recur( a1, a2, n, t1, t2, e1, e2, x1, x2, index+1, 2)));
  }
  if(lineNum == 1 ){
    return(min( a1[index]+recur(a1, a2, n, t1, t2, e1, e2, x1, x2, index+1, 1), t1[index-1] + a2[index] + recur(a1, a2, n, t1, t2, e1, e2, x1, x2, index+1, 2)));
  } 
  else{
    return(min( a2[index] + recur(a1, a2, n, t1, t2, e1, e2, x1, x2, index+1, 2), t2[index-1] + a1[index] + recur(a1, a2, n, t1, t2, e1, e2, x1, x2, index+1, 1)));
  }
}

std::pair<int,int> Assembly_Rec (int*& a1, int*& a2, int n, int*& t1, int*& t2, int e1, int e2, int x1, int x2)
{
    counterForRec = 0;
    std::pair<int,int> retVal;
    retVal.first = -1;
    retVal.second = -1;


    retVal.first = recur(a1, a2, n, t1, t2, e1, e2, x1, x2, 0, 1);
    retVal.second = counterForRec*2;
    return retVal;
}


int Memo(int*& a1, int*& a2, int memoArr[][2], int n, int*& t1, int*& t2, int e1, int e2, int x1, int x2, int index, int lineNum){
 
  if(index == -1){
    Memo(a1, a2, memoArr,n, t1, t2, e1, e2, x1, x2, index+1, 1);
    counterForMemo--;
    return 0;
  }
  counterForMemo++;
  if(index == n-1){
    memoArr[index][0] = x1 + a1[index];
    memoArr[index][1] = x2 + a2[index];
    counterForMemo++;
    Memo(a1, a2, memoArr, n, t1, t2, e1, e2, x1, x2, index-1, 0);
    
    return 0;
    
  }
  if(lineNum == 1 ){
    if(memoArr[index+1][0] > memoArr[index+1][1]){
      memoArr[index][1] = memoArr[index+1][1] + a2[index];
    }else if(memoArr[index+1][0] < memoArr[index+1][1]){
      memoArr[index][1] = memoArr[index+1][0] + t2[index] + a2[index];
    }
    else {
      memoArr[index][1] = min(memoArr[index+1][0] + t2[index] + a2[index],memoArr[index+1][1] + a2[index]);
    }
    
    return 0;
  } 
  else{
    if(memoArr[index+1][1] == 0){
      Memo(a1, a2, memoArr,n, t1, t2, e1, e2, x1, x2, index+1, 1);
    }
    if(memoArr[index+1][0] > memoArr[index+1][1]){
      memoArr[index][0] = memoArr[index+1][1] + t1[index] + a1[index];
    }
    else if(memoArr[index+1][0] < memoArr[index+1][1]){
      memoArr[index][0] = memoArr[index+1][0] + a1[index];
    }else{
      memoArr[index][0] = min(memoArr[index+1][1] + t1[index] + a1[index], memoArr[index+1][0] + a1[index]);
    }
    Memo(a1, a2, memoArr,n, t1, t2, e1, e2, x1, x2, index-1, 0);
    
    return 0; 
  }
  return 0;
}


std::pair<int,int> Assembly_Memo (int*& a1, int*& a2, int n, int*& t1, int*& t2, int e1, int e2, int x1, int x2)
{
    counterForMemo = 0;
    std::pair<int,int> retVal;
    int memoArr[n][2] = {0};

    Memo(a1, a2, memoArr,n, t1, t2, e1, e2, x1, x2, n-1, 0);

    retVal.first = min(memoArr[0][0]+e1, memoArr[0][1]+e2);
    retVal.second = counterForMemo*2;

    return retVal;

}

std::pair<int,int> Assembly_Tab (int*& a1, int*& a2, int n, int*& t1, int*& t2, int e1, int e2, int x1, int x2)
{
    counnterForTab = 0;
    std::pair<int,int> retVal;

    int tabArr[n][2] = {0};
    tabArr[0][0] = e1 + a1[0];
    tabArr[0][1] = e2 + a2[0];
    for(int i = 1; i < n; i++){
      int x0 = tabArr[i-1][0];
      int x1 = tabArr[i-1][1];
      if(x0 + a1[i] > x1+ t2[i-1] + a1[i]){
        tabArr[i][0] = x1+ t2[i-1] + a1[i];
      }else {
         tabArr[i][0] = x0 + a1[i];
      }
      if(x0 + t1[i-1] > x1){
        tabArr[i][1] = x1 + a2[i];
      }else{
        tabArr[i][1] = x0 + t1[i-1] + a2[i];
      }
    }
    retVal.first = min(tabArr[n-1][0] + x1, tabArr[n-1][1] + x2);
    retVal.second =  n;
    
    return retVal;

}
