// this file is for you for testing purposes, it won't be included in evaluation.

#include <iostream>
#include <random>
#include <ctime>
#include <utility>      // for pair


using namespace std;


// comment below line to stop DEBUG
#define DEBUG 
#define DEBUG_STDERR(x) do { std::cerr << (x) << endl; } while(0)
#define DEBUG_ARRAY(a, n) do { for (int i = 0; i < n; i++) std::cerr << a[i] << " ";  std::cerr << endl; } while(0)

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

void randomArray(int*& array, int size, int min, int interval) 
{
  
  for (int i = 0; i < size; i++) 
  {
    array[i] = min + (random() % interval);
  }
}


void test_case_1(bool r, bool m, bool t){
    int a1[] = { 7,9,3,4,8 };
    int* a11 = a1;
    int a2[] = { 8,5,6,4,5 };
    int* a22 = a2;
    int n = 5;
    int t1[] = { 2,3,1,3 };
    int* t11 = t1;
    int t2[] = { 2,1,2,2 };
    int* t22 = t2;
    int e1 = 2, e2 = 4, x1 = 3, x2 = 6;
    
    std::cout << "---------------------Test1--------------------" << std::endl;

    if(r){
        std::pair<int,int> min = Assembly_Rec(a11, a22, n, t11, t22, e1, e2, x1, x2);
        if(min.first == 35 && min.second == 62){
            std::cout << "Rec : Success";
        }
        else{
            std::cout << "Rec : Fail -> Expected (35,62) Found (" << min.first << "," << min.second << ")";
        }
    }
    if(m){
        std::pair<int,int> min = Assembly_Memo(a11, a22, n, t11, t22, e1, e2, x1, x2);
        if(min.first == 35 && min.second == 18){
            std::cout << "\tMemo : Success";
        }
        else{
            std::cout << "\tMemo : Fail -> Expected (35,18) Found (" << min.first << "," << min.second << ")";
        }
    }
    if(t){
        std::pair<int,int> min = Assembly_Tab(a11, a22, n, t11, t22, e1, e2, x1, x2);
        if(min.first == 35 && min.second == 5){
            std::cout << "\tTab : Success" << std::endl;
        }
        else{
            std::cout << "\tTab : Fail -> Expected (35,5) Found (" << min.first << "," << min.second << ")" << std::endl;
        }
    }
    std::cout << std::endl;

}

void test_case_2(bool r, bool m, bool t){
    int a1[] = {4, 5, 3, 2};
    int* a11 = a1;
    int a2[] = {2, 10, 1, 4};
    int* a22 = a2;
    int n = 4;
    int t1[] = {7,4,5};
    int* t11 = t1;
    int t2[] = {9,2,8};
    int* t22 = t2;
    int e1 = 10, e2 = 12, x1 = 18, x2 = 7;

    std::cout << "---------------------Test2--------------------" << std::endl;

    if(r){
        std::pair<int,int> min = Assembly_Rec(a11, a22, n, t11, t22, e1, e2, x1, x2);
        if(min.first == 35 && min.second == 30){
            std::cout << "Rec : Success";
        }
        else{
            std::cout << "Rec : Fail -> Expected (35,30) Found (" << min.first << "," << min.second << ")";
        }        
    }
    if(m){
        std::pair<int,int> min = Assembly_Memo(a11, a22, n, t11, t22, e1, e2, x1, x2);
        if(min.first == 35 && min.second == 14){
            std::cout << "\tMemo : Success";
        }
        else{
            std::cout << "\tMemo : Fail -> Expected (35,14) Found (" << min.first << "," << min.second << ")";
        }
    }
    if(t){
        std::pair<int,int> min = Assembly_Tab(a11, a22, n, t11, t22, e1, e2, x1, x2);
        if(min.first == 35 && min.second == 4){
            std::cout << "\tTab : Success" << std::endl;
        }
        else{
            std::cout << "\tTab : Fail -> Expected (35,4) Found (" << min.first << "," << min.second << ")" << std::endl;
        }
    }
    std::cout << std::endl;
}

void test_case_3(bool r, bool m, bool t){
    int a1[] = {4,5};
    int* a11 = a1;
    int a2[] = {2,10};
    int* a22 = a2;
    int n = 2;
    int t1[] = {2};
    int* t11 = t1;
    int t2[] = {1};
    int* t22 = t2;
    int e1 = 2, e2 = 1, x1 = 1, x2 = 1;

    std::cout << "---------------------Test3--------------------" << std::endl;


    if(r){
        std::pair<int,int> min = Assembly_Rec(a11, a22, n, t11, t22, e1, e2, x1, x2);
        if(min.first == 10 && min.second == 6){
            std::cout << "Rec : Success";
        }
        else{
            std::cout << "Rec : Fail -> Expected (10,6) Found (" << min.first << "," << min.second << ")";
        }
    }
    if(m){
        std::pair<int,int> min = Assembly_Memo(a11, a22, n, t11, t22, e1, e2, x1, x2);
        if(min.first == 10 && min.second == 6){
            std::cout << "\tMemo : Success";
        }
        else{
            std::cout << "\tMemo : Fail -> Expected (10,6) Found (" << min.first << "," << min.second << ")";
        }
    }
    if(t){
        std::pair<int,int> min = Assembly_Tab(a11, a22, n, t11, t22, e1, e2, x1, x2);
        //std::cout << "Cost : "<< min.first << " Loop : " << min.second << std::endl;
        if(min.first == 10 && min.second == 2){
            std::cout << "\tTab : Success" << std::endl;
        }
        else{
            std::cout << "\tTab : Fail -> Expected (10,2) Found (" << min.first << "," << min.second << ")" << std::endl;
        }
    }
}

int main(){


    // 1 -> TEST
    // 0 -> DON'T TEST

    // (TestRecursive, TestMemoization, TestTabular) 
    // Example -> test_case_1(1,0,0) will test for only recursive case.
    // Example -> test_case_1(1,1,0) will test for recursive and memoization case.

    test_case_1(1,1,1);
    test_case_2(1,1,1);
    test_case_3(1,1,1);
    

    



    return 0;
}